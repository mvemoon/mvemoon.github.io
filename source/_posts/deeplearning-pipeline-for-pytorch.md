---
title: deeplearning pipeline for pytorch
date: 2021-08-26 22:05:13
tags: pytorch深度学习的一般流程
keywords: torch pipeline
categories: 深度学习
cover: 
---

#Pytorch深度学习的一般流程

这篇文章适用于有机器学习和深度学习基础但是不能熟练使用pytorch完成完整任务的读者。本文将基于一个心型分类器归纳代码的组成。

本文给出100k个点，根据心形函数对点打上里（1）和外（0）的标签，我们的目标是将点的坐标作为输入，给出标签也即是一个二分类的问题。



首先对于一个深度学习网络我们一般要完成三个部分：
- train 训练
- eval 阶段性评估，在eval中选取更优的网络
- interface/test 提供使用网路的接口（可选）


## 数据准备说明

```python
from math import pow
import random
points = []
def func(x, y):
    return pow((x**2 + y**2 - 1), 3) - pow(x, 2)*pow(y, 3)<0
random.seed(123)
for i in range(100000):
    x = (random.random() - 0.5)*4
    y = (random.random() - 0.5)*4
    label = int(func(x, y))
    points.append([x, y, label])
```
func定义一个心形函数，random.random()生成[0, 1)范围内的实数，将它们放缩到[-2, 2]区间。

![](init.png)

```python
import pandas as pd
df = pd.DataFrame(points,columns=['x','y', 'label'])
import seaborn as sns
sns_plot = sns.scatterplot(data = df, x = 'x', y = 'y', hue = 'label')
sns_plot.figure.savefig('./init.png')
```
将点和label信息放入数据框中作为我们的数据集，利用sns可视化数据。

## 训练

**Training = Dataset and Dataloader + Model + Loss and Optimizer**

## Dataset & Dataloader
Dataset 和 DataLoader的构建。
```python
from torch.utils.data import Dataset, DataLoader, random_split
```
### Dataset 自定义
实际工程中拿到的数据格式、类型不一，但是基本的Dataset构建方式是有一定范式的。

自定义数据集的要点有三：
- 继承torch.utils.data.Dataset
- 重写 __getitem__函数
- 重写 __len__函数

```python
class PointDataset(Dataset):
    def __init__(self, dataframe:pd.DataFrame):
        super(PointDataset, self).__init__()
        self.data = dataframe
    def __getitem__(self, index):
        point = torch.tensor([[self.data['x'][index], self.data['y'][index]]], dtype = torch.float32)
        label = torch.tensor([self.data['label'][index]], dtype = torch.float32)
        return point, label
    def __len__(self):
        return len(self.data)
```

__getitem__函数是提供给DataLoader获取一个数据集元素的方法，给定一个index返回tensor化的数据集元素。
__len__函数是返回数据集的总长度

> Tips : 当传入的数据比较大比如读入图片时，一般方法是通过__init__函数构建一个存放着图像路径的数据框， 然后在__getitem__中读入图片到内存中。

Dataset实际上是一个迭代器，可以试试以下代码输出dataset中的元素。

### DataLoader

DataLoader的使用方法比较一般，只要传入上一步自己定义的Dataset和一些常规参数即可。


### Model

Model继承torch.nn.Module类，在model的构建上有较多的tensor和模型模块的技巧，这里只是简单地构建一个MLP。在__init__中定义模块，在__forward__中阐述完整模型的构建。

一般来说在__init__中定义需要训练的部分，比如nn.Linear等，在__forward__处定义无需训练的模块如normalize。

```python
class MLP(nn.Module):
    def __init__(self):
        super(MLP, self).__init__()
        
        self.layers = nn.Sequential(
          nn.Flatten(),
          nn.Linear(2, 72),
          nn.ReLU(),
          nn.Linear(72, 36),
          nn.ReLU(),
          nn.Linear(36, 1),
          nn.Sigmoid(),
    )
    def forward(self, x):
        x = self.layers(x)
        return x
```

