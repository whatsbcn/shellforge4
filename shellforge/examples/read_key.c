#define SF_USE_ERRNO 1
static int errno = 0;

int main(void) 
{
    char buff[256];
    //int fd = open("key", 0, 0);
    unsigned long fd = (unsigned long)open("/tmp/keya", 0, 0);

	if (fd >= (unsigned long)(-125)) {
		errno = -fd;
		fd = -1;
	}
	exit(fd);
	if (fd > 0) {
	    int bytes = read(fd, buff, 256);
	    write(1, buff, bytes);	
		write(1, "DONE\n", 5);
	}
	write(1, "NONE\n", 5);
}
