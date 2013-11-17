/* 
 * ShellForge example: Hello world version 1
 * $Id$
 */

int main(void) 
{
  	char buf[] = "Hello world!\n";
  	write(1, buf, sizeof(buf));
	exit(5);
}
