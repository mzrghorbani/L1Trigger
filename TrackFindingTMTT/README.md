# Setup instructions to just run

To checkout and run, or if your modifications won't need to be put into the central repository, do:

```
cmsrel CMSSW_10_4_0

cd CMSSW_10_4_0/src

cmsenv

git clone https://gitlab.cern.ch/mghorban1/L1Trigger.git

git checkout <version>

scram setup L1Trigger/TrackFindingTMTT/hls.xml 

scram b -j8

cmsRun L1Trigger/TrackFindingTMTT/test/tmtt_tf_analysis_cfg.py Events=10
```
