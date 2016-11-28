#include "SimCalorimetry/CaloSimAlgos/interface/CaloHitResponse.h" 
#include "SimDataFormats/CaloHit/interface/PCaloHit.h" 
#include "SimCalorimetry/CaloSimAlgos/interface/CaloVSimParameterMap.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloSimParameters.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloVShape.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloShapes.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloVHitCorrection.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloVHitFilter.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "FWCore/Utilities/interface/isFinite.h"

#include "CLHEP/Random/RandPoissonQ.h"
#include "CLHEP/Units/GlobalPhysicalConstants.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h" 

#include<iostream>
#include<sstream>
#include<algorithm>
#include<iterator>
#include<vector>

CaloHitResponse::CaloHitResponse(const CaloVSimParameterMap * parametersMap, 
                                 const CaloVShape * shape)
: theAnalogSignalMap(),
  theParameterMap(parametersMap), 
  theShapes(0),  
  theShape(shape),  
  theHitCorrection(0),
  thePECorrection(0),
  theHitFilter(0),
  theGeometry(0),
  theMinBunch(-10), 
  theMaxBunch(10),
  thePhaseShift_(1.), nevent(0) {}

CaloHitResponse::CaloHitResponse(const CaloVSimParameterMap * parametersMap,
                                 const CaloShapes * shapes)
: theAnalogSignalMap(),
  theParameterMap(parametersMap),
  theShapes(shapes),
  theShape(0),
  theHitCorrection(0),
  thePECorrection(0),
  theHitFilter(0),
  theGeometry(0),
  theMinBunch(-10),
  theMaxBunch(10),
  thePhaseShift_(1.), nevent(0) {}

CaloHitResponse::~CaloHitResponse() {
}

void CaloHitResponse::setBunchRange(int minBunch, int maxBunch) {
  theMinBunch = minBunch;
  theMaxBunch = maxBunch;
}

void CaloHitResponse::run(MixCollection<PCaloHit> & hits, CLHEP::HepRandomEngine* engine) {

  for(MixCollection<PCaloHit>::MixItr hitItr = hits.begin();
      hitItr != hits.end(); ++hitItr) {
    if(withinBunchRange(hitItr.bunch())) {
      add(*hitItr, engine);
    } // loop over hits
  }
}

void CaloHitResponse::initializeHits() {
  treemap.clear();
}

void CaloHitResponse::finalizeHits(CLHEP::HepRandomEngine* engine) {
  for(const auto& ntup : treemap){
    const HcalHPD2ntuple& item = ntup.second;
	DetId detId(item.id);
	const CaloSamples & result = *(findSignal(detId));
    std::stringstream s_energy          ; std::copy(item.energy         .begin()       ,item.energy         .end()       ,std::ostream_iterator<double>(s_energy         , " "));
    std::stringstream s_photons         ; std::copy(item.photons        .begin()       ,item.photons        .end()       ,std::ostream_iterator<int>(s_photons        , " "));
    std::stringstream s_time            ; std::copy(item.time           .begin()       ,item.time           .end()       ,std::ostream_iterator<double>(s_time           , " "));
    std::stringstream s_tof             ; std::copy(item.tof            .begin()       ,item.tof            .end()       ,std::ostream_iterator<double>(s_tof            , " "));
    std::stringstream s_tzero           ; std::copy(item.tzero          .begin()       ,item.tzero          .end()       ,std::ostream_iterator<double>(s_tzero          , " "));
    std::vector<double> signalTot = {result[0],result[1],result[2],result[3],result[4],result[5],result[6],result[7],result[8],result[9]};
    std::stringstream s_signalTot       ; std::copy(signalTot.begin()             ,signalTot.end()             ,std::ostream_iterator<double>(s_signalTot, " "));
    std::vector<double> signalTotPrecise;
    signalTotPrecise.reserve(result.preciseSize());
    for(int s = 0; s < result.preciseSize(); ++s){
        signalTotPrecise.push_back(result.preciseAt(s));
    }
    std::stringstream s_signalTotPrecise; std::copy(signalTotPrecise.begin()      ,signalTotPrecise.end()      ,std::ostream_iterator<double>(s_signalTotPrecise, " "));
    
    edm::LogInfo("HcalHPD2ntuple") << "HcalHPD2ntuple event"                  << " " << nevent                      << "\n"
                                   << "HcalHPD2ntuple id"                     << " " << item.id                     << "\n"
                                   << "HcalHPD2ntuple subdet"                 << " " << item.subdet                 << "\n"
                                   << "HcalHPD2ntuple ieta"                   << " " << item.ieta                   << "\n"
                                   << "HcalHPD2ntuple iphi"                   << " " << item.iphi                   << "\n"
                                   << "HcalHPD2ntuple depth"                  << " " << item.depth                  << "\n"
                                   << "HcalHPD2ntuple energy"                 << " " << s_energy.str()              << "\n"
                                   << "HcalHPD2ntuple photons"                << " " << s_photons.str()             << "\n"
                                   << "HcalHPD2ntuple time"                   << " " << s_time.str()                << "\n"
                                   << "HcalHPD2ntuple tof"                    << " " << s_tof.str()                 << "\n"
                                   << "HcalHPD2ntuple tzero"                  << " " << s_tzero.str()               << "\n"
                                   << "HcalHPD2ntuple signalTot"              << " " << s_signalTot.str()           << "\n"
                                   << "HcalHPD2ntuple signalTotPrecise"       << " " << s_signalTotPrecise.str()    << "\n";
  }
  ++nevent;
}

