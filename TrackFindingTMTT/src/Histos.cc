#include "L1Trigger/TrackFindingTMTT/interface/Histos.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"
#include "L1Trigger/TrackFindingTMTT/interface/TP.h"
#include "L1Trigger/TrackFindingTMTT/interface/Track.h"


#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

namespace TMTT {

Histos::Histos(const Settings *settings, Data *data) :
        settings_(settings),
        data_(data),
        maxQoverPt_(1. / settings->htMinPt()),
        numRegions_(settings->dtcNumRegions()),
        numDTCsPerTFP_(settings->dtcNumPerTFP()),
        numBinsPhi_(settings->htNumBinsPhi()),
        numBinsPt_(settings->htNumBinsPt()),
        numSectorsPhi_(settings->gpNumSectorsPhi()),
        numSectorsEta_(settings->gpNumSectorsEta()),
        numSectors_(settings->numSectors()),
        numFramesAlgoIn_(settings->numFramesAlgoIn()),
        numFramesAlgoOut_(settings->numFramesAlgoOut()),
        enableTruncation_(settings->enableTruncation())
        {

    TH1::SetDefaultSumw2(true);

    bookTracker();
    bookDTC();
    bookGP();
    bookHT();
    bookMHT();
    bookLR();
    bookLRHLS();
    bookDR();

    numTPs_.reserve(8);
    numTPs_["Tracker"] = 0;
    numTPs_["DTC"] = 0;
    numTPs_["GP"] = 0;
    numTPs_["HT"] = 0;
    numTPs_["MHT"] = 0;
    numTPs_["LR"] = 0;
    numTPs_["LRHLS"] = 0;
    numTPs_["DR"] = 0;

}

void Histos::analyze() {

    const TPs &tps = data_->tps();

    for (TP *tp : tps)
        tp->routingComplete();

    fillTracker();
    fillDTC();
    fillGP();
    fillHT();
    fillMHT();
    fillLR();
    fillLRHLS();
    fillDR();

}

void Histos::bookTracker() {

    TFileDirectory inputDir = fs_->mkdir("Tracker");

    profTracker_ = inputDir.make<TProfile>("Tracker Counts", ";", 2, 0.5, 2.5);
    profTracker_->GetXaxis()->SetBinLabel(1, "Stubs");
    profTracker_->GetXaxis()->SetBinLabel(2, "TPs");

}

void Histos::fillTracker() {

    const TPs &tps = data_->tps();
    const Stubs &stubs = data_->stubs();

    int nStubs = stubs.size();
    int nTPs(0);
    for (const TP *tp : tps)
        if (tp->useForAlgEff())
            nTPs++;
    profTracker_->Fill(1, nStubs);
    profTracker_->Fill(2, nTPs);
    numTPs_["Tracker"] += nTPs;

}

void Histos::bookDTC() {

    TFileDirectory inputDir = fs_->mkdir("DTC");

    profDTC_ = inputDir.make<TProfile>("DTC Counts", ";", 3, 0.5, 3.5);
    profDTC_->GetXaxis()->SetBinLabel(1, "Stubs");
    profDTC_->GetXaxis()->SetBinLabel(2, "lost Stubs");
    profDTC_->GetXaxis()->SetBinLabel(3, "TPs");

}

void Histos::fillDTC() {

    const TPs &tps = data_->tps();
    const Stubs &stubs = data_->stubs();

    int nStubs(0);
    int nPassedStubs(0);
    int nLostStubs(0);
    int nTPs(0);

    Stubss stubss(numRegions_ * numDTCsPerTFP_);
    for (Stub *stub : stubs) {
        const Range &rangePhi = stub->rangePhi();
        const Range &rangeRegion = make_pair(rangePhi.first / numSectorsPhi_, rangePhi.second / numSectorsPhi_);
        nStubs++;
        if (rangeRegion.first != rangeRegion.second)
            nStubs++;
        for (const int &id : stub->dtcStreamIds())
            stubss[id].push_back(stub);
    }

    for (int id = 0; id < numRegions_ * numDTCsPerTFP_; id++)
        for (Stub *stub : stubss[id])
            if (!enableTruncation_ || stub->dtcStreamPos(id) < numFramesAlgoOut_)
                nPassedStubs++;
    nLostStubs = nStubs - nPassedStubs;

    for (const TP *tp : tps)
        if (tp->reconstructableAfterDTC())
            nTPs++;

    profDTC_->Fill(1, nPassedStubs);
    profDTC_->Fill(2, nLostStubs);
    profDTC_->Fill(3, nTPs);
    numTPs_["DTC"] += nTPs;

}

void Histos::bookGP() {

    TFileDirectory inputDir = fs_->mkdir("GP");

    profGP_ = inputDir.make<TProfile>("GP Counts", ";", 4, 0.5, 4.5);
    profGP_->GetXaxis()->SetBinLabel(1, "Stubs");
    profGP_->GetXaxis()->SetBinLabel(2, "lost Stubs");
    profGP_->GetXaxis()->SetBinLabel(3, "Gaps");
    profGP_->GetXaxis()->SetBinLabel(4, "TPs");

}

void Histos::fillGP() {

    const TPs &tps = data_->tps();
    const Stubs &stubs = data_->stubs();

    int nStubs(0);
    int nLostStubs(0);
    int nGaps(0);
    int nTPs(0);

    Stubss stubss(numRegions_ * numSectors_);
    for (Stub *stub : stubs)
        for (const int &id : stub->gpStreamIds())
            stubss[id].push_back(stub);

    for (int id = 0; id < numRegions_ * numSectors_; id++) {
        Stubs &stubs = stubss[id];
        sort(stubs.begin(), stubs.end(),
             [id](Stub *lhs, Stub *rhs) { return lhs->gpStreamPos(id) < rhs->gpStreamPos(id); });
        for (int n = 0; n < (int) stubs.size(); n++) {
            const int &pos = stubs[n]->gpStreamPos(id);
            stubs.insert(stubs.begin() + n, pos - n, nullptr);
            n = pos;
        }
        for (int pos = 0; pos < (int) stubs.size(); pos++) {
            const Stub *stub = stubs[pos];
            if (stub == nullptr)
                nGaps++;
            else if (!enableTruncation_ || pos < numFramesAlgoIn_)
                nStubs++;
            else
                nLostStubs++;
        }
    }

    for (const TP *tp : tps)
        if (tp->reconstructableAfterGP())
            nTPs++;

    profGP_->Fill(1, nStubs);
    profGP_->Fill(2, nLostStubs);
    profGP_->Fill(3, nGaps);
    profGP_->Fill(4, nTPs);
    numTPs_["GP"] += nTPs;

}

void Histos::bookHT() {

    TFileDirectory inputDir = fs_->mkdir("HT");

    profHT_ = inputDir.make<TProfile>("HT Counts", ";", 3, 0.5, 3.5);
    profHT_->GetXaxis()->SetBinLabel(1, "Stubs");
    profHT_->GetXaxis()->SetBinLabel(2, "Tracks");
    profHT_->GetXaxis()->SetBinLabel(3, "TPs");

    hisHTqOverPtRes_ = inputDir.make<TH1F>("QoverPtRes", "; Track q/Pt minus TP q/Pt", 18, -1. / 3., 1. / 3.);
    ianHT_ = inputDir.make<TH2F>("ian", "ian", 100, -1. / 3., 1. / 3., 18, -9, 9);

}

void Histos::fillHT() {

    const Tracks &tracks = data_->tracksHT();

    int nStubs(0);
    int nTracks(0);
    int nTPs(0);

    nTracks = tracks.size();
    TPs tps;
    for (const Track *track : tracks) {
        nStubs += track->size();
        for (TP *tp : track->tps())
            if (tp->useForAlgEff()) {
                tps.push_back(tp);
                hisHTqOverPtRes_->Fill(tp->qOverPt() - track->qOverPt());
                ianHT_->Fill(tp->qOverPt(), track->binPt() + .5);
            }
    }
    sort(tps.begin(), tps.end());
    tps.erase(unique(tps.begin(), tps.end()), tps.end());
    nTPs = tps.size();

    profHT_->Fill(1, nStubs);
    profHT_->Fill(2, nTracks);
    profHT_->Fill(3, nTPs);
    numTPs_["HT"] += nTPs;

}

void Histos::bookMHT() {

    TFileDirectory inputDir = fs_->mkdir("MHT");

    profMHT_ = inputDir.make<TProfile>("MHT Counts", ";", 4, 0.5, 4.5);
    profMHT_->GetXaxis()->SetBinLabel(1, "Stubs");
    profMHT_->GetXaxis()->SetBinLabel(2, "Gaps");
    profMHT_->GetXaxis()->SetBinLabel(3, "Tracks");
    profMHT_->GetXaxis()->SetBinLabel(4, "TPs");

    hisMHTqOverPtRes_ = inputDir.make<TH1F>("QoverPtRes", "; Track q/Pt minus TP q/Pt", 36, -1. / 3., 1. / 3.);
    ianMHT_ = inputDir.make<TH2F>("ian", "ian", 100, -1. / 3., 1. / 3., 36, -18, 18);

}

void Histos::fillMHT() {

    const Tracks &tracks = data_->tracksMHT();

    int nStubs(0);
    int nGaps(0);
    int nTracks(0);
    int nTPs(0);

    nTracks = tracks.size();
    TPs tps;
    for (const Track *track : tracks) {
        nStubs += track->size();
        for (TP *tp : track->tps())
            if (tp->useForAlgEff()) {
                tps.push_back(tp);
                hisMHTqOverPtRes_->Fill(tp->qOverPt() - track->qOverPt());
                ianMHT_->Fill(tp->qOverPt(), track->binPt() + .5);
            }
    }
    sort(tps.begin(), tps.end());
    tps.erase(unique(tps.begin(), tps.end()), tps.end());
    nTPs = tps.size();

    Trackss trackss(numRegions_ * numBinsPt_);
    for (Track *track : tracks) {
        const int &id = track->streamId();
        trackss[id].push_back(track);
    }

    for (int id = 0; id < numRegions_ * numBinsPt_; id++) {
        Tracks &tracks = trackss[id];
        sort(tracks.begin(), tracks.end(),
             [id](Track *lhs, Track *rhs) { return lhs->streamPos() < rhs->streamPos(); });
        int pos(0);
        for (Track *track : tracks) {
            const int &startPos = track->streamPos();
            const int &size = track->size();
            nGaps += startPos - pos;
            pos = startPos + size;
        }
    }

    profMHT_->Fill(1, nStubs);
    profMHT_->Fill(2, nGaps);
    profMHT_->Fill(3, nTracks);
    profMHT_->Fill(4, nTPs);
    numTPs_["MHT"] += nTPs;

}

void Histos::bookLR() {

    TFileDirectory inputDir = fs_->mkdir("LR");

    profLR_ = inputDir.make<TProfile>("LR Counts", ";", 4, 0.5, 4.5);
    profLR_->GetXaxis()->SetBinLabel(1, "Stubs");
    profLR_->GetXaxis()->SetBinLabel(2, "Gaps");
    profLR_->GetXaxis()->SetBinLabel(3, "Tracks");
    profLR_->GetXaxis()->SetBinLabel(4, "TPs");

    LRsize_ = inputDir.make<TH1F>("size", "Track: 'size'", 100, 0, 250);
    LRqOverPt_ = inputDir.make<TH1F>("qOverPt", "Track: 'q/Pt'", 36, -1./3., 1./3.);
    LRphi_ = inputDir.make<TH1F>("phi", "Track: 'phi'", 1000, -6, 6);
    LRz_ = inputDir.make<TH1F>("z", "Track: 'z'", 10000, -600, 600);
    LRcot_ = inputDir.make<TH1F>("cot", "Track: 'cot'", 1000, -6, 6);
    LRchi2_ = inputDir.make<TH1F>("chi2", "Track: 'chi2'", 1000, -3, 3);

    LRqOverPtRes_ = inputDir.make<TH1F>("qOverPtRes", "; Track: 'q/Pt' minus TP: 'q/Pt'", 36, -1. / 3., 1. / 3.);
    LR_ = inputDir.make<TH2F>("LR", "tp->qOverPt(), track->binPt() + .5", 100, -1. / 3., 1. / 3., 36, -18, 18);

}

void Histos::fillLR() {

    const Tracks &tracks = data_->tracksLR();

    int nStubs(0);
    int nGaps(0);
    int nTracks(0);
    int nTPs(0);

    nTracks = tracks.size();
    TPs tps;

    for (const Track *track : tracks) {
        nStubs += track->size();
        LRsize_->Fill(track->size());
        LRqOverPt_->Fill(track->qOverPt());
        LRphi_->Fill(track->phi());
        LRz_->Fill(track->z());
        LRcot_->Fill(track->cot());
        LRchi2_->Fill(track->chi2());

        for (TP *tp : track->tps()) {
            if (tp->useForAlgEff()) {
                tps.push_back(tp);
                LRqOverPtRes_->Fill(tp->qOverPt() - track->qOverPt());
                LR_->Fill(tp->qOverPt(), track->binPt() + .5);
            }
        }
    }

    sort(tps.begin(), tps.end());
    tps.erase(unique(tps.begin(), tps.end()), tps.end());
    nTPs = tps.size();

    cout << nStubs << " " << nGaps << " " << nTracks << " " << nTPs << endl;
    profLR_->Fill(1, nStubs);
    profLR_->Fill(2, nGaps);
    profLR_->Fill(3, nTracks);
    profLR_->Fill(4, nTPs);
    numTPs_["LR"] += nTPs;

}

void Histos::bookLRHLS() {

    TFileDirectory inputDir = fs_->mkdir("LRHLS");

    profLRHLS_ = inputDir.make<TProfile>("LRHLS Counts", ";", 4, 0.5, 4.5);
    profLRHLS_->GetXaxis()->SetBinLabel(1, "Stubs");
    profLRHLS_->GetXaxis()->SetBinLabel(2, "Gaps");
    profLRHLS_->GetXaxis()->SetBinLabel(3, "Tracks");
    profLRHLS_->GetXaxis()->SetBinLabel(4, "TPs");

    LRHLSsize_ = inputDir.make<TH1F>("size", "Track: 'size'", 100, 0, 250);
    LRHLSqOverPt_ = inputDir.make<TH1F>("qOverPt", "Track: 'q/Pt'", 36, -1./3., 1./3.);
    LRHLSphi_ = inputDir.make<TH1F>("phi", "Track: 'phi'", 1000, -6, 6);
    LRHLSz_ = inputDir.make<TH1F>("z", "Track: 'z'", 10000, -600, 600);
    LRHLScot_ = inputDir.make<TH1F>("cot", "Track: 'cot'", 1000, -6, 6);
    LRHLSchi2_ = inputDir.make<TH1F>("chi2", "Track: 'chi2'", 1000, -3, 3);

    LRHLSqOverPtRes_ = inputDir.make<TH1F>("qOverPtRes", "; Track: 'q/Pt' minus TP: 'q/Pt'", 36, -1. / 3., 1. / 3.);
    LRHLS_ = inputDir.make<TH2F>("LR", "tp->qOverPt(), track->binPt() + .5", 100, -1. / 3., 1. / 3., 36, -18, 18);

}

void Histos::fillLRHLS() {

    const Tracks &tracks = data_->tracksLRHLS();

    int nStubs(0);
    int nGaps(0);
    int nTracks(0);
    int nTPs(0);

    nTracks = tracks.size();
    TPs tps;

    for (const Track *track : tracks) {
        nStubs += (track->size() + 1);
        LRHLSsize_->Fill(track->size());
        LRHLSqOverPt_->Fill(track->qOverPt());
        LRHLSphi_->Fill(track->phi());
        LRHLSz_->Fill(track->z());
        LRHLScot_->Fill(track->cot());
//        LRHLSchi2_->Fill(track->chi2());
        LRHLSchi2_->Fill(track->cot());
    }

    for (const Track *track_MHT : data_->tracksMHT()) {
        for (TP *tp : track_MHT->tps()) {
            if (tp->useForAlgEff()) {
                tps.push_back(tp);
                LRHLSqOverPtRes_->Fill(tp->qOverPt() - track_MHT->qOverPt());
                LRHLS_->Fill(tp->qOverPt(), track_MHT->binPt() + .5);
            }
        }
    }

    sort(tps.begin(), tps.end());
    tps.erase(unique(tps.begin(), tps.end()), tps.end());
    nTPs = tps.size();

    cout << nStubs << " " << nGaps << " " << nTracks << " " << nTPs << endl;
    profLRHLS_->Fill(1, nStubs);
    profLRHLS_->Fill(2, nGaps);
    profLRHLS_->Fill(3, nTracks);
    profLRHLS_->Fill(4, nTPs);
    numTPs_["LRHLS"] += nTPs;

}

void Histos::bookDR() {

    TFileDirectory inputDir = fs_->mkdir("DR");

    profDR_ = inputDir.make<TProfile>("DR Counts", ";", 4, 0.5, 4.5);
    profDR_->GetXaxis()->SetBinLabel(1, "Stubs");
    profDR_->GetXaxis()->SetBinLabel(2, "Gaps");
    profDR_->GetXaxis()->SetBinLabel(3, "Tracks");
    profDR_->GetXaxis()->SetBinLabel(4, "TPs");

}

void Histos::fillDR() {

    const Tracks &tracks = data_->tracksLR();

    int nStubs(0);
    int nGaps(0);
    int nTracks(0);
    int nTPs(0);

    TPs tps;
    for (const Track *track : tracks)
        if (track->valid()) {
            nTracks++;
            nStubs += track->size();
            for (TP *tp : track->tps())
                if (tp->useForAlgEff())
                    tps.push_back(tp);
        }
    sort(tps.begin(), tps.end());
    tps.erase(unique(tps.begin(), tps.end()), tps.end());
    nTPs = tps.size();

    profDR_->Fill(1, nStubs);
    profDR_->Fill(2, nGaps);
    profDR_->Fill(3, nTracks);
    profDR_->Fill(4, nTPs);
    numTPs_["DR"] += nTPs;

}

void Histos::printPerformanceTracker() {

    const double numStubs = profTracker_->GetBinContent(1);
    const double numTPs = profTracker_->GetBinContent(2);
    const double numStubsErr = profTracker_->GetBinError(1);
    const double numTPsErr = profTracker_->GetBinError(2);

    const int wa = ceil(log10(numStubs));
    const int wb = ceil(log10(numStubsErr));

    cout << "======================================================" << endl;
    cout << "                   TRACKER  SUMMARY                   " << endl;
    cout << "Number of Stubs per event = " << setw(wa) << (int) numStubs << " +- " << setw(wb) << (int) numStubsErr
         << endl;
    cout << "Number of TPs   per event = " << setw(wa) << (int) numTPs << " +- " << setw(wb) << (int) numTPsErr
         << endl;

}

void Histos::printPerformanceDTC() {

    const double numStubs = profDTC_->GetBinContent(1);
    const double numLostStubs = profDTC_->GetBinContent(2);
    const double numTPs = profDTC_->GetBinContent(3);
    const double numStubsErr = profDTC_->GetBinError(1);
    const double numLostStubsErr = profDTC_->GetBinError(2);
    const double numTPsErr = profDTC_->GetBinError(3);

    const double eff = (double) numTPs_["DTC"] / (double) numTPs_["Tracker"];
    const double effErr = sqrt(eff * (1. - eff) / (double) numTPs_["Tracker"]);

    const int wa = ceil(log10(numStubs));
    const int wb = ceil(log10(numStubsErr));

    cout << "======================================================" << endl;
    cout << "                     DTC  SUMMARY                     " << endl;
    cout << "Number of Stubs          per event = " << setw(wa) << (int) numStubs << " +- " << setw(wb)
         << (int) numStubsErr << endl;
    cout << "Number of rejected Stubs per event = " << setw(wa) << (int) numLostStubs << " +- " << setw(wb)
         << (int) numLostStubsErr << endl;
    cout << "Tracking efficiency = " << numTPs_["DTC"] << "/" << numTPs_["Tracker"] << " = " << eff << " +- "
         << effErr << endl;

}

void Histos::printPerformanceGP() {

    const double numStubs = profGP_->GetBinContent(1);
    const double numLostStubs = profGP_->GetBinContent(2);
    const double numGaps = profGP_->GetBinContent(3);
    const double numTPs = profGP_->GetBinContent(4);
    const double numStubsErr = profGP_->GetBinError(1);
    const double numLostStubsErr = profGP_->GetBinError(2);
    const double numGapsErr = profGP_->GetBinError(3);
    const double numTPsErr = profGP_->GetBinError(4);

    const double eff = (double) numTPs_["GP"] / (double) numTPs_["Tracker"];
    const double effErr = sqrt(eff * (1. - eff) / (double) numTPs_["Tracker"]);

    const int wa = ceil(log10(numStubs));
    const int wb = ceil(log10(numStubsErr));

    cout << "======================================================" << endl;
    cout << "                      GP SUMMARY                      " << endl;
    cout << "Number of Stubs           per event = " << setw(wa) << (int) numStubs << " +- " << setw(wb)
         << (int) numStubsErr << endl;
    cout << "Number of Gaps            per event = " << setw(wa) << (int) numGaps << " +- " << setw(wb)
         << (int) numGapsErr << endl;
    cout << "Number of truncated Stubs per event = " << setw(wa) << (int) numLostStubs << " +- " << setw(wb)
         << (int) numLostStubsErr << endl;
    cout << "Tracking efficiency = " << numTPs_["GP"] << "/" << numTPs_["Tracker"] << " = " << eff << " +- "
         << effErr << endl;

}

void Histos::printPerformanceHT() {

    const double numStubs = profHT_->GetBinContent(1);
    const double numTracks = profHT_->GetBinContent(2);
    const double numTPs = profHT_->GetBinContent(3);
    const double numStubsErr = profHT_->GetBinError(1);
    const double numTracksErr = profHT_->GetBinError(2);
    const double numTPsErr = profHT_->GetBinError(3);

    const double eff = (double) numTPs_["HT"] / (double) numTPs_["Tracker"];
    const double effErr = sqrt(eff * (1. - eff) / (double) numTPs_["Tracker"]);

    const int wa = ceil(log10(numStubs));
    const int wb = ceil(log10(numStubsErr));

    cout << "======================================================" << endl;
    cout << "                      HT SUMMARY                      " << endl;
    cout << "Number of Stubs  per event = " << setw(wa) << (int) numStubs << " +- " << setw(wb) << (int) numStubsErr
         << endl;
    cout << "Number of Tracks per event = " << setw(wa) << (int) numTracks << " +- " << setw(wb) << (int) numTracks
         << endl;
    cout << "Tracking efficiency = " << numTPs_["HT"] << "/" << numTPs_["Tracker"] << " = " << eff << " +- "
         << effErr << endl;

}

void Histos::printPerformanceMHT() {

    const double numStubs = profMHT_->GetBinContent(1);
    const double numGaps = profMHT_->GetBinContent(2);
    const double numTracks = profMHT_->GetBinContent(3);
    const double numTPs = profMHT_->GetBinContent(4);
    const double numStubsErr = profMHT_->GetBinError(1);
    const double numGapsErr = profMHT_->GetBinError(2);
    const double numTracksErr = profMHT_->GetBinError(3);
    const double numTPsErr = profMHT_->GetBinError(4);

    const double eff = (double) numTPs_["MHT"] / (double) numTPs_["Tracker"];
    const double effErr = sqrt(eff * (1. - eff) / (double) numTPs_["Tracker"]);

    const int wa = ceil(log10(numStubs));
    const int wb = ceil(log10(numStubsErr));

    cout << "======================================================" << endl;
    cout << "                     MHT  SUMMARY                     " << endl;
    cout << "Number of Stubs  per event = " << setw(wa) << (int) numStubs << " +- " << setw(wb) << (int) numStubsErr
         << endl;
    cout << "Number of Gaps   per event = " << setw(wa) << (int) numGaps << " +- " << setw(wb) << (int) numGapsErr
         << endl;
    cout << "Number of Tracks per event = " << setw(wa) << (int) numTracks << " +- " << setw(wb) << (int) numTracks
         << endl;
    cout << "Tracking efficiency = " << (int) numTPs_["MHT"] << "/" << (int) numTPs_["Tracker"] << " = " << eff
         << " +- " << effErr << endl;

}

void Histos::printPerformanceLR() {

    const double numStubs = profLR_->GetBinContent(1);
    const double numGaps = profLR_->GetBinContent(2);
    const double numTracks = profLR_->GetBinContent(3);
    const double numTPs = profLR_->GetBinContent(4);
    const double numStubsErr = profLR_->GetBinError(1);
    const double numGapsErr = profLR_->GetBinError(2);
    const double numTracksErr = profLR_->GetBinError(3);
    const double numTPsErr = profLR_->GetBinError(4);

    const double eff = (double) numTPs_["LR"] / (double) numTPs_["Tracker"];
    const double effErr = sqrt(eff * (1. - eff) / (double) numTPs_["Tracker"]);

    const int wa = ceil(log10(numStubs));
    const int wb = ceil(log10(numStubsErr));

    cout << "======================================================" << endl;
    cout << "                      LR SUMMARY                      " << endl;
    cout << "Number of Stubs  per event = " << setw(wa) << (int) numStubs << " +- " << setw(wb) << (int) numStubsErr
         << endl;
    cout << "Number of Gaps   per event = " << setw(wa) << (int) numGaps << " +- " << setw(wb) << (int) numGapsErr
         << endl;
    cout << "Number of Tracks per event = " << setw(wa) << (int) numTracks << " +- " << setw(wb) << (int) numTracks
         << endl;
    cout << "Tracking efficiency = " << (int) numTPs_["LR"] << "/" << (int) numTPs_["Tracker"] << " = " << eff
         << " +- " << effErr << endl;

}

void Histos::printPerformanceLRHLS() {

    const double numStubs = profLRHLS_->GetBinContent(1);
    const double numGaps = profLRHLS_->GetBinContent(2);
    const double numTracks = profLRHLS_->GetBinContent(3);
    const double numTPs = profLRHLS_->GetBinContent(4);
    const double numStubsErr = profLRHLS_->GetBinError(1);
    const double numGapsErr = profLRHLS_->GetBinError(2);
    const double numTracksErr = profLRHLS_->GetBinError(3);
    const double numTPsErr = profLRHLS_->GetBinError(4);

    const double eff = (double) numTPs_["LRHLS"] / (double) numTPs_["Tracker"];
    const double effErr = sqrt(eff * (1. - eff) / (double) numTPs_["Tracker"]);

    const int wa = ceil(log10(numStubs));
    const int wb = ceil(log10(numStubsErr));

    cout << "======================================================" << endl;
    cout << "                      LRHLS SUMMARY                   " << endl;
    cout << "Number of Stubs  per event = " << setw(wa) << (int) numStubs << " +- " << setw(wb) << (int) numStubsErr
         << endl;
    cout << "Number of Gaps   per event = " << setw(wa) << (int) numGaps << " +- " << setw(wb) << (int) numGapsErr
         << endl;
    cout << "Number of Tracks per event = " << setw(wa) << (int) numTracks << " +- " << setw(wb) << (int) numTracks
         << endl;
    cout << "Tracking efficiency = " << (int) numTPs_["LRHLS"] << "/" << (int) numTPs_["Tracker"] << " = " << eff
         << " +- " << effErr << endl;

}

void Histos::printPerformanceDR() {

    const double numStubs = profDR_->GetBinContent(1);
    const double numGaps = profDR_->GetBinContent(2);
    const double numTracks = profDR_->GetBinContent(3);
    const double numTPs = profDR_->GetBinContent(4);
    const double numStubsErr = profDR_->GetBinError(1);
    const double numGapsErr = profDR_->GetBinError(2);
    const double numTracksErr = profDR_->GetBinError(3);
    const double numTPsErr = profDR_->GetBinError(4);

    const double eff = (double) numTPs_["DR"] / (double) numTPs_["Tracker"];
    const double effErr = sqrt(eff * (1. - eff) / (double) numTPs_["Tracker"]);

    const int wa = ceil(log10(numStubs));
    const int wb = ceil(log10(numStubsErr));

    cout << "======================================================" << endl;
    cout << "                      DR SUMMARY                      " << endl;
    cout << "Number of Stubs  per event = " << setw(wa) << (int) numStubs << " +- " << setw(wb) << (int) numStubsErr
         << endl;
    cout << "Number of Gaps   per event = " << setw(wa) << (int) numGaps << " +- " << setw(wb) << (int) numGapsErr
         << endl;
    cout << "Number of Tracks per event = " << setw(wa) << (int) numTracks << " +- " << setw(wb) << (int) numTracks
         << endl;
    cout << "Tracking efficiency = " << (int) numTPs_["DR"] << "/" << (int) numTPs_["Tracker"] << " = " << eff
         << " +- " << effErr << endl;

}

void Histos::endJob() {

    printPerformanceTracker();
    printPerformanceDTC();
    printPerformanceGP();
    printPerformanceHT();
    printPerformanceMHT();
    printPerformanceLR();
    printPerformanceLRHLS();
    printPerformanceDR();
    cout << "======================================================" << endl;

}

}
