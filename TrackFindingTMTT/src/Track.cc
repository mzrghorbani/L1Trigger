#include "L1Trigger/TrackFindingTMTT/interface/Track.h"
#include "L1Trigger/TrackFindingTMTT/interface/Settings.h"
#include "L1Trigger/TrackFindingTMTT/interface/Stub.h"
#include "L1Trigger/TrackFindingTMTT/interface/TP.h"

#include <algorithm>
#include <utility>


using namespace std;

namespace TMTT {

Track::Track() {}

Track::Track(const Settings *settings, const int &htCell, Stubs stubs, const int &pos) :
        settings_(settings), parent_(nullptr), stubs_(std::move(stubs)) {

    const int &numBinsPhi = settings->htNumBinsPhi();
    const int &numBinsPt = settings->htNumBinsPt();
    const int &numSectors = settings->numSectors();
    const int &numCells = settings->htNumCells();
    const int &numSectorsPhi = settings->gpNumSectorsPhi();
    const int &numSectorsEta = settings->gpNumSectorsEta();
    const double &baseM = settings->baseM();
    const double &baseC = settings->baseC();
    const double &baseRegion = settings->baseRegion();
    const double &baseSector = settings->baseSector();
    const double &chosenRofPhi = settings->chosenRofPhi();
    const double &chosenRofZ = settings->chosenRofZ();
    const double &invPtToDphi = settings->invPtToDphi();
    const Doubles &sectorsCot = settings->cots();

    cellId_ = htCell;
    // transform unique track id into track parameter
    binPhi_ = htCell % numBinsPhi;
    binPt_ = (htCell / numBinsPhi) % numBinsPt;
    secEta_ = (htCell / numCells) % numSectorsEta;
    secPhi_ = (htCell / (numCells * numSectorsEta)) % numSectorsPhi;
    region_ = htCell / (numCells * numSectors);

    // identify stream id out of track parameter
    streamId_ = (numSectorsPhi * region_ + secPhi_) * (numBinsPt / 2) + binPt_ % (numBinsPt / 2);
    streamPos_ = pos;

    binPhi_ -= numBinsPhi / 2;
    binPt_ -= numBinsPt / 2;

    valid_ = true;
    chi2_ = -1.;

    // convert track parameter in floating point
    qOverPt_ = (binPt_ + .5) * baseM / invPtToDphi;
    phi_ = reco::deltaPhi((binPhi_ + .5) * baseC + (region_ + 1) * baseRegion + (secPhi_ - .5) * baseSector -
                          chosenRofPhi * invPtToDphi * qOverPt_, 0.);
    cot_ = sectorsCot[secEta_];
    z_ = 0.;

    // match track with tracking particles
    match();

}


Track::Track(const int &mhtCell, const Track *htTrack, Stubs stubs, const int &pos) : settings_(
        htTrack->settings_), parent_(htTrack), stubs_(std::move(stubs)) {

    const double &invPtToDphi = settings_->invPtToDphi();
    const int &numBinsPhi = settings_->mhtNumBinsPhi();
    const int &numBinsPt = settings_->mhtNumBinsPt();
    const int &numCells = settings_->mhtNumCells();
    const double &baseM = settings_->baseM();
    const double &baseC = settings_->baseC();

    binPhi_ = htTrack->binPhi_;
    binPt_ = htTrack->binPt_;
    secEta_ = htTrack->secEta_;
    secPhi_ = htTrack->secPhi_;
    region_ = htTrack->region_;

    // finer track parameter in r-phi
    binPt_ = numBinsPt * binPt_ + mhtCell / numBinsPhi;
    binPhi_ = numBinsPhi * binPhi_ + mhtCell % numBinsPhi;

    // same stream id as in HT
    cellId_ = htTrack->cellId_;
    streamId_ = htTrack->streamId_;
    streamPos_ = pos - size();

    cellId_ = cellId_ * numCells + mhtCell;

    valid_ = true;
    chi2_ = -1.;

    // convert track parameter in floating point
    qOverPt_ = (binPt_ + .5) / (double) numBinsPt * baseM / invPtToDphi;
    phi_ = (binPhi_ + .5) / (double) numBinsPhi * baseC;
    cot_ = htTrack->cot_;
    z_ = htTrack->z_;

    // match track with tracking particles
    match();

}


Track::Track(const Track *mhtTrack, Stubs stubs, const double &qOverPt, const double &phi, const double &cot,
             const double &z) :
        settings_(mhtTrack->settings_), parent_(mhtTrack), stubs_(std::move(stubs)) {

    const double &invPtToDphi = settings_->invPtToDphi();
    const double &chosenRofPhi = settings_->chosenRofPhi();
    const double &chosenRofZ = settings_->chosenRofZ();
    const int &numSectorsPhi = settings_->gpNumSectorsPhi();
    const int &numSectorsEta = settings_->gpNumSectorsEta();
    const int &htNumCells = settings_->htNumCells();
    const int &mhtNumCells = settings_->mhtNumCells();
    const int &htNumBinsPhi = settings_->htNumBinsPhi();
    const int &mhtNumBinsPhi = settings_->mhtNumBinsPhi();
    const int &mhtNumBinsPt = settings_->mhtNumBinsPt();
    const double &baseM = settings_->baseM();
    const double &baseC = settings_->baseC();

    secEta_ = mhtTrack->secEta_;
    secPhi_ = mhtTrack->secPhi_;
    region_ = mhtTrack->region_;
    streamId_ = mhtTrack->streamId_;
    streamPos_ = mhtTrack->streamPos_;

    valid_ = true;
    chi2_ = -1.;

    qOverPt_ = qOverPt;
    phi_ = phi;
    cot_ = cot;
    z_ = z;

    binPt_ = floor(qOverPt_ / baseM / mhtNumBinsPt);
    binPhi_ = floor(phi_ / baseC / mhtNumBinsPhi);

    qOverPt_ = -qOverPt_ * invPtToDphi;
    phi_ = phi_ + qOverPt_ * chosenRofPhi;
    z_ = z + cot_ * chosenRofZ;

    // convert fitted track parameter in HT track id space
    cellId_ = (region_ * numSectorsPhi + secPhi_ * numSectorsEta + secEta_) * htNumCells * mhtNumCells +
              binPt_ * htNumBinsPhi * mhtNumBinsPhi + binPhi_;

    // match track with tracking particles
    match();

}

void Track::match() {

    const int &numLayers = settings_->trackerMaxLayerId() + 1;
    const int &minLayersMatch = settings_->tpMinLayersMatch();

    TPss layerTPsMap(numLayers);
    for (Stub *stub : stubs_) {
        const TPs &assocTPs = stub->assocTPs();
        TPs &layerTPs = layerTPsMap[stub->uniqueLayerId()];
        layerTPs.insert(layerTPs.end(), assocTPs.begin(), assocTPs.end());
    }
    unordered_map<TP *, int> TPLayerCountMap;
    for (auto &layer : layerTPsMap) {
        sort(layer.begin(), layer.end());
        layer.erase(unique(layer.begin(), layer.end()), layer.end());
        for (TP *tp : layer)
            TPLayerCountMap[tp]++;
    }
    for (const auto &tp : TPLayerCountMap)
        if (tp.second >= minLayersMatch)
            tps_.push_back(tp.first);

}

}
