/*
Created by Maziar Ghorbani - Brunel University on 12/06/19.
*/

#ifndef __LRHLS_v5_H__
#define __LRHLS_v5_H__

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
class LRHLS_v5 {
public:

	LRHLS_v5() {}

	~LRHLS_v5() {}

	void produce(int13_t r[SZ], int14_t phi[SZ], int14_t z[SZ], uint3_t layerId[SZ], bool valid[SZ]);

public:

	unsigned int population_[LAYERS];

};

template<class T, int SZ, int LAYERS, int LIMIT>
void LRHLS_v5<T, SZ, LAYERS, LIMIT>::produce(int13_t r[SZ], int14_t phi[SZ], int14_t z[SZ], uint3_t layerId[SZ], bool valid[SZ]) {

	dtf_t r_tmp[SZ];
	dtf_t phi_tmp[SZ];
	dtf_t z_tmp[SZ];

	for(int i = 0; i < SZ; i++) {
		r_tmp[i] = dtf_t(r[i]) >> 1;
		phi_tmp[i] = dtf_t(phi[i]) >> 1;
		z_tmp[i] = dtf_t(z[i]) >> 1;
	}

	dtf_t rSum = 0;
	dtf_t phiSum = 0;
	dtf_t zSum = 0;

    dtf_t layerMinPos = 0;
    dtf_t layerMaxPos = 0;
    dtf_t layerPos = 0;

    dtf_t n = 0;

    dtf_t largestResid = 0;

    uint4_t idx = 0;

    uint4_t nStubs = SZ;

    bool single = false;

    dtf_t residuals[SZ];

	for(int i = 0; i < LAYERS; i++) {
		population_[i] = 0;
	}

	for(int i = 0; i < SZ; i++) {
		population_[layerId[i]] += 1;
	}

	for(int k = 0; k < 10; k++) {

		single = true;

		for(int i = 0; i < 7; i++) {
			if(population_[i] > 1) {
				single = false;
			}
		}

		if((single == true) && (nStubs <= LIMIT)) break;

	    for(int i = 0; i < LAYERS; i++) {
	       if(population_[i] > 0) {
	           n += 1;
	       }
	    }

        for(int i = 0; i < LAYERS; i++) {
            layerMinPos = 8191;
            layerMaxPos = -8192;
            layerPos = 0;
            for(int j = 0; j < SZ; j++) {
                if(valid[j]) {
                	dtf_t tmp = r_tmp[j];
                    if(layerId[j] == i) {
                        layerMinPos = min_t(layerMinPos, tmp);
                        layerMaxPos = max_t(layerMaxPos, tmp);
                        layerPos = layerMinPos + layerMaxPos;
                        layerPos >>= 2;
//                        layerPos /= 2;
            		}
        		}
			}
            rSum += layerPos;
		}

        for(int i = 0; i < LAYERS; i++) {
            layerMinPos = 8191;
            layerMaxPos = -8192;
            layerPos = 0;
            for(int j = 0; j < SZ; j++) {
                if(valid[j]) {
                	dtf_t tmp = phi_tmp[j];
                    if(layerId[j] == i) {
                        layerMinPos = min_t(layerMinPos, tmp);
                        layerMaxPos = max_t(layerMaxPos, tmp);
                        layerPos = layerMinPos + layerMaxPos;
                        layerPos >>= 2;
//                        layerPos /= 2;
            		}
        		}
			}
            phiSum += layerPos;
		}

        for(int i = 0; i < LAYERS; i++) {
            layerMinPos = 8191;
            layerMaxPos = -8192;
            layerPos = 0;
            for(int j = 0; j < SZ; j++) {
                if(valid[j]) {
                	dtf_t tmp = z_tmp[j];
                    if(layerId[j] == i) {
                        layerMinPos = min_t(layerMinPos, tmp);
                        layerMaxPos = max_t(layerMaxPos, tmp);
                        layerPos = layerMinPos + layerMaxPos;
                        layerPos >>= 2;
//                        layerPos /= 2;
            		}
        		}
			}
            zSum += layerPos;
		}

//        pair_t<dtf_t,dtf_t> rphi = calcFit(n, rSum, phiSum);
//        pair_t<dtf_t,dtf_t> rz = calcFit(n, rSum, zSum);

        dtf_t sp = (n * (rSum * phiSum) - (rSum * phiSum)) / (n * (phiSum * phiSum) - (rSum * rSum));
        dtf_t ip = (phiSum - sp * rSum) / n;

        dtf_t sz = (n * (rSum * zSum) - (rSum * zSum)) / (n * (zSum * zSum) - (rSum * rSum));
        dtf_t iz = (zSum - sz * rSum) / n;

        for(int i = 0; i < SZ; i++) {
        	residuals[i] = 0;
        }

        for(int i = 0; i < SZ; i++) {
            if(valid[i]) {
//                dtf_t phi_resid = phi[i] - (rphi.first * r[i] + rphi.second);
//                dtf_t z_resid = z[i] - (rz.first * r[i] + rz.second);
                dtf_t phi_resid = phi[i] - (sp * r[i] + ip);
                dtf_t z_resid = z[i] - (sz * r[i] + iz);

                residuals[i] = abs_t(phi_resid + z_resid);
            }
        }

        largestResid = residuals[0];

        for(int i = 0; i < SZ; i++) {
        	if(valid[i]) {
				if(residuals[i] > largestResid) {
					largestResid = residuals[i];
					idx = i;
				}
        	}
        }

        valid[idx] = false;

        population_[layerId[idx]] -= 1;
        nStubs -= 1;
	}
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
