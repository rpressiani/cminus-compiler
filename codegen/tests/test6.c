void main(void) {
	int a;
	a = 3;
	{
		int a;
		a = 1;
		output(a);
	}
	output(a);
}