
int main()
{
		dup2(1, 4);
        char *a[] = {"/bin/ls", "-al", 0}; 
        execve(*a, a, 0);
}

