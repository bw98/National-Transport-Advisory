/*************************************************************************
	> File Name: Algraph.h
	> Author: bw98
	> Mail: 
	> Created Time: 2018年01月02日 星期二 08时32分45秒
 ************************************************************************/

#ifndef _ALGRAPH_H
#define _ALGRAPH_H
#include<iostream>
#include<string>
#include<assert.h>
#define MaxFileSize 50
typedef struct Time {
    //时间格式: hour:minute,day
    int day;
    int hour;
    int minute;
    //运算符'-'重载，实现两Time对象做减法
    friend Time operator - (Time &et, Time &st) {
        Time t = {0, 0, 0};
        t.minute = et.minute - st.minute;
        if (t.minute < 0) {
            t.minute += 60;
            t.hour--;
        }
        
        t.hour = t.hour + et.hour - st.hour;
        if (t.hour < 0) {
            t.hour += 24;
            t.day--;
        }
        //由于默认日期的问题会造成不同天的时刻相减可能为负数，所以当时刻表相减为负数时需要让天数加一
        //比如甘肃到北京 T100 8:00,+0 10:00,+1  北京到长春 T101 7:00,+0 T102 8:30,+0 15:10,+0
        //实际上需要到北京后要再等待一天才能到，中途等待时间应该是 '7:00,+1' - '2:00,+1'
        if (et.day < st.day) {
            t.day++;
        }
        t.day = t.day + et.day - st.day;
        return t;
    }
}Time;

typedef struct InfoType {//边上信息
    std::string LineName; //航班或车次号，最多8字符(含\0) 
    Time StartTime, EndTime; //出发时间与目的时间
    Time SpendTime; //线路上花费的时间
    float SpendMoney; //票价
}TrafficLine;

typedef struct ArcNode {//表结点，边表
    std::string EndName; //该弧末尾的城市
    struct ArcNode *NextLine; //沿弧指向后继表结点的指针
    TrafficLine *Info; //该弧的路线信息 
}LineNode;

typedef struct VNode {//头结点,顶点表
    std::string CityName; //地名
    int CityOrd; //城市编号,便于匹配
    LineNode *FirstLine;//沿第一条弧指向后继表结点的指针
    int Amount;//交通工具班次
}VNode;

 //顶点节点，保存id和到源顶点的估算距离,优先队列需要的类型
struct Node; //计算最小花费
struct Node1; //计算最小路径

class ALGraph{
public:
    ALGraph(int size);
    ~ALGraph();
    int searchCityNum(const std::string CityName); //查询城市编号
    void addCity(const std::string CityName); //手动添加城市
    void addCityFromFile(const char FileName[MaxFileSize]); //从文件读取以添加城市
    void insert(std::string StartName, LineNode* temp, std::string EndName ); //插入线路
    void addLine(); //添加线路
    void addLineFromFile(const char FileName[MaxFileSize]); //从文件读取以添加线路
    void reSize(int size); //重新分配城市列表，仅考虑城市增多情况
    void delCityLine(int i); //删除以城市为起点的线路
    void delLine(); //删除线路
    void delCity(std::string CityName); //删除城市，并删除以该城市为起点的航班和列车
    void updateFile(const char FileName[MaxFileSize], const std::string type); //修改后更新文件

    void showCity(); //输出所有城市
    void showLine(); //输出所有线路
    void dijkstra_Money(int v0, int *parent, Node *dis); //最小花费路径
    int timeTransWeight (const Time& t); //时间转化为权值,即转化为分钟
    void dijkstra_Time(int v0, int *parent, Node1 *dis); //最小时间路径
    void showShortestPath(const std::string type); //调用并打印最短路径

private:
    VNode *CityList; //用链表数组表示城市列表
    int CityNum; //城市个数，即顶点数
    int MaxCityNum; //当前最大城市个数
    int ArcNum; // 弧个数
};
#endif
