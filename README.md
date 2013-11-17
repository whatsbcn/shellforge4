shellforge4
===========

Shellforge4 is an enhanced version of secdev's shellforge G3. More platforms, architectures and syscalls supported.

The original sources were from http://trac.secdev.org


Shellforge is a python program that uses the SFlib and gcc (cross-)compilers to generate shellcodes from C files.

Example
Given the hello.c program :

 int main(void) 
 {
         char buf[] = "Hello world!\n";
        write(1, buf, sizeof(buf));
        exit(5);
 }

We can make a shellcode :

 $ python sf.py examples/hello.c
 \x55\x89\xe5\x56\x83\xec\x14\x83\xe4\xf0\xbe\x01\x00\x00
 \x00\x83\xec\x10\x8d\x4d\xee\xb8\x04\x00\x00\x00\xba\x0e
 \x00\x00\x00\x53\xbb\x01\x00\x00\x00\xcd\x80\x5b\x89\xf0
 \x53\xbb\x05\x00\x00\x00\xcd\x80\x5b\x8b\x75\xfc\xc9\xc3


