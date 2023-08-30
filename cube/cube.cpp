#include"cube.h"

//--------------这里是Cube类的各个成员函数的实现------------

Cube::Cube(const Cube &c){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            m_up[i][j]=c.m_up[i][j];
            m_down[i][j]=c.m_down[i][j];
            m_left[i][j]=c.m_left[i][j];
            m_right[i][j]=c.m_right[i][j];
            m_front[i][j]=c.m_front[i][j];
            m_down[i][j]=c.m_down[i][j];
        }
    }
}

void Cube::inputCube(const std::string &filename){                 //从文件中读取魔方状态
    using namespace std;
    ifstream ifs;
    ifs.open(filename,ios::in);

    char buf[128];                      //数组暂时存储1行的数据
    if(ifs.is_open()){
        while(ifs.getline(buf,sizeof(buf))){
            if(buf[0]=='u'){                        //读取上面
                for(int i=0;i<3;i++){
                    ifs.getline(buf,sizeof(buf));           
                    m_up[i][0]=buf[0];
                    m_up[i][1]=buf[2];
                    m_up[i][2]=buf[4];
                }
                ifs.getline(buf,sizeof(buf));
            }
            if(buf[0]=='f'){                            //读取前面
                for(int i=0;i<3;i++){
                    ifs.getline(buf,sizeof(buf));
                    m_front[i][0]=buf[0];
                    m_front[i][1]=buf[2];
                    m_front[i][2]=buf[4];
                }
                ifs.getline(buf,sizeof(buf));
            }
            if(buf[0]=='l'){                            //读取左面
                for(int i=0;i<3;i++){
                    ifs.getline(buf,sizeof(buf));
                    m_left[i][0]=buf[0];
                    m_left[i][1]=buf[2];
                    m_left[i][2]=buf[4];
                }
                ifs.getline(buf,sizeof(buf));
            }
            if(buf[0]=='r'){
                for(int i=0;i<3;i++){
                    ifs.getline(buf,sizeof(buf));
                    m_right[i][0]=buf[0];
                    m_right[i][1]=buf[2];
                    m_right[i][2]=buf[4];
                }
                ifs.getline(buf,sizeof(buf));
            }
            if(buf[0]=='d'){
                for(int i=0;i<3;i++){
                    ifs.getline(buf,sizeof(buf));
                    m_down[i][0]=buf[0];
                    m_down[i][1]=buf[2];
                    m_down[i][2]=buf[4];
                }
                ifs.getline(buf,sizeof(buf));
            }
            if(buf[0]=='b'){
                for(int i=0;i<3;i++){
                    ifs.getline(buf,sizeof(buf));
                    m_back[i][0]=buf[0];
                    m_back[i][1]=buf[2];
                    m_back[i][2]=buf[4];
                }
                ifs.getline(buf,sizeof(buf));
            }
        }
    }else{
        cout<<"error!";
    }
    ifs.close();
}

bool Cube::operator==(const Cube &that) const{
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(!((m_back[i][j]==that.m_back[i][j]) && (m_up[i][j]==that.m_up[i][j]) && (m_left[i][j]==that.m_left[i][j]) &&
                (m_right[i][j]==that.m_right[i][j]) && (m_front[i][j]==that.m_front[i][j]) && (m_down[i][j]==that.m_down[i][j])))
                    return false;
        }
    }
    return true;
}

