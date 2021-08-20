---
title: Machine-Level Representation of Programs
date: 2021-08-01 10:36:57
tags:
mathjax: true
---

# Machine-Level Representation of Programs

## 编译与反汇编
先回忆一个C文件生成程序的过程：预处理，编译，汇编，链接。现阶段的编译器对代码的优化程度大，以至于反汇编时的汇编代码产生大幅度的变形，为了学习上的理解方便，我们使用gcc -Og的指令，使其代码不在编译阶段发生变形，同时使用objdump工具反汇编程序，得到汇编代码。
```bash
gcc -Og prog.c -o prog
objdump -d prog
```

## 信息的Accessing

### 寄存器
用来暂时储存运算结果，x86-64一共有16个64bit寄存器，每个中实际上4个寄存器，分别为64, 32, 16, 8 bit.

```
63                          31               15            7          0
-----------------------------------------------------------------------
|%rax                         |%eax            |%ax         |%al       |
-----------------------------------------------------------------------
```
### 操作符

|Type|Form|Value|
|:----:|:---:|:---:|
|立即数|$Imm|Imm|
|寄存器|r|寄存器内的值 R(r)|
|内存访问|(r)|访问寄存器中的地址指向的值|
|内存访问|Imm(rb, ri, s)|计算出的地址指向的值|

rb是base register, ri 是index register, s是缩放因子取1， 2， 4， 8，也即是数据类型的byte数。Imm(rb, ri, s) = Imm + R(rb) + R(ri)`*s。




### Data Movement Instructions
MOV & MOVZ & MOVS
由mov_ + b/w/l/q 组成，后缀代表的是操作数据的size。一般情况下，MOV指令只会改变寄存器的指定字节数中的值，但是**movl**是一个例外，它操作32-bit int值，但是同时将高32-bit设置为0。**movq**是另一个例外，它只接受32-bit补码表示的值，但是q暗示它处理的是4-word 64-bit的值，所以它会将其sign extend.**movabsq**只能接收一个64-bit立即数，且移动到一个寄存器中。

MOVZ, MOVS 分别代表将小字节移入大字节时进行zero拓展和符号拓展。

### Stack
当内存以栈的形式使用时，有两条指令来操作stack data, pushq 等价于 subq $8, %rsp; movq %rbp, (%rsp)。popq 等价于 movq (%rsp), %rax; addq $8, %rsp。
```
|  高地址  |      |        |
|          |      |        |
|          |增长  |        |
|  低地址  |  ==> |        |
                  |   new  |

```
## Arithmetic and Logical Operations
**leaq** S, D  ==>  D <-- &S。取S的地址写入D中。如leaq (%rax, %rax, 4), %rax，那么%rax的值被改为地址变量5R(%rax)+7。但是这个指令通常与地址计算无关,而是用来做计算。example:
```
C code:
long scale(long x, long y, long z)
{
	long t = x + 4*y + z;
	return t;
}

汇编:
# x in %rdi, y in %rsi, z in %rdx

leaq (%rdi, %rsi, 4), %rax
leaq (%rdx, %rdx, 2), %rdx
leaq (%rax, %rdx, 4), %rax
ret

```

### 其他的一些操作符
一元操作指令：incq, decq 分别使stack的最高8个字节自增，自减，也即++,--
二元操作指令：addq,subq; addq %rax, %rdx  ==> R(%rdx) += R(%rax); subq %rax, %rdx ==> R(%rdx) -= R(%rax); sarq/shrq salq/shlq 分别为算数/逻辑左移，算数/逻辑右移。二元运算操作符的结果都被第二个参数保存。

>Tips : 使用x^x = 0的性质：xorq %rdx, %rdx比 movq $0, %rdx更有效。


### 特殊的一元操作符
imulq, mulq, S 将S与R(%rax)相乘得到128-bit,其中的高64-bit存储在%rdx中，低64-bit存储在%rax中。cqto 将SignExtend(R[%rax])的结果以上述形式储存。
idivq, divq S 将R[%rdx:%rax]模后的数存储在%rdx中，将商存储在%rax中。一般来说%rdx预先被设置为0。


## control 
两种方式//TODO

### Condition Codes
操作指令完成后一般都alter condition code(leaq 除外)
|type|description|
|:---:|:---|
|CF|检测unsigned的overflow|
|ZF|检测是否为0|

可以改变状态码而不用寄存器的两类指令：CMP，TEST。他们都不会改变参数的值。
|Instructions|Args|Perform|
|:---:|:---:|:---:|
|CMP|S1 S2| **S2-S1**|
|TEST|S1 S2|S1&S2|
### Accessing the Condition Codes
- 通过不同状态码的组合将寄存器的1byte设置为0或1
- 通过状态码jump
- 有条件地transfer data

jumps的编码，常用的是PC relative,用编码后的相对地址. 举例如下：
```
4003fa : 74 02                                    je  XXXXXX
4003fc : ff d0                                    callq *%rax

```
XXXXXX = 4003fc + 0x02 = 4003fe


## Procedures

## Arrary Allocation and Access

## Heterogeneous Data Structures

## 

