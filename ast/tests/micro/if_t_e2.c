/* A test for if_else_then and if_then  */

void main(void){
int i;	
i=5;
if(i>10)
	{ i=0;
		if (i > 15)
		{
			int a;
			i = 1;
		} else {
			i = 10000;
		}
	}
	else{i=1;}	

}	
