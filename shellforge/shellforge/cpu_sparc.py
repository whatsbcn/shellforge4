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


class Loaders_sparc(Loaders):
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



class CodeTuner_gcc_sparc(CodeTuner):
    def __init__(self, stack_reloc = 0, save_regs = 0):
        CodeTuner.__init__(self)
        self.stack_reloc = stack_reloc
        self.save_regs = save_regs
        if stack_reloc or save_regs:
            error("Stack relocation and register saving not supported yet for this arch.\n")
            
        
        
    def tune_code(self, code):
        printinfo(1,"** Tuning assembler code")
        codelines = code.splitlines()
        preamb = []
        unktext = []
        getpcfunc = []
        getpc = []
        rodata = []
        textpream = []
        beforegetpc = []
        aftergetpc = []
        afterrestore = []
        end = []
        
        out = ["# Modified by shellforge v%s\n"%VERSION]
        st1 = []
        st2 = []
        st3 = []
        state=0
        for l in codelines:
            printinfo(4, "[%i] %s"% (state, l))
            if l.find("@PLT") >= 0:
                error("Error at [%s]: Symbol not found" % (l.strip()), err=2)

            if l.find("ld") >= 0 and l.find("[%l7+") >=0 :
                a = l.find("ld")
                b = l.find("[")
                c = l.find("+")
                d = l.find("]")
                l = l[:a]+"add  "+l[b+1:c]+", "+l[c+1:d]+l[d+1:]
            if l.find("(.LLC") >= 0:
                a = l.find(")")
                l = l[:a]+"-main"+l[a:]
                
                
            
            if state == 0:
                if l.find("text") >= 0:
                    state = 1
                    continue
                else:
                    preamb.append(l);
            if state == 1: 
                if l.find(".rodata") >= 0:
                    state = 2
                    getpcfunc = unktext
                    continue
                elif l.find("main:") >= 0:
                    state = 3
                    textpream = unktext
                else:
                    unktext.append(l)
            if state == 2:
                if l.find(".text") >= 0:
                    state = 3
                else:
                    rodata.append(l)
            if state == 3:
                textpream.append(l)
                if l.find("main:") >= 0:
                    if rodata:
                        state = 4
                    else:
                        state = 6
                continue
                    
            if state == 4:
                if l.find("_GLOBAL_OFFSET_TABLE_") >= 0:
                    state = 5
                    continue
                beforegetpc.append(l)
            if state == 5:
                if l.find("_GLOBAL_OFFSET_TABLE_") >= 0:
                    state = 6
                    continue
            if state == 6:
                if (l.find(".LLfe1:") >= 0 or 
                    (l.find(".size") >= 0 and l.find("main") >= 0)):
                    state = 9
                else:
                    aftergetpc.append(l)
            if state == 9:
                end.append(l)
        
        if state != 9:
            self.automaton_error()

        if rodata:
            getpc = [ "\tcall .LLGETPC0",
                      ".PCmark:",
                      "\tadd %g0, main-.PCmark+4, %l7",
                      ]

        

        out += reduce(lambda x,y: x+["#---------"]+y,
                      [preamb,textpream,beforegetpc,getpc,aftergetpc,
                       getpcfunc,rodata,end])
#                
#        if self.stack_reloc:
#            out += [ "\tpopl %eax\n",
#                     "\tcall .L649\n",
#                     ".L649:\n",
#                     "\tpopl %ebx\n",
#                     "\tpushl %eax\n",
#                     "\taddl $[main-.L649],%ebx\n",
#                     "\tmovl %ebx, %eax\n",
#                     "\txorl %esp, %eax\n",
#                     "\tshrl $16, %eax\n",
#                     "\ttest %eax, %eax\n",                       
#                     "\tjnz .Lnotinstack\n",                       
#                     "\tmovl %ebx,%esp\n",
#                     ".Lnotinstack:\n" ]+mkstkframe+beforeebx
#        else:
#            out += mkstkframe
#            if self.save_regs:
#                out.append("\tpusha\n")
#            out += beforeebx
#            if rodata:
#                out += ["\tpush %ebx\n",
#                         "\tcall .L649\n",
#                         ".L649:\n",
#                         "\tpopl %ebx\n",
#                         "\taddl $[main-.L649],%ebx\n" ]
#    
#        out += afterebx
#        if self.save_regs:
#            out.append("\tpopa\n")
#        out += afterleave+rodata+end
        return "\n".join(out)+"\n"


class Binutils_sparc(Binutils):
    pass
        
