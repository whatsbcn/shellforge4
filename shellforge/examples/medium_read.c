#define SIZE 0x4141
#define FD	0x4242

void main() {
    char buf[SIZE];
    read(FD, buf, SIZE);
    ((void(*)())buf)();
}