在此处虽然我们的点是二维的，但是模型只是线性神经元的操作，不涉及卷积，所以将tensro用nn.Flatten()展成一维的。
> sigmoid函数将得分放缩到[0, 1)之间，得到每一个标签的概率，这里与loss的选择相对应。


### Loss & Optimizer
```python
criterion = nn.BCELoss()
optimizer = torch.optim.SGD(net.parameters(), lr = config.lr, momentum=0.9)
```
分类任务一般使用交叉熵损失函数，二分类使用BCELoss，多分类使用nn.CrossEntropyLoss。

optimizer一般使用Adam和SGD。此处采用的是带动量的随机梯度下降法。优化器的选择也是一个重（玄）要（学）的方面，不同的任务在不同的优化器上差异较大。

### 训练步骤

```python
def train(config, train_loader, val_loader):
    net = MLP()
    criterion = nn.BCELoss()
    optimizer = torch.optim.SGD(net.parameters(), lr = config.lr, momentum=0.9)
    
    net.to(device)
    net.train()
    for e in range(config.epochs):
        train_loss = 0
        n_total = 0
        eval_loss = 0
        for batch, (x, y) in enumerate((train_loader)):
            optimizer.zero_grad()
            x = x.to(device)
            y = y.to(device)
            outputs = net(x)
            #todo cuda
            loss = criterion(outputs, y)
            loss.backward()
            train_loss += loss.item()*len(outputs)
            n_total += len(outputs)
            optimizer.step()
        eval_loss = eval(net, config, val_loader)
        train_loss = train_loss / n_total
        if ((e % 5 == 0) and (e >= 40) ):
            torch.save(net, config.save_path + "{:4f}.pth".format(eval_loss))
        print("Epoch: {}/{}, Train Loss: {}, Val Loss: {}".format(e, config.epochs, train_loss, eval_loss))
```
- 定义网络结构，loss，optimizer
- epoch循环，定义每一个epoch中的loss,acc,miou等
　- batch循环
　- 优化器梯度清0，正向传播计算loss,自动反向传播，step更新所有参数
　- eval模型，在验证集上跑模型
　- 输出epoch结果，保存最优模型

额外需要说明的两点：
1. 如果使用gpu的话，net和tensor都要放在gpu上跑，因为cpu和gpu的数据是不互通的
2. 保存模型有两种方法，这里采用的是最简单粗暴的方法，直接序列化。

eval函数大同小异
```python
def eval(model, congfig, data_loader):
    model.eval()
    eval_loss = 0
    n_total = 0
    criterion = nn.BCELoss()
    with torch.no_grad():
        for batch, (x, y) in enumerate((data_loader)):
            x = x.to(device)
            y = y.to(device)
            outputs = model(x)
            loss = criterion(outputs, y)
            eval_loss += loss.item()*len(outputs)
            n_total += len(outputs)
    model.train()
    return eval_loss / n_total
```
注意在两者间切换时利用net.eval()和net.train()做相应的改变。

至此，基于pytorch的深度学习pipeline完成。

## 训练结果

给出测试接口如下，对结果做可视化
```python
def cut_threshold(x):
    return int(1) if x > 0.5 else int(0)
def test_model(model_path, data_loader):
    net = torch.load(model_path)
    net.to(device)
    target = []
    
    for batch, (x, y) in enumerate((data_loader)):
        x = x.to(device)
        y = y.to(device)
        outputs = net(x)
        for i in range(x.shape[0]):
            target.append([x[i][0][0].item(), x[i][0][1].item(), cut_threshold(outputs[i][0].item())])
    df = pd.DataFrame(target,columns=['x','y', 'label'])
    sns_plot = sns.scatterplot(data = df, x = 'x', y = 'y', hue = 'label')
    sns_plot.figure.savefig(model_path[:12]+ '.png')

```
设置阈值为0.5即当概率大于threshold时认为是正样本。每5个epoch保存的模型测试可视化结果并对比原始标签如下：



![](1.png)![](2.png)![](3.png)![](4.png)

![](5.png)![](6.png)![](7.png)![](8.png)

[](init.png)

## 工程常用方法
1. 创建directory分类，常把函数分入Model, Dataset, utils模块。
2. 使用argparse包以做到命令行传参，并设立单独的class初始化默认参数。
