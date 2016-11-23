#include "SimCalorimetry/HcalSimAlgos/interface/HcalSiPMHitResponse.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalSiPM.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalSimParameters.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalShapes.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloVHitFilter.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloVSimParameterMap.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloSimParameters.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloVHitCorrection.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloVShape.h"
#include "FWCore/Utilities/interface/isFinite.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalSiPMShape.h"

#include "CLHEP/Random/RandPoissonQ.h"
#include "CLHEP/Random/RandFlat.h"

#include <math.h>
#include <list>
#include <algorithm>
#include <iterator>
#include <sstream>

HcalSiPMHitResponse::HcalSiPMHitResponse(const CaloVSimParameterMap * parameterMap,
					 const CaloShapes * shapes, bool PreMix1) :
  CaloHitResponse(parameterMap, shapes), theSiPM(), theRecoveryTime(250.), 
  TIMEMULT(1), Y11RANGE(80.), Y11MAX(0.04), Y11TIMETORISE(16.65), PreMixDigis(PreMix1) {
  theSiPM = new HcalSiPM(2500);
  
  nevent = 0;
}

HcalSiPMHitResponse::~HcalSiPMHitResponse() {
  if (theSiPM)
    delete theSiPM;
}

void HcalSiPMHitResponse::initializeHits() {
  precisionTimedPhotons.clear();
  treemap.clear();
}

