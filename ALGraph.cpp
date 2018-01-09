/*************************************************************************
	> File Name: ALGraph.cpp
	> Author: bw98
	> Mail: 
	> Created Time: 2018年01月02日 星期二 09时49分43秒
 ************************************************************************/
#include"ALGraph.h"
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<queue>
#include<stack>
using namespace std;
#define INF 0xfffff

//'>>' 重载，实现"hour:minute,+day" 的存储
istream& operator >> (istream &in, Time &time) {
    char c1, c2, c3;
    int hour, minute, day;
    in >> hour >> c1 >> minute >> c2 >> c3 >> day;
    time.day = day;
    time.minute = minute;
    time.hour = hour;
    return in;
} 

//运算元'<<'重载以美化 Time 对象的输入
ostream& operator << (ostream &out, Time &time) {
    //数据之间空白用'0'填充
    cout.fill('0');
    //数据的右对齐与天数的'+'号
    out << setw(2) << setiosflags(ios_base::right) <<time.hour << ":" << setw(2) << time.minute << resetiosflags(ios_base::right) << "," << setiosflags(ios_base::showpos) << time.day << resetiosflags(ios_base::showpos); //setiosflags(ios_base::showpos) 表示 "+" 正号
    cout.fill(' '); //fill setiosflags 需手动关闭
    return out;
} 

ALGraph::ALGraph (int size) { 
    CityList = new VNode[size];
    int i;
    for (i = 0; i < size; ++i) {
        CityList[i].CityName = "NULL";
        CityList[i].CityOrd = i;
        CityList[i].FirstLine = NULL;
        CityList[i].Amount = 0;
    }
    CityNum = 0;
    MaxCityNum = size;
    ArcNum = 0;
}//ALGraph::ALGraph (int size)  

//析构函数
ALGraph::~ALGraph() {
    delete []CityList;
}

//查询城市编号，找到返回序号，找不到返回-1
int ALGraph::searchCityNum (const string CityName) {
    int i;
    for (i = 0; i < CityNum; ++i) {
        if (CityList[i].CityName == CityName) {
            return i;
        }
    }
    return -1;
}//searchCityNum 

//调整图存储的大小，只考虑了城市增加情况
void ALGraph::reSize(int size) {
    if (size <= CityNum) {
        return;
    }
    VNode *NewList = new VNode[size];
    int i;
    for (i = 0; i < CityNum; ++i) {
        NewList[i] = CityList[i]; //C++可以直接赋值
    }
    for (i = CityNum; i < size; ++i) {
        CityList[i].CityName = "NULL";
        CityList[i].CityOrd = i;
        CityList[i].FirstLine = NULL;
        CityList[i].Amount = 0;
    }
    delete []CityList;
    CityList = NewList;
    MaxCityNum = size;
}//reSize 

//手动添加城市
void ALGraph::addCity (const string CityName) {
    //已存在城市
    if (searchCityNum(CityName) != -1) {
        cout << "The city has in the list!!!" << endl;
        return;
    }
    //空间已满
    if (CityNum >= MaxCityNum) {
        ++MaxCityNum;
        reSize(MaxCityNum);
    }
    CityList[CityNum].CityName = CityName;
    CityList[CityNum].CityOrd = CityNum;
    CityList[CityNum].FirstLine = NULL;
    CityList[CityNum].Amount = 0;
    ++CityNum;
}//addCity

//从文件中读取以添加城市
void ALGraph::addCityFromFile (const char FileName[MAXFILESIZE]) {
    cout << "开始从 " << FileName << "中读入并添加城市！" << endl;
    ifstream file(FileName); //打开失败返回NULL
    if (!file) {
        cout << "打开失败!" << endl;
        return;
    }
    
    string line; //读取第一行数据，第一行是城市总个数
    getline(file, line);
    istringstream istr(line); //利用stringstream简化 string 到 int 的类型转换
    int count;
    istr >> count;
    
    //空间已满
    if (CityNum + count >= MaxCityNum) {
        MaxCityNum = CityNum + count;
        reSize(MaxCityNum);
    }
    //从第二行开始读取城市名字
    while (getline(file,line)) {
        istringstream istr2(line); //利用istringstream类直接将string输入到name
        string name;
        istr2 >> name;
        addCity(name);
    }
    cout << "城市导入完毕！" << endl;
    file.close(); //读取后关闭文件
}//addCityFromFile

