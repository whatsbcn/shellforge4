#############################################################################
##                                                                         ##
## shellforge package --- base                                             ##
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

import logging,os,sys,imp
import ConfigParser

import warnings
warnings.filterwarnings("ignore","tempnam",RuntimeWarning, __name__)

try:
    REVISION="$Revision$".split()[1]
except:
    REVISION="??"

VERSION = "0.3.0-rev"+REVISION

CONFIG_FILE="shellforge.conf"
CONFIG_PATH=["/etc/shellforge/",
             os.getenv("HOME")+".shellforge/",
             "./" ]

class SFException(Exception):
    pass

class BuildError(SFException):
    pass


####
# Config file

class ShellforgeConfig(ConfigParser.SafeConfigParser):
    def getkey(self, arch, key, default=None):
        if arch:
            if self.has_option(arch, key):
                return self.get(arch, key)
            os,cpu = arch.split("_")
            if self.has_option(cpu, key):
                return self.get(cpu, key)
        return self.defaults().get(key, default)
        
config=ShellforgeConfig()
config.read(map(lambda x:x+CONFIG_FILE, CONFIG_PATH))



def get_arch():
    f = os.popen("uname -s")
    kernel = f.read().strip().lower()
    if f.close():
        kernel = "linux" # default to 'linux' if uname -s fails? this is probably wrong
    print "Kernel is "+kernel
    f = os.popen("uname -m")
    cpu = f.read().strip()
    if f.close():
        kernel = "i386" # default arch
    if cpu in ["i686","i586","i486"]:
        cpu = "i386"
    if cpu in ["x86_64"]:
        cpu = "amd64"
    return "%s_%s" % (kernel, cpu)


class EnumMetaclass(type):
    def from_string(cls,x):
        return cls.__dict__[x.upper()]
    def to_string(cls, x):
        for k in cls.__dict__:
            if cls.__dict__[k] == x:
                return k        
    def __new__(cls, name, bases, dct):
        all = filter(lambda x: x==x.upper(), dct.keys())
        dct["all"] = all
        dct["from_string"] = classmethod(cls.from_string)
        dct["to_string"] = classmethod(cls.to_string)
        return super(EnumMetaclass, cls).__new__(cls, name, bases, dct)

class Input:
    __metaclass__ =  EnumMetaclass
    C = 0
    ASM = 1
    TUNED_ASM = 2
    SHCODE = 3
    INLINE_C = 4
    INLINE_ASM = 5
    INLINE_TUNED_ASM = 5
    BIN = 6
    
    
class Output:
    __metaclass__ =  EnumMetaclass
    C = 1
    RAW = 2
    ASM = 3
    HEX = 4
    TUNED_ASM = 5
    OUTPUT_ASM = 6

class Options:
    def set_arch(self, arch):
        if self.arch is None:
            self.arch = arch
        if self.sflib_root is None:
            self.sflib_root = config.getkey(arch, "sflib")
        if self.cc is None:
            self.cc = config.getkey(arch, "cc")
        if self.objdump is None:
            self.objdump = config.getkey(arch, "objdump") 
        
    sflib_root = None
    cc = None
    objdump = None
    cflags = ""
    verb = 0
    test = 0
    keep = 0
    hexpairs = False
    arch = None
    host_arch = None
    input = Input.C
    output = Output.HEX
    loaders = []
    saveregs = 0
    stackreloc = 0
    src = ""
    use_errno = 0



sflog = logging.getLogger("shellforge")
console_handler = logging.StreamHandler()
console_handler.setFormatter(logging.Formatter("##[%(levelname)-5s]## %(message)s"))
sflog.addHandler(console_handler)

