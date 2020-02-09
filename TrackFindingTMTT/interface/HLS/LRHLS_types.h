#ifndef LRHLS_TYPES_H_
#define LRHLS_TYPES_H_

/**
 * This defines all types used in HLS code.
 *
 * Author: Maziar Ghorbani
 *
 */

#ifdef CMSSW_GIT_HASH
#include <iostream>
#include <ap_fixed.h>
#include <cmath>
#else
#include <ap_fixed.h>
#include <cmath>
#endif

#ifdef CMSSW_GIT_HASH
namespace TMTT {

namespace HLS {
#endif

enum {
    B14 = 14, B13 = 13, B4 = 4, B3 = 3, B1 = 1
};
enum {
    WB = 18, IB = 14, FB = WB - IB
};

// Fixed-point variables used in HW
// typedef ap_uint<B1> uint1_t;
// typedef ap_uint<B3> uint3_t;
// typedef ap_uint<B4> uint4_t;
// typedef ap_int<B13> int13_t;
// typedef ap_int<B14> int14_t;
// typedef ap_fixed<WB,IB> dtf_t;

// Fixed-point variables used in SW
typedef ap_uint<B1> uint1_t;
typedef ap_uint<B3> uint3_t;
typedef ap_uint<B4> uint4_t;
typedef ap_fixed<B13,9> int13_t;
typedef ap_fixed<B14,10> int14_t;
typedef ap_fixed<WB,IB> dtf_t;

// Fixed-point variables used in Tests
//typedef float int13_t;
//typedef float int14_t;
//typedef int uint4_t;
//typedef int uint3_t;
//typedef bool uint1_t;
//typedef float dtf_t;

template<typename T>
T abs_t(const T &a) {
    if (a < 0)
        return -a;
    return a;
}

template<class T>
const T &min_t(const T &a, const T &b) {
    if (a < b)
        return a;
    return b;
}

template<class T>
const T &max_t(const T &a, const T &b) {
    if (a > b)
        return a;
    return b;
}

template<typename T1, typename T2>
struct pair_t {
    T1 first;
    T2 second;
    pair_t() : first(0), second(0) {}
    pair_t(const T1 &a, const T2 &b) : first(a), second(b) {}
};

template<typename T1, typename T2>
pair_t<T1, T2> make_pair_t(const T1 &a, const T2 &b) {
    return (pair_t<T1, T2>(a, b));
}

template<typename T>
struct array_t {
    int size_;
    T data_[12];
    array_t() : size_(0) {}
    void push_back(const T &value) { data_[size_++] = value; }
    T &operator[](const int &idx) { return data_[idx]; }
    const T &operator[](const int &idx) const { return data_[idx]; }
    T *begin() { return &data_[0]; }
    const T *begin() const { return &data_[0]; }
    T *end() { return &data_[size_]; }
    const T *end() const { return &data_[size_]; }
    int size() const { return size_; }
    void clear() { size_ = 0; }
};

struct LRStub {
    int13_t r = 0;
    int14_t phi = 0;
    int14_t z = 0;
    uint3_t layerId = 0;
    bool valid = false;
};

struct LRTrack {
    dtf_t qOverPt = 0;
    dtf_t phiT = 0;
    dtf_t cotT = 0;
    dtf_t zT = 0;
};

struct stubData {
    dtf_t RPhi = 0;
    dtf_t Phi = 0;
    dtf_t RZ = 0;
    dtf_t Z = 0;

    stubData &operator<=(const stubData &a) {
        RPhi = min_t(RPhi, a.RPhi);
        Phi = min_t(Phi, a.Phi);
        RZ = min_t(RZ, a.RZ);
        Z = min_t(Z, a.Z);
        return *this;
    }

    stubData &operator>=(const stubData &a) {
        RPhi = max_t(RPhi, a.RPhi);
        Phi = max_t(Phi, a.Phi);
        RZ = max_t(RZ, a.RZ);
        Z = max_t(Z, a.Z);
        return *this;
    }

    stubData operator+(const stubData &a) {
        RPhi += a.RPhi;
        Phi += a.Phi;
        RZ += a.RZ;
        Z += a.Z;
        return *this;
    }

    stubData &operator/=(const dtf_t &a) {
        RPhi /= a;
        Phi /= a;
        RZ /= a;
        Z /= a;
        return *this;
    }
};

struct sumData {
    dtf_t n = 0;
    dtf_t xy = 0;
    dtf_t x = 0;
    dtf_t y = 0;
    dtf_t xx = 0;

    sumData &operator+=(const sumData &stub) {
        n++;
        xy += stub.xy;
        x += stub.x;
        y += stub.y;
        xx += stub.xx;
        return *this;
    }

