#ifndef __KF_H__
#define __KF_H__


using namespace std;

namespace TMTT {

class Data;
class Settings;
  
class KF {

public:

  KF( const Settings* settings, Data* data );

  ~KF(){}

  void produce();

private:

  const Settings* settings_;
        Data*     data_;

};

}

#endif