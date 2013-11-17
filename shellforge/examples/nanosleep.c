int main(void)
{
   struct timespec ts;
	ts.tv_sec = 10;
	ts.tv_nsec = 1000;
	nanosleep (&ts, NULL); 
}