void HcalSiPMHitResponse::finalizeHits(CLHEP::HepRandomEngine* engine) {
  //do not add PE noise for initial premix
  if(!PreMixDigis) addPEnoise(engine);

  photonTimeMap::iterator channelPhotons;
  for (channelPhotons = precisionTimedPhotons.begin();
       channelPhotons != precisionTimedPhotons.end();
       ++channelPhotons) {
    CaloSamples signal(makeSiPMSignal(channelPhotons->first, 
                                      channelPhotons->second,
                                      engine));
    bool keep( keepBlank() );
    if (!keep) {
      const unsigned int size ( signal.size() ) ;
      if( 0 != size ) {
        for( unsigned int i ( 0 ) ; i != size ; ++i ) {
          keep = keep || signal[i] > 1.e-7 ;
        }
      }
    }

    LogDebug("HcalSiPMHitResponse") << HcalDetId(signal.id()) << ' ' << signal;

    if (keep) add(signal);
  }
  
  for(const auto& ntup : treemap){
    const HcalSiPMntuple& item = ntup.second;
    std::stringstream s_energy          ; std::copy(item.energy         .begin()       ,item.energy         .end()       ,std::ostream_iterator<double>(s_energy         , " "));
    std::stringstream s_photons         ; std::copy(item.photons        .begin()       ,item.photons        .end()       ,std::ostream_iterator<int>(s_photons        , " "));
    std::stringstream s_time            ; std::copy(item.time           .begin()       ,item.time           .end()       ,std::ostream_iterator<double>(s_time           , " "));
    std::stringstream s_tof             ; std::copy(item.tof            .begin()       ,item.tof            .end()       ,std::ostream_iterator<double>(s_tof            , " "));
    std::stringstream s_tzero           ; std::copy(item.tzero          .begin()       ,item.tzero          .end()       ,std::ostream_iterator<double>(s_tzero          , " "));
    std::stringstream s_tzero_corrected ; std::copy(item.tzero_corrected.begin()       ,item.tzero_corrected.end()       ,std::ostream_iterator<double>(s_tzero_corrected, " "));
    std::stringstream s_t_pe            ; for(const auto& it_pe : item.t_pe) std::copy(it_pe.begin(),it_pe.end(),std::ostream_iterator<double>(s_t_pe, " "));
    std::stringstream s_t_bin           ; for(const auto& it_bin : item.t_bin) std::copy(it_bin.begin(),it_bin.end(),std::ostream_iterator<int>(s_t_bin, " "));
    std::stringstream s_elapsedTime     ; std::copy(item.elapsedTime.begin()           ,item.elapsedTime.end()           ,std::ostream_iterator<double>(s_elapsedTime, " "));
    std::stringstream s_sampleBin       ; std::copy(item.sampleBin.begin()             ,item.sampleBin.end()             ,std::ostream_iterator<int>(s_sampleBin, " "));
    std::stringstream s_preciseBin      ; std::copy(item.preciseBin.begin()            ,item.preciseBin.end()            ,std::ostream_iterator<int>(s_preciseBin, " "));
    std::stringstream s_pe              ; std::copy(item.pe.begin()                    ,item.pe.end()                    ,std::ostream_iterator<int>(s_pe, " "));
    std::stringstream s_hitPixels       ; std::copy(item.hitPixels.begin()             ,item.hitPixels.end()             ,std::ostream_iterator<int>(s_hitPixels, " "));
    std::stringstream s_signal          ; for(const auto& isignal : item.signal) std::copy(isignal.begin(),isignal.end(),std::ostream_iterator<double>(s_signal, " "));
    std::stringstream s_signalTot       ; std::copy(item.signalTot.begin()             ,item.signalTot.end()             ,std::ostream_iterator<double>(s_signalTot, " "));
    std::stringstream s_signalTotPrecise; std::copy(item.signalTotPrecise.begin()      ,item.signalTotPrecise.end()      ,std::ostream_iterator<double>(s_signalTotPrecise, " "));
    
    edm::LogInfo("HcalSiPMntuple") << "HcalSiPMntuple event"                  << " " << nevent                      << "\n"
                                   << "HcalSiPMntuple id"                     << " " << item.id                     << "\n"
                                   << "HcalSiPMntuple subdet"                 << " " << item.subdet                 << "\n"
                                   << "HcalSiPMntuple ieta"                   << " " << item.ieta                   << "\n"
                                   << "HcalSiPMntuple iphi"                   << " " << item.iphi                   << "\n"
                                   << "HcalSiPMntuple depth"                  << " " << item.depth                  << "\n"
                                   << "HcalSiPMntuple fCtoGeV"                << " " << item.fCtoGeV                << "\n"
                                   << "HcalSiPMntuple samplingFactor"         << " " << item.samplingFactor         << "\n"
                                   << "HcalSiPMntuple photoelectronsToAnalog" << " " << item.photoelectronsToAnalog << "\n"
                                   << "HcalSiPMntuple simhitToPhotoelectrons" << " " << item.simhitToPhotoelectrons << "\n"
                                   << "HcalSiPMntuple energy"                 << " " << s_energy.str()              << "\n"
                                   << "HcalSiPMntuple photons"                << " " << s_photons.str()             << "\n"
                                   << "HcalSiPMntuple time"                   << " " << s_time.str()                << "\n"
                                   << "HcalSiPMntuple tof"                    << " " << s_tof.str()                 << "\n"
                                   << "HcalSiPMntuple tzero"                  << " " << s_tzero.str()               << "\n"
                                   << "HcalSiPMntuple tzero_corrected"        << " " << s_tzero_corrected.str()     << "\n"
                                   << "HcalSiPMntuple t_pe"                   << " " << s_t_pe.str()                << "\n"
                                   << "HcalSiPMntuple t_bin"                  << " " << s_t_bin.str()               << "\n"
                                   << "HcalSiPMntuple elapsedTime"            << " " << s_elapsedTime.str()         << "\n"
                                   << "HcalSiPMntuple sampleBin"              << " " << s_sampleBin.str()           << "\n"
                                   << "HcalSiPMntuple preciseBin"             << " " << s_preciseBin.str()          << "\n"
                                   << "HcalSiPMntuple pe"                     << " " << s_pe.str()                  << "\n"
                                   << "HcalSiPMntuple hitPixels"              << " " << s_hitPixels.str()           << "\n"
                                   << "HcalSiPMntuple signal"                 << " " << s_signal.str()              << "\n"
                                   << "HcalSiPMntuple signalTot"              << " " << s_signalTot.str()           << "\n"
                                   << "HcalSiPMntuple signalTotPrecise"       << " " << s_signalTotPrecise.str()    << "\n"
                                   << "HcalSiPMntuple sumPE"                  << " " << item.sumPE                  << "\n" 
                                   << "HcalSiPMntuple sumHits"                << " " << item.sumHits                << "\n"; 
  }
  ++nevent;
}

void HcalSiPMHitResponse::add(const CaloSamples& signal) {
  DetId id(signal.id());
  CaloSamples * oldSignal = findSignal(id);
  if (oldSignal == 0) {
    theAnalogSignalMap[id] = signal;
  } else {
    (*oldSignal) += signal;
  }
}