bool Cube::isTarget() const{
    bool cdn[6]={0};        //六个面颜色是否各自相同
    if(m_up[0][0]==m_up[1][1] && m_up[0][1]==m_up[1][1] && m_up[0][2]==m_up[1][1] &&        //上面颜色是否相同
    m_up[1][0]==m_up[1][1] && m_up[1][2]==m_up[1][1] && m_up[2][0]==m_up[1][1] && 
    m_up[2][1]==m_up[1][1] && m_up[2][2]==m_up[1][1])
        cdn[0]=1;
    if(m_left[0][0]==m_left[1][1] && m_left[0][1]==m_left[1][1] && m_left[0][2]==m_left[1][1] &&        //左面颜色是否相同
    m_left[1][0]==m_left[1][1] && m_left[1][2]==m_left[1][1] && m_left[2][0]==m_left[1][1] && 
    m_left[2][1]==m_left[1][1] && m_left[2][2]==m_left[1][1])
        cdn[1]=1;
    if(m_right[0][0]==m_right[1][1] && m_right[0][1]==m_right[1][1] && m_right[0][2]==m_right[1][1] && 
    m_right[1][0]==m_right[1][1] && m_right[1][2]==m_right[1][1] && m_right[2][0]==m_right[1][1] && 
    m_right[2][1]==m_right[1][1] && m_right[2][2]==m_right[1][1])
        cdn[2]=1;
    if(m_back[0][0]==m_back[1][1] && m_back[0][1]==m_back[1][1] && m_back[0][2]==m_back[1][1] && 
    m_back[1][0]==m_back[1][1] && m_back[1][2]==m_back[1][1] && m_back[2][0]==m_back[1][1] && 
    m_back[2][1]==m_back[1][1] && m_back[2][2]==m_back[1][1])
        cdn[3]=1;
    if(m_front[0][0]==m_front[1][1] && m_front[0][1]==m_front[1][1] && m_front[0][2]==m_front[1][1] && 
    m_front[1][0]==m_front[1][1] && m_front[1][2]==m_front[1][1] && m_front[2][0]==m_front[1][1] && 
    m_front[2][1]==m_front[1][1] && m_front[2][2]==m_front[1][1])
        cdn[4]=1;
    if(m_down[0][0]==m_down[1][1] && m_down[0][1]==m_down[1][1] && m_down[0][2]==m_down[1][1] && 
    m_down[1][0]==m_down[1][1] && m_down[1][2]==m_down[1][1] && m_down[2][0]==m_down[1][1] && 
    m_down[2][1]==m_down[1][1] && m_down[2][2]==m_down[1][1])
        cdn[5]=1;

    return cdn[0] && cdn[1] && cdn[2] && cdn[3] && cdn[4] && cdn[5];
}

