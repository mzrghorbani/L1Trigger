The L1Trigger repository contains the algorithm for Track-Finder and added HLS directories to run LRHLS module compatible with CMSSW environment.

Prerequisites:

	Installation of Vivado HLS and adding the VIVADO_PATH environment variable and LD_LIBRARY_PATH to the system.

To run the process:

	source /cvmfs/cms.cern.ch/cmsset_default.sh

	mkdir git_projects
	cd git_projects

	cmsrel CMSSW_10_4_0
	cd CMSSW_10_4_0/src
	git clone https://gitlab.cern.ch/mghorban1/L1Trigger.git -b <version>

	cmsenv
	scram setup L1Trigger/TrackFindingTMTT/hls.xml 
	scram b -j4
	cmsRun L1Trigger/TrackFindingTMTT/test/tmtt_tf_analysis_cfg.py

Note: The L1Trigger repository is in sync with lr_make_hls_ip repository. To create HLS IP-Core for further processing ensure the versions of L1Trigger and lr_make_hls_ip match and kept in a shared directory:  

	cd git_projects
	git clone https://gitlab.cern.ch/mghorban1/L1Trigger.git -b <version>
