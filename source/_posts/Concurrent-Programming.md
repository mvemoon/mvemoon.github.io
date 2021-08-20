---
title: Concurrent Programming
date: 2021-08-11 20:18:44
tags:
---


并发编程

## 多台客户端在迭代服务器模型下的堵塞
在网络编程一章中，我们接触到了客户端与服务器连接的迭代服务器模型，服务器每次只连接一台客户端，此时另一台客户端向服务器发送请求是会堵塞，那么在连接的pipeline中，这一台客户端究竟堵在了哪一步呢？

- connect 这时客户端的连接请求虽然没有被accept，但服务端已经将它排列在等待队列中
- writen 数据写入服务端的buffer中
- read 堵塞在这里！服务端无法向客户端写入。

## 并发服务器
创建并发服务器有三种方式，Process-based, Event-based, Thread-based。

### Process-based