//添加线路
void ALGraph::addLine () {
    string StartName, EndName;
    //信息输入
    cout << "请输入起点城市：" << endl;
    cin >> StartName;
    cout << "请输入终点城市：" << endl;
    cin >> EndName;
    LineNode *temp = new LineNode;
    TrafficLine *info = new TrafficLine;
    temp->EndName = EndName;
    cout << "请输入班次名：";
    cin >> info->LineName;
    cout << "请输入出发时间:(hh:mm,+d)";
    cin >> info->StartTime;
    cout << "请输入到达时间：(hh:mm,+d)";
    cin >> info->EndTime;
    info->SpendTime = info->EndTime - info->StartTime;
    cout << "请输入票价：";
    cin >> info->SpendMoney;
    temp->Info = info;
    insert (StartName, temp, EndName);
}//addLine 

//插入线路
void ALGraph::insert (string StartName, LineNode *temp, string EndName) {
    int StartNum = searchCityNum(StartName);
    if (StartNum == -1) {
        addCity(StartName); //若无城市，则新建
        StartNum = searchCityNum(StartName);
    }
    if (searchCityNum(EndName) == -1) {
        addCity(EndName); //若无城市，则新建
    }
    LineNode *p, *q; //q是p的后继
    p = CityList[StartNum].FirstLine;
    
    //原本没有线路的情况
    if (p == NULL) { 
        CityList[StartNum].FirstLine = temp;
        temp->NextLine = NULL;
    }
    //原本有线路的情况
    else {
        q = p->NextLine; 
        while (q != NULL && EndName != q->EndName) {
            p = q;
            q = q->NextLine;
        }
        p->NextLine = temp;
        temp->NextLine = q;
    }
    CityList[StartNum].Amount++;
    ArcNum++;
}//insert

//从文件中读取以添加线路
void ALGraph::addLineFromFile (const char FileName[MAXFILESIZE]) {
   cout << "从" << FileName << "中读取并导入线路！" << endl;
    ifstream file(FileName);
    if(!file) {
        cout << "不能打开文件！" << endl;
        return;
    }
    string catalogue; //存储第一行的目录
    getline(file, catalogue);
    
    //从第二行开始读取真正需要存储的信息
    string line;
    while (getline(file, line)) {
        istringstream istr(line);
        LineNode *temp = new LineNode;
        TrafficLine *info = new TrafficLine;
        temp->Info = info;
        string StartName,EndName;
        istr >> StartName >> EndName >> info->LineName >> info->StartTime >> info->EndTime >> info->SpendTime >> info->SpendMoney;
        info->SpendTime = info->EndTime -info->StartTime;
        temp->EndName = EndName;
        insert(StartName, temp, EndName);
    }//while (getline(file,line))
    
    file.close(); //打开存储线路的文件完毕，关闭file
    cout << "线路导入完毕！" << endl;
}//addLineFromFile

//删除序号i城市为起点的所有路线
void ALGraph::delCityLine(int i) { 
    LineNode *p,*q;
    p = CityList[i].FirstLine;
    if (p == NULL) { //空表
        return;
    }
    //从第二个表结点开始删
    while(p->NextLine) {
        q = p;
        p = q->NextLine;
        delete q->Info;
        delete q;
    }
    //删除最后一个结点
    if (p->NextLine == NULL) {
        delete p->Info;
        delete p;
    }
}//delCityLine 

