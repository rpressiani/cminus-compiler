// input: 4, 3
// output: 64

int pow(int base, int exp){
	int r;

	r = base;
	if(exp>1){
		r = base * pow(base, exp-1);
	}
	
	return r;
}

void main(void){
	int x;
	int y;
	int out;
	
	x = input();
	y = input();	

	out = pow(x, y);
	
	output(out);
}
