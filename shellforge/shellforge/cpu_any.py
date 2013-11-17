#############################################################################
##                                                                         ##
## cpu_any.py --- base classes, architecture independant functions         ##
##                see http://www.secdev.org/projects/shellforge/           ##
##                for more informations                                    ##
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

# $Id : cpu_any.py,v 1.5 2004/04/30 10:12:48 pbi Exp $

import os,re,sys
import shellforge as sf


class Loaders:
    """Loaders factory class"""
    def __init__(self, **opts):
        for i in opts:
            if i not in self.options:
                raise TypeError("Unknown option [%s]" % i)
        self.opts = opts
        self.loaders = filter(lambda x: x.startswith("loader_"),
                              self.__class__.__dict__)
    def update_options(self, **opts):
        self.opts.update(opts)
    def delete_option(self, option_name):
        del(self.opts[option_name])

    def loader_debug(self, shcode, **options):
        sf.sflog.debug("Applying debug loader with options [%r]\n to shellcode [%r]" % (options, shcode))
        return shcode
    
        


class CodeTuner:
    def __init__(self):
        pass
    def tune(self, shcode):
        return shcode
    def automaton_error(self):
        sflog.error("Automaton failed. Complain at <phil@secdev.org>.\n"+
                    "Join your C file and the full output (stdout and stderr)\n"+
                    "of shellforge with -vvvvv.")
        
    
class Binutils:
    ld_script = None
    def __init__(self, arch=None, sflib_root=".", CC="gcc", OBJDUMP="objdump", CFLAGS="", LDFLAGS="", tmpdir="/tmp", keep_tmp=0):
        if arch is None:
            arch = self.__module__
            if arch.startswith("arch_"):
                arch = arch.split("_",1)[1]
        self.arch = arch
        self.CC = CC
        self.OBJDUMP = OBJDUMP
#        self.CFLAGS = "-O3 -S -fPIC -Winline -finline-functions -ffreestanding -fomit-frame-pointer -fno-zero-initialized-in-bss "+CFLAGS
        self.CFLAGS = "-O3 -S -fPIC -Winline -finline-functions -ffreestanding -fomit-frame-pointer "+CFLAGS
        self.INCLUDES = "-isystem '%s' -isystem '%s'" % (sflib_root, "%s/%s" %(sflib_root,arch))
        self.INCLUDES += " -include %s/%s/sflib.h" % (sflib_root,arch)
        self.LDFLAGS = LDFLAGS
        self.ASSEMBLEFLAGS = "-c"
        self.DISFLAGS = ""
        self.TEST_CFLAGS = " -static "
        self.CPU = arch.split("_")[1]
        
        self.keep_tmp = keep_tmp
        if self.ldscript is not None:
            self.ldscript_file = os.tempnam(tmpdir,"ldscript-")
            open(self.ldscript_file, "w").write(self.ldscript)
            sf.sflog.info("ld script is %s" %  self.ldscript_file)
            self.LDFLAGS += " -Wl,-T,%s" % self.ldscript_file
        else:
            self.ldscript_file = None
            sf.sflog.info("no ld script")

    def __del__(self):
        if not self.keep_tmp and self.ldscript_file is not None:
            os.unlink(self.ldscript_file)

    def show_versions(self):
        sf.sflog.debug("Machine: "+os.popen("uname -a").read().strip())
        sf.sflog.debug("Compiler : [%s]\n%s" % (self.CC,os.popen("%s --version 2>&1" % self.CC).read().strip()))
        sf.sflog.debug("objdump  : [%s]\n%s" % (self.OBJDUMP,os.popen("%s --version 2>&1" % self.OBJDUMP).read().strip()))

    def compile(self, infile):
        sf.sflog.info("Compiling %s" % infile)
        outfile = infile+".S"
        cmd = "%s %s %s -o %s %s 2>&1" % (self.CC, self.CFLAGS, self.INCLUDES, outfile, infile)
        sf.sflog.debug("       ------[Compilation command]------\n%s" % cmd)
        sf.sflog.debug("       ---------------------------------")
	print "Compile "+cmd
        f=os.popen(cmd)
        out = f.read()
        status = f.close()

        if status:
            if status & 0xff:
                sf.sflog.error("compiler got signal %i while compiling %s" % (status&0xff, infile))
            else:
                sf.sflog.error("compiler reported error %i while compiling %s" % (status>>8, infile))
            raise sf.BuildError(out)
        if out:
            sf.sflog.warning(out)
        return outfile

    def assemble(self, infile):
        sf.sflog.info("Assembling %s" % infile)
        outfile = infile+".o"
        cmd = "%s %s %s -o %s %s" % (self.CC, self.LDFLAGS, self.ASSEMBLEFLAGS, outfile, infile)
        sf.sflog.debug("       ------[Assembling command]------\n%s" % cmd)
        sf.sflog.debug("      ---------------------------------")
	print "Assemble: "+cmd
        f = os.popen(cmd)
        out = f.read()
        status = f.close()
        if status:
            if status & 0xff:
                sf.sflog.error("compiler killed by signal %i while assembling %s" % (status & 0xff, infile))
            else:
                sf.sflog.error("compiler reported error %i while assembling %s" % (status >> 8, infile))
            raise sf.BuildError(out)
        if out:
            sf.sflog.warning(out)
        return outfile
        

    def extract_shcode(self, infile):
        sf.sflog.info("Extracting shellcode from %s" % infile)
        cmd = "%s -j .text -s -z  %s" % (self.OBJDUMP, infile)
        sf.sflog.debug("      ------[Extraction command]------\n%s" % cmd)
        sf.sflog.debug("      --------------------------------")
	print "Objdump "+cmd
        f = os.popen(cmd)
        inp = f.readlines()
        status = f.close()
        if status:
            if  status&0xff:
                sf.sflog.error("objdump killed by signal %i while extracting from %s" % (status & 0xff, infile))
            elif status:
                sf.sflog.error("objdump reported error %i while extracting from %s" % (status >> 8, infile))
            raise sf.BuildError("")