//删除城市，并删除以城市为起点的航班和列车
void ALGraph::delCity (string CityName) {
    int i,j;
    i = searchCityNum(CityName);
    //未查询到该城市
    if (i == -1) {
        cout << "没有找到该城市!" << endl;
    }
    else {
        delCityLine(i); //删除该城市为起点的所有线路
        for (j = i; j < CityNum-1; ++j) {
            CityList[j] = CityList[j+1];
        }
        --CityNum;
        cout << "删除 " << CityName << " 成功！ " << endl;
    }
}//delCity

void ALGraph::delLine() {
    string StartName, EndName;
    int StartNum;
    cout << "请输入起点城市: ";
    cin >> StartName;
    StartNum = searchCityNum(StartName);
    if (StartNum == -1) {
        cout << "没有该城市出发的线路!" << endl;
        return;
    }
    cout << "请输入终点城市: ";
    cin >> EndName;
    
    //显示起点城市的所有线路，并判断是否能到达该终点城市
    bool flag = false;
    LineNode *p, *q;
    cout << "起点城市与终点城市的路线有: " << endl;
    cout << "出发城市|到达城市|班次名|出发时间|到达时间||||用时|||票价" << endl;
    for (p = CityList[StartNum].FirstLine; p != NULL; p = p->NextLine) {
        if (p->EndName == EndName) {
            cout << setw(8) << StartName << " " << setw(8) << EndName << " " << setw(6) << p->Info->LineName << " " << p->Info->StartTime << " " << p->Info->EndTime << " " << p->Info->SpendTime << " " << setiosflags(ios_base::showpoint) << p->Info->SpendMoney << resetiosflags(ios_base::showpoint);
            flag = true;
        }
    }
    cout << endl;
    if (flag == false) {
        cout << "该出发城市没有通往目地城市的班次！" << endl;
        return;
    }

    //该出发城市能到达目地城市，开始删除线路
    string LineName;
    cout << "请输入你想要删除的航班号：" << endl;
    cin >> LineName;
    p = CityList[StartNum].FirstLine;
    q = p->NextLine;//q 是 p 的后继
    
    //只有一条路，即一个表结点的情况
    if (q == NULL) {
        if (p->Info->LineName != LineName) { //唯一一条路不是通往目的地的情况
            cout << "没有该班次！" << endl;
            return;
        }else {
            CityList[StartNum].FirstLine = q; //注意原内存需要手动释放
            CityList[StartNum].Amount--;
            ArcNum--;
            //删除内存
            delete p->Info; //将通过指针p,将结构体里的结构体指针指向的内存free掉  
            delete p; //通过p，将结构体的内存free掉
            p = NULL; //置指针为NULL,防止野指针
            return;
        }
    }//if(q == NULL)
    
    //多个表结点的情况
    while (q->Info->LineName != LineName && q->NextLine) {
        p = q;
        q = q->NextLine;
    }

    //跳出循环有两种可能 1.名字相等 2.到头了
    //1.到头了的情况
    if (q->Info->LineName != LineName) {
        cout << "没有班次！" << endl;
        return;
    }
    //2.名字相等的情况
    else {
        p->NextLine = q->NextLine; 
        CityList[StartNum].Amount--;
        ArcNum--;
        delete q->Info;
        delete q;
        return;
    }

}//delLine 

//把修改完毕的数据更新到文件中
void ALGraph::updateFile(const char FileName[MAXFILESIZE], const string type) {
    ofstream file(FileName);
    if (!file) {//判断文件是否存在并能打开
        cout << "不能打开文件" << FileName << " ，只能写入已存在的文件" << FileName << "，请确认文件是否存在！" << endl;
        return;
    }
    
    int i;
    LineNode * p = NULL;
    if (type == "City" || type == "城市") {
        //第一行导入城市个数
        file << CityNum << endl;
        for (i = 0; i < CityNum; ++i) {
            file << CityList[i].CityName << endl;
        }
        cout << "将城市信息更新到 " << FileName << " 完成!" << endl;
    }//更新文件中的城市

    else if (type == "Line" || type == "线路") {
        file << "出发城市|到达城市|班次名|出发时间|到达时间||||用时|||票价" << endl;
        for (i = 0; i < CityNum; ++i) {
            for (p = CityList[i].FirstLine; p != NULL; p = p->NextLine) {
                file << setw(8) << CityList[i].CityName << " " << setw(8) << p->EndName << " "<< setw(6) << p->Info->LineName << " ";
                cout.fill('0');
                file << p->Info->StartTime << " " << p->Info->EndTime << " " << p->Info->SpendTime << " " << setiosflags(ios_base::showpoint) << p->Info->SpendMoney << resetiosflags(ios_base::showpoint);
                cout.fill(' ');
            }
        }
        cout << "将线路信息更新到 " << FileName << " 完成!" << endl;  
    }//更新文件中的线路
    file.close();
}//updateFile 

