#include"ALGraph.h"
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<queue>
#include<stack>
#include<algorithm>
#define INF 0xfffff

using std::cin;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
using std::istringstream;
using std::setw;
using std::setiosflags;
using std::resetiosflags;
using std::ios_base;
using std::ifstream;
using std::string;
using std::vector;
using std::map;
using std::set;
using std::pair;
using std::make_pair;

const Time getTimeByMinute (int minute) {
    int day = minute / 1440;
    minute -= day * 1440;
    int hour = minute / 60;
    minute -= hour * 60;
    return Time(hour, minute, day);
}

// Time 对象的"<"重载，实现两个 Time 对象的比较
bool operator < (Time t1, Time t2) {
    return ((t1.day * 1440 + t1.hour * 60 + t1.minute) < (t2.day *1440 + t2.hour * 60 + t2.minute));
}

// Time 对象的"-"重载，实现两 Time 对象做减法，默认 t2 晚于 t1 时才可以做减法
Time operator - (Time t1, Time t2) throw (std::runtime_error) {

    if (t1 < t2) {
        throw std::runtime_error ("Time Operator - 运算中 t1 小于 t2，结果将是负数!");
    }

    Time tmp_t;
    
    tmp_t.minute = t1.minute - t2.minute;
    if (tmp_t.minute < 0) {     
        tmp_t.minute += 60;
        --t1.hour;
    }
    
    tmp_t.hour = t1.hour - t2.hour;
    if (tmp_t.hour < 0) {
        tmp_t.hour += 24;
        --t1.day;
    }
    
    tmp_t.day = t1.day - t2.day;
    
    return tmp_t;
}

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

//运算符'<<'重载以美化 Time 对象的输入
ostream& operator << (ostream &out, const Time &time) {
    //数据之间空白用'0'填充
    cout.fill('0');

    //数据的右对齐与天数的'+'号
    out << setw(2) << setiosflags(ios_base::right) <<time.hour << ":" 
        << setw(2) << time.minute << resetiosflags(ios_base::right) 
        << "," << setiosflags(ios_base::showpos) << time.day << resetiosflags(ios_base::showpos); //setiosflags(ios_base::showpos) 表示 "+" 正号

    cout.fill(' '); //fill setiosflags 需手动关闭

    return out;
} 

//运算符'<<'重载以美化 LineNode 对象的输入
ostream& operator << (ostream &out, const LineNode &line) {

    cout << "出发城市|到达城市|班次名|出发时间|到达时间|路程用时|路程票价" << endl;
    cout << setw(10) << line.start_city_name << " " <<  setw(10) << line.end_city_name 
        << " " << setw(6) << line.amount << " ";
    cout.fill('0'); // 修改 fill() 的默认填充符为 0
    cout << line.start_time << " " << line.end_time << " " << line.spend_time << " "
        << setiosflags(ios_base::showpoint) << line.spend_money << endl;
    cout.fill(' '); // fill() 不像 setw(), 需要手动调整

    return out;
} 

//查询城市是否存在
bool ALGraph::ifCityExist (const std::string &city_name) {  // 查询城市是否存在

    auto it = m.find(Vnode(city_name));
    if (it != m.cend()) {
        return true;
    } else {
        return false;
    }

}


//查询城市编号，找到返回序号，找不到返回-1
int ALGraph::searchCityNum (const string &city_name) {
    
    auto it = m.cbegin();
    while (it != m.cend()) {
        if (city_name == it->first.start_city_name) {
            return (*it).first.city_id;
        }

        ++it;
    }

    return -1;
}//searchCityNum 

// 手动添加城市
void ALGraph::addCity (const string &city_name) {

    if (ifCityExist(city_name)) {
        cout << "该城市已经存在，无需重复添加！" << endl;
        return;
    }
    
    try {
        m.insert( {Vnode(city_name, city_num), vector<LineNode>() } );
        ++city_num;
    } catch (std::runtime_error err) {
        cout << "添加城市时报错（可能是城市已存在），报错信息如下：" << err.what() << endl;
    }
}

