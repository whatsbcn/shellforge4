#############################################################################
##                                                                         ##
## cpu_parisc.py --- Classes for Sparc CPUs                                ##
##     see http://www.secdev.org/projects/shellforge/                      ##
##     for more informations                                               ##
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

from cpu_any import *


class Loaders_amd64(Loaders):
    pass

#    options = Loaders.options+["xorkey"]
#    def loader_xor(self, shcode):
#        key = self.opts.get("xorkey", None)
#        if key is None:
#            if "\x00" not in shcode:
#                return shcode
#            ld=""
#            for i in range(1,256):
#                if chr(i) not in shcode:
#                    key=i
#                    break
#            if key is None:
#                error("xor loader : no suitable xor key found.")
#        printinfo(1,"** Applying xor loader. key=%#02x" % key)
#        shcode = "".join(map(lambda x: chr(ord(x)^key), shcode))
#        length = len(shcode)
#        if length < 0x100:
#            ld = ("\xeb\x0d\x5e\x31\xc9\xb1"+chr(length)+"\x80\x36"+
#                  chr(key)+"\x46\xe2\xfa\xeb\x05\xe8\xee\xff\xff\xff")
#        else:
#            if length & 0xff == 0:
#                length += 1
#            ld = ("\xeb\x0f\x5e\x31\xc9\x66\xb9"+
#                  chr(length&0xff)+chr(length>>8)+
#                  "\x80\x36"+chr(key)+
#                  "\x46\xe2\xfa\xeb\x05\xe8\xec\xff\xff\xff")
#        return ld+shcode
#



class CodeTuner_gcc_amd64(CodeTuner):
    def __init__(self, stack_reloc = 0, save_regs = 0):
        CodeTuner.__init__(self)
        self.stack_reloc = stack_reloc
        self.save_regs = save_regs
        
    def ____tune(self, code):
        sf.sflog.info("Tuning assembler code")
        codelines = code.splitlines()
        preamb = []
        rodata = []
        textpream = []
        mkstkframe = []
        beforeebx = []
        setebx = []
        afterebx = []
        afterleave = []
        end = []
        
        out = [["# Modified by shellforge v%s\n"%VERSION]]
        st1 = []
        st2 = []
        st3 = []
        state=0
        for l in codelines:
            sf.sflog.debug("[%i] %s"% (state, l))
            if l.find("@PLT") >= 0:
                error("Error at [%s]: Symbol not found" % (l.strip()), err=2)
            if l.find("@GOT(") >= 0:
                l = l.replace("mov","lea").replace("GOT","GOTOFF")
            if state == 0:
                if l.find(".rodata") >= 0:
                    state = 1
                    continue
                elif l.find(".text") >= 0:
                    state = 2
                else:
                    preamb.append(l);
            if state == 1:
                if l.find(".text") >= 0:
                    state = 2
                else:
                    rodata.append(l)
            if state == 2:
                textpream.append(l)
                if l.find("main:") >= 0:
                    state = 3
                continue
                    
            if state == 3:
                mkstkframe.append(l)
                if l.find("mov") >=0 and l.find("%rsp") >= 0 and l.find("%rbp") >= 0:
                    state = 4
                continue
    
            if state == 4:
                if l.find("sub") >=0 and l.find(",%rsp") >=0:
                    mkstkframe.append(l)
                else:
                    if rodata:
                        state = 5
                    else:
                        state = 7
                    
            if state == 5:
                if l.find("call") >= 0 and l.find(".L") >= 0:
                    state = 6
                else:
                    beforeebx.append(l)
    
            if state == 6:
                setebx.append(l)
                if l.find("GLOBAL_OFFSET_TABLE") >= 0:
                    state = 7
                continue
            if state == 7:
                if l.find("leave") >= 0:
                    state = 8
                else:
                    afterebx.append(l)
            if state == 8:
                if (l.find(".Lfe1:") >= 0 or 
                    (l.find(".size") >= 0 and l.find("main") >= 0)):
                    state = 9
                else:
                    afterleave.append(l)
            if state == 9:
                end.append(l)
    
        if state != 9:
            self.automaton_error()
    
        out += [preamb, textpream]
                
        if self.stack_reloc:
            out += [[ "\tpop %rax",
                      "\tcall .L649",
                      ".L649:",
                      "\tpop %rbx",
                      "\tpushl %rax",
                      "\tadd $[main-.L649],%rbx",
                      "\tmov %rbx, %rax",
                      "\txor %rsp, %rax",
                      "\tshr $16, %rax",
                      "\ttest %rax, %rax",                       
                      "\tjnz .Lnotinstack",                       
                      "\tmov %rbx,%rsp",
                      ".Lnotinstack:" ], mkstkframe, beforeebx ]
        else:
            out += [mkstkframe]
            if self.save_regs:
                out += [["\tpusha"]]
            out += [beforeebx]
            if rodata:
                out += [["\tcall .L649",
                         ".L649:",
                         "\tpop %rbx",
                         "\tadd $[main-.L649],%rbx" ]]
    
        out += [afterebx]
        if self.save_regs:
            out += [["\tpopa"]]
        out += [afterleave, rodata, end]

        out = reduce(lambda x,y: x+["#---------"]+y, out)

        return "\n".join(out)+"\n"


class Binutils_amd64(Binutils_BinaryTarget):
    def __init__(self, *args, **kargs):
        Binutils_BinaryTarget.__init__(self, *args, **kargs)
        #self.CFLAGS  += "-m32 -march=i386 -fPIC"
        ##self.LDFLAGS += "-m32 -z execstack"
        #self.TEST_CFLAGS += "-m32 -z execstack"
        self.LDFLAGS += "-m64 -z execstack"
        self.TEST_CFLAGS += "-m64 -z execstack"
        self.CFLAGS  += "-m64 -march=x86-64 -fPIC"
	# GNU
        #self.LDFLAGS += "-m64 -z execstack"
        #self.CFLAGS  += "-m64 -march=x86_64 -fPIE -arch x86_64-fPIC"
        #self.TEST_CFLAGS += "-m64 -z execstack"
        #OSX self.CFLAGS  += "-fPIC"
        #OSX self.LDFLAGS += "-z execstack"
