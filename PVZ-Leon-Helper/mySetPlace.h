#ifndef __MYSETPLACE_H__
#define __MYSETPLACE_H__
#include"myPT.h"


void DE_3P5E(){
    mySetRowType(3,1);
}
void DE_15D5E(){
    mySetRowType(1,1);
    mySetRowType(2,0);
    mySetRowType(3,0);
    mySetRowType(4,0);
    mySetRowType(5,1);
}
void DE_135P5E(){
    mySetRowType(3,1);
    mySetRowType(1,1);
    mySetRowType(5,1);
    mySetRowType(2,0);
    mySetRowType(4,0);
}
void PE_4P6E(){
    mySetRowType(3,0);
}
void DE_3L5E(){
    mySetRowType(1,1);
    mySetRowType(2,1);
    mySetRowType(3,0);
    
    mySetRowType(4,1);
    mySetRowType(5,1);
}
void FE_16P6E(){
    mySetRowType(1,1);
    mySetRowType(2,0);
    mySetRowType(3,0);
    mySetRowType(4,0);
    mySetRowType(5,0);
    mySetRowType(6,1);
}
#endif