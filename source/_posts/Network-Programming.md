---
title: Network Programming
date: 2021-08-02 11:15:32
tags:
mathjax: true
---

# The Client-Server Programming Model

**transaction**


```
---------------                                              ------------------
|    Client    |  ------1. Client sends request----------->  |     Server     |                           ---------------
|    Process   |                                             |     Process    |   <--2.Server process--> |    Resource   |
----------------  <-----3. Server sends respose------------  ------------------                           ---------------
4. Client processes reponse
```

**clients and servers are processes and not machines, or hosts**

A single host can run many different clients and servers concurrently（并发地）. And a client and server transaction can be on tne same or different hosts. 



# Web Servers
HTTP 是client与server交互的协议。

## Content
Content是通过HTTP传输的内容，有html. plain. postcript. gif, pang, jpeg等多种类型，通过servers提供content的方式又可以分为serving static conten, serving dynamic content。
传输的文件都有一个独特的名字URL（universal resource locator）以http://www.google.com:80/index.html为例,client用前缀http:/www.google.com:80决定联系的服务器，服务器位置以及listening的port。servers 用/index.html寻找文件并判断它是动态资源还是静态资源。
