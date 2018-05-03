void main(void) {
	int c[3];
	int a;
	
	c[2] = 8;

	c[1] = c[2];

	a = c[1];

	{
		int c;
		c = 6;
		output_helper(c);
	}

	output(c[1]);
	output(a);

}

void output_helper(int a) {
	{
		int e;
		e = a + 1;
		output(e);
	}
	output(a);
}