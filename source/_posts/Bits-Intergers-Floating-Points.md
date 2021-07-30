---
title: Bits & Intergers & Floating Points
date: 2021-07-28 22:01:50
tags:
mathjax: true
---

# Bits & Interters & Floating Points

## Bit
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

## Intergers


### Unsigned & Two's Complement

假设有一个data的bit vector, [w<sub>k-1</sub>, w<sub>k-2</sub> .. w<sub>0</sub>], unsigned : 求和每一位的2的次幂乘以w<sub>x</sub>, Two's Complement ： 第一位为符号位，其拥有负权重2<sub>k-1</sup>。

这也即当符号为为1时，其值为负而0为正的原因：最大的负数 -1 ： 2<sub>k-1</sup> + (2<sub>k-1</sup> -1)，同时记U至T的转换为U2T(x) = x<sub>t</sub> - w<sub>k-1</sub> * 2<sub>k</sup>

---

### Bit-Level Operations
&（位与） |（位或）^(异或) >>（右移） <<(左移)	~(取补码)
```
&  0 1       |  0 1     ^ 0 1    ~ 0 1
0  0 0       0  0 1     0 0 1      
1  0 1       1  1 1     1 1 0      1 0
```

#### 关于 &,&&　　|,|| 
当逻辑运算符&&，||操作的对象为一个bit时，相当于&, |

#### 关于~
**x　+　~x+1　=　0**　也即~x+1为x的加法逆元

#### 关于 ^
* 用于符号位的计算较为方便，对于一个非0值，非TMIN值，对其符号位^1，相当于求其加法逆元(~x+1更方便)
* x^x = 0

####  左移　右移
左移、右移顾名思义就是将bit vector向左、右移动，多出来的bit自然舍去，问题在于缺少的bit位是用0/1来补，而这在U/T的表现又不同

U/T 左移 ： 用0补齐缺少位，称为逻辑左移

U 右移 ： 用0补齐缺少位，称为逻辑右移

T 右移 ： 补齐位与符号位一致，可以为0/1，称为算术右移
　
左移k位，所有对应位的权重都增大k,相当于乘以 2<sub>k</sup>  （int符号位虽然是负权重，但是结果相同）

右移k位，所有对应位的权重都减少k,相当于除以 2<sub>k</sup> ,需要注意的是结果需要**Rounding**(取整)。C中的整数取整都是向0取整，而应用>>时正数：向下取整，也即向0取整，而对于负数来说，其依然是向下取整，所有需要加上相应的bias,使其满足向0取整。bias = 1 << k-1

### Conversion
转变分为三种
- int 与 unsigned int
- 小字节类型转大字节类型
- 大字节类型转小字节类型

int 与 unsigned int 的转换在C中并不改变其bit-represention,只是改变其解读形式。

大字节转小字节则是要截断高位。

小字节转大字节，则需要拓展高位，这其中又分为sign expansion && zero expansion。

　　首先考虑U-U的小转大，采用零拓展，即高位全用0填补。
　　T-T的拓展，采用符号拓展，即高位拓展取决与符号为。
　　那么U-T,T-U的小字节转大字节是怎么实现的呢？规律是先变size再改变类型。也就是其拓展与改变前的拓展类型一致，举例如下：
```C
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
```
```bash
i_u : cfff
i_t : ffffcfff
```

## Arithmetic

这些运算以及数据本身构成了群，如无整型与加法构成了无整型加群，在这里引入群中的一个概念，加法逆元，若a+b=0，则称b为a的逆元。

### Modular addition
加法其实是Modualr addition,即对最后的结果取模。
```C
        U                    T
	 x=0:    x          x=Tmin: Tmin 
-x                         
	 x>0:   2**w - x    x>Tmin: -x


溢出检测：
     !(x + y < x)       !(x<0 && y<0 && sum >=0) && !(x>=0 && y>=0 && sum<0)
```
加法逆元的C表达式：
> ~x+1

### Muplication
unsigned 计算时同样时将结果mod 2<sub>w</sub>, signed计算则是先将其转为unsigned计算，最后转回signed。这也同样表明了两种类型的乘法运算结果truncted后的bit-representation是一样的。

溢出检测的两种方式：
```C
///1.群的性质
int tmult_ok(int x, int y)
{
	int p = x*y;
	return !x || p/x == y;
}

///2.用更大的数位表示
int tmult_ok(int x, int y)
{
	in64_t pll = (in64_t)x*y;
	return pll == (int)pll;
}

```
> Tips : size_t被定义为unsigned类型，而malloc接收一个size_t类型，那么溢出后得到一个较小的整数的话是无法被发现的，只能在malloc前对其siz做溢出检测。

## Floating Point
首先给出浮点数的一般表达式

$$
v = (-1)^s*M*2^E
$$
