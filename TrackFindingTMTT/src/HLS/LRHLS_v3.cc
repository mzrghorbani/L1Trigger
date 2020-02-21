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

LRHLS_v3::LRHLS_v3(StubHLS *stubIn, StubHLS *stubOut) {

	unsigned int i, j;

	for(i = 0; i < 12; i++) {
		for (j = 11; j > 0; --j)
			stubs_[j] = stubs_[j-1];

		stubs_[0].r = stubIn->r();
		stubs_[0].phi = stubIn->phi();
		stubs_[0].z = stubIn->z();
		stubs_[0].layerId = stubIn->layerId();
		stubs_[0].valid = stubIn->valid();
	}

	// initialize signals
	for(i = 0; i < 7; ++i) 
		layerPopulation_[i] = 0;

	nLayers_ = 0;
	nStubs_ = 0;
	valid_ = false;

	// initfit
	for(i = 0; i < 12; ++i) {
		if(stubs_[i].valid){
			switch(stubs_[i].layerId) {
				case 1:
				layerPopulation_[1]++;
				nStubs_++;
				break;

				case 2:
				layerPopulation_[2]++;
				nStubs_++;
				break;

				case 3:
				layerPopulation_[3]++;
				nStubs_++;
				break;

				case 4:
				layerPopulation_[4]++;
				nStubs_++;
				break;

				case 5:
				layerPopulation_[5]++;
				nStubs_++;
				break;

				case 6:
				layerPopulation_[6]++;
				nStubs_++;
				break;

				default:
				layerPopulation_[0]++;
				nStubs_++;
				break;
			}
		}
	}

	// checkValidity

	for(i = 0; i < 7; ++i)
		if(layerPopulation_[i] > 1)
			valid_ = true;

	if(not valid_) {
		for(i = 0; i < 12; i++) {
			stubOut->r_ = stubs_[11].r;
			stubOut->phi_ = stubs_[11].phi;
			stubOut->z_ = stubs_[11].z;
			stubOut->layerId_ = stubs_[11].layerId;
			stubOut->valid_ = stubs_[11].valid;
			for (j = 11; j > 0; --j)
				stubs_[j] = stubs_[j-1];
		}

		return;
	}

	for(unsigned int l = 0; l < 1; l++) {

		// calcHelix

		stubData layerMinPos[7];

		for (i = 0; i < 7; i++) {
			layerMinPos[i].RPhi = 4095;
			layerMinPos[i].Phi = 4095;
			layerMinPos[i].RZ = 4095;
			layerMinPos[i].Z = 4095;
		}

		stubData layerMaxPos[7];

		for (i = 0; i < 7; i++) {
			layerMaxPos[i].RPhi = -4096;
			layerMaxPos[i].Phi = -4096;
			layerMaxPos[i].RZ = -4096;
			layerMaxPos[i].Z = -4096;
		}

		for (i = 0; i < 12; i++) {

			if (stubs_[i].valid) {
				stubData pos(stubs_[i].r, stubs_[i].phi, stubs_[i].r, stubs_[i].z);

				switch (stubs_[i].layerId) {
					case 1:
						layerMinPos[1] <= pos;
						layerMaxPos[1] >= pos;
						layerPos_[1] = layerMinPos[1] + layerMaxPos[1];
						layerPos_[1] /= 2;
						break;

					case 2:
						layerMinPos[2] <= pos;
						layerMaxPos[2] >= pos;
						layerPos_[2] = layerMinPos[2] + layerMaxPos[2];
						layerPos_[2] /= 2;
						break;

					case 3:
						layerMinPos[3] <= pos;
						layerMaxPos[3] >= pos;
						layerPos_[3] = layerMinPos[3] + layerMaxPos[3];
						layerPos_[3] /= 2;
						break;

					case 4:
						layerMinPos[4] <= pos;
						layerMaxPos[4] >= pos;
						layerPos_[4] = layerMinPos[4] + layerMaxPos[4];
						layerPos_[4] /= 2;
						break;

					case 5:
						layerMinPos[5] <= pos;
						layerMaxPos[5] >= pos;
						layerPos_[5] = layerMinPos[5] + layerMaxPos[5];
						layerPos_[5] /= 2;
						break;

					case 6:
						layerMinPos[6] <= pos;
						layerMaxPos[6] >= pos;
						layerPos_[6] = layerMinPos[6] + layerMaxPos[6];
						layerPos_[6] /= 2;
						break;

					default:
						layerMinPos[0] <= pos;
						layerMaxPos[0] >= pos;
						layerPos_[0] = layerMinPos[0] + layerMaxPos[0];
						layerPos_[0] /= 2;
						break;

				}
			}
		}


		sumData phiSum[7];

		for (i = 0; i < 7; i++) {
			if (layerPopulation_[i] > 0) {
				phiSum[i].n++;
				phiSum[i].xy += dtf_t(layerPos_[i].RPhi * layerPos_[i].Phi);
				phiSum[i].x += layerPos_[i].RPhi;
				phiSum[i].y += layerPos_[i].Phi;
				phiSum[i].xx += dtf_t(layerPos_[i].RPhi * layerPos_[i].RPhi);
			}
		}

		sumData zSum[7];

		for (i = 0; i < 7; i++) {
			if (layerPopulation_[i] > 0) {
				zSum[i].n++;
				zSum[i].xy += dtf_t(layerPos_[i].RZ * layerPos_[i].Z);
				zSum[i].x += layerPos_[i].RZ;
				zSum[i].y += layerPos_[i].Z;
				zSum[i].xx += dtf_t(layerPos_[i].RZ * layerPos_[i].RZ);
			}
		}

		sumData phiSums;

		for (i = 0; i < 7; i++) {
			phiSums.n += phiSum[i].n;
			phiSums.xy += phiSum[i].xy;
			phiSums.x += phiSum[i].x;
			phiSums.y += phiSum[i].y;
			phiSums.xx += phiSum[i].xx;
		}

		sumData zSums;

		for (i = 0; i < 7; i++) {
			zSums.n += zSum[i].n;
			zSums.xy += zSum[i].xy;
			zSums.x += zSum[i].x;
			zSums.y += zSum[i].y;
			zSums.xx += zSum[i].xx;
		}

		// dtf_t temp1 = dtf_t(phiSums.n * phiSums.xy - phiSums.x * phiSums.y);
		// dtf_t temp2 = dtf_t(phiSums.y * phiSums.xx - phiSums.x * phiSums.xy);
		// dtf_t temp3 = dtf_t(phiSums.n * phiSums.xx - phiSums.x * phiSums.x);

		// pair_t<dtf_t, dtf_t> phiParameter;
		// phiParameter.first = dtf_t(temp1 / temp3);
		// phiParameter.second = dtf_t(temp2 / temp3);

		// dtf_t temp4 = dtf_t(zSums.n * zSums.xy - zSums.x * zSums.y);
		// dtf_t temp5 = dtf_t(zSums.y * zSums.xx - zSums.x * zSums.xy);
		// dtf_t temp6 = dtf_t(zSums.n * zSums.xx - zSums.x * zSums.x);

		// pair_t<dtf_t, dtf_t> zParameter;
		// zParameter.first = dtf_t(temp4 / temp6);
		// zParameter.second = dtf_t(temp5 / temp6);
		
		const pair_t<dtf_t,dtf_t>& phiParameter = phiSums.calcLinearParameter();
    	const pair_t<dtf_t,dtf_t>& zParameter = zSums.calcLinearParameter();

		LRParameter_.qOverPt = phiParameter.first;
		LRParameter_.phiT = phiParameter.second;
		LRParameter_.cotTheta = zParameter.first;
		LRParameter_.zT = zParameter.second;

		// calcResidual
		dtf_t phiResid[7];
		dtf_t zResid[7];

		for (i = 0; i < 7; i++) {
			if (layerPopulation_[i] > 0) {
				phiResid[i] = dtf_t(LRParameter_.phiT + dtf_t(LRParameter_.qOverPt * layerPos_[i].RPhi));
				zResid[i] = dtf_t(LRParameter_.zT + dtf_t(LRParameter_.cotTheta * layerPos_[i].RZ));

			} else {
				phiResid[i] = 0;
				zResid[i] = 0;
			}
		}

		for (i = 0; i < 12; i++) {
			residuals_[i].phi = 0;
			residuals_[i].z = 0;
			residuals_[i].stubId = 0;
			residuals_[i].layerId = 0;
			residuals_[i].valid = false;
		}

		for (i = 0; i < 12; i++) {
			LRStub stub = stubs_[i];

			if (stub.valid) {
				residData residual;

				for (j = 0; j < 7; j++) {
					if (stub.layerId == j) {

						residual.phi = abs_t(stub.phi - phiResid[j]);
						residual.z = abs_t(stub.z - zResid[j]);
						residual.stubId = i;
						residual.layerId = j;
						residual.valid = true;
						residuals_[i] = residual;
					}
				}
			}
		}

		// findLargestResidual

		largestResid_.phi = -1.;
		largestResid_.z = -1.;
		largestResid_.stubId = 0;
		largestResid_.layerId = 0;
		largestResid_.valid = false;

		for (i = 0; i < 12; i++) {
			residData residual = residuals_[i];

			if (residual.valid) {
				if (residual.combined() > largestResid_.combined()) {
					largestResid_ = residual;
				}
			}
		}

		// killLargestResidual

		bool stubCritical = false;
		bool layerCritical = false;
		bool single = true;

		if (nStubs_ <= 4)
			stubCritical = true;

		if(nLayers_ == 2)
			layerCritical = true;

		for(i = 0; i < 7; i++)
			if(layerPopulation_[i] > 1)
				single = false;

		if(stubCritical && layerCritical && single)
			break;

		stubs_[largestResid_.stubId].valid = false;
		layerPopulation_[largestResid_.layerId]--;

		if(layerPopulation_[largestResid_.layerId] == 0)
			nLayers_--;

		nStubs_--;

	}


	// create
	for(i = 0; i < 12; i++) {
		stubOut->r_ = stubs_[11].r;
		stubOut->phi_ = stubs_[11].phi;
		stubOut->z_ = stubs_[11].z;
		stubOut->layerId_ = stubs_[11].layerId;
		stubOut->valid_ = stubs_[11].valid;
		for (j = 11; j > 0; --j)
			stubs_[j] = stubs_[j-1];
	}

}

#ifdef CMSSW_GIT_HASH
}

}
#endif

