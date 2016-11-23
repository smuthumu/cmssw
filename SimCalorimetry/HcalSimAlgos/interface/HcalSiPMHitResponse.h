// -*- C++ -*- 
#ifndef HcalSimAlgos_HcalSiPMHitResponse_h
#define HcalSimAlgos_HcalSiPMHitResponse_h

#include "SimCalorimetry/CaloSimAlgos/interface/CaloHitResponse.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalTDCParameters.h"

#include <map>
#include <set>
#include <vector>
#include <unordered_map>

class HcalSiPM;

struct HcalSiPMntuple {
	uint32_t id = 0;
	int subdet = 0;
	int ieta = 0;
	int iphi = 0;
	int depth = 0;
	double fCtoGeV = 0.;
	double samplingFactor = 0.;
	double photoelectronsToAnalog = 0.;
	double simhitToPhotoelectrons = 0.;
	std::vector<double> energy;
	std::vector<int> photons;
	std::vector<double> time;
	std::vector<double> tof;
	std::vector<double> tzero;
	std::vector<double> tzero_corrected;
	std::vector<std::vector<double>> t_pe;
	std::vector<std::vector<int>> t_bin;
	int npixels = 0;
	std::vector<double> elapsedTime;
	std::vector<int>    sampleBin;
	std::vector<int>    preciseBin;
	std::vector<int>    pe;
	std::vector<int>    hitPixels;
	std::vector<std::vector<double>> signal;
	std::vector<double> signalTot;
	std::vector<double> signalTotPrecise;
	int sumPE = 0;
	int sumHits = 0;
};

namespace CLHEP {
  class HepRandomEngine;
}

class PCaloHitCompareTimes {
public:
  bool operator()(const PCaloHit * a, 
		  const PCaloHit * b) const {
    return a->time()<b->time();
  }
};

class HcalSiPMHitResponse : public CaloHitResponse {

public:
  HcalSiPMHitResponse(const CaloVSimParameterMap * parameterMap, 
		      const CaloShapes * shapes, bool PreMix1 = false);

  virtual ~HcalSiPMHitResponse();

  typedef std::vector<unsigned int> photonTimeHist;
  typedef std::map< DetId, photonTimeHist > photonTimeMap;

  virtual void initializeHits();

  virtual void finalizeHits(CLHEP::HepRandomEngine*) override;

  virtual void add(const PCaloHit& hit, CLHEP::HepRandomEngine*) override;

  virtual void add(const CaloSamples& signal);

  virtual void addPEnoise(CLHEP::HepRandomEngine* engine);

  virtual CaloSamples makeBlankSignal(const DetId & detId) const;

  virtual void setDetIds(const std::vector<DetId> & detIds);

  static double Y11TimePDF( double t );

  double generatePhotonTime(CLHEP::HepRandomEngine*) const;

protected:
  virtual CaloSamples makeSiPMSignal(DetId const& id, photonTimeHist const& photons, CLHEP::HepRandomEngine*);

private:
  HcalSiPM * theSiPM;
  double theRecoveryTime;
  int const TIMEMULT;
  float const Y11RANGE;
  float const Y11MAX;
  float const Y11TIMETORISE;
  bool PreMixDigis;

  photonTimeMap precisionTimedPhotons;
  HcalTDCParameters theTDCParams;

  const std::vector<DetId>* theDetIds;
  std::unordered_map<uint32_t,HcalSiPMntuple> treemap;
  int nevent;
};

#endif //HcalSimAlgos_HcalSiPMHitResponse_h
