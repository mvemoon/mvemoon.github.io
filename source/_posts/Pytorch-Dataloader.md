---
title: Pytorch Dataloader
date: 2021-08-10 10:32:41
tags:
---

# Pytorch DataLoader
DataLoader是pytorch中的一个高级的迭代器，支持
- Batching the data
- Shuffling the data
- Load the data in parallel using multiprocessing workers

使用时需要继承torch.utils.dadta.Dataset类，并重写函数：
- **__len__** 返回数据集的总量
- **__getitem__** 通过索引访问数据


