
//-----------贪吃蛇小游戏-----------

/*
    这个贪吃蛇小游戏基于广度优先搜索,先搜索出附近容易获得的食物,再通过曼哈顿距离确定前往食物的最佳路径,搜索食物的过程中已经考虑
    了其他蛇,陷阱等
*/
#include<iostream>
#include<deque>
#include<cmath>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define ROW 30              //地图的参数宏
#define COLOMN 40
#define HEAD -3
#define BODY -3
using namespace std;

//-----------全局变量----------
int Scene[ROW][COLOMN]={0};         //游戏地图
int lefttime=0;                           //剩余游戏时刻
int k=0;                            //剩余道具数量
int sum=0;                           // 场上剩余蛇的数量
const int my_id=2022201308;             //我的学号

int target_x=ROW/4,target_y=COLOMN/3;
int d[4][2]={{0,-1},{-1,0},{0,1},{1,0}};

class node{                         //节点类    蛇的身子,食物和陷阱将由一些节点表示
public:
    node(int x=0,int y=0):m_x(x),m_y(y){};
    int getX(){
        return m_x;
    }
    int getY(){
        return m_y;
    }
private:
    int m_x;                            //行坐标
    int m_y;                            //列坐标
};

class Snake{                            //蛇类
public:
    Snake(){}
    Snake(int id,int snakeSize,int score,int dir,int cool,int duration,const deque<node> &body,bool dead=0);
    friend void get_info();
    void move();                    //移动
    void dead_behave();                 //死亡与否
    void drop();                    //死亡掉金币
    int crash();                    //发生了碰撞
    friend int search(node target);
    friend void setTarget();
    friend node find_food();
    friend bool danger();
private:
    deque<node> s_snakeBody;             //蛇全身体
    node s_snakeHead;                     //蛇头
    int s_snakeSize;                      //蛇身长度
    int s_score;                          //蛇的得分
    int s_shieldDuration;                 //护盾持续时间
    int s_shieldCooldown;                 //护盾剩余时间
    int s_dir;                              //蛇目前移动的方向
    bool s_dead;                            //蛇是否死
    int s_id;                          //学号
};

Snake me;                   //自己
deque<Snake> others;        //其他蛇

class Food{                             //食物类,一个节点成员表示食物的坐标,value值表示食物所加分数或增长豆
public:
    Food(const node &pos,int value=0);
public:
    node f_pos;
    int f_value;
};

class Trap{                                     //陷阱类,类似于食物类,value值用来标识陷阱
public:
    Trap(const node &pos,int value=0);
public:
    node t_pos;
    int t_value;
};

//----------以下为全局的辅助函数声明-----------

void get_info();                //输入数据
int search(node target);                  //搜索函数
void setTarget();               //在计算路径的时寻找目标
deque<node> openList;
deque<node> abandonList;        //用于A*寻路
node find_food();
bool danger();                  //评估附近是否有危险,决定是否开盾

//----------以下为蛇类的函数实现---------------

Snake::Snake(int id,int snakeSize,int score,int dir,int cool,int duration,const deque<node> &body,bool dead){
    s_id=id;
    s_snakeSize=snakeSize;
    s_score=score;
    s_dir=dir;
    s_shieldCooldown=cool;
    s_shieldDuration=duration;
    s_snakeBody=body;
    s_snakeHead=s_snakeBody.at(0);
}

