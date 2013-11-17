#############################################################################
##                                                                         ##
## arch_linux_i386.py --- classes for Linux i386 platform                  ##
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

from shellforge.cpu_amd64 import CodeTuner_gcc_amd64 as The_CodeTuner, Loaders_amd64 as The_Loaders, Binutils_amd64

class Binutils_linux_amd64(Binutils_amd64): # need to subclass for self.__module__ to return the correct arch
    pass

The_Binutils = Binutils_linux_amd64
    