void CaloHitResponse::add( const PCaloHit& hit, CLHEP::HepRandomEngine* engine ) {
  // check the hit time makes sense
  if ( edm::isNotFinite(hit.time()) ) { return; }

  // maybe it's not from this subdetector
  if(theHitFilter == 0 || theHitFilter->accepts(hit)) {
    LogDebug("CaloHitResponse") << hit;
    CaloSamples signal( makeAnalogSignal( hit, engine ) ) ;
    bool keep ( keepBlank() ) ;  // here we  check for blank signal if not keeping them
    if( !keep )
    {
       const unsigned int size ( signal.size() ) ;
       if( 0 != size )
       {
	  for( unsigned int i ( 0 ) ; i != size ; ++i )
	  {
	     keep = keep || signal[i] > 1.e-7 ;
	  }
       }
    }

    if( keep ) add(signal);
  }
}


void CaloHitResponse::add(const CaloSamples & signal)
{
  DetId id(signal.id());
  CaloSamples * oldSignal = findSignal(id);
  if (oldSignal == 0) {
    theAnalogSignalMap[id] = signal;

  } else  {
    // need a "+=" to CaloSamples
    int sampleSize =  oldSignal->size();
    assert(sampleSize <= signal.size());
    assert(signal.presamples() == oldSignal->presamples());

    for(int i = 0; i < sampleSize; ++i) {
      (*oldSignal)[i] += signal[i];
    }
  }
}


CaloSamples CaloHitResponse::makeAnalogSignal(const PCaloHit & hit, CLHEP::HepRandomEngine* engine) {

  DetId detId(hit.id());
  const CaloSimParameters & parameters = theParameterMap->simParameters(detId);
  double signal = analogSignalAmplitude(detId, hit.energy(), parameters, engine);

  double time = hit.time();
  //double time = timeOfFlight(detId);
  if(theHitCorrection != 0) {
    time += theHitCorrection->delay(hit, engine);
  }
  double jitter = time - timeOfFlight(detId);

  const CaloVShape * shape = theShape;
  if(!shape) {
    shape = theShapes->shape(detId);
  }
  // assume bins count from zero, go for center of bin
  const double tzero = ( shape->timeToRise()
			 + parameters.timePhase() 
			 - jitter 
			 - BUNCHSPACE*( parameters.binOfMaximum()
					- thePhaseShift_          ) ) ;
  double binTime = tzero;

  CaloSamples result(makeBlankSignal(detId));

  if(detId.det()==DetId::Hcal && (detId.subdetId()==1 || detId.subdetId()==2)){
    result.resetPrecise();
    int sampleBin(0), preciseBin(0);
    double const dt(1.0);
    double const invdt(1./dt);
    for(int bin = 0; bin < 250; bin++) {
      preciseBin = bin;
      sampleBin = preciseBin/25;
      double pulseBit = (*shape)(binTime)* signal;
      result[sampleBin] += pulseBit;
      result.preciseAtMod(preciseBin) += pulseBit*invdt;
      binTime += dt;
    }
  }
  else{
    for(int bin = 0; bin < result.size(); bin++) {
      result[bin] += (*shape)(binTime)* signal;
      binTime += BUNCHSPACE;
    }
  }
  std::vector<double> signalTot = {result[0],result[1],result[2],result[3],result[4],result[5],result[6],result[7],result[8],result[9]};
  std::stringstream s_signalTot      ; std::copy(signalTot.begin()             ,signalTot.end()             ,std::ostream_iterator<double>(s_signalTot, " "));
  std::vector<double> signalTotPrecise;
  signalTotPrecise.reserve(result.preciseSize());
  for(int s = 0; s < result.preciseSize(); ++s){
	  signalTotPrecise.push_back(result.preciseAt(s));
  }
  std::stringstream s_signalTotPrecise; std::copy(signalTotPrecise.begin()      ,signalTotPrecise.end()      ,std::ostream_iterator<double>(s_signalTotPrecise, " "));
  
  if(detId.det()==DetId::Hcal && (detId.subdetId()==1 || detId.subdetId()==2)){
	HcalDetId hid(hit.id());
    edm::LogInfo("HcalHPDntuple") << "HcalHPDntuple event"                  << " " << 0                                      << "\n"
                                  << "HcalHPDntuple id"                     << " " << hid.rawId()                            << "\n"
                                  << "HcalHPDntuple subdet"                 << " " << hid.subdet()                           << "\n"
                                  << "HcalHPDntuple ieta"                   << " " << hid.ieta()                             << "\n"
                                  << "HcalHPDntuple iphi"                   << " " << hid.iphi()                             << "\n"
                                  << "HcalHPDntuple depth"                  << " " << hid.depth()                            << "\n"
                                  << "HcalHPDntuple energy"                 << " " << hit.energy()                           << "\n"
                                  << "HcalHPDntuple photons"                << " " << signal                                 << "\n"
                                  << "HcalHPDntuple time"                   << " " << time                                   << "\n"
                                  << "HcalHPDntuple tof"                    << " " << timeOfFlight(detId)                    << "\n"
                                  << "HcalHPDntuple tzero"                  << " " << tzero                                  << "\n"
                                  << "HcalHPDntuple signalTot"              << " " << s_signalTot.str()                      << "\n"
                                  << "HcalHPDntuple signalTotPrecise"       << " " << s_signalTotPrecise.str()               << "\n";
    HcalHPD2ntuple& ntup = treemap[hid.rawId()];
    ntup.id = hid.rawId();
    ntup.subdet = hid.subdet();
    ntup.ieta  = hid.ieta();
    ntup.iphi  = hid.iphi();
    ntup.depth = hid.depth();
    ntup.energy .push_back(hit.energy());
    ntup.photons.push_back(signal);
    ntup.time   .push_back(time);
    ntup.tof    .push_back(timeOfFlight(detId));
    ntup.tzero  .push_back(tzero);
  }
  return result;
} 

