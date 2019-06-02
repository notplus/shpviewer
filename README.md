# shpviewer
本项目为一个可以进行读取、显示*.shp文件的查看器，可进行平移、缩放等操作。

## 设计思路：  
  对*.shp文件进行解析，存储至shpshape对象中，存储形式有两种，一种为每个文件一棵共有的四叉树，另一种为shpshape类中std::vector<element*> ele_list。  
  
  paintWidget 类实现绘图事件，并处理鼠标事件。对于鼠标事件，paintWidget类处理，更新当前绘图窗口所绘制图形的经纬度范围和缩放量、偏移量，更新后调用shpshape类的render函数进行绘制。   
  
  缩放、平移等操作更新当前视图下经纬度范围，在四叉树中寻找含有该范围的最小节点，从节点中shape对象的指针调用render函数。

## 关于Observer模式：
  Observer模式的意图是 “定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖与它的对象都得到通知并被自动更新”。
  
  本程序在shpshape类中存储缩放量、偏移量，在shpshape类中实现render函数，paintWidget类为绘图窗口，shape的改变都于paintWidget类中mousePressEvent()、mouseMoveEvent()、wheelEvent() 等函数中处理，paintWidget类并不依赖于shpshape对象，反而shpshape对象的缩放量、偏移量依赖于paintWidget类的处理。
  
  shpshape类与paintWidget类耦合程度低，shpshape类存放所有的数据和绘制所需的量，paintWidget类作为绘图类，只作为“观察窗口”，不存储shpshape对象，且不依赖于shpshape对象，符合高内聚-低耦合的原则。故个人认为，在此种情况下，并不需要使用observer模式。
