// input: 67, 8
// output: 3 
// division remainder --> mod

int mod(int a, int b){
	int div;
	div = a/b;
	
	return (a-(b*div));
}

void main(void){
	int x;
	int y;

	x = input();
	y = input();

	output( mod(x,y) );

	return;
}

