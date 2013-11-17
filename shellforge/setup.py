#!/usr/bin/env python

from distutils.core import setup

setup(name='ShellForge',
      version='0.3',
      description='ShellForge',
      author='Philippe BIONDI',
      author_email='phil@secdev.org',
      url='http://www.secdev.org/projects/shellforge/',
      packages=['shellforge'],
      scripts=['sf.py'],
      data_files=[('/etc/shellforge/',['shellforge.conf'])],
     )


