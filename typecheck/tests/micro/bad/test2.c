
int a(int k, int j) {
	return 1;
}

void b(void) {
	{
		return 2;
	}
}

int c(int h[]) {
	return ;
}

int d(void) {
	if (b())
	{
		return 6;
	}
	if (a(3,3) == b())
	{
		return 7;
	}
	{}
	if (1)
	{
		return 1;
	} else {
		return 2;
	}

}

int main(void) {
	// int a[2];
	int rr;
	// rr = 9;
	// a[0] = rr;
	// rr=a[b()];
	rr = a(2, 2, 2);

	rr = b();

	return b() + 3;
}

