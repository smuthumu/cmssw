#ifndef HcalSimAlgos_HcalQIE10Traits_h
#define HcalSimAlgos_HcalQIE10Traits_h

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalElectronicsSim.h"

class HcalQIE10DigitizerTraits {

public:
  typedef QIE10DigiCollection DigiCollection;
  typedef QIE10DataFrame Digi;
  typedef HcalElectronicsSim ElectronicsSim;
};

#endif
