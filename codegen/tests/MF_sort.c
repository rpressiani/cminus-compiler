// input: 5 numbers
// output: sorted low to high

void sort(int array[], int size){
	int i;
	int j;
	i=0;
	
	while(i<size){
		j=i;
		while(j<size){
			int tmp;
			if(array[j] < array[i]){
				tmp = array[i];
				array[i] = array[j];
				array[j] = tmp;	
			}
			j=j+1;
		}
		i=i+1;
	}
}

void main(void){
	int x[5];
	int i;

	i = 0;
	while(i<=4){
		x[i] = input();
		i=i+1;
	}

	sort(x,5);

	i=0;
	while(i<=4){
		output(x[i]);
		i=i+1;
	}
}
