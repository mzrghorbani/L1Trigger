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

	void produce(T& indata, T& outdata);

public:

	LRTrack LRParameter_;
	int layerPopulation_[LAYERS]{};

};

template<class T, int SZ, int LAYERS, int LIMIT>
void LRHLS_v5<T, SZ, LAYERS, LIMIT>::produce(T& indata, T& outdata) {

	static int nStubs = 0;

	if(indata.valid()) {
		layerPopulation_[indata.layerId()]++;
		nStubs++;
	}

	static T stubs[SZ];

	L1: for(int i = SZ-1; i > 0; i--) {
		stubs[i] = stubs[i-1];
	}
	stubs[0] = indata;

	if(nStubs == SZ) {

		stubData layerMinPos(4096, 4096, 4096, 4096);
		stubData layerMaxPos(-4096, -4096, -4096, -4096);

		dtf_t xyp = 0;
		dtf_t xp = 0;
		dtf_t yp = 0;
		dtf_t xxp = 0;

		dtf_t xyz = 0;
		dtf_t xz = 0;
		dtf_t yz = 0;
		dtf_t xxz = 0;

		dtf_t denominator = 0;
		dtf_t slope = 0;
		dtf_t intercept = 0;

		stubData layerPos;
		stubData pos;

		L2: for(int k = 0; k < SZ; k++) {

			bool single = true;

			L3: for(int i = LAYERS-1; i >= 0; i--)
				if(layerPopulation_[i] > 1)
					single = false;

			if(nStubs == LIMIT && single) break;

			L4: for(int i = SZ-1; i >= 0; i--) {
				if(stubs[i].valid()) {

					pos.RPhi = stubs[i].r();
					pos.Phi = stubs[i].phi();
					pos.RZ = stubs[i].r();
					pos.Z = stubs[i].z();

					layerMinPos.RPhi = (layerMinPos.RPhi <= pos.RPhi) ? layerMinPos.RPhi : pos.RPhi;
					layerMinPos.Phi = (layerMinPos.Phi <= pos.Phi) ? layerMinPos.Phi : pos.Phi;
					layerMinPos.RZ = (layerMinPos.RZ <= pos.RZ) ? layerMinPos.RZ : pos.RZ;
					layerMinPos.Z = (layerMinPos.Z <= pos.Z) ? layerMinPos.Z : pos.Z;

					layerMaxPos.RPhi = (layerMaxPos.RPhi >= pos.RPhi) ? layerMaxPos.RPhi : pos.RPhi;
					layerMaxPos.Phi = (layerMaxPos.Phi >= pos.Phi) ? layerMaxPos.Phi : pos.Phi;
					layerMaxPos.RZ = (layerMaxPos.RZ >= pos.RZ) ? layerMaxPos.RZ : pos.RZ;
					layerMaxPos.Z = (layerMaxPos.Z >= pos.Z) ? layerMaxPos.Z : pos.Z;

					layerPos.RPhi = layerMinPos.RPhi + layerMaxPos.RPhi;
					layerPos.Phi = layerMinPos.Phi + layerMaxPos.Phi;
					layerPos.RZ = layerMinPos.RZ + layerMaxPos.RZ;
					layerPos.Z = layerMinPos.Z + layerMaxPos.Z;

					layerPos.RPhi /= 2;
					layerPos.Phi /= 2;
					layerPos.RZ /= 2;
					layerPos.Z /= 2;

					xyp += dtf_t(layerPos.RPhi * layerPos.Phi);
					xp += layerPos.RPhi;
					yp += layerPos.Phi;
					xxp += dtf_t(layerPos.RPhi * layerPos.RPhi);

					xyz += dtf_t(layerPos.RZ * layerPos.Z);
					xz += layerPos.RZ;
					yz += layerPos.Z;
					xxz += dtf_t(layerPos.RZ * layerPos.RZ);
				}
			}

			denominator = dtf_t(nStubs * xxp - xp * xp);
			slope = dtf_t(dtf_t(nStubs * xyp - xp * yp) / denominator);
			intercept = dtf_t(dtf_t(yp * xxp - xp * xyp) / denominator);

			pair_t<dtf_t,dtf_t> phiParameter = make_pair_t(slope, intercept);

			denominator = dtf_t(nStubs * xxz - xz * xz);
			slope = dtf_t(dtf_t(nStubs * xyz - xz * yz) / denominator);
			intercept = dtf_t(dtf_t(yz * xxz - xz * xyz) / denominator);

			pair_t<dtf_t,dtf_t> zParameter = make_pair_t(slope, intercept);

			LRTrack LRParameter_(phiParameter.first, phiParameter.second, zParameter.first, zParameter.second);

			residData largestResid_ = residData(-1.);

			L5: for(int i = SZ-1; i >= 0; i--) {
				if(stubs[i].valid()) {

					pos.RPhi = stubs[i].r();
					pos.Phi = stubs[i].phi();
					pos.RZ = stubs[i].r();
					pos.Z = stubs[i].z();

					float phiResid = pos.Phi - (LRParameter_.phiT + dtf_t(LRParameter_.qOverPt * pos.RPhi));
					float zResid = pos.Z - (LRParameter_.zT + dtf_t(LRParameter_.cotTheta * pos.RZ));

					residData resid(abs_t(phiResid), abs_t(zResid), stubs[i].layerId(), i, true);

					if(resid.combined() > largestResid_.combined())
						largestResid_ = resid;
				}
			}

			stubs[largestResid_.stubId].valid_ = false;
			layerPopulation_[largestResid_.layerId]--;
			nStubs--;
		}
		nStubs = 0;
	}
	outdata = stubs[SZ-1];
}

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