void HcalSiPMHitResponse::add(const PCaloHit& hit, CLHEP::HepRandomEngine* engine) {
    if (!edm::isNotFinite(hit.time()) &&
        ((theHitFilter == 0) || (theHitFilter->accepts(hit)))) {
      HcalDetId id(hit.id());
      const HcalSimParameters& pars = dynamic_cast<const HcalSimParameters&>(theParameterMap->simParameters(id));
      //divide out mean of crosstalk distribution 1/(1-lambda) = multiply by (1-lambda)
      double signal(analogSignalAmplitude(id, hit.energy(), pars, engine)*(1-pars.sipmCrossTalk(id)));
      unsigned int photons(signal + 0.5);
      double time( hit.time() );
      //double time( timeOfFlight(id) );

      if (photons > 0)
        if (precisionTimedPhotons.find(id)==precisionTimedPhotons.end()) {
          precisionTimedPhotons.insert(
            std::pair<DetId, photonTimeHist >(id, 
              photonTimeHist(theTDCParams.nbins() * TIMEMULT *
                             pars.readoutFrameSize(), 0)
                                              )
                                       );
        }

      HcalSiPMntuple& ntup = treemap[id.rawId()];
      ntup.id = id.rawId();
      ntup.subdet = id.subdet();
      ntup.ieta  = id.ieta();
      ntup.iphi  = id.iphi();
      ntup.depth = id.depth();
      ntup.fCtoGeV                = pars.fCtoGeV(id);
      ntup.samplingFactor         = pars.samplingFactor(id);
      ntup.photoelectronsToAnalog = pars.photoelectronsToAnalog(id);
      ntup.simhitToPhotoelectrons = pars.simHitToPhotoelectrons(id);
      ntup.energy .push_back(hit.energy());
      ntup.photons.push_back(photons);
      ntup.time   .push_back(time);
      ntup.tof    .push_back(timeOfFlight(id));
      LogDebug("HcalSiPMHitResponse") << id;
      LogDebug("HcalSiPMHitResponse") << " fCtoGeV: " << pars.fCtoGeV(id)
                << " samplingFactor: " << pars.samplingFactor(id)
                << " photoelectronsToAnalog: " << pars.photoelectronsToAnalog(id)
                << " simHitToPhotoelectrons: " << pars.simHitToPhotoelectrons(id);
      LogDebug("HcalSiPMHitResponse") << " energy: " << hit.energy()
                << " photons: " << photons 
                << " time: " << time;
      LogDebug("HcalSiPMHitResponse") << " timePhase: " << pars.timePhase()
                << " tof: " << timeOfFlight(id)
                << " binOfMaximum: " << pars.binOfMaximum()
                << " phaseShift: " << thePhaseShift_;
      double tzero(0.0*Y11TIMETORISE + pars.timePhase() - 
                   (time - timeOfFlight(id)) - 
                   BUNCHSPACE*( pars.binOfMaximum() - thePhaseShift_));
      LogDebug("HcalSiPMHitResponse") << " tzero: " << tzero;
      ntup.tzero.push_back(tzero);
      double tzero_bin(-tzero/(theTDCParams.deltaT()/TIMEMULT));
      LogDebug("HcalSiPMHitResponse") << " corrected tzero: " << tzero_bin << '\n';
      ntup.tzero_corrected.push_back(tzero_bin);
      double t_pe(0.);
      int t_bin(0);
      std::vector<double> vt_pe;
      std::vector<int> vt_bin;
      for (unsigned int pe(0); pe<photons; ++pe) {
        t_pe = generatePhotonTime(engine);
        t_bin = int(t_pe/(theTDCParams.deltaT()/TIMEMULT)  + tzero_bin + 0.5);
        LogDebug("HcalSiPMHitResponse") << "t_pe: " << t_pe << " t_pe + tzero: " << (t_pe+tzero_bin*(theTDCParams.deltaT()/TIMEMULT) )
                  << " t_bin: " << t_bin << '\n';
        vt_pe.push_back(t_pe);
        vt_bin.push_back(t_bin);
        if ((t_bin >= 0) && 
            (static_cast<unsigned int>(t_bin) < precisionTimedPhotons[id].size()))
            precisionTimedPhotons[id][t_bin] += 1;
      }
      ntup.t_pe.push_back(vt_pe);
      ntup.t_bin.push_back(vt_bin);
    }
}

