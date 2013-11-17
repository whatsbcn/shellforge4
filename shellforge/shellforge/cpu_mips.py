#############################################################################
##                                                                         ##
## cpu_mips.py --- Classes for MIPS CPUs                                   ##
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


class Loaders_mips_common(Loaders):
    def loader_xor(self, shcode):
        key = self.opts.get("xorkey", None)
        if key is None:
            if "\x00" not in shcode:
                return shcode
            ld=""
            for i in range(1,256):
                if chr(i) not in shcode:
                    key=i
                    break
            if key is None:
                error("xor loader : no suitable xor key found.")
        printinfo(1,"** Applying xor loader. key=%#02x" % key)
        shcode = "".join(map(lambda x: chr(ord(x)^key), shcode))
        length = len(shcode)
        if length < 0x100:
            ld = ("\xeb\x0d\x5e\x31\xc9\xb1"+chr(length)+"\x80\x36"+
                  chr(key)+"\x46\xe2\xfa\xeb\x05\xe8\xee\xff\xff\xff")
        else:
            if length & 0xff == 0:
                length += 1
            ld = ("\xeb\x0f\x5e\x31\xc9\x66\xb9"+
                  chr(length&0xff)+chr(length>>8)+
                  "\x80\x36"+chr(key)+
                  "\x46\xe2\xfa\xeb\x05\xe8\xec\xff\xff\xff")
        return ld+shcode

class Loaders_mips(Loaders_mips_common):
    def loader_nocontrol(self, shcode):
        sh2 =  "\x23\xbd\xdf\xe0"  # addi    $29, $29, -0x2020 
        sh2 += "\xaf\xa0\x20\x20"  # sw      $0, 0x2020($29)   
        sh2 += "\x8f\xaa\x20\x20"  # lw      $10, 0x2020($29)
        sh2 += "\x21\x4b\x20\xb3"  # addi    $11,$10,8371
        sh2 += "\x25\x6b\xef\x80"  # addiu   $11,$11,-4224
        sh2 += "\xaf\xab\x20\x20"  # sw      $11,8224($29)
        sh2 += "\x8f\xa2\x20\x20"  # lw      $2,8224($29)   $2 <- 4147 (cacheflush)
        for i in range(len(shcode)/2,0,-1):
            a = ord(shcode[2*i-1])
            b = ord(shcode[2*i])
            c=d=0
            if b < 0x20:
                b += 0x80
                d = 0x80
                a -= 1
                a %= 256
                if a >= 0x40:
                    a -= 0x20
                    c = 0x20
                else:
                    a += 0x20
                    c = 0xe0
            else:
                if a < 0x20:
                    if b >= 0x40:
                        b -= 0x20
                        d = 0x20
                    else:
                        b += 0x20
                        d = 0xe0
                        a -= 1
                        a %= 256
                    if a >= 0x40:
                        a -= 0x20
                        c = 0x20
                    else:
                        a += 0x20
                        c = 0xe0
            
            sh2 += "\x21\x4b"+chr(a)+chr(b)     # addi    $11, $10, 0x1234
            if c or d:
                sh2 += "\x25\x6b"+chr(c)+chr(d) # addiu   $11, $11, 0x4567
            sh2 += "\xa7\xab\x20\x20"  # sh      $11, 0x2020($29)
            sh2 += "\x23\xbd\xff\xfe"  # addi    $29,  $29, -2
        sh2 += "\x23\xbd\x20\x20"  # addi    $29, $29, 0x2020
        sh2 += "\x25\x6b\x20\x5c"  # addiu   $11, $11, 0x205c  # add a backslash
        sh2 += "\x01\xc8\xd1\x4c"  # syscall 0x72345
  
        sh2 += "\x03\xa0\xf8\x09"  # jal     $29   XXX: passer en jalx
	return sh2


class Loaders_mipsel(Loaders_mips_common):
    def loader_nocontrol(self, shcode):
        sh2 =  "\xe0\xdf\xbd\x23"  # addi    $29, $29, -0x2020 
        sh2 += "\x20\x20\xa0\xaf"  # sw      $0, 0x2020($29)   
        sh2 += "\x20\x20\xaa\x8f"  # lw      $10, 0x2020($29)
        sh2 += "\xb3\x20\x4b\x21"  # addi    $11,$10,8371
        sh2 += "\x80\xef\x6b\x25"  # addiu   $11,$11,-4224
        sh2 += "\x20\x20\xab\xaf"  # sw      $11,8224($29)
        sh2 += "\x20\x20\xa2\x8f"  # lw      $2,8224($29)   $2 <- 4147 (cacheflush)
        for i in range(len(shcode)/2-1,-1,-1):
            sh2 += "\xfe\xff\xbd\x23"  # addi    $29,  $29, -2
            a = ord(shcode[2*(i^0)+1])
            b = ord(shcode[2*(i^0)])
