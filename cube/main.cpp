#include"cube.h"
#include"search.h"

int main(void){
    Cube c;
    c.inputCube("test.txt");
    if(search(c)==false)
        std::cout<<"fail!"<<std::endl;
    return 0;
}