void HcalSiPMHitResponse::addPEnoise(CLHEP::HepRandomEngine* engine)
{
  double const dt(theTDCParams.deltaT()/TIMEMULT);

  // Add SiPM dark current noise to all cells
  for(std::vector<DetId>::const_iterator idItr = theDetIds->begin();
      idItr != theDetIds->end(); ++idItr) {
    HcalDetId id(*idItr);
    const HcalSimParameters& pars =
      static_cast<const HcalSimParameters&>(theParameterMap->simParameters(id));

    // uA * ns / (fC/pe) = pe!
    double dc_pe_avg =
      pars.sipmDarkCurrentuA(id) * dt / 
      pars.photoelectronsToAnalog(id);

    if (dc_pe_avg <= 0.) continue;

    int nPreciseBins = theTDCParams.nbins() * TIMEMULT * pars.readoutFrameSize();

    unsigned int sumnoisePE(0);
    double  elapsedTime(0.);
    for (int tprecise(0); tprecise < nPreciseBins; ++tprecise) {
      int noisepe = CLHEP::RandPoissonQ::shoot(engine, dc_pe_avg); // add dark current noise

      if (noisepe > 0) {
        if (precisionTimedPhotons.find(id)==precisionTimedPhotons.end()) {
          photonTimeHist photons(nPreciseBins, 0);
          photons[tprecise] = noisepe;
          precisionTimedPhotons.insert
            (std::pair<DetId, photonTimeHist >(id, photons ) );
        } else {
          precisionTimedPhotons[id][tprecise] += noisepe;
        }

        sumnoisePE += noisepe;
      }
      elapsedTime += dt;

    } // precise time loop

    LogDebug("HcalSiPMHitResponse") << id;
    LogDebug("HcalSiPMHitResponse") << " total noise (PEs): " << sumnoisePE;

  } // detId loop
}                                               // HcalSiPMHitResponse::addPEnoise()

CaloSamples HcalSiPMHitResponse::makeBlankSignal(const DetId& detId) const {
  const CaloSimParameters & parameters = theParameterMap->simParameters(detId);
  int preciseSize(parameters.readoutFrameSize()*theTDCParams.nbins());
  CaloSamples result(detId, parameters.readoutFrameSize(), preciseSize);
  result.setPresamples(parameters.binOfMaximum()-1);
  result.setPrecise(result.presamples()*theTDCParams.nbins(), 
                    theTDCParams.deltaT());
  return result;
}

