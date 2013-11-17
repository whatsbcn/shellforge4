int main(void) 
{
    char buff[256];
    int bytes = 0;
    int fd = open("/etc/fstab", O_RDONLY, 0);
    while((bytes = read(fd, buff, 256)) > 0) {
      	write(1, buff, bytes);
    }
}