void Snake::move(){             //移动
    if(s_dir==0){               //左
        for(int i=s_snakeSize;i>0;i--){
            s_snakeBody[i-1]=s_snakeBody[i];
        }
        s_snakeBody.pop_back();
        s_snakeBody.push_front(node(s_snakeHead.getX(),s_snakeHead.getY()-1));
        s_snakeHead=s_snakeBody.at(0);
    }
    if(s_dir==1){               //上
        for(int i=s_snakeSize;i>0;i--){
            s_snakeBody[i-1]=s_snakeBody[i];
        }
        s_snakeBody.pop_back();
        s_snakeBody.push_front(node(s_snakeHead.getX()-1,s_snakeHead.getY()));
        s_snakeHead=s_snakeBody.at(0);
    }
    if(s_dir==2){               //右
        for(int i=s_snakeSize;i>0;i--){
            s_snakeBody[i-1]=s_snakeBody[i];
        }
        s_snakeBody.pop_back();
        s_snakeBody.push_front(node(s_snakeHead.getX(),s_snakeHead.getY()+1));
        s_snakeHead=s_snakeBody.at(0);
    }
    if(s_dir==3){               //下
        for(int i=s_snakeSize;i>0;i--){
            s_snakeBody[i-1]=s_snakeBody[i];
        }
        s_snakeBody.pop_back();
        s_snakeBody.push_front(node(s_snakeHead.getX()+1,s_snakeHead.getY()));
        s_snakeHead=s_snakeBody.at(0);
    }
}

void Snake::dead_behave(){
    drop();
    s_dead=true;
}

void Snake::drop(){
    int n=s_score/10;
    for(int i=0;i<n;i++){
        int x=s_snakeBody.at(i).getX();
        int y=s_snakeBody.at(i).getY();
        Scene[x][y]=10;
    }
    int r=s_score%10;
    if(!r){
        int x=s_snakeBody.at(n).getX();
        int y=s_snakeBody.at(n).getY();
        Scene[x][y]=r;
    }
    s_snakeSize=0;
}

