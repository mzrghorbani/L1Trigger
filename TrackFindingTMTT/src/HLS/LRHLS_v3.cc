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

	static unsigned int counter = 0;

	static StubHLS stubs_[10];

	for(unsigned int i = 9; i > 0; i--)
		stubs_[i] = stubs_[i-1];
	stubs_[0] = stubIn;

	counter++;

	do {

	//initFit

	bool single;

	unsigned int layerPopulation_[7];

	for(unsigned int i = 0; i < 7; i++)
		layerPopulation_[i] = 0;

	unsigned int nStubs_ = 0;

	for(unsigned int i = 0; i < 10; i++)
		if(stubs_[i].valid()) {
			layerPopulation_[stubs_[i].layerId()]++;
			nStubs_++;
		}

		single = true;

		for(unsigned int i = 0; i < 7; i++)
			if(layerPopulation_[i] > 1)
				single = false;

		if((nStubs_ < 5) && (single == true))
			break;

		//calcHelix

		sumData phiSums, zSums;

		for(unsigned int i = 0; i < 10; i++) {
			if(stubs_[i].valid()) {

				StubHLS stub = stubs_[i];

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

		residData largestResid_ = residData(-1.);

		for(unsigned int i = 0; i < 10; i++) {
			if(stubs_[i].valid()) {

				StubHLS stub = stubs_[i];

				stubData pos(stub.r(), stub.phi(), stub.r(), stub.z());

				float phiResid = pos.Phi - (LRParameter_.phiT + LRParameter_.qOverPt * pos.RPhi);
				float zResid = pos.Z - (LRParameter_.zT + LRParameter_.cotTheta * pos.RZ);

				residData resid(abs_t(phiResid), abs_t(zResid), stub.layerId(), i, true);

				if(resid.combined() > largestResid_.combined())
					largestResid_ = resid;
			}
		}

		 //killLargestResidual

		stubs_[largestResid_.stubId].valid_ = false;
		layerPopulation_[largestResid_.layerId]--;
		nStubs_--;

	} while((counter % 10) == 0);

	stubOut = stubs_[9];
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
