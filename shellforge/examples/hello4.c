/* 
 * ShellForge example: Hello world version 4
 * $Id$
 */

#define STR "Hello world!\n"

int main(void) 
{
	char buf[1024]=STR;

	write(1, STR, sizeof(STR));
	exit(5);
}
