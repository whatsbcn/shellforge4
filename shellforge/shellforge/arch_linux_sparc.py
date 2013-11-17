#############################################################################
##                                                                         ##
## arch_linux_parisc.py --- classes for Linux Sparc platform               ##
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

from shellforge.cpu_sparc import Binutils_sparc, CodeTuner_gcc_sparc as The_CodeTuner, Loaders_sparc as The_Loaders

class Binutils_linux_sparc(Binutils_sparc): # need to subclass for self.__module__ to return the correct arch
    pass

The_Binutils = Binutils_linux_sparc
    
