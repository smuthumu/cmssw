#include "SimCalorimetry/HcalSimAlgos/interface/HcalShapes.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "SimCalorimetry/CaloSimAlgos/interface/CaloCachedShapeIntegrator.h"
#include "CondFormats/HcalObjects/interface/HcalMCParam.h"
#include "CondFormats/HcalObjects/interface/HcalMCParams.h"
#include "CondFormats/DataRecord/interface/HcalMCParamsRcd.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"

HcalShapes::HcalShapes()
: theMCParams(0),
  theShapes(),
  theShapesPrecise(),
  theZDCShape(),
  theHcalShape101(),
  theHcalShape102(),
  theHcalShape103(),
  theHcalShape104(),
  theHcalShape105(),
  theHcalShape123(),
  theHcalShape124(),
  theHcalShape125(),
  theHcalShape201(),
  theHcalShape202(),
  theHcalShape203(),
  theHcalShape301(),
  theHcalShape401()
 {
/*
         00 - not used (reserved)
        101 - regular HPD  HB/HE/HO shape
        102 - "special" HB HPD#14 long shape
        201 - SiPMs Zecotec shape   (HO)
        202 - SiPMs Hamamatsu shape (HO)
        203 - SiPMs Hamamatsu shape (HE 2017)
        301 - regular HF PMT shape
        401 - regular ZDC shape
  */

  theHcalShape101.setShape(101); 
  theShapesPrecise[101] = &theHcalShape101;
  theShapes[101] = new CaloCachedShapeIntegrator(&theHcalShape101);
  theHcalShape102.setShape(102);                  
  theShapesPrecise[102] = &theHcalShape102;
  theShapes[102] = new CaloCachedShapeIntegrator(&theHcalShape102);
  theHcalShape103.setShape(103);                  
  theShapesPrecise[103] = &theHcalShape103;
  theShapes[103] = new CaloCachedShapeIntegrator(&theHcalShape103);
  theHcalShape104.setShape(104);                  
  theShapesPrecise[104] = &theHcalShape104;
  theShapes[104] = new CaloCachedShapeIntegrator(&theHcalShape104);
  theHcalShape105.setShape(105);
  theShapesPrecise[105] = &theHcalShape105;
  theShapes[105] = new CaloCachedShapeIntegrator(&theHcalShape105); // HPD new 
  theHcalShape123.setShape(123);                  
  theShapesPrecise[123] = &theHcalShape123;
  theShapes[123] = new CaloCachedShapeIntegrator(&theHcalShape123);
  theHcalShape124.setShape(124);                  
  theShapesPrecise[124] = &theHcalShape124;
  theShapes[124] = new CaloCachedShapeIntegrator(&theHcalShape124);
  theHcalShape125.setShape(125);
  theShapesPrecise[125] = &theHcalShape125;
  theShapes[125] = new CaloCachedShapeIntegrator(&theHcalShape125);
  theHcalShape201.setShape(201);                  
  theShapesPrecise[201] = &theHcalShape201;
  theShapes[201] = new CaloCachedShapeIntegrator(&theHcalShape201);
  theHcalShape202.setShape(202);                  
  theShapesPrecise[202] = &theHcalShape202;
  theShapes[202] = new CaloCachedShapeIntegrator(&theHcalShape202);
  theHcalShape203.setShape(203);
  theShapesPrecise[203] = &theHcalShape203;
  theShapes[203] = new CaloCachedShapeIntegrator(&theHcalShape203);
  theHcalShape301.setShape(301);
  theShapesPrecise[301] = &theHcalShape301;
  theShapes[301] = new CaloCachedShapeIntegrator(&theHcalShape301);
  //    ZDC not yet defined in CalibCalorimetry/HcalAlgos/src/HcalPulseShapes.cc
  // theHcalShape401(401);
  // theShapes[401] = new CaloCachedShapeIntegrator(&theHcalShape401);
  theShapesPrecise[ZDC] = &theZDCShape;
  theShapes[ZDC] = new CaloCachedShapeIntegrator(&theZDCShape);

  theMCParams=0;
  theTopology=0;
}


HcalShapes::~HcalShapes()
{
  for(ShapeMap::const_iterator shapeItr = theShapes.begin();
      shapeItr != theShapes.end();  ++shapeItr)
  {
    delete shapeItr->second;
  }
  theShapes.clear();
  if (theMCParams!=0) delete theMCParams;
  if (theTopology!=0) delete theTopology;
}


void HcalShapes::beginRun(edm::EventSetup const & es)
{
  edm::ESHandle<HcalMCParams> p;
  es.get<HcalMCParamsRcd>().get(p);
  theMCParams = new HcalMCParams(*p.product()); 

// here we are making a _copy_ so we need to add a copy of the topology...
  
  edm::ESHandle<HcalTopology> htopo;
  es.get<HcalRecNumberingRecord>().get(htopo);
  theTopology=new HcalTopology(*htopo);
  theMCParams->setTopo(theTopology);
}


void HcalShapes::endRun()
{
  if (theMCParams) delete theMCParams;
  theMCParams = 0;
  if (theTopology) delete theTopology;
  theTopology = 0;
}


const CaloVShape * HcalShapes::shape(const DetId & detId, bool precise) const
{
  if(!theMCParams) {
    return defaultShape(detId);
  }
  int shapeType = theMCParams->getValues(detId)->signalShape();
  const auto& myShapes = getShapeMap(precise);
  ShapeMap::const_iterator shapeMapItr = myShapes.find(shapeType);
  if(shapeMapItr == myShapes.end()) {
       edm::LogWarning("HcalShapes") << "HcalShapes::shape - shapeType ?  = "
				     << shapeType << std::endl;
    return defaultShape(detId,precise);
  } else {
    return shapeMapItr->second;
  }
}

const CaloVShape * HcalShapes::defaultShape(const DetId & detId, bool precise) const
{
  // try to figure the appropriate shape
  const CaloVShape * result;
  const auto& myShapes = getShapeMap(precise);
  HcalGenericDetId::HcalGenericSubdetector subdet 
    = HcalGenericDetId(detId).genericSubdet();
  if(subdet == HcalGenericDetId::HcalGenBarrel 
  || subdet == HcalGenericDetId::HcalGenEndcap) result = myShapes.find(HPD)->second;
  else if(subdet == HcalGenericDetId::HcalGenOuter) result = myShapes.find(HPD)->second;
  else if(subdet == HcalGenericDetId::HcalGenForward) result = myShapes.find(HF)->second;
  else if(subdet == HcalGenericDetId::HcalGenZDC) result = myShapes.find(ZDC)->second;
  else result = 0;

  edm::LogWarning("HcalShapes") << "Cannot find HCAL MC Params, so the default one is taken for subdet " << subdet;  

  return result;
}

const HcalShapes::ShapeMap& HcalShapes::getShapeMap(bool precise) const {
  return precise ? theShapesPrecise : theShapes;
}
