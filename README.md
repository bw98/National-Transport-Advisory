# 全国交通咨询系统 by C++ on Linux and OS X
### 1.信息存储
- 利用邻接表存储城市信息与线路信息，比邻接矩阵更加高效。
    实现方法：map 映射 vector 实现

### 2.主要数据结构
- Time，规范时间的输入输出格式
- VNode，头结点，用于建立顶点表，存储城市信息
- LineNode，存储线路信息

### 3.主要函数
- 查询城市是否存在
- 查询城市编号
- 手动添加城市
- 从文件读取以添加城市
- 输出所有城市
- 手动添加线路
- 从文件中读取线路
- 输出所有线路
- 删除城市
- 删除线路
- 求从起点城市到终点城市的所有路径
- 求最小中转的路径与其次数
- 求最少花费的路径与其费用
- 求耗时最少的路径与其时间
### 4.核心算法 去环DFS、dijkstra 分析
- DFS算法的参考文献：【https://blog.csdn.net/HUSTLX/article/details/52163923】
- dijkstra的参考文献：【https://blog.csdn.net/qq_29762941/article/details/80904553?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task】
- PS: DFS中的 visited 访问标记数组决定了剪枝的方案，可以好好分析一番，研究DFS的去环方法
### 5.项目的总结性报告
- 我在文件夹 [Report](https://github.com/bw98/National-Transport-Advisory/tree/master/Report "Report") 里放了关于这次项目的设计思路以及函数的主要功能、数据流程图，如果对我项目的部分细节有疑惑，可以 clone 后仔细阅读 report.pdf
- PS: 这是我大二写的，大四寒假重构了一波项目，所以参考价值低
### 6.截图
![部分线路截图](https://images2017.cnblogs.com/blog/1199740/201801/1199740-20180105114038628-1177620256.png "部分线路截图")



[1]: http://www.cnblogs.com/Bw98blogs/p/8205445.html "dijkstra最小路径算法"
