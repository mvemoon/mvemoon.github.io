---
title: System Level I/O
date: 2021-08-10 21:28:15
categories: 深入理解计算机系统
tags: Linux文件
cover:
---


# System Level I/O

## Unix I/O

在Unix系统中，一切对象都是**文件**。而文件只是一组字节序列。甚至连接到系统的设备，Socket都视为文件。

文件的分类
- Regular file: Contains arbitrary data
- Directory: Index for a related group of files
- Socket: For communicating with a process on another machine
- Named pipes(管道)
- Symbolic links
- Character and block decvices


### Regula files 
C语言标准库中的API将常规文件分为text file, binary file。但是系统内核是不对这两者做区分的。

>Linux/MAC 用'\n', '\r\n'

### Directories
一组links组成了目录文件，每一个link是文件名到文件的映射。

## **File Descriptor**
文件操作符，一个小的整数，标识打开的文件。

每个进程都会维护一个文件描述符表，文件描述符也就是表的索引，表的每一项是对打开的文件的引用。同时系统还维护另外两张表，分别是打开文件表，v-node表，这两者被所有进程共享。文件描述符表只由每一个进程访问，但是子进程可以继承父进程的文件描述符表。

![threeTable](table.jpg)

> 每个进程的描述符0， 1， 2对应的指针是固定指向stdin, stdout, stderr的。

## 重定向
可以通过C语言的函数调用**dup(old, new)**实现。首先将new的文件关闭，再把new指向old所指向的。重定向也可以用linux实现，即 **>**。如 ls>test.txt。



-----

## Unix I/O
read, write, open, close是最底层的API,属于系统调用。

### Reading Files & Writing Files
需要文件描述符fd和指向buffer的指针及其大小。
> Tips : strace 可以检查程序的系统级调用。
读写文件时，当字节数大于1小于最大值时，称作Short Counts,发生在以下三种情况

- 遇到EOF
- 在终端读入时遇到换行符
- 读写networks sockets

不会发生Short Counts的情况
- 从disk读入
- 向disk写入


### 带缓冲与非缓冲I/O

RIO是健壮的I/O包，提供了非缓存和缓存的两类函数。

非缓存函数将bytes直接读入到目标内存。而缓存函数通过调用非缓存函数将bytes读入到特定的内存区域中，再实现内存到内存的读写。相对于内存的读写来说，系统的调用效率非常低。通过带缓冲的函数可以提高效率。

但是问题在于两者不都是需要先调用系统函数吗，带缓冲的究竟快在哪儿呢？原因在于可以减少系统调用的次数，因为缓冲区足够大，所以可以尽可能多的一次取完，再由目标地址分批从该缓存内读入，如果非缓存会增多系统调用的次数。

RIO中的缓存区由一个结构体控制，其中的指针指向一片内存。

### File Metadata （文件元数据）
描述文件信息的数据，它们存放在结构体stat中。这些数据描述了文件是否可读，文件类型等基本信息。

![metadataInfo](info.jpg)


## Standard I/O
Standard I/O 指的其实就是Ｃ语言中常见的I/O函数，如scanf,fscanf,printf,它们都是带缓冲的I/O。

> Standard I/O 常用于终端文件读写，而RIO常用于网络编程。




