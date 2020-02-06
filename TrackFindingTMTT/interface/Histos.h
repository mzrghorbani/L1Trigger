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

    TH1F *LRsize_;
    TH1F *LRqOverPt_;
    TH1F *LRphi_;
    TH1F *LRz_;
    TH1F *LRcot_;
    TH1F *LRchi2_;
    TH1F *LRqOverPtRes_;
    TH2F *LR_;

    TH1F *LRHLSsize_;
    TH1F *LRHLSqOverPt_;
    TH1F *LRHLSphi_;
    TH1F *LRHLSz_;
    TH1F *LRHLScot_;
    TH1F *LRHLSchi2_;
    TH1F *LRHLSqOverPtRes_;
    TH2F *LRHLS_;

};

}

#endif