class ShellcodeForge:
    
    def __init__(self, binutils, code_tuner, loaders=None, tmpdir="/tmp", keep_tmp=0):
        self.binutils = binutils
        self.code_tuner = code_tuner
        self.loaders = loaders


        self.tmp = os.tempnam(tmpdir,"sf")+"/"
        os.mkdir(self.tmp)

        self.inputpoint = {}
        for i in Input.all:
            setattr(self, i, None)
        self.BIN_FILE=None
        self.C_FILE=None
        self.ASM_FILE=None
        self.TUNED_ASM_FILE=None
        self.C_TEST_FILE=None
        self.TEST_FILE=None
        self.SHCODE_FILE=None

        self.keep_tmp = keep_tmp
        
    def __del__(self):
        if not self.keep_tmp:
            os.system("#rm -rf '%s'" % self.tmp)

    ## Tools

    def set_input(self, kind, inp):
        setattr(self, Input.to_string(kind), inp)

    def set_input_file(self, kind, src):
        dest = self.tmp+os.path.basename(src)
        self.write_file(dest, open(src).read())
        setattr(self, Input.to_string(kind)+"_FILE", dest)

    def get_output(self, kind):
        return getattr(self, "output_%s" % Output.to_string(kind))()

    def write_file(self, filename, txt):
        open(filename,"w").write(txt)

    def read_file(self, filename):
        return open(filename).read()

    ## Ensure

    def ensure_C_FILE(self):
        if not self.C_FILE:
            if not self.INLINE_C:
                raise SFException("No premise found. Missing input.")
            self.C_FILE = self.tmp+"shcode_inline.c"
            self.write_file(self.C_FILE, self.INLINE_C)

    def ensure_ASM_FILE(self):
        if not self.ASM_FILE:
            if self.INLINE_ASM:
                self.ASM_FILE = self.tmp+"shcode_inline.S"
                self.write_file(self.ASM_FILE, self.INLINE_ASM)
            else:
                self.ensure_C_FILE()
                self.ASM_FILE=self.binutils.compile(self.C_FILE)

    def ensure_ASM(self):
        if not self.ASM:
            if self.INLINE_ASM:
                self.ASM=self.INLINE_ASM
            else:
                self.ensure_ASM_FILE()
                self.ASM = self.read_file(self.ASM_FILE)

    def ensure_TUNED_ASM_FILE(self):
        if not self.TUNED_ASM_FILE:
            if self.INLINE_TUNED_ASM:
                self.TUNED_ASM_FILE = self.tmp+"shcode_inline.tuned.S"
                self.write_file(self.TUNED_ASM_FILE, self.TUNED_ASM)
            else:
                self.ensure_ASM_FILE()
                self.TUNED_ASM_FILE = self.code_tuner.tune(self.ASM_FILE)
                
    def ensure_TUNED_ASM(self):
        if self.INLINE_TUNED_ASM:
            self.TUNED_ASM = self.INLINE_TUNED_ASM
        else:
            self.ensure_TUNED_ASM_FILE()
            self.TUNED_ASM = self.read_file(self.TUNED_ASM_FILE)

    def ensure_BIN_FILE(self):
        if not self.BIN_FILE:
            self.ensure_TUNED_ASM_FILE()
            self.BIN_FILE = self.binutils.assemble(self.TUNED_ASM_FILE)
            

    def ensure_SHCODE(self):
        if not self.SHCODE:
            self.ensure_BIN_FILE()
            self.SHCODE=self.binutils.extract_shcode(self.BIN_FILE)

    def ensure_SHCODE_FILE(self):
        if not self.SHCODE_FILE:
            self.ensure_SHCODE()
            self.SHCODE_FILE=self.BIN_FILE+".shcode"
            self.write_file(self.SHCODE_FILE, self.SHCODE)

    def ensure_C_TEST_FILE(self):
        if not self.C_TEST_FILE:
            C = self.output_C()
            self.C_TEST_FILE = self.BIN_FILE+".tst.c"
            self.write_file(self.C_TEST_FILE, C)

    def ensure_TEST_FILE(self):
        if not self.TEST_FILE:
            self.ensure_C_TEST_FILE()
            self.TEST_FILE = self.binutils.compile_test(self.C_TEST_FILE)


    ## Output
    
    def output_ASM(self):
        self.ensure_ASM()
        return self.ASM

    def output_TUNED_ASM(self):
        self.ensure_TUNED_ASM()
        return self.TUNED_ASM

    def output_RAW(self):
        self.ensure_SHCODE()
        return self.SHCODE
    
    def output_HEX(self):
        raw = self.output_RAW()
        return "".join(map(lambda x: "\\x%02x" % ord(x), raw))

    def output_C(self):
        hexa = self.output_HEX()
        ret = 'unsigned char shellcode[] = \n'
        for i in range(0, len(hexa), 76):
            ret += '"%s"\n' % hexa[i:i+76]
        ret += ";int main(void) { ((void (*)())shellcode)(); }\n"
        return ret

    def output_OUTPUT_ASM(self):
        self.ensure_SHCODE_FILE()
        return self.binutils.disassemble(self.SHCODE_FILE)

    def __repr__(self):
        return "<ShellcodeForge arch=%s>" % self.binutils.arch

    def __str__(self):
        return self.output_RAW()

    def __getattr__(self, attr):
        return getattr(str(self), attr)
    
        
    ## Actions

    def apply_loader(self, loader, options={}):
        self.ensure_SHCODE()
        if not hasattr(self.loaders, "loader_%s" % loader):
            raise BuildError("loader [%s] not found" % loader)
        self.SHCODE = getattr(self.loaders, "loader_%s" % loader)(self.SHCODE, **options)

    def build_test(self):
        self.ensure_TEST_FILE()

    def run_test(self):
        self.ensure_TEST_FILE()
        self.binutils.run(self.TEST_FILE)


def forge(arch=None, loaders=None, options=None):
    if options is None:
        options = Options()
        if arch is None:
            arch = get_arch()
        options.set_arch(arch)
        if loaders is not None:
            options.loaders=loaders

#    sys.path.append("shellforge") 
    
    archmodule = "arch_%s" % options.arch
    try:
        modfind = imp.find_module(archmodule,__path__)
        try:
            arch = imp.load_module(archmodule, *modfind)
        finally:
            if modfind[0]:
                modfind[0].close()
    except ImportError,msg:
        sflog.error("Architecture [%s]  (module %s) can't be loaded: %s" % (options.arch,archmodule,msg))
        raise

    the_binutils = arch.The_Binutils(sflib_root=options.sflib_root, CC=options.cc, OBJDUMP=options.objdump, CFLAGS=options.cflags, keep_tmp=options.keep)
    the_binutils.show_versions()

    return ShellcodeForge(the_binutils , arch.The_CodeTuner(), arch.The_Loaders(), keep_tmp=options.keep)


__all__ = ["forge", "get_arch", "Options", "Output", "Input", "SFException", "ShellcodeForge", "VERSION"]
