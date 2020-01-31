#ifndef __HISTOS_H__
#define __HISTOS_H__

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "L1Trigger/TrackFindingTMTT/interface/Data.h"

#include <TH1F.h>
#include <TH2F.h>
#include <TH2Poly.h>
#include <TF1.h>
#include <TPad.h>
#include <TProfile.h>
#include <TGraphAsymmErrors.h>
#include <TGraph.h>
#include <TEfficiency.h>


using namespace std;

namespace TMTT {

class Settings;

class Track;

class Stub;

class TP;

class Histos {

public:

    Histos(const Settings *settings, Data *data);

    ~Histos() {}

    void analyze();

    void endJob();

private:

    void bookTracker();

    void bookDTC();

    void bookGP();

    void bookHT();

    void bookMHT();

    void bookLR();

    void bookLRHLS();

    void bookDR();

    void fillTracker();

    void fillDTC();

    void fillGP();

    void fillHT();

    void fillMHT();

    void fillLR();

    void fillLRHLS();

    void fillDR();

    void printPerformanceTracker();

    void printPerformanceDTC();

    void printPerformanceGP();

    void printPerformanceHT();

    void printPerformanceMHT();

    void printPerformanceLR();

    void printPerformanceLRHLS();

    void printPerformanceDR();

    // Configuration parameters.

    const Settings *settings_;
    Data *data_;

    const double maxQoverPt_;
    const int numRegions_;
    const int numDTCsPerTFP_;
    const int numBinsPhi_;
    const int numBinsPt_;
    const int numSectorsPhi_;
    const int numSectorsEta_;
    const int numSectors_;
    const int numFramesAlgoIn_;
    const int numFramesAlgoOut_;
    const bool enableTruncation_;
    edm::Service<TFileService> fs_;

    TProfile *profTracker_;
    TProfile *profDTC_;

    TProfile *profGP_;
    TProfile *profHT_;
    TProfile *profMHT_;
    TProfile *profLR_;
    TProfile *profLRHLS_;
    TProfile *profDR_;

    unordered_map<string, int> numTPs_;

    TH1 *hisHTqOverPtRes_;
    TH1 *hisMHTqOverPtRes_;
    TH2F *ianHT_;
    TH2F *ianMHT_;

    TH1F *hisTPinvPtDTC_;
    TH1F *hisTPinvPtGP_;
    TH1F *hisTPinvPtHT_;
    TH1F *hisTPinvPtMHT_;

    /*TH1F *hisLRsize_;
    TH1F *hisLRqOverPt_;
    TH1F *hisLRphi_;
    TH1F *hisLRz_;
    TH1F *hisLRcot_;
    TH1F *hisLRchi2_;
    TH1F *hisLRqOverPtRes_;
    TH2F *mazLR_;*/

};

}

#endif