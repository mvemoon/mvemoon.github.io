---
title: Convolution
date: 2021-08-27 10:06:19
tags: 卷积
keywords: 卷积
categories: 深度学习
mathjax: true
cover:
---

# 卷积

卷积是对高维数据的常规操作，用于对高维数据的特征提取。

## 为什么需要卷积

在深度学习上应用卷积的历史需要从图像说起。在没有卷积时，图像识别的一般操作是利用神经网络提取特征，再对特征应用另一个神经网络作为解码器，以做图像分类。但是单纯的神经网络全连接层做特征提取是有很大的问题，即对位置敏感。

例如给定一组马的照片喂入神经网络训练，假设一半马头朝向左边，一半马头朝向右边，那么神经网络学习到什么呢？在这个神经网络的认知里，马是双头的。这是因为全连接层其实没有利用二维的信息，它还是将图像展成一组一维的向量，导致学到的特征必须出现在特定的位置。如今卷积布置局限于图像识别，在文本分类中也出现了textcnn的卷积分类网络。

卷积操作是线性的，利用卷积提取的特征的方式是非线性的，这样的特征对位置不敏感。卷积是一个三维的卷积核在三维的矩阵由左至右，由上至下滑动运算得到一个二维的矩阵。从如下的示意图中可以看出，卷积后的特征的宽和高回缩小，而深度取决于卷积核的个数。

![](c.gif)

## torch 卷积

### conv2d

pytroch中的二维卷积定义如下：
```python
torch.nn.Conv2d(in_channels, out_channels, kernel_size, stride=1, padding=0, dilation=1, groups=1, bias=True, padding_mode='zeros', device=None, dtype=None)
```

对应给定的[N, C, H, W]输入（N为batch_size, C为Channels数，彩色图像中为3，H、W分别为高和宽），卷积操作后得到[N, C_hat, H_hat, W_hat]，C_hat为卷积核个数。不考虑padd,H_hat, W_hat的公式如下：

(图像尺寸 - 卷积核尺寸 + 2`*填充值)/步长 + 1

torch的一个示例：

```python
import torch
input = torch.tensor([[1., 2.,  3., 4.,],
                      [5., 6., 7., 8. ],
                      [9., 10., 11., 12.],
                      [13., 14., 15., 16.]])
input = input.unsqueeze(0)
input = input.unsqueeze(0)
conv = nn.Conv2d(1, 1, 2, stride = 1, padding = 0)
output = conv(input)
print(output)
print(output.shape)
conv = nn.Conv2d(1, 2, 2, stride = 1, padding = 0)
output = conv(input)
print(output)
print(output.shape)
```


```
//output
tensor([[[[0.3269, 0.4242, 0.5214],
          [0.7158, 0.8131, 0.9103],
          [1.1047, 1.2020, 1.2992]]]], grad_fn=<ThnnConv2DBackward>)

torch.Size([1, 1, 3, 3])

tensor([[[[2.4007, 2.8337, 3.2666],
          [4.1324, 4.5653, 4.9982],
          [5.8640, 6.2969, 6.7298]],

         [[0.1943, 0.3063, 0.4184],
          [0.6425, 0.7546, 0.8667],
          [1.0908, 1.2029, 1.3150]]]], grad_fn=<ThnnConv2DBackward>)
torch.Size([1, 2, 3, 3])

```

### padding

单纯的卷积必然伴随着图像高和宽度的减小，但是我们并不想使得特征过窄，padding即填充操作可以**先**把图像扩充，然后**卷积**。

示例对比如下：
```python
print(input.shape)
conv = nn.Conv2d(1, 1, 2, stride = 1, padding = 2)
output = conv(input)
print(output.shape)
```

```
//输出
[1, 1, 4, 4]
[1, 1, 7, 7]
```
7 = 4+2+2-1


### 感受野

网友给出的定义
> 某一层feature map中某个位置的特征向量，是由前面某一层固定区域的输入计算出来的那这个区域就是感受野。
conv(5, 5)的感受野即是 $ 5 \ast 5$ 的区域，一般来说感受野越大越好。


### 其他卷积

**空洞卷积**示意图如下：
![](d.gif)
作用
- 扩大感受野
- 捕获多尺度上下文信息

**深度可分离卷积**

作用：
应用于轻量级网络，mobielnetv+系列，极大减少参数量，适用于web端以及移动端部署，代价是精度的一定程度下降。
