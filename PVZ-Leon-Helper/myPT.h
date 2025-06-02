//#pragma once
#ifndef __MYPT_H__
#define __MYPT_H__

#include <avz.h>
#include <cstdio>
#include<dsl/shorthand.h>
using namespace std;

// 伤害常数
__ANodiscard int& DMG_PEA() noexcept { return AMRef<int>(0x69F1C8); }
__ANodiscard int& DMG_SNOW_PEA() noexcept { return AMRef<int>(0x69F1D4); }
__ANodiscard int& DMG_CABBAGE() noexcept { return AMRef<int>(0x69F1E0); }
__ANodiscard int& DMG_MELON() noexcept { return AMRef<int>(0x69F1EC); }
// SHROOM
__ANodiscard int& DMG_SPORE() noexcept { return AMRef<int>(0x69F1F8); }
__ANodiscard int& DMG_WINTER_MELON() noexcept { return AMRef<int>(0x69F204); }
__ANodiscard int& DMG_FLAME_PEA() noexcept { return AMRef<int>(0x69F210); }
__ANodiscard int& DMG_STAR() noexcept { return AMRef<int>(0x69F21C); }
__ANodiscard int& DMG_CACTUS() noexcept { return AMRef<int>(0x69F228); }
__ANodiscard int& DMG_KERNEL() noexcept { return AMRef<int>(0x69F240); }
__ANodiscard int& DMG_BUTTER() noexcept { return AMRef<int>(0x69F258); }
__ANodiscard int& DMG_ASH() noexcept { return AMRef<int>(0x532FDC); }
__ANodiscard int& DMG_POTATO() noexcept { return AMRef<int>(0x41D931); }
__ANodiscard int& DMG_SQUASH() noexcept { return AMRef<int>(0x4607A9); }
__ANodiscard char& DMG_ICE() noexcept { return AMRef<char>(0x532493); }
// DPG YYG DC DCW
__ANodiscard char& DMG_AOE() noexcept { return AMRef<char>(0x45EDEF); }



// 场地行
void mySetGridType(int row,int col,int type){
    AGetMainObject()->MRef<int>(0x168+0x4*(row+6*col))=type;
}
void mySetRowZombieType(int row,int type){
    AGetMainObject()->MRef<int>(0x5D8+0x4*row)=type;
}
void mySetRowType(int row,bool ispool){
    mySetRowZombieType(row-1,ispool?2:1);
    for(int i=0;i<9;i++)    mySetGridType(row-1,i,ispool?3:1);
}
int myGetRowType(int ro){return AGetMainObject()->MRef<int>(0x5D8+0x4*ro);}

// 指针得栈位
int myGetPlantIdx(APlant* p){ 
    if(p==nullptr)return -1;
    return AGetPlantIndex(p->Row()+1,p->Col()+1,p->Type());
}

// 种子
bool MyIsSeedUsable(ASeed* seed){
    if(seed==nullptr)return 0;
    return seed->IsUsable() or seed->InitialCd()==0;
}

// 曾永动
ATickRunner YYGalwaysRunner;
vector<APlant*> YYGalwaysList;
void mySetYYGAlways(){
    int fix = 5;
    for(auto p:YYGalwaysList)
        if(p==nullptr)continue;
        else if( p->MRef<int>(0x58) == 1)
            AConnect(ANowDelayTime(1+fix),[=]{
                p->ShootCountdown() = 200-fix;
            });
}
void mySetYYGAlwaysList(const std::vector<AGrid>& lst){
    YYGalwaysList.clear();
    for(auto grid:lst){
        auto p = AGetPlantPtr(grid.row, grid.col,AYYG_42);
        if(p!=nullptr)
            YYGalwaysList.push_back(p);
    }
}
void myYYGAlways(const std::vector<AGrid>& lst){
    mySetYYGAlwaysList(lst);
    YYGalwaysRunner.Start(mySetYYGAlways);
}

#endif