// 从文件中读取以添加城市
void ALGraph::addCityFromFile (const char FILENAME[MAXFILESIZE]) { 
    
    cout << "开始从" << FILENAME << "中读入并添加城市！" << endl; 
    ifstream file (FILENAME);  // 打开失败返回NULL
    if (!file) {
        cout << "打开文件失败！" << endl;
        return;
    }
    
    // 读取城市名
    string line;
    while (getline(file, line)) {
        istringstream istr(line);  // 利用 istringstream 类直接将 string 输入到 name
        string name;
        istr >> name;
        addCity (name);
    }
    cout << "城市导入完毕，当前系统中有" << city_num << "个城市！" << endl;
    file.close();  // 读取后关闭文件
}//addCityFromFile

//手动添加线路
void ALGraph::addLine () {
    string start_city_name;
    //信息输入
    cout << "请输入起点城市：";
    cin >> start_city_name;
    
    if (!ifCityExist(start_city_name)) {
        cout << "起点该城市并不存在，请先创建该城市！" << endl;
        return;
    }
 
    string end_city_name;
    Time start_time, end_time;
    Time spend_time;
    float spend_money;
    string amount;  // 火车或飞机的班次

    cout << "请输入终点城市：";
    cin >> end_city_name;
    if (!ifCityExist(end_city_name)) {
        cout << "终点城市" << end_city_name << "并不存在，请先创建该城市！" << endl;
        return;
    }
    cout << "请输入班次名：";
    cin >> amount;
    cout << "请输入出发时间(格式为hh:mm,+d)：";
    cin >> start_time;
    cout << "请输入到达时间(格式为hh:mm,+d)：";
    cin >> end_time;

    if (start_time < end_time) {
        spend_time = end_time - start_time;
    } else{
        cout << "出发时间大于到达时间，输入错误，请重新输入！" << endl;
        return;
    }

    cout << "请输入票价：";
    cin >> spend_money;

    auto it = m.find(Vnode(start_city_name));
    if (it != m.end()) {
        (*it).second.push_back(LineNode (start_city_name, end_city_name, start_time, end_time, 
                    spend_time, spend_money, amount));
        cout << "添加路线成功！" << endl;
    }

    ++line_num;

}//addLine 

//从文件中读取以添加线路
void ALGraph::addLineFromFile (const char FILENAME[MAXFILESIZE]) {
    cout << "从" << FILENAME << "中读取并导入线路！" << endl;
    ifstream file(FILENAME);
    if(!file) {
        cout << "打开文件" << FILENAME << "失败！" << endl;
        return;
    }
    string catalogue; //存储第一行的目录
    getline(file, catalogue);
    
    //从第二行开始读取真正需要存储的信息
    string line;
    int temp_line_num = line_num;
    while (getline(file, line)) {
        istringstream istr(line);
        string start_city_name;
        string end_city_name;
        Time start_time, end_time;
        Time spend_time;
        float spend_money;
        string amount;  // 火车或飞机的班次

        istr >> start_city_name >> end_city_name >> amount >> start_time >> end_time >> spend_time >> spend_money;

        auto it = m.find(Vnode(start_city_name));
        if (it != m.end()) {
            (*it).second.push_back (LineNode (start_city_name, end_city_name, start_time, end_time, spend_time,
                        spend_money, amount) );
            ++line_num;
        }
    }
    file.close(); //打开存储线路的文件完毕，关闭file
    cout << "从文件" << FILENAME << "中导入了 " << line_num - temp_line_num << " 条路线信息！" << endl;
}//addLineFromFile

// 删除城市，并删除以该城市为起点与终点的航班和列车
void ALGraph::delCity (const string &city_name) {

    if (!ifCityExist(city_name)) {
    
        cout << city_name << "不存在！请重新输入正确的城市名或者新建该城市！" << endl;
        return;
    }

    cout << "正在删除" << city_name << "城市及其相关线路！" << endl;
    city_num = city_num - 1;

    // 删除以该城市为起点的线路数据
    auto key = Vnode(city_name);
    auto _it = m.find (key);
    if (_it != m.end()) {
        line_num = line_num - (*_it).second.size();
        m.erase(key);
    }

    // 删除以该城市为终点的线路数据
    for (auto it = m.begin(); it != m.end(); ++it) {
    
        auto &vec = (*it).second;
        auto beg = vec.begin();
        while (beg != vec.end()) {
            if ((*beg).end_city_name == city_name) {
                beg = vec.erase(beg);
                line_num = line_num - 1;
            } 
            else {
                ++beg;
            }
        }
    } 

    cout << "已删除" << city_name << "城市及其相关线路！" << endl;

}//delCity

