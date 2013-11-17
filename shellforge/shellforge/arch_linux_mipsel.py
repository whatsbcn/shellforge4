#############################################################################
##                                                                         ##
## arch_linux_mips.py --- classes for Linux/MIPS platform                  ##
##              see http://www.secdev.org/projects/shellforge.html         ##
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

from shellforge.cpu_mips import Binutils_mips, CodeTuner_gcc_mips as The_CodeTuner, Loaders_mipsel as The_Loaders

class Binutils_linux_mipsel(Binutils_mips): # need to subclass for self.__module__ to return the correct arch
    def __init__(self, *args, **kargs):
        Binutils_mips.__init__(self, *args, **kargs)
        self.DISFLAGS = self.DISFLAGS.replace(" -EB "," -EL ")
        self.CPU = "mips"


The_Binutils = Binutils_linux_mipsel
    
