void main(void) {
	int a[2];
	int b;
	a[0] = 1;
	a[1] = 2;
	b = a[0];
	a[0] = a[1];
	a[1] = b;
	output(a[0]);
	output(a[1]);
}