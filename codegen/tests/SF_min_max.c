// input: 5 numbers
// output: min and max
// get the maximum and minimum in an array of integers

void main(void){
	int group_number[5];
	int i;
	int max;
	int min;

	i=0;
	while(i!=5){
		group_number[i] = input();
		i=i+1;
	}
	
	max=group_number[0];
	min=group_number[0];
	
	i=0;
	while(i<=4)
	{
		if(max < group_number[i])
			max = group_number[i];
		
		if(min >= group_number[i])
			min = group_number[i];
		
		i=i+1;
	}

	output(min);
	output(max);

	return;
}
