#include<iostream>
#include<string>
#include"ALGraph.h"

using std::cin;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;
using std::string;

string Account = "admin";
string Password = "admin";
ALGraph graph;
void admin();
bool login();
void user();
void changePassword();
void adminALG();


int main(void) {
    system("clear");
    int identity = 1; //身份选项 1.游客 2.管理员，输入 0 用于退出
    while (identity) {
        cout << endl << "==============欢迎全国交通咨询系统==============" << endl;
        cout << "你的身份是？" << endl;
        cout << "    1.管理员" << endl << "    2.普通用户" << endl;
        cout << "请输入数字(0 for exit)：";
        cin >> identity;
        if (!identity) {
            break;
        }
        switch(identity) {
            case 1:
                admin();
                system("clear");
                break;

            case 2:
                user();
                system("clear");
                break;

            default:
                cout << "输入不正确，请重新输入!" << endl;
                break;
        }//switch(identity)
    }//while (indentity)
    system("clear");
    cout << endl << "==================感谢你的使用==================" << endl;
    return 0;
}//main

//管理员界面
void admin() {
    cout <<"    开启管理员模式！" << endl;
    while(!login()) {
        cout << endl << "帐号或密码不正确，请再次输入" << endl;
    }
    cout << endl << "登陆成功！" << endl;
    int option = 1;
    while (option) {
        cout << "请输入你的选项：" << endl;
        cout << "1.更改密码" << endl << "2.全国交通图的相关操作" << endl;
        cout << "请输入数字(按0返回选择身份界面)：" << endl;
        cin >> option;
        cout << endl;
        system("clear");
        if (!option) {
            break;
        }
        switch (option) {
            case 1:
                changePassword();
                break;
            case 2:
                adminALG();
                system("clear");
                break;
            default:
                cout << "请输入正确的选项！" << endl;
                break;
        }
    }//while(!login())
    cout << endl << endl << "操作完毕，退出管理员用户！" << endl << endl;
}//admin

//登入管理员账户函数
bool login () {
    string account, password;
    cout << "输入账户：";
    cin >> account;
    cout << "输入密码：";
    system("stty -echo");
    cin >> password;
    system("stty echo");
    if (account == Account && password == Password) {
        return true;
    }
    else {
        return false;
    }
}//login

//修改管理员账户密码函数
void changePassword () {
    string NewPassword1 = "1", NewPassword2 = "2";
    while (NewPassword1 != NewPassword2) {
        cout << "请输入新密码：";
        system("stty -echo");
        cin >> NewPassword1;
        system("stty echo");
        cout << "请再次输入密码：";
        system("stty -echo");
        cin >> NewPassword2;
        system("stty echo");
        if (NewPassword1 != NewPassword2) {
            cout << "两次输入的密码不同，请再次输入！" << endl;
        }
    }
    Password = NewPassword1;
    cout << "修改密码成功！" << endl;
}//changePassword