#        if out:
#            sf.sflog.warning(out)
                
        # Extract machine code
        dump=re.compile("^ [0-9a-f]{4}")
        out = []
        for l in inp:
            if dump.match(l):
                out += l[:42].split()[1:]
        out = "".join(out)
        shcode = ""
        for i in range(len(out)/2):
            shcode += chr(int(out[2*i:2*i+2],16))
        
        if shcode:
            return shcode
        raise sf.BuildError("No code in .text section of %s !?"%infile)
    
    def compile_test(self, infile):
        sf.sflog.info("Compiling test program %s" % infile)
        if infile.endswith(".c"):
            outfile = infile[:-2]
        else:
            outfile = infile+".tst"
        cmd = "%s %s -o %s %s" % (self.CC, self.TEST_CFLAGS, outfile, infile)
        sf.sflog.debug("      ------[Test compilation command]------\n%s" % cmd)
        sf.sflog.debug("      --------------------------------------")
        f = os.popen(cmd)
        out = f.read()
        status = f.close()
        if status:
            if status &0xff:
                sf.sflog.error("compiler killed by signal %i while compiling test program" % (status & 0xff))
            elif status:
                sf.sflog.error("compiler reported error %i while compiling test program" % (status >> 8))
            raise sf.BuildError(out)
        if out:
            sf.sflog.warning(out)
        return outfile
                
            
    def run(self, file):
        sf.sflog.info("Running test program %s" % file)
        abs=""
        if file[0] != "/":
            abs = "./"
#        sf.sflog.info("########################################\n")
        sys.stderr.flush()
	print "RUnning "+abs+" "+file
        status=os.system(abs+file)
        sys.stdout.flush()
#        sf.sflog.info("\n########################################\n")
        if status & 0xff:
            sf.sflog.error("Test program killed by signal %i" % (status & 0xff))
        else:
            if status >> 8:                
                sf.sflog.warning("Test program exited with exitcode=%i" % (status >> 8))
            else:
                sf.sflog.info("Test program exited with exitcode=%i" % (status >> 8))
        return status

    def disassemble(self, file):
        sf.sflog.info("Disassembling file %s" % file)
        cmd = "%s %s -m %s -b binary -D %s" % (self.OBJDUMP, self.DISFLAGS, self.CPU, file)
        sf.sflog.debug("      ------[Disassembling command]------\n%s" % cmd)
        sf.sflog.debug("      -----------------------------------")
        fin,fout,ferr = os.popen3(cmd)
        out = fout.read()
        err = ferr.read()

        if err:
            sf.sflog.error("objdump reported error while working on test program")
            raise sf.BuildError(err)
        return out


        
#        status = ferr.close()        
#        if status:
#            if status & 0xff:
#                sf.sflog.error("objdump killed by signal %i while working on test program" % (status & 0xff))
#            elif status:
#                sf.sflog.error("objdump reported error %i while working on test program" % (status >> 8))
#            sf.sflog.error(ferr.read())
#        else:
#            return out
#        
        

class Binutils_BinaryTarget(Binutils):
    
    def __init__(self, tmpdir="/tmp", keep_tmp=0, *args, **kargs):
        Binutils.__init__(self, *args, **kargs)
        self.LDFLAGS +=" -nostdlib -nodefaultlibs -nostartfiles -Wl,-s,--gc-sections,--oformat,binary "
        self.ASSEMBLEFLAGS = ""

    def extract_shcode(self, file):
        sf.sflog.info("Extracting shellcode from %s" % file)
        return open(file).read() 

    ldscript = """
SECTIONS {
        _GLOBAL_OFFSET_TABLE_ = .;
        /DISCARD/ : {
                *(.stab*)
                *(.comment)
                *(.note)
        }
        all : {*(.text, .rodata, .rdata, .data, .bss) }
}
"""
