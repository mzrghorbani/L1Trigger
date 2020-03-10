/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifdef CMSSW_GIT_HASH
#include "L1Trigger/TrackFindingTMTT/interface/HLS/LRHLS_v3.h"
#else
#include "LRHLS_v3.h"
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

LRHLS_v3::LRHLS_v3(StubHLS& stubIn, StubHLS& stubOut) {

	static StubHLS stubs[10];
#pragma HLS ARRAY_PARTITION variable=stubs complete dim=1

	for (unsigned int i = 9; i > 0; i--)
		stubs[i] = stubs[i-1];

	stubs[0] = stubIn;

	//initFit

	static unsigned int nStubs = 0;

	static bool single = true;

	nStubs++;

	if(nStubs == 10) {

		unsigned int layerPopulation[7];

		for(unsigned int i = 0; i < 7; i++)
			layerPopulation[i] = 0;

		for(unsigned int i = 0; i < 10; i++)
			if(stubs[i].valid())
				layerPopulation[stubs[i].layerId()]++;

		while(true) {

			//calcHelix

			sumData phiSums, zSums;

			for(unsigned int i = 0; i < 10; i++) {
				if(stubs[i].valid()) {

					StubHLS stub = stubs[i];

					stubData layerMinPos(4096, 4096, 4096, 4096);
					stubData layerMaxPos(-4096, -4096, -4096, -4096);

					stubData layerPos(0, 0, 0, 0);

					stubData pos(stub.r(), stub.phi(), stub.r(), stub.z());

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

			// calcResidual

			largestResid_ = residData(-1.);

			for(unsigned int i = 0; i < 10; i++) {
				if(stubs[i].valid()) {

					StubHLS stub = stubs[i];

					stubData pos(stub.r(), stub.phi(), stub.r(), stub.z());

					float phiResid = pos.Phi - (LRParameter_.phiT + LRParameter_.qOverPt * pos.RPhi);
					float zResid = pos.Z - (LRParameter_.zT + LRParameter_.cotTheta * pos.RZ);

					residData resid(abs_t(phiResid), abs_t(zResid), stub.layerId(), i, true);

					if(resid.combined() > largestResid_.combined())
						largestResid_ = resid;
				}
			}

			 //killLargestResidual
			stubs[largestResid_.stubId].valid_ = false;
			layerPopulation[largestResid_.layerId]--;
			nStubs--;

			single = true;

			for(unsigned int i = 0; i < 7; i++)
				if(layerPopulation[i] > 1)
					single = false;

			if ((nStubs < 5) || (single == true))
				break;

		}

		nStubs = 0;
	}

	stubOut = stubs[9];
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
