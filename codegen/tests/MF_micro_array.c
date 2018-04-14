// output: 40

int sum(int array[], int size){
	int i;
	int total;
	i = 0;
	total = 0;

	while(i<size){
		total = total+array[i];
		i = i+1;
	}

	return total;
}

void main(void){
	int x[4];
	int arrsum;
	
	x[0] = 3;
	x[1] = 9;
	x[2] = 21;
	x[3] = 7;

	arrsum = sum(x,4);
	
	output(arrsum);
}
