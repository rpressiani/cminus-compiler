// input: 3, 5
// output: 7

int foo(int a, int b){
	int r;
	r = a*b;
	return bar(r);
}

int bar(int a){
	return (a/2);
}

void main(void){
	int x;
	int y;
	int val;

	x = input();
	y = input();
	
	val = foo(x,y);
	output(val);
}
