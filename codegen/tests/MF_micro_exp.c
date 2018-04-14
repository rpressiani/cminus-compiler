// input: 8
// output: 16

int foo(int a, int b){
	int r;
	r = a*b;
	return r;
}

void main(void){
	int x;
	
	x = input();
	
	output(foo(x,2));
}
