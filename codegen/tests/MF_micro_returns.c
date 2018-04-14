// output: -3, 0

int positive(int a){
	int err;
	err = 0 - 1;	

	if(a>=0) return 1;
	else return 0;
	
	return err;
}

void main(void){
	int x;
	int y;
	int num;
	int p_or_n;

	x = 5;
	y = 8;
	
	num = x-y;
	output(num);
	
	p_or_n = positive(num);
	output(p_or_n);

	return;
}
