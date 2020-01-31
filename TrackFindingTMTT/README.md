# Setup instructions to just run

To checkout and run, or if your modifications won't need to be put into the central repository, do:

```
cmsrel CMSSW_10_4_0
cd CMSSW_10_4_0/src
cmsenv

git init
git remote add origin https://gitlab.cern.ch/cms-uk-tracktrigger/software/tmtt.git
git pull origin master

scram b -j 8

cmsRun L1Trigger/TrackFindingTMTT/test/tmtt_tf_analysis_cfg.py Events=10
```
