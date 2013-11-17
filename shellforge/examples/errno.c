/* 
 * ShellForge example: using errno. Run ShellForge with -e or define SF_USE_ERRNO
 * $Id$
 */

#define SF_USE_ERRNO

int main(void) 
{
	int fd = 0;
	if ((fd = open("/donotexist/prettysureofthat",O_RDONLY,0)) < 0) {
		write(1,"Can't open file!\n", 17);
		write(1,&errno,4);
	}
	else
		write(1,"Ok.\n",4);
		write(1, &fd, 4);

	exit(fd);
}
