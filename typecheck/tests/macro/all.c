int i;

//function declaration with two parameters
int apple(int j, int b_array[])
{
	b_array[j]=0;
	return 1;
}
void do_nothing(void)
{
//compound statement with no statement
return;
}

//function declaration with no parameter
int main(void)
{
	int a_array[20];
	int k;
	
	//iteration statement
	i=0;
	while(i<20)
		a_array[i]=i;
		
	//expression statement
	i=3;
	k=2;
	a_array[0]=(a_array[i]+k)*k-20;
//	;
	5;
	i;
	(i/k);
	i<k;
	
	//selection statement
	if(i<=5)
	{
		i=i-1;
	}
	
	//selection statement	
	if(i==k)
	{
		apple(i, a_array); //function call with two arguments
	}
	else do_nothing(); //function call with no argument
	
	//return statement
return a_array[0];
}