void ALGraph::delLine (const string &sc, const string &ec, const string &amt) {

    if (!ifCityExist(sc)) {
        cout << sc << "不存在！请重新输入正确的城市名或者新建该城市！" << endl;
        return;
    }
    if (!ifCityExist(ec)) {
        cout << ec << "不存在！请重新输入正确的城市名或者新建该城市！" << endl;
        return;
    }
    auto line_node_vec = getLineNode(sc, ec, amt);
    if (line_node_vec.size() == 0) {
        cout << "从" << sc << "到" << ec << "，班次号为" << amt << "的线路并不存在，请重新输入！" << endl;
    } 

    cout << "正在删除从" << sc << "到" << ec << "，班次号为" << amt << "的线路！" << endl;

    auto it = m.find(Vnode(sc));
    for (auto beg = (*it).second.begin(); beg != (*it).second.end(); ++beg) {
    
        if ( ((*beg).end_city_name == ec) && ((*beg).amount == amt)) {
            beg = (*it).second.erase(beg);
            break;
        }
    }

    cout << "已删除从" << sc << "到" << ec << "，班次号为" << amt << "的线路！" << endl;

}//delLine

////把修改完毕的数据更新到文件中
//void ALGraph::updateFile(const char FileName[MAXFILESIZE], const string type) {
//    ofstream file(FileName);
//    if (!file) {//判断文件是否存在并能打开
//        cout << "不能打开文件" << FileName << " ，只能写入已存在的文件" << FileName << "，请确认文件是否存在！" << endl;
//        return;
//    }
//    
//    int i;
//    LineNode * p = NULL;
//    if (type == "City" || type == "城市") {
//        //第一行导入城市个数
//        file << CityNum << endl;
//        for (i = 0; i < CityNum; ++i) {
//            file << CityList[i].CityName << endl;
//        }
//        cout << "将城市信息更新到 " << FileName << " 完成!" << endl;
//    }//更新文件中的城市
//
//    else if (type == "Line" || type == "线路") {
//        file << "出发城市|到达城市|班次名|出发时间|到达时间||||用时|||票价" << endl;
//        for (i = 0; i < CityNum; ++i) {
//            for (p = CityList[i].FirstLine; p != NULL; p = p->NextLine) {
//                file << setw(8) << CityList[i].CityName << " " << setw(8) << p->EndName << " "<< setw(6) << p->Info->LineName << " ";
//                cout.fill('0');
//                file << p->Info->StartTime << " " << p->Info->EndTime << " " << p->Info->SpendTime << " " << setiosflags(ios_base::showpoint) << p->Info->SpendMoney << resetiosflags(ios_base::showpoint);
//                cout.fill(' ');
//            }
//        }
//        cout << "将线路信息更新到 " << FileName << " 完成!" << endl;  
//    }//更新文件中的线路
//    file.close();
//}//updateFile 

//输出所有城市
void ALGraph::showAllCity() {

    if (city_num == 0) {
        cout << "系统中没有任何城市的信息!" << endl;
        return;
    }

    cout << "系统中有 " << city_num  << " 座城市的信息" << endl;
    auto it = m.cbegin();
    while (it != m.cend()) {
        cout << (*it).first.start_city_name << endl;
        //cout << (*it).first.city_id << endl;  // 打印城市id
        ++it;
    }

}//showAllCity 

void ALGraph::showAllLine () {

    if (line_num == 0) {
        cout << "系统中没有任何线路的信息！" << endl;
        return;
    } else {
        cout << "系统当前一共有 " << line_num << " 条线路的信息！" << endl;
    }
    auto it = m.cbegin();
    while (it != m.cend()) {
        for (auto beg = (*it).second.cbegin(); beg != (*it).second.cend(); ++beg) {
            
            cout << (*beg);
        }
        ++it;
    }
}  // showAllLine 

