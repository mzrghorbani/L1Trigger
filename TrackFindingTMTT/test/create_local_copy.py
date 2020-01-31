import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("COPY")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))

options = VarParsing.VarParsing ('analysis')

options.register('inputMC', 'L1Trigger/TrackFindingTMTT/test/MCsamples/1040/RelVal/TTbar/PU200.txt',
                 VarParsing.VarParsing.multiplicity.singleton, VarParsing.VarParsing.varType.string, "Files to be processed")

#--- input and output
list = FileUtils.loadListFromFile(options.inputMC)
readFiles = cms.untracked.vstring(*list)

process.source = cms.Source ("PoolSource",
                             fileNames = readFiles,
                             duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
                             inputCommands = cms.untracked.vstring(
                                 'keep *_*_*_*',
                                 'drop l1tEMTFHit2016*_*_*_*',
                                 'drop l1tEMTFTrack2016*_*_*_*'
                             )
                             )

process.copyAll = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string("FFF48AB4-E5E6-3842-8A5B-20E2B7E497BC.root") )

process.out = cms.EndPath(process.copyAll)