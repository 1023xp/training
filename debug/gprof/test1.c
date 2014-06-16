void b()
{
	volatile int i = 5000000;
	while(i--);
	a();
	c();
}

