#! /usr/bin/env python


#############################################################################
##                                                                         ##
## sf.py --- C to shellcode conversion programm                            ##
##              see http://www.secdev.org/projects/shellforge/             ##
##              for more informations                                      ##
##                                                                         ##
## Copyright (C) 2003  Philippe Biondi <phil@secdev.org>                   ##
##                                                                         ##
## This program is free software; you can redistribute it and/or modify it ##
## under the terms of the GNU General Public License version 2 as          ##
## published by the Free Software Foundation.                              ##
##                                                                         ##
## This program is distributed in the hope that it will be useful, but     ##
## WITHOUT ANY WARRANTY; without even the implied warranty of              ##
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       ##
## General Public License for more details.                                ##
##                                                                         ##
#############################################################################

# $Id$

import shellforge as sf
import getopt,sys,imp

#import shellforge.cpu_any


def usage():
    print >>sys.stderr,"""ShellForge v%s\nCopyright (C) 2003 Philippe Biondi <phil@secdev.org>
Usage: sf.py [-v] [-k] [-t] [-e] [-a|--in=..] [-A|-C|-R|-H|-out=..] [--loader=..]
             [-c <cc>] [-o <objdump>] [--cflags="-Os .."] <C file>
"""%sf.VERSION


options = sf.Options()

LOGLEVEL=1
if __name__ == "__main__":
    options.arch = options.host_arch = sf.get_arch()
    options.hexpairs = False
#    options.sflib_root = "../sflib/sflib/" # XXX
    try:
        opts = getopt.getopt(sys.argv[1:], "hvVeaCARHtkc:o:", ["in=","out=","loader=","arch=","errno","cflags=","sflib="])
        for opt,optarg in opts[0]:
            if opt == "-h":
                usage()
                sys.exit(0)
            elif opt == "-V":
                print >>sys.stderr,"Shellforge v%s\nCopyright (C) 2003 Philippe Biondi <phil@secdev.org>"%sf.VERSION
                sys.exit(0)
            elif opt == "-v":
                LOGLEVEL += 1
            elif opt in ["-e","--errno"]:
                options.use_errno = 1
            elif opt == "-a":
                options.input = sf.Input.ASM
            elif opt == "-C":
                options.output = sf.Output.C
            elif opt == "-A":
                options.output = sf.Output.TUNED_ASM
            elif opt == "-R":
                options.output = sf.Output.RAW
            elif opt == "-t":
                options.test += 1
            elif opt == "-k":
                options.keep += 1
            elif opt == "-c":
                options.cc = optarg
            elif opt == "-H":
                options.hexpairs = True
            elif opt == "-o":
                options.objdump = optarg
            elif opt == "--cflags":
                options.cflags = optarg
            elif opt == "--sflib":
                options.sflib_root = optarg
            elif opt == "--in":
                options.input = sf.Input.from_string(optarg)
            elif opt == "--out":
                options.output = sf.Output.from_string(optarg)
            elif opt == "--loader":
                lopt = {}
                loader,lopts = (optarg.split(":",1)+[None])[:2]
                if lopts:
                    for o in lopts.split(":"):
                        n,v = (o.split("=",1)+[None])[:2]
                        lopt[n] = v
                options.loaders.append((loader.lower(),lopt))
            elif opt == "--arch":
                options.arch = optarg.replace("-","_")
        if len(opts[1]) != 1:
            raise getopt.GetoptError("<C file> missing")

    except getopt.GetoptError,msg:
        sf.sflog.error(msg)
        sys.exit(1)
    except KeyError, msg:
        sf.sflog.error("unkonwn in/out type: [%s]" % msg)
        sys.exit(1)

    sf.sflog.setLevel(max(1,40-10*LOGLEVEL))

    options.set_arch(options.arch)

    sf.sflog.info("Running ShellForge v%s" % sf.VERSION)
    sf.sflog.info("Host arch is [%s]" % options.host_arch)
    sf.sflog.info("Input=%s Output=%s ARCH=%s" % (sf.Input.to_string(options.input),
                                                  sf.Output.to_string(options.output),
                                                  options.arch))

    sf.sflog.debug("keep=%i test=%i use_errno=%i" % (options.keep, options.test, options.use_errno))
    sf.sflog.debug("loaders=%r" % options.loaders)


    forge=sf.forge(options=options)
    forge.set_input_file(options.input, opts[1][0])
    
#
#    sys.path.append("./shellforge") # XXX
#    
#    archmodule = "arch_%s" % options.arch
#    try:
#        modfind = imp.find_module(archmodule)
#        try:
#            arch = imp.load_module(archmodule, *modfind)
#        finally:
#            if modfind[0]:
#                modfind[0].close()
#    except ImportError,msg:
#        sf.sflog.error("Architecture [%s]  (module %s) can't be loaded: %s" % (options.arch,archmodule,msg))
#        sys.exit(1)
#
#
#    
#
#    the_loaders = arch.The_Loaders()
#    the_binutils = arch.The_Binutils(options.arch, SFLIB_ROOT, CC=options.cc, OBJDUMP=options.objdump, CFLAGS=CFLAGS)
#    the_codetuner = arch.The_CodeTuner()
#
#    if LOGLEVEL >= 3:
#        the_binutils.show_versions()
#        
#    
#    
#    forge = sf.ShellcodeForge(the_binutils, the_codetuner, the_loaders)
#    forge.set_input_file(options.input, opts[1][0])
    try:
        for loader,args in options.loaders:
            forge.apply_loader(loader,args)
            
        if options.test == 0:
	    if options.hexpairs:
		print forge.get_output(options.output).replace ("\\x", "")
	    else:
            	print forge.get_output(options.output)
        elif options.test == 1:
            forge.build_test()
            print forge.TEST_FILE
        else:
            forge.run_test()
    except sf.BuildError,msg:
        sf.sflog.error(msg)
        sys.exit(1)