#            print chr(a),chr(b),2*(i^1),2*(i^1)+1
            c=d=0
            if b < 0x20:
                b += 0x80
                d = 0x80
                a -= 1
                a %= 256
                if a >= 0x40:
                    a -= 0x20
                    c = 0x20
                else:
                    a += 0x20
                    c = 0xe0
            else:
                if a < 0x20:
                    if b >= 0x40:
                        b -= 0x20
                        d = 0x20
                    else:
                        b += 0x20
                        d = 0xe0
                        a -= 1
                        a %= 256
                    if a >= 0x40:
                        a -= 0x20
                        c = 0x20
                    else:
                        a += 0x20
                        c = 0xe0
           
            sh2 += chr(b)+chr(a)+"\x4b\x21"     # addi    $11, $10, 0x1234
            if c or d:
                sh2 += chr(d)+chr(c)+"\x6b\x25" # addiu   $11, $11, 0x4567
            sh2 += "\x20\x20\xab\xa7"  # sh      $11, 0x2020($29)

        sh2 += "\x20\x20\xbd\x23"  # addi    $29, $29, 0x2020
        sh2 += "\x4c\xd1\x5c\x01"  # syscall 0x57345
  
#        sh2 += "\x5c\x20\x6b\x25"  # addiu   $11, $11, 0x205c  # add a backslash
        sh2 += "\x09\xf8\xa0\x03"  # jalr     $29   XXX: passer en jalx
#        sh2 += "\x20\x20\xb0\xbf"   # cache   0x10,4128($29)
                                   # (0x10=Hit_Invalidate_I)
	return sh2






class CodeTuner_gcc_mips(CodeTuner):
    def __init__(self, stack_reloc = 0, save_regs = 0):
        CodeTuner.__init__(self)
        self.stack_reloc = stack_reloc
        self.save_regs = save_regs
        
    def tune_code(self, code):
#	return code


        printinfo(1,"** Tuning assembler code")
        codelines = code.splitlines()
        preamb = []
        rodata = []
        textpream = []
        text = []
        end = []
        
        out = [["# Modified by shellforge v%s\n"%VERSION]]

        state=0
        for l in codelines:
            printinfo(4, "[%i] %s"% (state, l))
            if l.find("@PLT") >= 0:
                error("Error at [%s]: Symbol not found" % (l.strip()), err=2)
            if state == 0:
                if l.find(".rdata") >= 0:
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
                if l.find("main:") >= 0:
                    state = 3
                else:
                    textpream.append(l)
            if state == 3:
                if l.find(".end") >= 0:
                    state = 4
                else:
                    text.append(l)
            if state == 4:
                end.append(l)
                
        if state != 4:
            self.automaton_error()

	text.insert(1,"""
        li      $9,-17
.set noreorder
next:
        bltzal  $8, next
.set reorder
        slti    $8, $0, 0x8282
        nor     $9, $9, $0
        addu    $25, $9, $31
        sw      $25, -31416($sp)
        addi    $25, $sp, -31416
""")
    
        out += [preamb, textpream, text]
                
    
        out += [rodata, end]

        out = reduce(lambda x,y: x+["#---------"]+y, out)

        return "\n".join(out)+"\n"


class Binutils_mips_test(Binutils):
    def __init__(self, *args, **kargs):
        Binutils.__init__(self, *args, **kargs)

class Binutils_mips(Binutils):
    def __init__(self, *args, **kargs):
        Binutils.__init__(self, *args, **kargs)
        self.DISFLAGS += " -EB "
        self.LDFLAGS +=" -nostdlib -nodefaultlibs -nostartfiles  "
        self.ASSEMBLEFLAGS = ""
    ldscript = """
SECTIONS {
         .text : { *(.text .rodata .got .rodata .rdata .data .bss .got)}
         /DISCARD/ : { *(.comment .mdebug* .pdr .reginfo) }
        _GLOBAL_OFFSET_TABLE_ = .; 
        _gp_disp = .;
}
"""


#old_ldscript  = """
#SECTIONS {
#        /DISCARD/ : {
#                *(.*)
#                *(.stab*)
#                *(.comment)
#                *(.note)
#                *(.mdebug*)
#        }
#        all : {*(.text, .got, .rodata, .rdata, .data, .bss, .pdr, .reginfo) }        
#        _GLOBAL_OFFSET_TABLE_ = .;
#}
#"""
