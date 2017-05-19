#!/usr/bin/env python

import os, glob, sys
import commands, shutil
from optparse import OptionParser

commands.getoutput("make -j 8")


parser = OptionParser()
parser.add_option("-d", "--dir", dest="sample_dir", default="./",
                      help="location of skimmed samples")

parser.add_option("-o", "--outputdir", dest="outputdir", 
		     default="./",
		     help="location where output files will be kept")
parser.add_option("-p", "--process", dest="processsample",
                     default="signal",
                     help="what to process, signal, background, or both")

(options, args) = parser.parse_args()

if options.processsample not in ["signal", "background", "both"]:
   print "please say what to process, options are \
   1. signal \
   2. background\
   3. both"
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
       print "nohup ./runLooper %s/%s %s %s %s %s/log_%s.txt & "%(options.sample_dir,sample,options.outputdir,xsec,nevts,logdir,sample)
       print commands.getoutput("nohup ./runLooper %s/%s %s %s %s %s/log_%s.txt"%(options.sample_dir,sample,options.outputdir,xsec,nevts,logdir,sample))
       sys.exit()

def process_signal(): 
   for afile in files_to_run: 
     print file


