/* 
 * ShellForge example: Hello world with pusha/popa
 * $Id$
 */

#define STR "Hello world!\n"

int main(void) 
{
	__asm__("pusha");

	write(1, STR, sizeof(STR));

	__asm__("popa");
}