void Cube::move(int act){
    if(act==0){
        char front_color[3]={m_front[0][0],m_front[1][0],m_front[2][0]};
        char up_color[3]={m_up[0][0],m_up[1][0],m_up[2][0]};
        char back_color[3]={m_back[0][0],m_back[1][0],m_back[2][0]};
        char down_color[3]={m_down[0][0],m_down[1][0],m_down[2][0]};

        m_front[0][0]=down_color[0];
        m_front[1][0]=down_color[1];
        m_front[2][0]=down_color[2];

        m_up[0][0]=front_color[0];
        m_up[1][0]=front_color[1];
        m_up[2][0]=front_color[2];

        m_back[0][0]=up_color[0];
        m_back[1][0]=up_color[1];
        m_back[2][0]=up_color[2];

        m_down[0][0]=back_color[0];
        m_down[1][0]=back_color[1];
        m_down[2][0]=back_color[2];
    }else if(act==1){
        char front_color[3]={m_front[0][0],m_front[1][0],m_front[2][0]};
        char up_color[3]={m_up[0][0],m_up[1][0],m_up[2][0]};
        char back_color[3]={m_back[0][0],m_back[1][0],m_back[2][0]};
        char down_color[3]={m_down[0][0],m_down[1][0],m_down[2][0]};

        m_front[0][0]=up_color[0];
        m_front[1][0]=up_color[1];
        m_front[2][0]=up_color[2];

        m_up[0][0]=back_color[0];
        m_up[1][0]=back_color[1];
        m_up[2][0]=back_color[2];

        m_back[0][0]=down_color[0];
        m_back[1][0]=down_color[1];
        m_back[2][0]=down_color[2];

        m_down[0][0]=front_color[0];
        m_down[1][0]=front_color[1];
        m_down[2][0]=front_color[2];
    }else if(act==2){
        char front_color[3]={m_front[0][1],m_front[1][1],m_front[2][1]};
        char up_color[3]={m_up[0][1],m_up[1][1],m_up[2][1]};
        char back_color[3]={m_back[0][1],m_back[1][1],m_back[2][1]};
        char down_color[3]={m_down[0][1],m_down[1][1],m_down[2][1]};

        m_front[0][1]=down_color[0];
        m_front[1][1]=down_color[1];
        m_front[2][1]=down_color[2];

        m_up[0][1]=front_color[0];
        m_up[1][1]=front_color[1];
        m_up[2][1]=front_color[2];

        m_back[0][1]=up_color[0];
        m_back[1][1]=up_color[1];
        m_back[2][1]=up_color[2];

        m_down[0][1]=back_color[0];
        m_down[1][1]=back_color[1];
        m_down[2][1]=back_color[2];
    }else if(act==3){
        char front_color[3]={m_front[0][1],m_front[1][1],m_front[2][1]};
        char up_color[3]={m_up[0][1],m_up[1][1],m_up[2][1]};
        char back_color[3]={m_back[0][1],m_back[1][1],m_back[2][1]};
        char down_color[3]={m_down[0][1],m_down[1][1],m_down[2][1]};

        m_front[0][1]=up_color[0];
        m_front[1][1]=up_color[1];
        m_front[2][1]=up_color[2];

        m_up[0][1]=back_color[0];
        m_up[1][1]=back_color[1];
        m_up[2][1]=back_color[2];

        m_back[0][1]=down_color[0];
        m_back[1][1]=down_color[1];
        m_back[2][1]=down_color[2];

        m_down[0][1]=front_color[0];
        m_down[1][1]=front_color[1];
        m_down[2][1]=front_color[2];
    }else if(act==4){
        char front_color[3]={m_front[0][2],m_front[1][2],m_front[2][2]};
        char up_color[3]={m_up[0][2],m_up[1][2],m_up[2][2]};
        char back_color[3]={m_back[0][2],m_back[1][2],m_back[2][2]};
        char down_color[3]={m_down[0][2],m_down[1][2],m_down[2][2]};

        m_front[0][2]=down_color[0];
        m_front[1][2]=down_color[1];
        m_front[2][2]=down_color[2];

        m_up[0][2]=front_color[0];
        m_up[1][2]=front_color[1];
        m_up[2][2]=front_color[2];

        m_back[0][2]=up_color[0];
        m_back[1][2]=up_color[1];
        m_back[2][2]=up_color[2];

        m_down[0][2]=back_color[0];
        m_down[1][2]=back_color[1];
        m_down[2][2]=back_color[2];
    }else if(act==5){
        char front_color[3]={m_front[0][2],m_front[1][2],m_front[2][2]};
        char up_color[3]={m_up[0][2],m_up[1][2],m_up[2][2]};
        char back_color[3]={m_back[0][2],m_back[1][2],m_back[2][2]};
        char down_color[3]={m_down[0][2],m_down[1][2],m_down[2][2]};

        m_front[0][2]=up_color[0];
        m_front[1][2]=up_color[1];
        m_front[2][2]=up_color[2];

        m_up[0][2]=back_color[0];
        m_up[1][2]=back_color[1];
        m_up[2][2]=back_color[2];

        m_back[0][2]=down_color[0];
        m_back[1][2]=down_color[1];
        m_back[2][2]=down_color[2];

        m_down[0][2]=front_color[0];
        m_down[1][2]=front_color[1];
        m_down[2][2]=front_color[2];
    }else if(act==6){
        char front_color[3]={m_front[2][0],m_front[2][1],m_front[2][2]};
        char right_color[3]={m_right[2][2],m_right[1][2],m_right[0][2]};
        char back_color[3]={m_back[0][2],m_back[0][1],m_back[0][0]};
        char left_color[3]={m_left[0][0],m_left[1][0],m_left[2][0]};

        m_front[2][0]=left_color[0];
        m_front[2][1]=left_color[1];
        m_front[2][2]=left_color[2];

        m_right[2][2]=front_color[0];
        m_right[1][2]=front_color[1];
        m_right[0][2]=front_color[2];

        m_back[0][2]=right_color[0];
        m_back[0][1]=right_color[1];
        m_back[0][0]=right_color[2];

        m_left[0][0]=back_color[0];
        m_left[1][0]=back_color[1];
        m_left[2][0]=back_color[2];
    }else if(act==7){
        char front_color[3]={m_front[2][0],m_front[2][1],m_front[2][2]};
        char right_color[3]={m_right[2][2],m_right[1][2],m_right[0][2]};
        char back_color[3]={m_back[0][2],m_back[0][1],m_back[0][0]};
        char left_color[3]={m_left[0][0],m_left[1][0],m_left[2][0]};

        m_front[2][0]=right_color[0];
        m_front[2][1]=right_color[1];
        m_front[2][2]=right_color[2];

        m_right[2][2]=back_color[0];
        m_right[1][2]=back_color[1];
        m_right[0][2]=back_color[2];

        m_back[0][2]=left_color[0];
        m_back[0][1]=left_color[1];
        m_back[0][0]=left_color[2];

        m_left[0][0]=front_color[0];
        m_left[1][0]=front_color[1];
        m_left[2][0]=front_color[2];
    }else if(act==8){
        char front_color[3]={m_front[1][0],m_front[1][1],m_front[1][2]};
        char right_color[3]={m_right[2][1],m_right[1][1],m_right[0][1]};
        char back_color[3]={m_back[1][2],m_back[1][1],m_back[1][0]};
        char left_color[3]={m_left[0][1],m_left[1][1],m_left[2][1]};

        m_front[1][0]=left_color[0];
        m_front[1][1]=left_color[1];
        m_front[1][2]=left_color[2];

        m_right[2][1]=front_color[0];
        m_right[1][1]=front_color[1];
        m_right[0][1]=front_color[2];

        m_back[1][2]=right_color[0];
        m_back[1][1]=right_color[1];
        m_back[1][0]=right_color[2];

        m_left[0][1]=back_color[0];
        m_left[1][1]=back_color[1];
        m_left[2][1]=back_color[2];
    }else if(act==9){
        char front_color[3]={m_front[1][0],m_front[1][1],m_front[1][2]};
        char right_color[3]={m_right[2][1],m_right[1][1],m_right[0][1]};
        char back_color[3]={m_back[1][2],m_back[1][1],m_back[1][0]};
        char left_color[3]={m_left[0][1],m_left[1][1],m_left[2][1]};

        m_front[1][0]=right_color[0];
        m_front[1][1]=right_color[1];
        m_front[1][2]=right_color[2];

        m_right[2][1]=back_color[0];
        m_right[1][1]=back_color[1];
        m_right[0][1]=back_color[2];

        m_back[1][2]=left_color[0];
        m_back[1][1]=left_color[1];
        m_back[1][0]=left_color[2];

        m_left[0][1]=front_color[0];
        m_left[1][1]=front_color[1];
        m_left[2][1]=front_color[2];
    }else if(act==10){
        char front_color[3]={m_front[0][0],m_front[0][1],m_front[0][2]};
        char right_color[3]={m_right[2][0],m_right[1][0],m_right[0][0]};
        char back_color[3]={m_back[2][2],m_back[2][1],m_back[2][0]};
        char left_color[3]={m_left[0][2],m_left[1][2],m_left[2][2]};

        m_front[0][0]=left_color[0];
        m_front[0][1]=left_color[1];
        m_front[0][2]=left_color[2];

        m_right[2][0]=front_color[0];
        m_right[1][0]=front_color[1];
        m_right[0][0]=front_color[2];

        m_back[2][2]=right_color[0];
        m_back[2][1]=right_color[1];
        m_back[2][0]=right_color[2];

        m_left[0][2]=back_color[0];
        m_left[1][2]=back_color[1];
        m_left[2][2]=back_color[2];
    }else if(act==11){
        char front_color[3]={m_front[0][0],m_front[0][1],m_front[0][2]};
        char right_color[3]={m_right[2][0],m_right[1][0],m_right[0][0]};
        char back_color[3]={m_back[2][2],m_back[2][1],m_back[2][0]};
        char left_color[3]={m_left[0][2],m_left[1][2],m_left[2][2]};

        m_front[0][0]=right_color[0];
        m_front[0][1]=right_color[1];
        m_front[0][2]=right_color[2];

        m_right[2][0]=back_color[0];
        m_right[1][0]=back_color[1];
        m_right[0][0]=back_color[2];

        m_back[2][2]=left_color[0];
        m_back[2][1]=left_color[1];
        m_back[2][0]=left_color[2];

        m_left[0][2]=front_color[0];
        m_left[1][2]=front_color[1];
        m_left[2][2]=front_color[2];
    }else if(act==12){
        char left_color[3]={m_left[2][2],m_left[2][1],m_left[2][0]};
        char up_color[3]={m_up[2][2],m_up[2][1],m_up[2][0]};
        char right_color[3]={m_right[2][2],m_right[2][1],m_right[2][0]};
        char down_color[3]={m_down[0][0],m_down[0][1],m_down[0][2]};

        m_left[2][2]=up_color[0];
        m_left[2][1]=up_color[1];
        m_left[2][0]=up_color[2];

        m_up[2][2]=right_color[0];
        m_up[2][1]=right_color[1];
        m_up[2][0]=right_color[2];

        m_right[2][2]=down_color[0];
        m_right[2][1]=down_color[1];
        m_right[2][0]=down_color[2];

        m_down[0][0]=left_color[0];
        m_down[0][1]=left_color[1];
        m_down[0][2]=left_color[2];
    }else if(act==13){
        char left_color[3]={m_left[2][2],m_left[2][1],m_left[2][0]};
        char up_color[3]={m_up[2][2],m_up[2][1],m_up[2][0]};
        char right_color[3]={m_right[2][2],m_right[2][1],m_right[2][0]};
        char down_color[3]={m_down[0][0],m_down[0][1],m_down[0][2]};

        m_left[2][2]=down_color[0];
        m_left[2][1]=down_color[1];
        m_left[2][0]=down_color[2];

        m_up[2][2]=left_color[0];
        m_up[2][1]=left_color[1];
        m_up[2][0]=left_color[2];

        m_right[2][2]=up_color[0];
        m_right[2][1]=up_color[1];
        m_right[2][0]=up_color[2];

        m_down[0][0]=right_color[0];
        m_down[0][1]=right_color[1];
        m_down[0][2]=right_color[2];
    }else if(act==14){
        char left_color[3]={m_left[1][2],m_left[1][1],m_left[1][0]};
        char up_color[3]={m_up[1][2],m_up[1][1],m_up[1][0]};
        char right_color[3]={m_right[1][2],m_right[1][1],m_right[1][0]};
        char down_color[3]={m_down[1][0],m_down[1][1],m_down[1][2]};

        m_left[1][2]=up_color[0];
        m_left[1][1]=up_color[1];
        m_left[1][0]=up_color[2];

        m_up[1][2]=right_color[0];
        m_up[1][1]=right_color[1];
        m_up[1][0]=right_color[2];

        m_right[1][2]=down_color[0];
        m_right[1][1]=down_color[1];
        m_right[1][0]=down_color[2];

        m_down[1][0]=left_color[0];
        m_down[1][1]=left_color[1];
        m_down[1][2]=left_color[2];
    }else if(act==15){
        char left_color[3]={m_left[1][2],m_left[1][1],m_left[1][0]};
        char up_color[3]={m_up[1][2],m_up[1][1],m_up[1][0]};
        char right_color[3]={m_right[1][2],m_right[1][1],m_right[1][0]};
        char down_color[3]={m_down[1][0],m_down[1][1],m_down[1][2]};

        m_left[1][2]=down_color[0];
        m_left[1][1]=down_color[1];
        m_left[1][0]=down_color[2];

        m_up[1][2]=left_color[0];
        m_up[1][1]=left_color[1];
        m_up[1][0]=left_color[2];

        m_right[1][2]=up_color[0];
        m_right[1][1]=up_color[1];
        m_right[1][0]=up_color[2];

        m_down[1][0]=right_color[0];
        m_down[1][1]=right_color[1];
        m_down[1][2]=right_color[2];
    }else if(act==16){
        char left_color[3]={m_left[0][2],m_left[0][1],m_left[0][0]};
        char up_color[3]={m_up[0][2],m_up[0][1],m_up[0][0]};
        char right_color[3]={m_right[0][2],m_right[0][1],m_right[0][0]};
        char down_color[3]={m_down[2][0],m_down[2][1],m_down[2][2]};

        m_left[0][2]=up_color[0];
        m_left[0][1]=up_color[1];
        m_left[0][0]=up_color[2];

        m_up[0][2]=right_color[0];
        m_up[0][1]=right_color[1];
        m_up[0][0]=right_color[2];

        m_right[0][2]=down_color[0];
        m_right[0][1]=down_color[1];
        m_right[0][0]=down_color[2];

        m_down[2][0]=left_color[0];
        m_down[2][1]=left_color[1];
        m_down[2][2]=left_color[2];
    }else if(act==17){
        char left_color[3]={m_left[0][2],m_left[0][1],m_left[0][0]};
        char up_color[3]={m_up[0][2],m_up[0][1],m_up[0][0]};
        char right_color[3]={m_right[0][2],m_right[0][1],m_right[0][0]};
        char down_color[3]={m_down[2][0],m_down[2][1],m_down[2][2]};

        m_left[0][2]=down_color[0];
        m_left[0][1]=down_color[1];
        m_left[0][0]=down_color[2];

        m_up[0][2]=left_color[0];
        m_up[0][1]=left_color[1];
        m_up[0][0]=left_color[2];

        m_right[0][2]=up_color[0];
        m_right[0][1]=up_color[1];
        m_right[0][0]=up_color[2];

        m_down[2][0]=right_color[0];
        m_down[2][1]=right_color[1];
        m_down[2][2]=right_color[2];
    }else{
        std::cout<<"error!"<<std::endl;
    }
}