//输出所有城市
void ALGraph::showCity() {
    int i;
    string a;
    if (CityNum == 0) {
        cout << "系统中没有任何城市的信息!" << endl;
        return;
    }
    cout << "系统中有 " << CityNum  << " 座城市的信息" << endl;
    for (i = 0; i < CityNum; ++i) {
        cout << i << CityList[i].CityName << endl;
    }
}//showCity 

void ALGraph::showLine() {
    if (ArcNum == 0) {
        cout << "系统中没有任何线路的信息！" << endl;
        return;
    }
    cout << "系统中有 " << ArcNum << " 条线路的信息" << endl;
    cout << "出发城市|到达城市|班次名|出发时间|到达时间||||用时|||票价" << endl;
    //输出所有城市线路 
    int i;
    LineNode * p = NULL;
    for (i = 0; i < CityNum; ++i) {
        p = CityList[i].FirstLine;
        while(p) {
            cout << setw(8) << CityList[i].CityName  << " " <<  setw(8) << p->EndName << " " << setw(6) << p->Info->LineName << " ";
            cout.fill('0');
            cout << p->Info->StartTime << " " << p->Info->EndTime << " " << p->Info->SpendTime << " " << setiosflags(ios_base::showpoint) << p->Info->SpendMoney << endl;
            cout.fill(' '); //fill()不像setw(),需要手动调整
            p = p->NextLine;
        }
    }
}//showLine 

//最少花费路径
struct Node {
    int id; //源顶点id
    float money; //估算距离（费用）
    //由于stl中优先队列的第三个参数是greater,而我们需要的是小顶堆，所以因重载运算符 <
    friend bool operator < (struct Node a, struct Node b) {
        return a.money > b.money;
    }
};
void ALGraph::dijkstra_Money (int v0, int *parent, Node *dis) {
    priority_queue<Node> q; //优化插入(更新)和取出最小值两个操作，队列存储最短距离与索引的编号
    //parent[]记录每个顶点的父亲结点
    //dis[]记录源点到每个估算距离，最后更新为源点到所有顶点的最短距离
    bool visited[MaxCityNum]; //判断下标对应的顶点是否算出最短路径或者说是否在最短路径树中
    //初始化
    int i;
    for (i = 0; i < CityNum; ++i) {
        dis[i].id = i;
        dis[i].money = INF;
        parent[i] = -1; //每个顶点都没有父结点
        visited[i] = false; //都未找到最短路
    }
    dis[v0].money = 0; //源点到源点最短路权值为0
    q.push(dis[v0]); //压入队列
    while (!q.empty()) { //队列空说明完成了求解v0到其余各顶点的最短路径
        Node cd = q.top(); //取最小估算距离顶点
        q.pop();
        int u = cd.id;
           
        if (visited[u]) { //被标记了，就无需对其进行更新最短距离等等操作
            continue;  
        }
        visited[u] = true;
        LineNode *p = CityList[u].FirstLine;
        //松弛操作
        while(p) { //找所有与它相邻的顶点，进行松弛操作，更新估算距离，压入队列
            int v = searchCityNum(p->EndName);
            float m = p->Info->SpendMoney;
            if (!visited[v] && dis[v].money > dis[u].money + m) {
                dis[v].money = dis[u].money + m;
                parent[v] = u;
                q.push(dis[v]);
            }
            p = p->NextLine;
        }
    }// while (!q.empty()) 
}//dijkstra_Money

