#ifndef HcalLutMetadatum_h
#define HcalLutMetadatum_h

#include "CondFormats/Serialization/interface/Serializable.h"

/*
\class HcalLutMetadatum
\author Gena Kukartsev
contains RecHit calibration, LUT granularity, LUT threshold for one HCAL channel
*/

class HcalLutMetadatum
{
 public:
  HcalLutMetadatum(): mId(0), mRCalib(1.0) {}
    HcalLutMetadatum(unsigned long fid,
		     float rCalib,
		     uint8_t lutGranularity,
		     uint8_t LutThreshold):
      mId(fid), 
      mRCalib(rCalib),
      mLutGranularity(lutGranularity),
      mLutThreshold(LutThreshold) {}

  uint32_t rawId() const {return mId;}

  float   getRCalib() const {return mRCalib;}
  uint8_t getLutGranularity() const {return mLutGranularity;}
  uint8_t getLutThreshold() const {return mLutThreshold;}

 private:
  uint32_t mId;
  float    mRCalib;
  uint8_t  mLutGranularity;
  uint8_t  mLutThreshold;

 COND_SERIALIZABLE;
};

#endif
