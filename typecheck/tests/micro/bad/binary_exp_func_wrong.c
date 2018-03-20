//binary expression for literals and identifier
void f(void){
	int i;
	i=1 + 2;
return;
}
void main(void)
{
	//good
	int i;
	int j;
	int a[20];
	i+j;
	i-j;
	i*j;
	i/j;
	i=j;
	i==j;
	i!=j;
	i<j;
	i>j;
	i<=j;
	i>=f; //should have a problem here
return;
}
