#ifndef __SLV_H__
#define __SLV_H__

#include <bitset>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

namespace TMTT {

class SLV {

private:

static constexpr int S = 64;

public:

SLV( const unsigned long long int& value, const int& size ) : signed_( false ), size_( size ), bs_( bitset< S >( value ) ) {}

SLV( const int& value, const int& size, const bool& Signed = false ) :
  signed_( Signed ),
  size_( size ),
  bs_( bitset< S >( ( !Signed || value >= 0 ) ? value : value + pow( 2, size_ ) ) ) {}

SLV( const double& value, const double& base = 1., const int& size = -1, const bool& Signed = false ) : SLV( (int)floor( value / base ), size, Signed ) {}

SLV( const SLV& slv ) : signed_( slv.Signed() ), size_( slv.size() ), bs_( slv.bs_ ) {}

SLV( const string& str, const bool& Signed = false ) : signed_( Signed ), size_( str.size() ), bs_( bitset< S >( str ) ) {}

int  size()   const { return size_;   }
bool Signed() const { return signed_; }

bool                   operator[]( const int& pos ) const { return bs_[ pos ]; }
bitset< S >::reference operator[]( const int& pos )       { return bs_[ pos ]; }

bool all  () const { return bs_.all();   }
bool any  () const { return bs_.any();   }
bool none () const { return bs_.none();  }
int  count() const { return bs_.count(); }

bool operator==( const SLV& rhs ) const { return bs_ == rhs.bs_; }
bool operator!=( const SLV& rhs ) const { return bs_ != rhs.bs_; }

SLV& operator&=( const SLV& rhs ) { const int m( max( size_, rhs.size() ) ); this->resize( m ); SLV slv( rhs ); slv.resize( m ); bs_ &= slv.bs_; return *this; }
SLV& operator|=( const SLV& rhs ) { const int m( max( size_, rhs.size() ) ); this->resize( m ); SLV slv( rhs ); slv.resize( m ); bs_ |= slv.bs_; return *this; }
SLV& operator^=( const SLV& rhs ) { const int m( max( size_, rhs.size() ) ); this->resize( m ); SLV slv( rhs ); slv.resize( m ); bs_ ^= slv.bs_; return *this; }

SLV  operator~() const { SLV slv( *this ); return slv.flip(); }

SLV& operator>>=( const int& pos ) { bs_ >>= pos; size_ -= pos; return *this; }
SLV& operator<<=( const int& pos ) { bs_ <<= S - size_ + pos; bs_ >>= S - size_ + pos; size_ -= pos; return *this; }

SLV  operator<<( const int& pos ) const { SLV slv( *this ); return slv >>= pos; }
SLV  operator>>( const int& pos ) const { SLV slv( *this ); return slv <<= pos; }

SLV& reset() { bs_.reset(); return *this; }
SLV& set()   { for ( int n = 0; n < size_; n++ ) bs_.set  ( n ); return *this; }
SLV& flip()  { for ( int n = 0; n < size_; n++ ) bs_.flip ( n ); return *this; }

SLV& set  ( const int& pos ) { bs_.set  ( pos ); return *this; }
SLV& reset( const int& pos ) { bs_.reset( pos ); return *this; }
SLV& flip ( const int& pos ) { bs_.flip ( pos ); return *this; }

string str() const { return bs_.to_string().substr( S - size_, S ); }

string str( const int& start, const int& end = 0 ) const { return this->str().substr( size_ - start, size_ - end ); }

int val() const { return ( signed_ && this->msb() ) ? (int)bs_.to_ullong() - pow( 2, size_ ) : bs_.to_ullong(); }

int val( const int& start, const int& end = 0, const bool& Signed = false ) const { return SLV( this->str( start, end ), Signed ).val(); }

double val( const double& base ) const { return ( this->val() + .5 ) * base; }

SLV& operator +=( const SLV& rhs ) { bs_ <<= rhs.size(); bs_ |= rhs.bs_; size_ += rhs.size(); return *this; }

bool                   msb() const { return bs_[ size_-1 ]; }
bitset< S >::reference msb()       { return bs_[ size_-1 ]; }

SLV& operator++() { bs_ = bitset< S >( bs_.to_ullong() + 1 ); this->resize( size_ ); return *this; }

SLV& abs() { if ( signed_ ) { signed_ = false; if ( this->msb() ) { this->flip(); this->operator++(); } size_--; } return *this; }

SLV& resize( const int& size ) {

  bool msb = this->msb();
  if ( size > size_ ) {
    if ( signed_ )
      for ( int n = size_; n < size; n++ )
        bs_.set( n, msb );
    size_ = size;
  } else if ( size < size_ && size > 0 ) {
    this->operator <<=( size - size_ );
    if ( signed_ )
      this->msb() = msb;
  }
  return *this;

}

int plEncode( const bool& b = true ) const { for ( int e = 0; e < size_; e++ ) if ( bs_[ e ] == b ) return e; return size_; }
int pmEncode( const bool& b = true ) const { for ( int e = size_ - 1; e > -1; e-- ) if ( bs_[ e ] == b ) return e; return size_; }

private:

  bool        signed_;
  int         size_;
  bitset< S > bs_;

};

}

#endif