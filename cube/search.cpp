#include"search.h"

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