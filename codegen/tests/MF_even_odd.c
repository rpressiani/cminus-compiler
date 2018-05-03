// input: 5 numbers
// output: all even numbers followed by all odd numbers 
// split an interger array into two arrays, one for odds, the other for evens

int odd_or_even(int number)
{
	int x;
	if(number == (number/2*2))
		x=0;
	else x=1;

	return (x);
}

void main(void){
	int number[5];
	int odds[5];
	int evens[5];
	int number_p;
	int odds_p;
	int evens_p;
	int i;

	i=0;
	
	while(i!=5)
	{
		number[i]=input();
		i=i+1;
	}
	
	number_p=0;
	odds_p=0;
	evens_p=0;
	
	while(number_p<5)
	{
		if(odd_or_even(number[number_p])==1)
		{
			odds[odds_p]=number[number_p];
			odds_p=odds_p+1;
		}
		else
		{
			evens[evens_p]=number[number_p];
			evens_p=evens_p+1;
		}
		number_p=number_p+1;
	}

	i=0;
	while(i<evens_p){
		output(evens[i]);
		i=i+1;
	}

	i=0;
	while(i<odds_p){
		output(odds[i]);
		i=i+1;
	}

	return;
}