//最少时间路径
struct Node1 {
    int id; //源顶点id
    int tt; //估算距离(时间)
    Time et; //到达时间
    friend bool operator < (struct Node1 a, struct Node1 b){
        return a.tt > b.tt;
    }
};
int ALGraph::timeTransWeight (const Time& t) {
    return (t.day*24 + t.hour)*60 + t.minute;
}

void ALGraph::dijkstra_Time (int v0, int *parent, Node1 *dis) {
    priority_queue<Node1> q1; 
    //parent[]记录每个顶点的父亲结点
    //dis[]记录源点到每个估算距离，最后更新为源点到所有顶点的最短距离
    bool visited[MaxCityNum]; //判断下标对应的顶点是否算出最短路径或者说是否在最短路径树中
    int i;
    for (i = 0; i < CityNum; ++i) {
        dis[i].id = i;
        dis[i].tt = INF;
        dis[i].et = {0, 0, 0};
        parent[i] = -1; //都一个顶点都没有父结点
        visited[i] = false; //都未找到最短路径
    }
    dis[v0].tt = 0;
    q1.push(dis[v0]);
    while (!q1.empty()) {
        Node1 cd = q1.top(); //取出最短距离的点的序号
        q1.pop();
        int u = cd.id;

        if (visited[u]) {
            continue;
        }
        visited[u] = 1;

        LineNode *p = CityList[u].FirstLine;
        //找出所有相邻点，进行松弛操作，即更新dis
        while (p) {
            int v = searchCityNum(p->EndName);
            int t = timeTransWeight(p->Info->SpendTime);
            Time st = p->Info->StartTime; //本条线路开始时间
            //计算中转的时间开销
            if (u != v0) { //注意源点到任何点都没有中转时间
                int change = timeTransWeight(st - dis[u].et); //当前路线的开车时间-始发站的上一到站时间 
                t += change;
            }
            if (!visited[v] && dis[v].tt > dis[u].tt + t) {
                dis[v].tt = dis[u].tt + t;
                dis[v].et = p->Info->EndTime;
                parent[v] = u;
                q1.push(dis[v]);
            }
            p = p->NextLine;
        }//while (p)
    }//while (!q1.empty()) 
}//dijkstra_Time

