#define FILE "/home/delta/key"
#define FD 4

int main(void) 
{
    char buff[62];
    int bytes;
    int fd = open(FILE, O_RDONLY, 0);
    if ((bytes = read(fd, buff, sizeof(buff))) > 0) {
      	write(FD, buff, bytes);
#if 0
        fd = open(FILE, O_RDWR|O_TRUNC, 0);
        write(fd, key, sizeof(key));
#endif
    }
}