CaloSamples HcalSiPMHitResponse::makeSiPMSignal(DetId const& id, 
                                                photonTimeHist const& photonTimeBins,
                                                CLHEP::HepRandomEngine* engine) {
  const HcalSimParameters& pars = static_cast<const HcalSimParameters&>(theParameterMap->simParameters(id));  
  theSiPM->setNCells(pars.pixels(id));
  theSiPM->setTau(pars.sipmTau());
  theSiPM->setCrossTalk(pars.sipmCrossTalk(id));
  theSiPM->setSaturationPars(pars.sipmNonlinearity(id));

  //use to make signal
  CaloSamples signal( makeBlankSignal(id) );
  double const dt(theTDCParams.deltaT()/TIMEMULT);
  double const invdt(1./theTDCParams.deltaT());
  int sampleBin(0), preciseBin(0);
  signal.resetPrecise();
  unsigned int pe(0);
  double hitPixels(0.), elapsedTime(0.);
  unsigned int sumPE(0);
  double sumHits(0.);

  HcalSiPMShape sipmPulseShape(pars.signalShape(id));

  std::list< std::pair<double, double> > pulses;
  std::list< std::pair<double, double> >::iterator pulse;
  double timeDiff, pulseBit;
  LogDebug("HcalSiPMHitResponse") << "makeSiPMSignal for " << HcalDetId(id);

  HcalSiPMntuple& ntup = treemap[id.rawId()];
  ntup.npixels = theSiPM->getNCells();
  for (unsigned int tbin(0); tbin < photonTimeBins.size(); ++tbin) {
    CaloSamples signaltmp( makeBlankSignal(id) );
    signaltmp.resetPrecise();
    pe = photonTimeBins[tbin];
    sumPE += pe;
    preciseBin = tbin/TIMEMULT;
    sampleBin = preciseBin/theTDCParams.nbins();
    if (pe > 0) {
      hitPixels = theSiPM->hitCells(engine, pe, 0., elapsedTime);
      sumHits += hitPixels;
      LogDebug("HcalSiPMHitResponse") << " elapsedTime: " << elapsedTime
                                      << " sampleBin: " << sampleBin
                                      << " preciseBin: " << preciseBin
                                      << " pe: " << pe 
                                      << " hitPixels: " << hitPixels ;
      ntup.elapsedTime.push_back(elapsedTime);
      ntup.sampleBin.push_back(sampleBin);
      ntup.preciseBin.push_back(preciseBin);
      ntup.pe.push_back(pe);
      ntup.hitPixels.push_back(hitPixels);
      if (pars.doSiPMSmearing()) {
        pulses.push_back( std::pair<double, double>(elapsedTime, hitPixels) );
      } else {
        signal[sampleBin] += hitPixels;
        signaltmp[sampleBin] += hitPixels;
        hitPixels *= invdt;
        signal.preciseAtMod(preciseBin) += 0.6*hitPixels;
        signaltmp.preciseAtMod(preciseBin) += 0.6*hitPixels;
        if (preciseBin > 0)
          signal.preciseAtMod(preciseBin-1) += 0.2*hitPixels;
          signaltmp.preciseAtMod(preciseBin-1) += 0.2*hitPixels;
        if (preciseBin < signal.preciseSize() -1)
          signal.preciseAtMod(preciseBin+1) += 0.2*hitPixels;
          signaltmp.preciseAtMod(preciseBin+1) += 0.2*hitPixels;
      }
    
      if (pars.doSiPMSmearing()) {
        pulse = pulses.begin();
        while (pulse != pulses.end()) {
          timeDiff = elapsedTime - pulse->first;
          pulseBit = sipmPulseShape(timeDiff)*pulse->second;
          LogDebug("HcalSiPMHitResponse") << " pulse t: " << pulse->first 
                                          << " pulse A: " << pulse->second
                                          << " timeDiff: " << timeDiff
                                          << " pulseBit: " << pulseBit;
          signal[sampleBin] += pulseBit;
          signaltmp[sampleBin] += pulseBit;
          signal.preciseAtMod(preciseBin) += pulseBit*invdt;
          signaltmp.preciseAtMod(preciseBin) += pulseBit*invdt;
      
          if (timeDiff > 1 && sipmPulseShape(timeDiff) < 1e-6)
            pulse = pulses.erase(pulse);
          else
            ++pulse;
        }
      }

      ntup.signal.push_back({signaltmp[0],signaltmp[1],signaltmp[2],signaltmp[3],signaltmp[4],signaltmp[5],signaltmp[6],signaltmp[7],signaltmp[8],signaltmp[9]});
    }
    
    elapsedTime += dt;
  }
  ntup.sumPE = sumPE;
  ntup.sumHits = sumHits;
  ntup.signalTot = {signal[0],signal[1],signal[2],signal[3],signal[4],signal[5],signal[6],signal[7],signal[8],signal[9]};
  ntup.signalTotPrecise.reserve(signal.preciseSize());
  for(int s = 0; s < signal.preciseSize(); ++s){
	  ntup.signalTotPrecise.push_back(signal.preciseAt(s));
  }

  return signal;
}

double HcalSiPMHitResponse::generatePhotonTime(CLHEP::HepRandomEngine* engine) const {
  double result(0.);
  while (true) {
    result = CLHEP::RandFlat::shoot(engine, Y11RANGE);
    if (CLHEP::RandFlat::shoot(engine, Y11MAX) < Y11TimePDF(result))
      return result;
  }
}

double HcalSiPMHitResponse::Y11TimePDF(double t) {
  return exp(-0.0635-0.1518*t)*pow(t, 2.528)/2485.9;
}

void HcalSiPMHitResponse::setDetIds(const std::vector<DetId> & detIds) {
  theDetIds = &detIds;
}
