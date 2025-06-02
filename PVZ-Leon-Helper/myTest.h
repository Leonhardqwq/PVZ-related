//#pragma once
#ifndef __MYTEST_H__
#define __MYTEST_H__

#include<myTas.h>

bool haveCount=0;
bool isskiping=1;
int my_cnt=0,my_sum=0,my_flag=0;
ATickRunner countRunner;
ATickRunner infoRunner;
void myShowinfo(){
    myText(  "cnt:"+std::to_string(my_cnt)+"\n"
            +"sum:"+std::to_string(my_sum)+"\n"
            +"flag:"+std::to_string(my_flag)+"\n"
        ,10,100);
}


void myInfHp(){
    for(auto& p:aAlivePlantFilter)
        //if(p.Type()==ANGT_30 || p.Type()==AYYG_42)
            p.Hp()=999999;
}


namespace {AZombie* __zombie;}
void FreezeZombie(AZombie* zombie) {
    __zombie = zombie;
    __asm__ __volatile__(
        "movl %[__zombie], %%eax;"
        "movl $0x5323C0, %%edx;"
        "calll *%%edx;"
        :
        : [__zombie] "m"(__zombie)
        : ASaveAllRegister);
}
ATimeline myN=(ATimeline)[]() mutable{
    for(auto& z:aAliveZombieFilter){
        if(z.Hp()>1800) 
            z.Hp()-=1800;
        else if(z.Type()!=AXG_24 && z.Type()!=AKG_17)   
            z.State()=3;    
    }
};
ATimeline myI=(ATimeline)[]() mutable{
    for(auto& z:aAliveZombieFilter)
        FreezeZombie(&z);
    /*
    {
        if(z.Type()==ABC_12 || z.Type()==AKG_17)
            continue;
        z.SlowCountdown()=2000;
        if(z.Type()==AQQ_16 || z.Type()==ATT_18 ||z.Type()==AXG_24)
            continue;
        z.Hp()-=20;
        z.FreezeCountdown()=(z.SlowCountdown()>0)?300:400;
        
    }
    */
};

ATickRunner gigaRunner;
void myKillGiga(){
    for(auto& z:aAliveZombieFilter)
    if((z.Type()==ABY_23 || z.Type()==AHY_32) && z.State()==70){
        float completing = (AGetAnimationArray()+z.MRef<uint16_t>(0x118))->CirculationRate();
        if(0.64242 <= completing && completing<= 0.64485)
            z.State()=3;
    }
}

//countRunner.Start(counter);
//ASkipTick([=](){return isskiping;});
void myTest(){
    myTAS();
    haveCount=0;
    isskiping=1;
    
    ASetGameSpeed(10);
    infoRunner.Start(myShowinfo,myMode);
    gigaRunner.Start(myKillGiga,myMode);

    myInfHp();
    OnWave(20) At(600) Do{isskiping=1;my_flag+=2;};
}
#endif