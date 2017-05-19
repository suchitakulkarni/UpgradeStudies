#!/usr/bin/env python

import os, glob, sys
import commands, shutil
from optparse import OptionParser
#import ROOT

commands.getoutput("make -j 8")


parser = OptionParser()
parser.add_option("-d", "--dir", dest="sample_dir", default="./",
                      help="location of skimmed samples")

parser.add_option("-o", "--outputdir", dest="outputdir", 
		     default="./",
		     help="location where output files will be kept")
parser.add_option("-p", "--process", dest="processsample",
                     default="signal",
                     help="what to process, signal, background")
parser.add_option("-n", "--nevts", dest="nevts", 
                     default="100",
                     help="number of events of signal sample, takes only one value right now")
parser.add_option("-s", "--xsec", dest="xsec", 
                     default="1",
                     help="cross section of signal sample, takes only one value right now")
(options, args) = parser.parse_args()

if options.processsample not in ["signal", "background"]:
   print "please say what to process, options are \
   1. signal \
   2. background"
if not os.path.exists(options.outputdir): os.mkdir(options.outputdir)

currentdir = os.getcwd()
files_to_run = [y for x in os.walk(options.sample_dir) for y in glob.glob(os.path.join(x[0], '*.root'))]

logdir = options.outputdir+"/logs/"

if not os.path.exists(logdir): os.mkdir(logdir)

def process_background():
  for afile in files_to_run:
     print afile
     config_file = "/".join(afile.split("/")[:-1])+"/skimmed_configSkimmer.txt"
     print config_file
     startread = False
     if not os.path.exists(config_file): 
       print "config file not found"
       sys.exit()
     xseclines = open(config_file).read().split('\n')
     for line in xseclines:
       if '[inputfiles-begin]' in line: 
        startread = True
        continue
       if '[inputfiles-end]' in line:
        startread = False
        continue
     print line
     if "outputfile.root" in line: continue
     if startread == True:
       xsec =  line.split(",")[-4]
       nevts = line.split(",")[-3]
       sample = line.split(",")[0]
       print sample, xsec, nevts
       print "nohup ./runLooper %s %s %s %s %s & "%(options.sample_dir,sample,options.outputdir,xsec,nevts)
       print commands.getoutput("nohup ./runLooper %s %s %s %s %s"%(options.sample_dir,sample,options.outputdir,xsec,nevts))
       sys.exit()

def process_signal(): 
   for afile in files_to_run:
        print afile
	#rootfile=ROOT.TFile(afile,"READ")
        #tree=rootfile.Get("Delphes")
        '''if not exists(tree,treeName,debug):
            print "Tree",treeName,"does not exist, so will abort"
            return False'''
 	# Create object of class ExRootTreeReader
	#nevts = tree.GetEntries()
	#print nevts
        print "nohup ./runLooper %s %s %s %s %s"%(options.sample_dir,afile.split("/")[-1].replace(".root",""),options.outputdir, options.xsec,str(11000))
        print commands.getoutput("nohup ./runLooper %s %s %s %s %s"%(options.sample_dir,afile.split("/")[-1].replace(".root",""),options.outputdir,options.xsec,str(11000)))
	sys.exit()

if __name__ == "__main__":
   if options.processsample == "signal": process_signal()
   if options.processsample == "background": process_background()
   if options.processsample == "both": 
      process_background()
      process_signal()

