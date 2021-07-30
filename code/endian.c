#include <stdio.h>

void show_bytes(char* c_p, size_t n)
{
		for(size_t i = 0; i<n; i++)
		{
				printf("%X ", *c_p);
				c_p++;
		}
		puts("");
}

int main(void)
{
	int m = 0x12345678;
	printf("%X\n", m);
	show_bytes((char*)(&m), sizeof(int));
	
	return 0;
}














