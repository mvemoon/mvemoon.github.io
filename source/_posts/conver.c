#include <stdio.h>

int main(void)
{
	unsigned short int s_u = 0xcfff;
	short int s_t = 0xcfff;
	int i_u = (int)s_u;
	int i_t = (int)s_t;
	printf("i_u : %x\n", i_u);
	printf("i_t : %x\n", i_t);
	return 0;
}
