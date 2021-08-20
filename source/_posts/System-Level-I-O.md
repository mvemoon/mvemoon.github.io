---
title: System Level I/O
date: 2021-08-10 21:28:15
tags:
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

> seek 指示文件读到的位置

### Regula files 
> C语言标准库中的API将常规文件分为text file, binary file。Linux/MAC 用'\n', '\r\n'

### Directories
一组links组成了目录文件，每一个link是文件名到文件的映射。

## Open Files

### **File Descriptor**
文件操作符，一个小的整数，标识打开的文件。
> 文件操作符也是函数返回值，每次系统调用时都应该检查返回值

## Close Files
close(fd)

> 关闭文件时也是可能出错的，多线程时共享内存的情况，所以也应该检查返回值

## Reading Files & Writing Files
需要文件描述符fd和指向buffer的指针及其大小。
> Tips : strace 可以检查程序的系统级调用。
读写文件时，当字节数大于1小于最大值时，称作Short Counts,发生在以下三种情况

- 遇到EOF
- 在终端读入时遇到换行符
- 读写networks sockets

不会发生Short Counts的情况
- 从disk读入
- 向disk写入

## RIO
RIO是健壮的I/O包，在Socket中非常有用。RIO提供了非缓存和缓存的两类函数。
> Buffered RIO are thread-safe

### Unbuffered
循环读入每一个字节，根据返回值进行讨论

### Buffered
先读入一段字节到缓冲区，通过指针访问读入字节，避免了重复调用系统的I/O函数，提高了效率。


## Unix Kernel 中的文件表示
//TODO

### Fiel Metadata （文件元数据）
描述文件信息的数据，它们存放在结构体stat中。这些数据描述了文件是否可读，文件类型等基本信息。


### sharing

### 重定向


## Standard I/O
Standard I/O 指的其实就是Ｃ语言中常见的I/O函数，如scanf,fscanf,printf,它们都是待缓冲的I/O。

> Standard I/O 常用于终端文件读写，而RIO常用于网络编程。