    sumData &operator+=(const pair_t<dtf_t, dtf_t> &stub) {
        n++;
        xy += dtf_t(stub.first * stub.second);
        x += stub.first;
        y += stub.second;
        xx += dtf_t(stub.first * stub.first);
        return *this;
    }

    pair_t<dtf_t, dtf_t> calcLinearParameter() const {
        auto denominator = dtf_t(n * xx - x * x);
        auto slope = dtf_t(dtf_t(n * xy - x * y) / denominator);
        auto intercept = dtf_t(dtf_t(y * xx - x * xy) / denominator);
        return make_pair_t(slope, intercept);
    }
};

struct residData {
    dtf_t phi = 0;
    dtf_t z = 0;
    uint3_t layerId = 0;
    uint4_t stubId = 0;
    uint1_t keep = false;
    uint1_t valid = false;

    dtf_t combined() const {
        return (phi + z);
    }
};

//template <typename T>
//T multiply_t(const T &multiplicand, const T &multiplier) {
//
//    dtf_t A = multiplicand;
//    dtf_t B = multiplier;
//
//    ap_fixed<WB+WB,IB+IB> R = 0;
//
//    bool A_neg_flag = false;
//    bool B_neg_flag = false;
//
//    if(A.is_neg()) {
//        A = -A;
//        A_neg_flag = true;
//    }
//
//    if(B.is_neg()) {
//        B = -B;
//        B_neg_flag = true;
//    }
//
//    ap_uint<WB+FB> A_sh = ap_fixed<WB+FB,IB+FB>(A) << FB;
//    ap_uint<WB+FB> B_sh = ap_fixed<WB+FB,IB+FB>(B) << FB;
//    ap_uint<2*(WB+FB)> pv = 0;
//    ap_uint<2*(WB+FB)> bp = 0;
//
//    bp.range((WB+FB)-1, 0) = B_sh;
//
//    for(int i = 0; i < WB+FB; i++) {
//        if(A_sh.bit(i) == 1) {
//            pv = pv + bp;
//        }
//        bp = bp << 1;
//    }
//
//    ap_uint<2*(WB+FB)> res_uint = pv;
//    R = ap_fixed<2*(WB+FB),2*(IB+FB)>(res_uint) >> (FB+FB);
//
//    if(A_neg_flag) {
//        R = -R;
//    }
//    if(B_neg_flag) {
//        R = -R;
//    }
//    return dtf_t(R);
//}

//template <typename T>
//T divide_t(const T &dividend, const T &divisor) {
//
//    dtf_t Q = 0;
//    dtf_t R = 0;
//    dtf_t div = dividend;
//    dtf_t dis = divisor;
//
//    bool div_neg_flag = false;
//    bool dis_neg_flag = false;
//
//    if(div.is_neg()) {
//        div = -div;
//        div_neg_flag = true;
//    }
//
//    if(dis.is_neg()) {
//        dis = -dis;
//        dis_neg_flag = true;
//    }
//
//    ap_uint<WB+FB> div_sh = ap_fixed<WB+FB,IB+FB>(div) << FB;
//    ap_uint<WB+FB> dis_sh = ap_fixed<WB+FB,IB+FB>(dis) << FB;
//    ap_uint<WB+FB+1> B = 0;
//    B.range((WB+FB-1), 0) = dis_sh;
//    ap_uint<2*(WB+FB)+1> AQ = 0;
//    AQ.range((WB+FB-1), 0) = div_sh;
//
//    for(int i = 0; i < WB+FB+FB; i++) {
//
//        AQ = AQ << 1;
//        AQ.range((2*(WB+FB)), (WB+FB)) = AQ.range((2*(WB+FB)), (WB+FB)) - B;
//
//        if(AQ.bit((2*(WB+FB))) == 1) {
//            AQ.bit(0) = 0;
//            AQ.range((2*(WB+FB)), (WB+FB)) = AQ.range((2*(WB+FB)), (WB+FB)) + B;
//
//        } else {
//            AQ.bit(0) = 1;
//
//        }
//    }
//
//    ap_uint<WB+FB+1> rem_uint = AQ.range((2*(WB+FB)), (WB+FB));
//    ap_uint<WB+FB> quo_uint = AQ.range((WB+FB-1), 0);
//
//    R = ap_fixed<WB+WB,IB+WB>(rem_uint) >> FB;
//    Q = ap_fixed<WB+WB,IB+WB>(quo_uint) >> FB;
//
//    if(div_neg_flag) {
//        Q = -Q;
//    }
//
//    if(dis_neg_flag) {
//        Q = -Q;
//    }
//
//    return dtf_t(Q);
//}

#ifdef CMSSW_GIT_HASH
}

}
#endif

#endif
