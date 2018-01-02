/*************************************************************************
	> File Name: Algraph.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月02日 星期二 08时32分45秒
 ************************************************************************/

#ifndef _ALGRAPH_H
#define _ALGRAPH_H
#include<iostream>
#include<string>
typedef struct Time {//时间格式: hour:minute,day
    int day;
    int hour;
    int minute;
}Time;

typedef struct InfoType {//边上信息
    string LineName; //航班或车次号，最多8字符(含\0) 
    Time StartTime, EndTime;
    Time SpendTime;
    float SpendMoney;
}TrafficLine;

typedef struct ArcNode {//表结点，边表
    string EndCityName; //该弧指向的城市
    struct ArcNode *NextLine; //指向下一条弧的指针
    TrafficLine *Info; //该弧的路线信息 
}LineNode;

typedef struct VNode {//头结点,顶点表
    string CityName; //地名
    int CityOrd; //城市编号,便于匹配
    LineNode *FirstLine;//指向第一条弧末尾结点的指针
    int Amount;//交通工具班次
}VNode;

class ALGraph{
public:
    ALGraph(int size);
    ~ALGraph();
    int searchCityNum(const string CityName) const; //查询城市编号
    void addCity(const string CityName); //手动添加城市
    void addCityFromFile(); //从文件读取以添加城市
    void addLine(); //添加线路
    void addLineFromFile(); //从文件读取以添加线路
    void reSize(int size); //重新分配城市列表，仅考虑城市增多情况
    void delCityLine(int i); //删除以城市为起点的线路
    void delLine(); //删除线路
    void showCity() const; //输出所有城市
    void showLine() const; //输出所有线路
    void dijkstra_Money(); //最小花费路径
    void dijkstra_Time(); //最小时间路径
    void showShortestPath(); //调用并打印最短路径

private:
    VNode *CityList; //用链表数组表示城市列表
    int CityNum; //城市个数，即顶点数
    int MaxCityNum; //当前最大城市个数
    int ArcNum; // 弧个数
}
#endif