int Snake::crash(){             //0没发生碰撞,1普通豆,2增长豆,3陷阱,4自己,5其他蛇
    int nx=s_snakeHead.getX();
    int ny=s_snakeHead.getY();

    if(Scene[nx][ny]==1||Scene[nx][ny]==2||Scene[nx][ny]==3||Scene[nx][ny]==5){     //吃到普通豆
        if((s_score+Scene[nx][ny])/10==s_score/10+1){
            if(s_snakeSize>1){                          //蛇有多节变长
                int x=s_snakeBody.back().getX();
                int y=s_snakeBody.back().getY();
                if(s_snakeBody.at(s_snakeSize-1).getX()==x && s_snakeBody.at(s_snakeSize-1).getY()==y-1){
                    if(x>=0&&x<ROW&&y-1>=0&&y-1<COLOMN)
                        s_snakeBody.push_back(node(x,y-1));
                }
                if(s_snakeBody.at(s_snakeSize-1).getX()==x && s_snakeBody.at(s_snakeSize-1).getY()==y+1){
                    if(x>=0&&x<ROW&&y+1>=0&&y+1<COLOMN)
                        s_snakeBody.push_back(node(x,y+1));
                }
                if(s_snakeBody.at(s_snakeSize-1).getX()==x-1&& s_snakeBody.at(s_snakeSize-1).getY()==y){
                    if(x-1>=0&&x-1<ROW&&y>=0&&y<COLOMN)
                        s_snakeBody.push_back(node(x-1,y));
                }
                if(s_snakeBody.at(s_snakeSize-1).getX()==x+1&& s_snakeBody.at(s_snakeSize-1).getY()==y){
                    if(x+1>=0&&x+1<ROW&&y>=0&&y<COLOMN)
                        s_snakeBody.push_back(node(x+1,y));
                }
            }
            if(s_snakeSize==1){                     //蛇只有一节变长
                int r=rand()%4;
                if(r==0){
                    if(s_snakeBody.at(0).getX()>=0&&s_snakeBody.at(0).getX()<ROW&&s_snakeBody.at(0).getY()+1>=0&&s_snakeBody.at(0).getY()+1<COLOMN)
                        s_snakeBody.push_back(node(s_snakeBody.at(0).getX(),s_snakeBody.at(0).getY()+1));
                }
                if(r==1){
                    if(s_snakeBody.at(0).getX()>=0&&s_snakeBody.at(0).getX()<ROW&&s_snakeBody.at(0).getY()-1>=0&&s_snakeBody.at(0).getY()-1<COLOMN)
                        s_snakeBody.push_back(node(s_snakeBody.at(0).getX(),s_snakeBody.at(0).getY()-1));
                }
                if(r==2){
                    if(s_snakeBody.at(0).getX()+1>=0&&s_snakeBody.at(0).getX()+1<ROW&&s_snakeBody.at(0).getY()>=0&&s_snakeBody.at(0).getY()<COLOMN)
                        s_snakeBody.push_back(node(s_snakeBody.at(0).getX()+1,s_snakeBody.at(0).getY()));
                }
                if(r==3){
                    if(s_snakeBody.at(0).getX()-1>=0&&s_snakeBody.at(0).getX()-1<ROW&&s_snakeBody.at(0).getY()>=0&&s_snakeBody.at(0).getY()<COLOMN)
                        s_snakeBody.push_back(node(s_snakeBody.at(0).getX()-1,s_snakeBody.at(0).getY()));
                }
                s_snakeHead=s_snakeBody.at(0);
            }
            s_snakeSize++;
        }
        s_score+=Scene[nx][ny];
        return 1;
    }
    if(Scene[nx][ny]==-1){                    //吃到增长豆
        if(s_snakeSize>1){                          //蛇有多节变长
                int x=s_snakeBody.back().getX();
                int y=s_snakeBody.back().getY();
                if(s_snakeBody.at(s_snakeSize-1).getX()==x && s_snakeBody.at(s_snakeSize-1).getY()==y-1){
                    if(x>=0&&x<ROW&&y-1>=0&&y-1<COLOMN)
                        s_snakeBody.push_back(node(x,y-1));
                }
                if(s_snakeBody.at(s_snakeSize-1).getX()==x && s_snakeBody.at(s_snakeSize-1).getY()==y+1){
                    if(x>=0&&x<ROW&&y+1>=0&&y+1<COLOMN)
                        s_snakeBody.push_back(node(x,y+1));
                }
                if(s_snakeBody.at(s_snakeSize-1).getX()==x-1&& s_snakeBody.at(s_snakeSize-1).getY()==y){
                    if(x-1>=0&&x-1<ROW&&y>=0&&y<COLOMN)
                        s_snakeBody.push_back(node(x-1,y));
                }
                if(s_snakeBody.at(s_snakeSize-1).getX()==x+1&& s_snakeBody.at(s_snakeSize-1).getY()==y){
                    if(x+1>=0&&x+1<ROW&&y>=0&&y<COLOMN)
                        s_snakeBody.push_back(node(x+1,y));
                }
        }
            if(s_snakeSize==1){                     //蛇只有一节变长
                int r=rand()%4;
                if(r==0){
                    if(s_snakeBody.at(0).getX()>=0&&s_snakeBody.at(0).getX()<ROW&&s_snakeBody.at(0).getY()+1>=0&&s_snakeBody.at(0).getY()+1<COLOMN)
                        s_snakeBody.push_back(node(s_snakeBody.at(0).getX(),s_snakeBody.at(0).getY()+1));
                }
                if(r==1){
                    if(s_snakeBody.at(0).getX()>=0&&s_snakeBody.at(0).getX()<ROW&&s_snakeBody.at(0).getY()-1>=0&&s_snakeBody.at(0).getY()-1<COLOMN)
                        s_snakeBody.push_back(node(s_snakeBody.at(0).getX(),s_snakeBody.at(0).getY()-1));
                }
                if(r==2){
                    if(s_snakeBody.at(0).getX()+1>=0&&s_snakeBody.at(0).getX()+1<ROW&&s_snakeBody.at(0).getY()>=0&&s_snakeBody.at(0).getY()<COLOMN)
                        s_snakeBody.push_back(node(s_snakeBody.at(0).getX()+1,s_snakeBody.at(0).getY()));
                }
                if(r==3){
                    if(s_snakeBody.at(0).getX()-1>=0&&s_snakeBody.at(0).getX()-1<ROW&&s_snakeBody.at(0).getY()>=0&&s_snakeBody.at(0).getY()<COLOMN)
                        s_snakeBody.push_back(node(s_snakeBody.at(0).getX()-1,s_snakeBody.at(0).getY()));
                }
                s_snakeHead=s_snakeBody.at(0);
            }
            s_snakeSize++;
        return 2;
    }
    if(Scene[nx][ny]==-2){                    //吃到陷阱
        if(s_snakeSize>1){
            int x=s_snakeBody.back().getX();
            int y=s_snakeBody.back().getY();
            Scene[x][y]=0;
            s_snakeBody.pop_back();
            s_snakeSize--;
        }
        if(s_score<10)
            s_score=0;
        if(s_score>=10)
            s_score-=10;
        return 3;
    }
    if(Scene[nx][ny]==-3){                      //是蛇
        if(s_shieldDuration==0){
            bool is_self=false;
            for(size_t i=0;i<s_snakeBody.size();i++){
                if(s_snakeBody.at(i).getX()==nx&&s_snakeBody.at(i).getY()==ny){
                    is_self=true;
                    break;
                }
            }
            if(is_self)                                 //是自己身体
                return 4;
            if(!is_self){                                //不是自己身体
                dead_behave();
                return 5;
            }

        }
        if(s_shieldDuration>0){
            return 4;
        }
    }
    return 0;                                 
}