// 通过起点城市、终点城市、班次，查询一条线路信息
vector<LineNode> ALGraph::getLineNode (const std::string sc, const std::string ec, const std::string amt) {

    vector<LineNode> result;
    auto it = m.find (Vnode(sc));

    for(auto beg = it->second.begin(); beg != it->second.end(); ++beg) {

        if ((beg->end_city_name == ec) && (beg->amount == amt)) {
            result.push_back(*beg);
        }
    }

    return result;
}  // getLineNode

// 输出从起点城市到终点城市的线路
std::vector<std::vector<LineNode>> ALGraph::getPathsByCity (const std::string &sc, const std::string &ec) {

    cout << "正在查询从" << sc << "到" << ec << "的线路信息！" << endl;

    if (line_num == 0) {
        cout << "系统中没有任何线路！" << endl;
        return vector<vector<LineNode>>();
    } 

    auto it = m.find(sc);
    if (it == m.cend()) {
        cout << "没有任何从" << sc << "出发的线路！" << endl;
        return vector<vector<LineNode>> ();
    } 
    else {
        if ((*it).second.size() == 0) {
            cout << "没有任何从" << sc << "到其他城市的线路！" << endl;
            return vector<vector<LineNode>> ();
        }
        
        std::vector<std::vector<LineNode> > result;  // 存放 sc 到 ec 的多条路径

        auto vec = (*it).second;
        for (auto beg = vec.cbegin(); beg != vec.cend(); ++beg) {

            if ((*beg).end_city_name == ec) {

                vector<LineNode> direct_path;
                direct_path.push_back(*beg);
                result.push_back(direct_path);
                continue;
            }
        
            vector<LineNode> path;  // 用于DFS时存储路径
            set<string> visited_1;  // 利用班次号判断一条路线是否已被访问
            //set<pair<string, string>> visited_2;  // 如果只用班次号的 visited 会有 BUG, 会不断重入同一个城市，
                                                // （还是有BUG）解决办法是综合利用"班次" + "pair<终点，起点>" 去环
                                                //              还是有 BUG，无法去掉间接的环，以及错剪问题 
            set<string> visited_2;  // 用城市名去环，判断邻接边的"end_city_name"是否已在 visit_2 中，
                                    // 如果存在，说明加入该边的话就会入环，因此不加入该边，以达到不重入旧城市的目的
            
            // 自定义DFS根节点访问操作，比如这里的存储路径
            path.push_back(*beg);
            visited_1.insert (beg->amount);
            //visited_2.insert (make_pair(beg->end_city_name, beg->start_city_name));
            
            visited_2.insert (beg->start_city_name);  // set 能一直保持建唯一，所以不用担心插入重复键的问题
            //visited_2.insert (beg->end_city_name);

            while (!path.empty()) {
                LineNode lnode = path.back();
                
                //访问相邻的线路，并在 visited 中标记
                vector<LineNode> possible_way = m.at(Vnode(lnode.end_city_name));
                std::size_t i;
                for (i = 0; i < possible_way.size(); ++i) {

                    auto n = possible_way[i];
                    //if (visited.find(n.amount) == visited.end()) {
                    //if (( visited_1.find(n.amount) == visited_1.end() ) &&
                    //    ( visited_2.find(make_pair(n.start_city_name, n.end_city_name)) == visited_2.end() )) {
                    if (( visited_1.find(n.amount) == visited_1.end() ) &&
                        ( visited_2.find(n.end_city_name) == visited_2.end() )) {

                            path.push_back(n);
                            visited_1.insert (n.amount);
                            //VISITED.insert (make_pair(n.start_city_name, n.end_city_name));
                            //VISITED.insert (make_pair(n.end_city_name, n.start_city_name));
                            //visited_2.insert (make_pair(n.end_city_name, n.start_city_name));
                            visited_2.insert (n.start_city_name);
                            //cout << "被加入的边的开始城市为" << n.start_city_name << "，此后该城市不可访问" << endl;
                            //visited_2.insert (n.end_city_name);
                            //cout << "被加入的边的末尾城市为" << n.start_city_name << endl;
                            

                        if (n.end_city_name == ec) {  // 在这里自定义DFS的访问操作，比如保存路径
                            result.push_back(path);
                            path.pop_back();
                            break;  // 必须 break，不然就变成广度遍历了
                        } 
                        else {
                            break;  // 必须 break，不然就变成广度遍历了
                        }
                    }
                }

                // 当前指向的节点没有邻边，或所有结点都已经被访问
                if (i == possible_way.size()) {
                    path.pop_back();
                }
            }
        }

        return result;
    }

} // getPathsByCity

