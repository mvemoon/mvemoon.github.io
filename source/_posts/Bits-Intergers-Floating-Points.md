---
title: Bits & Intergers & Floating Points
date: 2021-07-28 22:01:50
tags:
---

# Bits & Interters & Floating Points

计算机以二进制理解世界，也就是一串0和1，重要的是bit representation（二进制表示）以及 decoding（解读方式)。
以文件的传输为例，ASCII文件中的文字首先被编码成二进制形式，然后发送给接收者，接收端以一定的规则解码得到原来的文件。
```
发送者 (encoding) ------> 01100110 (decoding)------> 接收者
```
decoding方式带来的影响：
```
以4-bit的data type 为例
1111 --       int     --> -1
1111 --  unsigned int --> 15
```
由于 int 采用补码形式表示，而usigned不是导致值不同（后文详解）

### Data Size
1 bit 即是1个0或1，8 bits == 1 byte, 16 bits == 2 bytes == 1 words
word size 就是指针所占的字位数

### Address
当我们谈到计算机的内存与地址时，一般指的是虚拟内存，每一bit都有它的地址
### Byte Ordering
既然bit都有自己的地址，那么连续的bits的地址一定是连续的，它们存储方式有两种big endian, little endian。
```
address        0x100  0x101  0x102 0x103
big endian        01     23     45    67
little endian     67     45     23    01
```
big endian 即是大地址结尾，而little endian 即是以小地址结尾。
在我的机子上，其以little endian方式存储，可以用以下的C代码验证
```C
#include <stdio.h>

void show_bytes(char* c_p, size_t n)
{i
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
```

```bash
./endian
12345678
78 56 34 12  
```

了解大小端的意义如下：
- 网络传输大多用big-endian, 而计算机一般用little-endian
- 汇编、机器码的顺序
- C中的cast操作，如union可能带来问题

### String (ASCII)
由于ASCII的范围0~127，每一次都读取一个byte，所以不受大小端的影响

---


## Unsigned & Two's Complement

假设有一个data的bit vector如下
[w<sub>k-1</sub>, w<sub>k-2</sub> .. w<sub>0</sub>]
unsigned : 求和每一位的2的次幂乘以w<sub>x</sub>
Two's Complement ： 第一位为符号位，其拥有负权重2<sub>k-1</sup>

这也即当符号为为1时，其值为负而0为正的原因：最大的负数 -1 ： 2<sub>k-1</sup> + (2<sub>k-1</sup> -1)

同时记U至T的转换为
U2T(x) = x<sub>t</sub> - w<sub>k-1</sub> * 2<sub>k</sup>

---

## Bit-Level Operations
&（位与） |（位或）^(异或) >>（右移） <<(左移)	~(取补码)
```
&  0 1       |  0 1     ^ 0 1    ~ 0 1
0  0 0       0  0 1     0 0 1      
1  0 1       1  1 1     1 1 0      1 0
```
关于 &,&&　　|,|| 
当逻辑运算符&&，||操作的对象为一个bit时，相当于&, |
关于~
**x + ~x+1 = 0**
也即~x+1为x的加法逆元
关于 ^
- 用于符号位的计算较为方便，对于一个非0值，非TMIN值，对其符号位^1，相当于求其加法逆元(~x+1更方便)
- x^x = 0

关于<< >>