//-------------以下为食物类函数实现------------

Food::Food(const node &pos,int value){
    f_pos=pos;
    f_value=value;
}

//-------------以下为陷阱类函数实现-----------

Trap::Trap(const node &pos,int value){
    t_pos=pos;
    t_value=value;
}

//-------------以下为辅助函数的实现-------------
void get_info(){
    cin>>lefttime;

    cin>>k;                     //输入道具
    for(int i=0;i<k;i++){
        int x,y,val;
        x=y=val=0;
        cin>>x>>y>>val;
        Scene[x][y]=val;
    }

    cin>>sum;                   //输入现存蛇
    for(int i=0;i<sum;i++){
        int id,size,score,dir,cool,duration;
        cin>>id>>size>>score>>dir>>cool>>duration;

        deque<node> is_me;
        for(int i=0;i<size;i++){
            int x,y;
            cin>>x>>y;
            if(id==my_id)               //是自己
                is_me.push_back(node(x,y));
            Scene[x][y]=-3;
        }
        
        if(id==my_id){                  //me赋初值
            me.s_dir=dir;
            me.s_id=id;
            me.s_score=score;
            me.s_shieldCooldown=cool;
            me.s_shieldDuration=duration;
            me.s_snakeBody=is_me;
            me.s_snakeHead=is_me.at(0);
            me.s_snakeSize=size;
        }
    }
}

int search(node target){                                               //设定目标之后的贪心
    int r=2000,tdir=0;
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COLOMN;j++){
            if(Scene[i][j]==-2){
                openList.push_back(node(i,j));
            }
            if(Scene[i][j]==-3){
                if(me.s_snakeSize==1){
                    openList.push_back(node(i,j));
                    continue;
                }
                if(me.s_snakeSize>1){
                    if(i==me.s_snakeBody.at(1).getX()&&j==me.s_snakeBody.at(1).getY()){
                        openList.push_back(node(i,j));
                    }
                }
                bool myself=false;
                for(size_t k=0;k<me.s_snakeBody.size();k++){
                    if(i==me.s_snakeBody.at(k).getX()&&j==me.s_snakeBody.at(k).getY()){
                        myself=true;
                        break;
                    }
                }
                if(!myself)
                    openList.push_back(node(i,j));
            }
        }
    }
    
    for(int dir=0;dir<4;dir++){
        int x=me.s_snakeHead.getX();
        int y=me.s_snakeHead.getY();
        x+=d[dir][0];
        y+=d[dir][1];

        if(x>=0 && x<ROW && y>=0 && y<COLOMN){
            bool ok=true;
            for(size_t i=0;i<openList.size();i++){
                if(x==openList.at(i).getX()&&y==openList.at(i).getY()){
                    ok=false;
                    break;
                }
            }

            if(ok){
                int nowr=abs(target.getX()-x)+abs(target.getY()-y);
                if(r>nowr){
                    tdir=dir;
                    r=nowr;
                }
            }
        }
    }
    if(me.s_shieldDuration==0){
        if(me.s_shieldCooldown==0 && me.s_score>20){
            if(danger()){
                tdir=4;
            }
        }
    }

    return tdir;
}

