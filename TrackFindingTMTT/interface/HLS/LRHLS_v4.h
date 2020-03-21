/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_V4_H__
#define __LRHLS_V4_H__

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_types.h"
#include "L1Trigger/TrackFindingTMTT/interface/HLS/StubHLS.h"
#else
#include "LRHLS_types.h"
#include "StubHLS.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

template<class T, int SZ, int LAYERS, int LIMIT>
class LRHLS_v4 {
public:

	LRHLS_v4() {}

	~LRHLS_v4() {}

	void produce(T *indata, T *outdata);

public:

	T stubs_[SZ];
	LRTrack LRParameter_;
	int layerPopulation_[LAYERS];

};



template<class T, int SZ, int LAYERS, int LIMIT>
void LRHLS_v4<T, SZ, LAYERS, LIMIT>::produce(T *indata, T *outdata) {

	int nStubs = 0;

    for(int i = 0; i < LAYERS; i++)
        layerPopulation_[i] = 0;

	for(int i = 0; i < SZ; i++) {
		stubs_[i] = indata[i];
		if(stubs_[i].valid()) {
            layerPopulation_[stubs_[i].layerId()]++;
            nStubs++;
        }
	}

    bool single = true;

    produce_label0:
	while(true) {

		single = true;

		for(int i = 0; i < LAYERS; i++)
			if(layerPopulation_[i] > 1)
				single = false;

		if(nStubs == LIMIT && single) break;

		sumData phiSums, zSums;

		for(int i = 0; i < SZ; i++) {
			if(stubs_[i].valid()) {

				stubData layerMinPos(4096, 4096, 4096, 4096);
				stubData layerMaxPos(-4096, -4096, -4096, -4096);

				stubData layerPos(0, 0, 0, 0);

				stubData pos(stubs_[i].r(), stubs_[i].phi(), stubs_[i].r(), stubs_[i].z());

				layerMinPos <= pos;
				layerMaxPos >= pos;
				layerPos = layerMinPos + layerMaxPos;
				layerPos /= 2;

				phiSums += make_pair_t(layerPos.RPhi, layerPos.Phi);
				zSums += make_pair_t(layerPos.RZ, layerPos.Z);

			}
		}

		const pair_t<dtf_t,dtf_t>& phiParameter = phiSums.calcLinearParameter();
		const pair_t<dtf_t,dtf_t>& zParameter = zSums.calcLinearParameter();

		LRParameter_ = LRTrack(phiParameter.first, phiParameter.second, zParameter.first, zParameter.second);

		residData largestResid = residData(-1.);

		for(unsigned int i = 0; i < SZ; i++) {
			if(stubs_[i].valid()) {

				stubData pos(stubs_[i].r(), stubs_[i].phi(), stubs_[i].r(), stubs_[i].z());

				float phiResid = pos.Phi - (LRParameter_.phiT + LRParameter_.qOverPt * pos.RPhi);
				float zResid = pos.Z - (LRParameter_.zT + LRParameter_.cotTheta * pos.RZ);

				residData resid(abs_t(phiResid), abs_t(zResid), stubs_[i].layerId(), i, true);

				if(resid.combined() > largestResid.combined())
					largestResid = resid;
			}
		}

		stubs_[largestResid.stubId].valid_ = false;
		layerPopulation_[largestResid.layerId]--;
		nStubs--;
	}

	for(int i = 0; i < SZ; i++)
		outdata[i] = stubs_[i];
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
