// input: 5000
// output: 1, 23, 20
// change the format of n seconds to that of hour, minute and second
// eg: 5000 seconds can be presented as 1 hour, 23 minutes and  20 seconds

void main(void){
	int n; //seconds
	int hour_n;
	int minute_n;
	int second_n;
	
	n=input();
	
	hour_n=n/3600;
	minute_n=( n - hour_n * 3600)/60;
	second_n= n - hour_n * 3600 - minute_n * 60;

	output(hour_n);
	output(minute_n);
	output(second_n);

	return;
}