// 打印从起点城市到终点城市的所有路径
void ALGraph::printPathsByCity (const std::string &sc, const std::string &ec) {

    auto path_vec = getPathsByCity(sc, ec);

    if (path_vec.size() == 0) {
        cout << "从" << sc << "到" << ec << "并没有路径！" << endl;    
        return;
    }

    unsigned i = 0;
    for (auto path : path_vec) {

        ++i;
        cout << "从" << sc << "到" << ec<< "的第" << i << "条路径如下：" << endl;
        for (auto line : path) {

                    cout << line;
        }
    }
}  // printPathsByCity

// 输出从起点城市到终点城市，中转次数最少的路径
void ALGraph::printLeastTransferPath (const std::string &sc, const std::string &ec) {

    if ((!ifCityExist(sc)) && (!ifCityExist(ec))) {
        cout << "系统中未存在" << sc << "和" << ec << "这两座城市，请先添加这两座城市！" << endl;
        return;
    }

    if (!ifCityExist(sc)) {
        cout << "系统中未存在" << sc << "这座城市，请先添加该城市！" << endl;
        return;
    }

    if (!ifCityExist(ec)) {
        cout << "系统中未存在" << ec << "这座城市，请先添加该城市！" << endl;
        return;
    }

    auto path_vec = getPathsByCity(sc, ec);

    if (path_vec.size() == 0) {
        cout << "从" << sc << "到" << ec<< "没有可以通行的路径！" << endl;
        return;
    }

    size_t pos = -1;
    size_t min_size = INT_MAX;

    for (size_t i=0; i < path_vec.size(); ++i) {

        if (path_vec[i].size() < min_size) {
            min_size = path_vec[i].size();
            pos = i;
        }
    }

   vector<LineNode> min_transfer_path = path_vec.at(pos);

   cout << "从" << sc << "到" << ec << "的中转次数最少的路径如下：" << endl;
   //cout << "出发城市|到达城市|班次名|出发时间|到达时间||||用时|||票价" << endl;
   for (auto line : min_transfer_path) {

       cout << line;
   }
   cout << "从" << sc << "到" << ec << "的最少中转次数为：" << min_transfer_path.size() << endl;

}

// 输出从起点城市到终点城市，总时间最短的线路
void ALGraph::printLeastTimePath (const std::string &sc, const std::string &ec) {

    if ((!ifCityExist(sc)) && (!ifCityExist(ec))) {
        cout << "系统中未存在" << sc << "和" << ec << "这两座城市，请先添加这两座城市！" << endl;
        return;
    }

    if (!ifCityExist(sc)) {
        cout << "系统中未存在" << sc << "这座城市，请先添加该城市！" << endl;
        return;
    }

    if (!ifCityExist(ec)) {
        cout << "系统中未存在" << ec << "这座城市，请先添加该城市！" << endl;
        return;
    }

    auto path_vec = getPathsByCity(sc, ec);

    if (path_vec.size() == 0) {
        cout << "从" << sc << "到" << ec<< "没有可以通行的路径！" << endl;
        return;
    }

    int min_time_path_idx = -1;
    int min_time_sum = INT_MAX;
    int path_num_idx = -1;
    for (const auto &path : path_vec) {
        
        ++path_num_idx;
        
        int time_sum = 0;
        for (size_t i=0; i < path.size(); ++i) {
            if (i == 0) {  // 某路径的第一条线路，不用考虑隔天等待的问题
                time_sum += path.at(i).spend_time.getTotalMintue();
            }
            else {  // 需要考虑中转时，隔天等待的问题
                if (path.at(i-1).end_time < path.at(i).start_time) {  // 可以当天中转下一班次，不用等待

                    time_sum += path.at(i).spend_time.getTotalMintue();
                }
                else {  // 不能当天中转下一班次，需要隔一天

                    Time next_station_start_time = path.at(i).start_time;
                    next_station_start_time.day += 1; 
                    time_sum = time_sum + (next_station_start_time - path.at(i-1).end_time).getTotalMintue()
                                    + path.at(i).spend_time.getTotalMintue();
                }
            }
        }
        if (time_sum < min_time_sum) {
            min_time_sum = time_sum;
            min_time_path_idx = path_num_idx;
        }
    }

    cout << "从" << sc << "到" << ec << "的耗时最少的路径如下：" << endl;
    for (auto line : path_vec.at(min_time_path_idx)) {
        cout << line;
    }
    Time min_time_obj = getTimeByMinute(min_time_sum);
    cout << "从" << sc << "到" << ec << "的最少耗时为：" << min_time_obj.day << "天"
       << min_time_obj.hour << "小时" << min_time_obj.minute << "分钟！" << endl;

}