//调用并打印最短路经
void ALGraph::showShortestPath (const string type){
    string StartCity, EndCity;
    int StartNum, EndNum;
    //确定出发城市
    showCity();
    cout << "城市列表如上，请输入出发城市：";
    cin >> StartCity;
    StartNum = searchCityNum(StartCity);
    //输入城市不存在
    while (StartNum == -1) {
        cout << "查询不到该城市，请再次输入出发城市：" << endl;
        cin >> StartCity;
        StartNum = searchCityNum(StartCity);
    }
    //确定到达目的城市
    showCity();
    cout << "城市列表如上，请输入目的城市：";
    cin >> EndCity;
    EndNum = searchCityNum(EndCity);
    //输入城市不存在
    while (EndNum == -1) {
        cout << "查询不到该城市，请再次输入目的城市：" << endl;
        cin >> EndCity;
        EndNum = searchCityNum(EndCity);
    }

    int path[MaxCityNum]; //记录每个顶点的父亲结点，相当于是一条路径
    int time_minute = 0; //起始地到达目的地的最少时间， 单位:分钟
    float money_yuan = 0; //起始地到达目的地的最少费用，单位:元

    if (type == "Money") {
        Node dis[MaxCityNum];
        dijkstra_Money(StartNum, path, dis);
        money_yuan = dis[EndNum].money;
    }
    else {
        Node1 dis[MaxCityNum];
        dijkstra_Time(StartNum, path, dis);
        time_minute = dis[EndNum].tt;
    }

    //若v0 到 EndCity 不存在路，即EndCity 在最短路径树中没有父结点
    if (path[EndNum] == -1) {
        cout << "对不起，从" << StartCity << "到" << EndCity << "暂时没有路线到达！" << endl;
        return;
    }
    else {
        stack<int> s;
        int step = EndNum;
        while (step != StartNum) {
            s.push(step);
            step = path[step];
        }
        int father = step;
        int child;

        //输出最省钱路径
        if (type == "Money") {
            cout << "最省钱路径" << endl;
            int tt = 0; //total time,总时间开销
            Time et = {0, 0, 0};
            while (!s.empty()) {
                child = s.top();
                s.pop();
                LineNode *p = CityList[father].FirstLine;
                float mm = INF; //min money,当前记录到的最少金钱,不是总金钱
                int i = 0;
                int count; //记录指针移动的次数，方便定位
                while (p) {
                    if (p->EndName == CityList[child].CityName && mm >= p->Info->SpendMoney) {
                        mm = p->Info->SpendMoney;
                        count = i;
                    }
                    p = p->NextLine;
                    ++i;
                }
                p = CityList[father].FirstLine;
                i = 0;
                while (i != count) {
                    p = p->NextLine;
                    ++i;
                }
                tt += timeTransWeight(p->Info->SpendTime);
                if (father != StartNum) {
                    tt = tt + timeTransWeight(p->Info->StartTime - et);
                    cout << "需要中转等待 " << timeTransWeight(p->Info->StartTime - et) << "分钟！" << endl;
                }
                    cout << setw(8) << CityList[father].CityName << " " << setw(8) << p->EndName << " " << setw(6) << p->Info->LineName << " ";
                    cout.fill('0');
                    cout << p->Info->StartTime << " " << p->Info->EndTime << " " << p->Info->SpendTime << " " << setiosflags(ios_base::showpoint) << p->Info->SpendMoney << endl; 
                    cout.fill(' ');

                    et = p->Info->EndTime;
                    father = child;
                }//while(!s.empty())
                cout << "一共花费" << money_yuan << "元和" << tt << "分钟！" << endl;
            }//输出最省钱路径

        //输出最省时间的路径
        else {
            cout << "最省时间路径: " << endl;
            float mm = 0; //总共需要的金钱
            Time et = {0, 0, 0};
            while (!s.empty()) {
                child = s.top();
                s.pop();
                LineNode *p = CityList[father].FirstLine;
                int tt = INF; //当前记录到的最小时间,不是总时间
                int ot = 0; //ot 是算上换乘的总时间
                int i = 0, count; //count记录指针移动的次数，方便定位
                while (p) {
                    if (p->EndName == CityList[child].CityName) {
                        if (!s.empty() && child != EndNum) {
                            ot = timeTransWeight(p->Info->SpendTime) + timeTransWeight(p->Info->StartTime - et);
                        }
                        else {
                            ot = timeTransWeight(p->Info->SpendTime);
                        }
                        if (tt >= ot) {
                            tt = ot;
                            count = i;
                        }
                    }
                    p = p->NextLine;
                    ++i;
                }//while (p)
                p = CityList[father].FirstLine;
                i = 0;
                while (i != count) {
                    p = p->NextLine;
                    mm += p->Info->SpendMoney;
                    ++i;
                }
                if (father != StartNum) {
                    cout << "需要中转等待 " << timeTransWeight(p->Info->StartTime - et) << "分钟！" << endl;
                }
                cout << setw(8) << CityList[father].CityName << " " << setw(8) << p->EndName << " " << setw(6) << p->Info->LineName << " ";
                cout.fill('0');
                cout << p->Info->StartTime << " " << p->Info->EndTime << " " << p->Info->SpendTime << " " << setiosflags(ios_base::showpoint) << p->Info->SpendMoney << endl; //showpoint只对float有效,不用手动取消
                cout.fill(' ');
                
                et = p->Info->EndTime;
                father = child;
            }//while(!s.empty())
            cout << "一共花费" << mm << "元和" << time_minute << "分钟！" << endl; 
        }//输出最省时间的路径
    }//有路径
}//showShortestPath