void setTarget(){
    int x=me.s_snakeHead.getX();
    int y=me.s_snakeHead.getY();

    int manhaton=2000;              //计算食物点的曼哈顿距离
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COLOMN;j++){
            if(Scene[i][j]==1||Scene[i][j]==2||Scene[i][j]==3||Scene[i][j]==5){
                int h=abs(i-x)+abs(y-j);
                if(h<=manhaton){
                    target_x=i;
                    target_y=j;
                }
            }
        }
    }
}

node find_food(){
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COLOMN;j++){
            if(Scene[i][j]==-2){
                abandonList.push_back(node(i,j));
            }
            if(Scene[i][j]==-3){
                if(me.s_snakeSize==1){
                    abandonList.push_back(node(i,j));
                    continue;
                }
                if(me.s_snakeSize>1){
                    if(i==me.s_snakeBody.at(1).getX()&&j==me.s_snakeBody.at(1).getY()){
                        abandonList.push_back(node(i,j));
                    }
                }
                bool myself=false;
                for(size_t k=0;k<me.s_snakeBody.size();k++){
                    if(i==me.s_snakeBody.at(k).getX()&&j==me.s_snakeBody.at(k).getY()){
                        myself=true;
                        break;
                    }
                }
                if(!myself)
                    abandonList.push_back(node(i,j));
            }
        }
    }

    deque<node> q;
    node head=me.s_snakeHead;
    q.push_back(head);

    while(!q.empty()){
        head=q.front();
        int x=head.getX(),y=head.getY();
        if(Scene[x][y]>=1 && Scene[x][y]<=10){
            return head;
        }
        q.pop_front();

        for(int dir=0;dir<4;dir++){
            x+=d[dir][0];
            y+=d[dir][1];
            if(x<0||x>ROW||y<0||y>COLOMN){
                x-=d[dir][0];
                y-=d[dir][1];
                continue;
            }

            node next(x,y);

            bool ok=true;
            for(size_t i=0;i<abandonList.size();i++){
                if(x==abandonList.at(i).getX()&&y==abandonList.at(i).getY()){
                    ok=false;
                    x-=d[dir][0];
                    y-=d[dir][1];
                    break;
                }
            }
            
            if(ok){
                q.push_back(next);
                abandonList.push_back(next);
                x-=d[dir][0];
                y-=d[dir][1];
            }
        }
    }
    return me.s_snakeHead;
}

bool danger(){
    for(int i=-2;i<=2;i++){
        for(int j=-2;j<=2;j++){
            int x=me.s_snakeHead.getX()+i;
            int y=me.s_snakeHead.getY()+j;

            if(x<0||x>ROW||y<0||y>COLOMN)
                continue;
            if(x==0&&y==0)
                continue;

            if(Scene[x][y]==-3){
                bool myself=false;
                for(size_t k=0;k<me.s_snakeBody.size();k++){
                    if(x==me.s_snakeBody.at(k).getX() && y==me.s_snakeBody.at(k).getY()){
                        myself=true;
                        break;
                    }
                }

                if(!myself){
                    return true;
                }
            }
        }
    }
    return false;
}

int main(void){
    memset(Scene,0,sizeof(Scene));
    get_info();
    //setTarget();
    //cout<<target_x<<target_y;
    //int res=search();
    //cout<<res;
    node res=find_food();
    int ans=search(res);
    cout<<ans;
    
    return 0;
}