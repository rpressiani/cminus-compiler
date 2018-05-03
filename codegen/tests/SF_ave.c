// output: 15
// get the average value the integers from 1 to 29

void main(void)
{
	int i;
	int ave;
	int total;
	
	i=1;
	total=0;
	
	while(i<30)
	{
		total = total + i;
		i=i+1;
	}

	ave = total / 30;
	
	output(ave);

	return;
}
