/*************************************************************************
	> File Name: ALGraph.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年01月02日 星期二 09时49分43秒
 ************************************************************************/

#include<iostream>
#include"ALGraph.h"
using namespace std;
istream& operator >> (istream &in, Time &time) { //全局函数 输入重载 
    char c1, c2, c3;
    in >> time.hour >> c1 >> time.minute >> c2 >> c3 >> time.day;
    return in;
}

ALGraph::ALGraph (int size) { //构造函数
    CityList = new VNode[size];
    int i;
    for (i = 0; i < size; ++i) {
        CityName = "NULL";
        CityOrd = i;
        FirstLine = NULL;
        Amount = 0;
    }
    CityNum = 0;
    MaxCityNum = size;
    ArcNum = 0;
}

int ALGraph::searchCityNum (const string CityName) const { //查询城市编号，找不到返回 -1
    int i;
    for (i = 0; i < CityNum; ++i) {
        if (CityList[i].CityName == CityName) {
            return i;
        }
    }
    return -1;
}

void ALGraph::reSize(int size) {
    if (size <= CityNum) {
        return;
    }
    VNode *NewList = new VNode[size];
    int i;
    for (i = 0; i < CityNum; ++i) {
        NewList[i] = CityList[i];
    }

    for (i = CityNum; i < size; ++i) {
        CityName = "NULL";
        CityOrd = i;
        FirstLine = NULL;
        Amount = 0;
    }
    delete []CityList;
    CityList = NewList;
}

void ALGraph::addCity (const string CityName) {
    if (searchCityNum(CityName) != -1) {
    
    }
    if (CityNum >= City)
}