double CaloHitResponse::analogSignalAmplitude(const DetId & detId, float energy, const CaloSimParameters & parameters, CLHEP::HepRandomEngine* engine) const {

  // OK, the "energy" in the hit could be a real energy, deposited energy,
  // or pe count.  This factor converts to photoelectrons
  //GMA Smeared in photon production it self  
  double npe = energy * parameters.simHitToPhotoelectrons(detId);
  // do we need to doPoisson statistics for the photoelectrons?
  if(parameters.doPhotostatistics()) {
    npe = CLHEP::RandPoissonQ::shoot(engine,npe);
  }
  if(thePECorrection) npe = thePECorrection->correctPE(detId, npe, engine);
  return npe;
}


CaloSamples * CaloHitResponse::findSignal(const DetId & detId) {
  CaloSamples * result = 0;
  AnalogSignalMap::iterator signalItr = theAnalogSignalMap.find(detId);
  if(signalItr == theAnalogSignalMap.end()) {
    result = 0;
  } else {
    result = &(signalItr->second);
  }
  return result;
}


CaloSamples CaloHitResponse::makeBlankSignal(const DetId & detId) const {
  const CaloSimParameters & parameters = theParameterMap->simParameters(detId);
  CaloSamples result(detId, parameters.readoutFrameSize());
  result.setPresamples(parameters.binOfMaximum()-1);
  if(detId.det()==DetId::Hcal && (detId.subdetId()==1 || detId.subdetId()==2)){
    int preciseSize(parameters.readoutFrameSize()*25);
    result = CaloSamples(detId,parameters.readoutFrameSize(),preciseSize);
	result.setPresamples(parameters.binOfMaximum()-1);
    result.setPrecise(result.presamples()*25,1.0);
  }
  return result;
}


double CaloHitResponse::timeOfFlight(const DetId & detId) const {
  // not going to assume there's one of these per subdetector.
  // Take the whole CaloGeometry and find the right subdet
  double result = 0.;
  if(theGeometry == 0) {
    edm::LogWarning("CaloHitResponse") << "No Calo Geometry set, so no time of flight correction";
  } 
  else {
    const CaloCellGeometry* cellGeometry = theGeometry->getSubdetectorGeometry(detId)->getGeometry(detId);
    if(cellGeometry == 0) {
       edm::LogWarning("CaloHitResponse") << "No Calo cell found for ID"
         << detId.rawId() << " so no time-of-flight subtraction will be done";
    }
    else {
      double distance = cellGeometry->getPosition().mag();
      result =  distance * cm / c_light; // Units of c_light: mm/ns
    }
  }
  return result;
}


