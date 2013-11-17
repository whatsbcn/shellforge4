/* 
 * ShellForge example: declare and call a function
 * Declarations in the global namespace must be static.
 *
 * $Id$
 */

static void foo(char *s, int l)
{
	write(1, s, l);
}

int main(void)
{
	foo("bar\n",4);
	exit(0);
}