//管理员管理交通图的操作函数
void adminALG () {
    int func = 1;
    while (func) {
        cout << "请选择下列操作之一" << endl;
        cout << "    1.从文件中添加城市" << endl;
        cout << "    2.手动添加城市" << endl;
        cout << "    3.删除城市" << endl;
        cout << "    4.从文件中添加线路" << endl;
        cout << "    5.手动添加线路" << endl;
        cout << "    6.删除线路" << endl;
        cout << "    7.显示所有城市" << endl;
        cout << "    8.显示所有线路" << endl;
	    cout << "    9.查询两城市间，中转最少的路径" << endl;
        cout << "    10.查询两城市间，花费最少的路径" << endl;
        cout << "    11.查询两城市间，耗时最少的路径" << endl;
        
        cout << "请输入数字(输入0退出操作界面)：";
        cin >> func;
        cout << endl;
        system("clear");

        //对应功能的调用
        if (!func) {
            break;
        }

        // switch 内部不能定义对象，只能写在这里
        // 否则报错：cannot jump from switch statement to this case
        string file_name;
        string city_name;
        string sc;
        string ec;
        string amt;
        switch(func) {
            case 1:    
                cout << "    1.从文件中添加城市（可选文件：FlightCity.txt, TrainCity.txt)，请输入文件名：";
                cin >> file_name;
                graph.addCityFromFile (file_name.c_str());
                break;
            case 2:
                cout << "    2.手动添加城市!" << endl;
                cout << "请输入城市名字：";
                cin >> city_name;
                graph.addCity(city_name);
                break;
            case 3:
                cout << "    3.删除城市!" << endl;
                cin >> city_name;
                graph.delCity(city_name);
                break;
            case 4:
                cout << "    4.从文件中添加路线（可选文件：Flight.txt, Train.txt），请输入文件名：";
                cin >> file_name;
                graph.addLineFromFile (file_name.c_str());
                break;
            case 5:
                cout << "    5.手动添加线路!" << endl;
                graph.addLine();
                break;
            case 6:
                cout << "    6.删除线路!" << endl;
                cout << "请输入要删除线路的起点城市：" << endl;
                cin >> sc;
                cout << "请输入要删除线路的终点城市：" << endl;
                cin >> ec;
                cout << "请输入要删除线路的班次号：" << endl;
                cin >> amt;
                graph.delLine(sc, ec, amt); //删除线路
                break;
            case 7:
                cout << "    7.显示所有城市!" << endl;
                graph.showAllCity();
                break;
            case 8:
                cout << "    8.显示所有线路!" << endl;
                graph.showAllLine();
                break;
            case 9:
	            cout << "    9.查询两城市间，中转最少的路径!" << endl;
                cout << "输入要查询的起点城市：";
                cin >> sc;
                cout << "输入要查询的终点城市：";
                cin >> ec;
                graph.printLeastTransferPath(sc, ec);
                break;
            //case 9:
            //    cout << "    9.保存修改到文件!" << endl;
            //    ALG[option-2].updateFile(cityfile[option-2], "City");
            //    ALG[option-2].updateFile(cityfile[option-2], "Line");
            //    break;
	        case 10:
		        cout << "    10.查询两城市间，花钱最少的路径!" << endl;
                cout << "输入要查询的起点城市：";
                cin >> sc;
                cout << "输入要查询的终点城市：";
                cin >> ec;
		        graph.printLeastMoneyPath(sc, ec);
		        break;
	        case 11:
        	    cout << "    11.查询两城市间，耗时最少的路径!" << endl;
                cout << "输入要查询的起点城市：";
                cin >> sc;
                cout << "输入要查询的终点城市：";
                cin >> ec;
                graph.printLeastTimePath(sc, ec);
		        break;
            default:
                cout << "输入不正确，请重新输入" << endl;
                break;
        }//switch(func) 
        cout << endl;
    }//while(func)
}//adminGPL 管理员对图进行操作 

//用户登陆界面
void user() {
    cout << "    开启普通用户模式" << endl << endl;
    int func = 1;
    while (func) {
        cout << "    现在是全国交通图的信息查询！" << endl;
        cout << "请选择下列操作之一" << endl;
        cout << "    1.查询两城市间，花费最少的路径" << endl;
        cout << "    2.查询两城市间，耗时最少的路径" << endl;
        cout << "    3.显示所有城市" << endl;
        cout << "    4.显示所有线路" << endl;
        cout << "    5.查询两城市间，中转次数最少的路径" << endl;
        cout << "请输入数字(按0返回选择身份选择界面):";
        cin >> func;
        if (!func) {
            break;
        }
        cout << endl;

        // switch 内部不能定义对象，只能写在这里
        // 否则报错：cannot jump from switch statement to this case
        string sc;
        string ec;
        switch(func) {
            case 1: 
                cout << "1.查询两城市间，花费最少的路径" << endl;
                cout << "输入要查询的起点城市：";
                cin >> sc;
                cout << "输入要查询的终点城市：";
                cin >> ec;
                graph.printLeastMoneyPath(sc, ec);
                break;
            case 2:
                cout << "2.查询两城市间，耗时最少的路径" << endl;
                cout << "输入要查询的起点城市：";
                cin >> sc;
                cout << "输入要查询的终点城市：";
                cin >> ec;
                graph.printLeastTimePath(sc, ec);
                break;
            case 3:
                cout << "3.显示所有城市！" << endl;
                graph.showAllCity();
                break;
            case 4:
                cout << "4.显示所有线路！" << endl;
                graph.showAllLine();
                break;
            case 5:
                cout << "5.查询两城市间，中转次数最少的路径" << endl;
                cout << "输入要查询的起点城市：";
                cin >> sc;
                cout << "输入要查询的终点城市：";
                cin >> ec;
                graph.printLeastTransferPath(sc, ec);
                break;
            default:
                cout << "输入不正确，请重新输入！" << endl;
                break;
        }//switch(func)
        cout << endl;
        
    }//while(func)
}//user 