// 输出某个城市到其他各城市的花费最少的路线
void ALGraph::printLeastMoneyPath (const std::string &sc, const std::string &ec) {
    
    if ((!ifCityExist(sc)) && (!ifCityExist(ec))) {
        cout << "系统中未存在" << sc << "和" << ec << "这两座城市，请先添加这两座城市！" << endl;
        return;
    }

    if (!ifCityExist(sc)) {
        cout << "系统中未存在" << sc << "这座城市，请先添加该城市！" << endl;
        return;
    }

    if (!ifCityExist(ec)) {
        cout << "系统中未存在" << ec << "这座城市，请先添加该城市！" << endl;
        return;
    }

    
    struct cmp_pair_ec_and_amount_by_ec {
    	bool operator() (const pair<string, string> &x, const pair<string, string> &k) const {
    			if(x.first >= k.first)
    			return false;  //STL源码规定必须这么返回，而不能改成 true
    			else
    			return true;
    	}
    };

    // 这里采用 dijkstra 算法求解
    // 其实也可以采用DFS求解，比如调用 getPathsByCity, 然后计算一下钱再比个大小就行
    set<string> visited;  // 利用城市名的唯一性建立 visited，表示该结点已计算出最小距离，无需再次计算
    map<string, float> distanced;  // distanced 为"城市名 -> 估计最小距离"的映射
    map<pair<string, string>, string, cmp_pair_ec_and_amount_by_ec> 
        parent;  // parent 存储路径上结点的关系，可以沿 ec 一直找到 sc，映射结构是 <终点城市, 班次> -> 起点城市
    vector<LineNode> result;  // 存储最终要打印的路径

    // 初始化 distance 信息集合，以用于松弛操作
    // 不需要初始化 visit 和 parent，visit 只需要判断城市名在不在 set 里就能知道该城市是否被访问，
    // parent 只需要判断 map 中有没有某城市名这个键就知道路径怎么走
    auto m_iter = m.cbegin();
    while (m_iter != m.cend()) {
        distanced.insert ( {(*m_iter).first.start_city_name, INT_MAX} );
        ++m_iter;
    }

    // 初始化 distanced 中起点城市的信息
    auto _dist_iter = distanced.find(sc);
    if (_dist_iter != distanced.end()) {
        (*_dist_iter).second = 0;
    }

    
    for (int i=0; i < city_num; ++i) {

        map<string, float>::const_iterator min_dist_iter = distanced.cend();  // 指向 dist 中的值最小的结点
        float min_dist_value = INT_MAX;

        auto dist_iter = distanced.cbegin();
        while (dist_iter != distanced.cend()) {
            
            if ((visited.find(dist_iter->first) == visited.cend()) && ((dist_iter->second) <= min_dist_value)) {
                
                min_dist_value = dist_iter->second;
                min_dist_iter = dist_iter;
            }
            ++dist_iter;
        }

        if (min_dist_iter == distanced.cend()) {
            break;  // 起点到所有其他结点的最小距离都计算完毕
        }
        
        visited.insert (min_dist_iter->first);
        //cout << min_dist_iter->first << "  " << min_dist_iter->second << endl;
        visited.insert (min_dist_iter->first);

        // 刚刚被访问的最小距离的结点为 m.at(min_dister->first)
        // 执行松弛操作，更新该结点所有邻接结点的最小估计距离
        const auto &vec = m.at (min_dist_iter->first);
        for (auto lnode : vec) {
            
            if (visited.find(lnode.end_city_name) == visited.cend()) {
                if ((min_dist_iter->second + lnode.spend_money) < distanced.at(lnode.end_city_name)) {

                    distanced[lnode.end_city_name] = min_dist_iter->second + lnode.spend_money;
                    auto __it_parent = parent.find(make_pair(lnode.end_city_name, lnode.amount));
                    if (__it_parent == parent.end()) {
                        parent[make_pair(lnode.end_city_name, lnode.amount)] = lnode.start_city_name;
                    }
                    else {
                        // 如果键已存在，则删除旧键，因为旧键中的 amount 是旧信息，而且没法被直接覆盖
                        parent.erase(__it_parent);  
                        parent[make_pair(lnode.end_city_name, lnode.amount)] = lnode.start_city_name;
                    }
                }
            }
        }
    }

    //cout << "我到 1104 行了！！！" << endl;
    // （已解决）有 BUG! parent 得到的数据并不是正确的数据：{{南昌, T162}, 永安}, {{上海, T184}, 南昌}
    //     而是 BUG 数据：{{南昌, T162}, 永安}, {{福州, T161}, 永安}
    //cout  << "执行完 dijkstra 后，parent 中的元素个数是" << parent.size() << endl;
    //for (auto __it = parent.cbegin(); __it != parent.cend(); ++__it) {
    //    cout << __it->first.first << "  " << __it->first.second << "  " << __it->second << endl;
    //}

    // 执行完 dijkstra 算法后，根据 parent 得到最优路径
    // 假设 sc = 福州，ec = 南昌，求福州到南昌花费最少的路线
    // parent 中的数据格式为：
    //     {(福州, T131), 永安}, {(上海, T106), 福州}
    auto iter_parent = parent.find(make_pair(ec, ""));

    // （已解决）输出结果是错的！此时 dist["南昌"] = 26, 南昌的前一个结点应该是永安
    //cout << "代码到了1118 行！" << endl;
    //cout << distanced.at("南昌") << endl;
    //cout << iter_parent->second << "    "  << iter_parent->first.first << "    " << iter_parent->first.second << endl;

    if (iter_parent == parent.cend()) {
        
        cout << "系统中没有路径可以从" << sc << "到达" << ec << "！" << endl; 
    } 
    else {
        // 提取 parent 中存储的路径，存入最终的结果 result
        while (iter_parent != parent.cend()) {
            
            auto ret = getLineNode (iter_parent->second, iter_parent->first.first, iter_parent->first.second);
            result.push_back(ret.back());
            iter_parent = parent.find(make_pair(iter_parent->second, ""));
        }

        std::reverse (result.begin(), result.end());  // 由于 parent 中线路是反向存储，要将 result 中的线路反转
        cout << "从" << sc << "到" << ec << "的花费最少的路径如下：" << endl;
        //cout << "出发城市|到达城市|班次名|出发时间|到达时间||||用时|||票价" << endl;
        for (auto line : result) {

            cout << line;
            //cout << setw(8) << line.start_city_name << " " <<  setw(8) << line.end_city_name 
            //    << " " << setw(6) << line.amount << " ";
            //cout.fill('0'); // 修改 fill() 的默认填充符为 0
            //cout << line.start_time << " " << line.end_time << " " << line.spend_time << " " 
            //    << setiosflags(ios_base::showpoint) << line.spend_money << endl;
            //cout.fill(' '); // fill() 不像 setw(), 需要手动调整
            //cout << endl;
        }
        cout << "从" << sc << "到" << ec << "的最少花费" << distanced.at(ec) << "元！" << endl;
        
    }
    
}

//int main() {
//
//    ALGraph graph;
//    graph.addLine();
//    graph.addCityFromFile("TrainCity.txt");
//    graph.addLineFromFile("Train.txt");
//    //graph.showAllLine();
//
//    graph.printPathsByCity ("永安", "上海");
//    
//    //graph.printLeastTransferPath ("永安", "上海");
//    
//    graph.printLeastTimePath("永安", "上海");
//    
//    //graph.printLeastMoneyPath ("永安", "上海");  
//
//
//    //++graph.city_num;
//    //++graph.line_num;
//    //cout << "当前系统中的城市与线路总数分别为: " << graph.city_num << " " << graph.line_num << endl;
//
//    return 0;
//}

