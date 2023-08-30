#ifndef _SEARCH_H
#define _SEARCH_H

//----------------这个头文件声明了搜索函数和关于游戏结果的一些类和函数--------------

#include<deque>
#include<stack>             //使用stl的栈和队列

class ans_node{             //表示游戏结果的节点，按照“数字+正负号”的形式表示操作
public:         //无参构造
    void setAns_node(int &act);     //设置当前移动情况
    int &getAct();
    char &getDir();
private:
    int m_act;                      //数字动作
    char m_dir;                     //正负号表示右螺旋旋转、左螺旋旋转
};

namespace answer{           //储存游戏结果和搜索最大深度
    using namespace std;
    const int MAXSTEP=10;
    int now_step=0;
    deque<ans_node> ans;
}

void createChild(Cube* &child,Cube* &crntnode,int &act);

void printAns(std::deque<ans_node> ans);            //输出结果

bool search(Cube &c);              //搜索函数
#endif