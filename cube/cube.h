#ifndef _CUBE_H
#define _CUBE_H

#include<iostream>
#include<fstream>
#include<deque>
#include<stack>             //使用stl的栈和队列

class Cube{
public:
    Cube(){}
    Cube(const Cube &c);
    void inputCube(const std::string &filename);
    bool operator==(const Cube &that) const;
    bool isTarget() const;
    void move(int act);
    int m_action;
    char m_up[3][3];
    char m_down[3][3];
    char m_left[3][3];
    char m_right[3][3];
    char m_front[3][3];
    char m_back[3][3];
};

void copy(Cube &target,Cube &source);

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