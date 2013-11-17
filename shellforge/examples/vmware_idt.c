/* 
 * ShellForge example: Return IDT and GDT, enabling virtual machine discovery
 * $Id$
 */


int main(int argc, char *argv[])
{
	int a[4];
	a[0]=a[1]=a[2]=a[3]=0;

	__asm__("sidt %0 \n"
		"sgdt %1 \n"
		: "=m" (a), "=m" (a[2]));
	write(1,a,16);
}

