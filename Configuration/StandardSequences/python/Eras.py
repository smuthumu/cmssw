import FWCore.ParameterSet.Config as cms
from  FWCore.ParameterSet.Config import ModifierChain,Modifier

class Eras (object):
    """
    Dummy container for all the cms.Modifier instances that config fragments
    can use to selectively configure depending on what scenario is active.
    """
    def __init__(self):
        from Configuration.StandardSequences.ErasList import allEras, internalUseMods, internalUseModChains

        self.pythonCfgLines = {}

        for e in allEras:
            eObj=getattr(__import__('Configuration.Eras.Era_'+e+'_cff',globals(),locals(),[e],0),e)
            self.addEra(e,eObj)
            self.pythonCfgLines[e] = 'from Configuration.Eras.Era_'+e+'_cff import '+e

        for e in internalUseMods:
            eObj=getattr(__import__('Configuration.Eras.Modifier_'+e+'_cff',globals(),locals(),[e],0),e)
            self.addEra(e,eObj)
            self.pythonCfgLines[e] = 'from Configuration.Eras.Modifier_'+e+'_cff import '+e

        for e in internalUseModChains:
            eObj=getattr(__import__('Configuration.Eras.ModifierChain_'+e+'_cff',globals(),locals(),[e],0),e)
            self.addEra(e,eObj)
            self.pythonCfgLines[e] = 'from Configuration.Eras.ModifierChain_'+e+'_cff import '+e


    def addEra(self,name,obj):
        setattr(self,name,obj)

    def inspectModifier(self,m,details):
        print '      ',m.__dict__ ['_Modifier__processModifiers']

    def inspectEra(self,e,details):
        print '\nEra:',e
        print '   isChosen:',getattr(self,e).isChosen()
        if details: print '   Modifiers:'
        nmod=0
        for value in getattr(self,e).__dict__['_ModifierChain__chain']:
            if type(value)==Modifier:
                nmod=nmod+1
                if details: self.inspectModifier(value,details)
        print '   ',nmod,'modifiers defined' 

    def inspect(self,name=None,onlyChosen=False,details=True):
        if name==None:
            print 'Inspecting the known eras',
            if onlyChosen: print ' (all active)'
            else: print '(all eras defined)'
        else:
            print 'Inspecting the '+name+' era',

        allEras=[]
        for key, value in self.__dict__.items():
            if type(value)==ModifierChain: allEras.append(key)

        for e in allEras:
            if name is not None and name==e:
                self.inspectEra(e,details)
            if name is None:
                if not onlyChosen or getattr(self,e).isChosen(): 
                    self.inspectEra(e,details)
        
eras=Eras()


#eras.inspect()
