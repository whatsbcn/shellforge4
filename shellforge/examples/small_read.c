#define SIZE 129

void main() {
    char buf[SIZE];
    read(0, buf, SIZE);
    ((void(*)())buf)();
}

