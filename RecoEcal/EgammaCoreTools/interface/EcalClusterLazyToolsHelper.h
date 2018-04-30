#ifndef RecoEcal_EgammaCoreTools_EcalClusterLazyToolsHelper
#define RecoEcal_EgammaCoreTools_EcalClusterLazyToolsHelper

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ESWatcher.h"

#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "CondFormats/DataRecord/interface/EcalADCToGeVConstantRcd.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

#include <memory>

//allows reusing pointers/handles to event setup info without re-fetching each time (only fetches if watcher signals a change)
template <class Tool> class EcalClusterLazyToolsHelper {
  public:
    EcalClusterLazyToolsHelper() : geometry_(nullptr), topology_(nullptr) {}
    ~EcalClusterLazyToolsHelper() {}

    //accessors
    const Tool& getTools(const edm::Event &ev, const edm::EventSetup &es, edm::EDGetTokenT<EcalRecHitCollection> token1, edm::EDGetTokenT<EcalRecHitCollection> token2)
    {
      checkES(es);
      tool_ = std::make_unique<Tool>(ev,geometry_,topology_,ical_,agc_,laser_,token1,token2);
      return *tool_;
    }

    const Tool& getTools(const edm::Event &ev, const edm::EventSetup &es, edm::EDGetTokenT<EcalRecHitCollection> token1, edm::EDGetTokenT<EcalRecHitCollection> token2, edm::EDGetTokenT<EcalRecHitCollection> token3)
    {
      checkES(es);
      tool_ = std::make_unique<Tool>(ev,geometry_,topology_,ical_,agc_,laser_,token1,token2,token3);
      return *tool_;
    }

  private:
    //internal fns
    void checkES(const edm::EventSetup &es){
      if(geometryWatcher_.check(es)){
        edm::ESHandle<CaloGeometry> pGeometry;
        es.get<CaloGeometryRecord>().get(pGeometry);
        geometry_ = pGeometry.product();
      }
      if(topologyWatcher_.check(es)){
        edm::ESHandle<CaloTopology> pTopology;
        es.get<CaloTopologyRecord>().get(pTopology);
        topology_ = pTopology.product();
      }
      if(icalWatcher_.check(es)){
        es.get<EcalIntercalibConstantsRcd>().get(ical_);
      }
      if(agcWatcher_.check(es)){
        es.get<EcalADCToGeVConstantRcd>().get(agc_);
      }
      if(laserWatcher_.check(es)){
        es.get<EcalLaserDbRecord>().get(laser_);
      }
    }

    //member vars
    const CaloGeometry *geometry_;
    const CaloTopology *topology_;
    edm::ESHandle<EcalIntercalibConstants> ical_;
    edm::ESHandle<EcalADCToGeVConstant>    agc_;
    edm::ESHandle<EcalLaserDbService> laser_;
    std::unique_ptr<Tool> tool_;

    //watchers
    edm::ESWatcher<CaloGeometryRecord> geometryWatcher_;
    edm::ESWatcher<CaloTopologyRecord> topologyWatcher_;
    edm::ESWatcher<EcalIntercalibConstantsRcd> icalWatcher_;
    edm::ESWatcher<EcalADCToGeVConstantRcd> agcWatcher_;
    edm::ESWatcher<EcalLaserDbRecord> laserWatcher_;
};

#endif