void copy(Cube &target,Cube &source){
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            target.m_up[i][j]=source.m_up[i][j];
            target.m_down[i][j]=source.m_down[i][j];
            target.m_left[i][j]=source.m_left[i][j];
            target.m_right[i][j]=source.m_right[i][j];
            target.m_front[i][j]=source.m_front[i][j];
            target.m_down[i][j]=source.m_down[i][j];
        }
    }
}

//--------------这里是搜索函数和游戏结果的某些函数的定义------------

void ans_node::setAns_node(int &act){
    m_act=act/2;
    if(act%2){
        m_dir='-';
    }else{
        m_dir='+';
    }
}

int &ans_node::getAct(){
        return m_act;
}

char &ans_node::getDir(){
        return m_dir;
}

void printAns(std::deque<ans_node> ans){
    typedef std::deque<ans_node>::iterator It;
    std::ofstream ofs;

    ofs.open("solution.txt",std::ios::out);
    if(ofs.is_open()){
        for(It it=ans.begin();it!=ans.end();++it){
            ofs<<(*it).getAct()<<(*it).getDir()<<" ";
        }
        ofs<<std::endl;
    }else{
        std::cout<<"open solution file error!"<<std::endl;
    }

    ofs.close();
}

void createChild(Cube* &child,Cube* &crntnode,int &act){
    copy(*child,*crntnode);
    child->move(act);
}

