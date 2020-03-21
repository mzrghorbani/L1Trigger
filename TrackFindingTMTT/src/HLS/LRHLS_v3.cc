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

LRHLS_v3::LRHLS_v3() {

}

void LRHLS_v3::produce(StubHLS &stubIn, StubHLS &stubOut) {

	StubHLS temp = stubIn;

	static stubData layerMinPos(4096, 4096, 4096, 4096);
	static stubData layerMaxPos(-4096, -4096, -4096, -4096);
	static residData largestResid_ = residData(-1.);

	static dtf_t xyp = 0;
	static dtf_t xp = 0;
	static dtf_t yp = 0;
	static dtf_t xxp = 0;

	static dtf_t xyz = 0;
	static dtf_t xz = 0;
	static dtf_t yz = 0;
	static dtf_t xxz = 0;

	stubData layerPos;

	dtf_t denominator = 0;
	dtf_t slope = 0;
	dtf_t intercept = 0;

	if(temp.valid()) {

		stubData pos(temp.r() ,temp.phi() ,temp.r(), temp.z());

		layerMinPos <= pos;
		layerMaxPos >= pos;
		layerPos = layerMinPos + layerMaxPos;
		layerPos /= 2;

		xyp += dtf_t(layerPos.RPhi * layerPos.Phi);
		xp += layerPos.RPhi;
		yp += layerPos.Phi;
		xxp += dtf_t(layerPos.RPhi * layerPos.RPhi);

		denominator = dtf_t(2 * xxp - xp * xp);
		slope = dtf_t(dtf_t(2 * xyp - xp * yp) / denominator);
		intercept = dtf_t(dtf_t(yp * xxp - xp * xyp) / denominator);

		pair_t<dtf_t,dtf_t> phiParameter = make_pair_t(slope, intercept);

		xyz += dtf_t(layerPos.RZ * layerPos.Z);
		xz += layerPos.RZ;
		yz += layerPos.Z;
		xxz += dtf_t(pos.RZ * pos.RZ);

		denominator = dtf_t(2 * xxz - xz * xz);
		slope = dtf_t(dtf_t(2 * xyz - xz * yz) / denominator);
		intercept = dtf_t(dtf_t(yz * xxz - xz * xyz) / denominator);

		pair_t<dtf_t,dtf_t> zParameter = make_pair_t(slope, intercept);

		LRTrack LRParameter_(phiParameter.first, phiParameter.second, zParameter.first, zParameter.second);

		float phiResid = pos.Phi - (LRParameter_.phiT + dtf_t(LRParameter_.qOverPt * pos.RPhi));
		float zResid = pos.Z - (LRParameter_.zT + dtf_t(LRParameter_.cotTheta * pos.RZ));

		residData resid(abs_t(phiResid), abs_t(zResid), 0, 0, true);

		if(resid.combined() > largestResid_.combined()) {
			largestResid_ = resid;
			temp.valid_ = false;
		}

		stubOut = temp;
	}
}

#ifdef CMSSW_GIT_HASH
}

}
#endif