bool search(Cube &c){
    using namespace std;
    deque<Cube> s;
    s.push_back(c);

    while(!s.empty()){
        Cube *crntnode=new Cube(s.back());
        if(crntnode->isTarget()){
            printAns(answer::ans);
            return true;
        }

        if(answer::now_step>answer::MAXSTEP)
            continue;
        
        while(crntnode->m_action<18){
            Cube *child = new Cube;
            createChild(child,crntnode,crntnode->m_action);
            if(answer::now_step>=1){
                typedef deque<Cube>::iterator It;
                It it=s.begin();
                for(int i=1;i<s.size()-1;i++)
                    ++it;
                if((*it)==*child){
                    crntnode->m_action++;
                    delete child;
                    continue;
                }

                s.push_back(*child);
                answer::now_step++;
                ans_node *pans = new ans_node;
                pans->setAns_node(crntnode->m_action);
                crntnode->m_action++;
                answer::ans.push_back(*pans);
                delete pans;
                *crntnode=s.back();
                delete child;
            }

            s.push_back(*child);
            answer::now_step++;
            ans_node *pans = new ans_node;
            pans->setAns_node(crntnode->m_action);
            crntnode->m_action++;
            answer::ans.push_back(*pans);
            delete pans;
            *crntnode=s.back();
            delete child;
        }
        s.pop_back();
        answer::ans.pop_back();
        delete crntnode;
    }
    return false;
}
