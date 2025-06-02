//#pragma once
#ifndef __MYTAS_H__
#define __MYTAS_H__

#include <string>
#include <myPT.h>
#include <vector>
ALogger<APvzGui> pvzLogger; 

const bool allow_back = true;
const int replay_max_ticks=7000;
const int back_tick = 10;

bool allow_replay = true;
bool allow_autoRecording = true;
bool back_constraint = true;


bool super_pause = true;
auto myMode = super_pause?ATickRunner::GLOBAL:ATickRunner::ONLY_FIGHT;
//PS1 请在非选卡或战斗界面启动本插件
//PS2 请先使用pt/ptk修改出怪功能后再启用本插件，否则可能导致崩溃
//PS3 回档功能会在D盘下生成"AAA_LH_SL"文件夹存放存档文件，大小约10M
// 1~10 游戏变速，其中5为原速

// Tab 回档0.5s前(最多回档到10s前)
// Caps 暂停状态下跳1cs

// *Q 快捷铲子 edit
// *W 高级暂停 edit
// E 一键启动/停用(AFGZXC) edit(latest)
// *R 智能铲套 edit
// *T 刷新指示灯 edit
// *Y 显示雾下血条 

// A 显示信息(血量、波数、时间) edit
// *S 普僵系隐身 edit
// *D 女仆秘籍(不前进) edit
// F 气球进家、南瓜啃穿提示 edit
// G 红眼橄榄计数 edit

// Z (在显示信息模式下)显示最右防御域 edit
// X 显示僵尸总血量 edit
// C 卡槽高亮 edit
// *V 显示小喷/阳光菇偏移

//P 展示录制
//Backspace 结束录制/展示
//Enter 开始录制


void myQuickShovel(){
    int& tmp=AGetMainObject()->MouseAttribution()->Type();
    if(!tmp)tmp=6;
}


bool is_pausing=0;
void myPause(){
    is_pausing=!is_pausing;
    if(!super_pause)
        ASetUpdateWindow(is_pausing);
    else
        ASetAdvancedPause(is_pausing,false,AArgb(0, 0, 0, 0));
}
void myPauseAndTick(){
    if(!is_pausing)return;
    myPause();
    AConnect(ANowDelayTime(1),myPause);
}
void myBigPauseAndTick(){
    if(!is_pausing)return;
    myPause();
    AConnect(ANowDelayTime(10),myPause);
}
void myHugePauseAndTick(){
    if(!is_pausing)return;
    myPause();
    AConnect(ANowDelayTime(100),myPause);
}

bool is_Stealth=0;
ATickRunner StealRunner;
void mySteal(){
    for(auto& z:aAliveZombieFilter){
        if( 
            z.Type()==APJ_0 
        ||  z.Type()==ALZ_2
        ||  z.Type()==ATT_4
        ||  z.Type()==ADB_5
        ||  z.Type()==ATM_6
        ||  z.Type()==AYZ_10
        ){
            if(is_Stealth){
                if(z.MRef<bool>(0x18))
                z.MRef<bool>(0x18)=0;//隐身
            } 
            else{
                if(!z.MRef<bool>(0x18))
                z.MRef<bool>(0x18)=1;//显身
            }                
        }
    }
}


APainter myPainter;
enum myColor{
    clr_red=0,
    clr_green=1,
    clr_yellow=2,
    clr_purple_white=3,
    clr_chocolate=4,
    clr_white=5,
    clr_white_80=6,
    clr_black=7,
    clr_white_40=8,
    clr_black_40=9,
    clr_green_80=10,
    clr_blue=11,
    clr_red_80=12,
    clr_yellow_40=13,
};
void mySetColor(int x){
    if(AGetMainObject()==nullptr) return;
    switch (x){
        case 0: myPainter.SetRectColor(0xFFFF0000);   break;
        case 1: myPainter.SetRectColor(0xFF00FF00);   break;
        case 2: myPainter.SetRectColor(0xFFFFFF00);   break;
        case 3: myPainter.SetRectColor(0xFFCCAAE6);   break;
        case 4: myPainter.SetRectColor(0xFFD2691E);   break;
        case 5: myPainter.SetRectColor(0xFFFFFFFF);   break;
        case 6: myPainter.SetRectColor(0x80FFFFFF);   break;
        case 7: myPainter.SetRectColor(0xFF000000);   break;
        case 8: myPainter.SetRectColor(0x40FFFFFF);   break;
        case 9: myPainter.SetRectColor(0x40000000);   break;
        case 10:myPainter.SetRectColor(0x8000FF00);   break;
        case 11:myPainter.SetRectColor(0xFF0000FF);   break;
        case 12:myPainter.SetRectColor(0x80FF0000);   break;
        case 13:myPainter.SetRectColor(0x40FFFF00);   break;
    }
}
void myBar(int xl,int xr,int yl,int yr){ 
    if(AGetMainObject()==nullptr) return;   
    myPainter.Draw(ARect(xl,yl,xr-xl+1,yr-yl+1));    
}
void myText(std::string text,int x,int y){
    if(AGetMainObject()==nullptr) return;
    aPainter.Draw(AText(text,x,y));
    //myPainter.Draw(AText(text,x,y));
}
void myPureText(std::string text,int x,int y){
    if(AGetMainObject()==nullptr) return;
    auto tmp = aPainter.GetRectColor();
    aPainter.SetRectColor(0x00000000);
    aPainter.Draw(AText(text,x,y));
    aPainter.SetRectColor(tmp);
}
void myLogger(std::string text){
    if(AGetMainObject()==nullptr) return;
    pvzLogger.Info(text);
}
void mySetTextColor(){
    if(AGetMainObject()==nullptr) return;
    aPainter.SetTextColor(0xFFFFFFFF);
    myPainter.SetTextColor(0xFFFFFFFF);
}
void mySetInitLogger(){
    if(AGetMainObject()==nullptr) return;
    pvzLogger.SetHeaderStyle("");
}
void myDrawBar_zombie(int Xl,int Xr,int Yl,int Yr,int y_progerss,const std::vector<int> ylst,int clr){
    int Yn=Yr+1-y_progerss;
    mySetColor(clr);
    myBar(Xl,Xr,Yn,Yr);
    mySetColor(clr_white_80);
    myBar(Xl,Xr,Yl,Yn-1);

    mySetColor(clr_black);
    myBar(Xl,Xr,Yn,Yn);

    myBar(Xl,Xl,Yl,Yr);
    myBar(Xr,Xr,Yl,Yr);
    myBar(Xl,Xr,Yl,Yl);//fix ?
    myBar(Xl,Xr,Yr,Yr);//fix ?

    for(auto pro:ylst){
        int y_line = Yr+1-pro;
        myBar(Xl,Xr,y_line,y_line);
    }
}
void myDrawBar_plant(int Xl,int Xr,int Yl,int Yr,int x_progerss,int clr_body,int clr_bg){
    int Xn = Xl+x_progerss;
    mySetColor(clr_body);
    myBar(Xl,Xn-1,Yl,Yr);
    mySetColor(clr_bg);
    myBar(Xn,Xr,Yl,Yr);
}


bool is_seeing_through_fog = 0;
float fog_x = 10000;
bool CheckPlantVisable(APlant* p){
    return is_seeing_through_fog or p->Abscissa()+10<fog_x;
    //return 1;
}
bool CheckZombieVisable(AZombie* z){
    if(is_seeing_through_fog)return 1;
    if(z->Type()==AGL_7)
        return z->Abscissa()+40 < fog_x;
    else if(z->Type()==ABY_23 or z->Type()==AHY_32)
        return z->Abscissa() < fog_x;
    return 1;
}


bool is_counting=0;
int giga_total_num=0;
ATickRunner CountGigaRunner;
void myShowCount(){
    if(!is_counting)return;
    int giga_cnt[6] = {0,0,0,0,0,0};
    int gl_cnt[6] = {0,0,0,0,0,0};

    for(auto& z:aAliveZombieFilter){
        if(!CheckZombieVisable(&z))continue;
        if(z.Type()==AHY_32)
            giga_cnt[z.Row()]++;
        else if(z.Type()==AGL_7)
            gl_cnt[z.Row()]++;
    }
    for(int i =0;i<6;i++){
        if(myGetRowType(i) != 1)    continue;
        int yi = AAsm::GridToOrdinate(i, 0)+30;
        myText(std::to_string(giga_cnt[i])+"/"+std::to_string(gl_cnt[i])
        ,10,yi);
    }

    myText(std::to_string(giga_total_num), 10, 85);
    int t = ANowTime(ANowWave());
    if(t!=1)return;
    giga_total_num = 0;
    int w = AGetMainObject()->Wave();
    auto list = AGetMainObject()->ZombieList();
    for(int i = 0;i<50*w;i++)
        if( *(list+i) == AHY_32 )
            giga_total_num++;
}


bool is_SmartS=0;
ATickRunner SmartSRunner;
/*
struct myPlant{
    APlant* PlantPtr;
    bool _exist;
    myPlant(APlant* p){
        _exist=1;
        PlantPtr=p;
    }
    void shovel(){  _exist=0;}
    bool exist(){   return _exist;  }
};
struct myPair{
    AZombie* ZombiePtr;
    vector<myPlant*> myPlantPtrV;
    myPair(AZombie* z){
        ZombiePtr=z;
        myPlantPtrV.clear();
    }
    bool isPlantExist(int i){
        if(i<myPlantPtrV.size())    return myPlantPtrV[i]->exist();
        return 0;
    }
    APlant* getPlant(int i){
        if(isPlantExist(i)) return myPlantPtrV[i]->PlantPtr;
        return nullptr;
    }
    myPlant* getMyPlant(int i){
        if(isPlantExist(i)) return myPlantPtrV[i];
        return nullptr;
    }
    void pushPlant(myPlant* p){ myPlantPtrV.push_back(p);   }
    void shovelPlant(int i){myPlantPtrV[i]->shovel();}
    int getSize(){return myPlantPtrV.size();}
};
int myGetPlantId(myPlant* myPlantPtr){ return AGetPlantIndex(myPlantPtr->PlantPtr->Row()+1,myPlantPtr->PlantPtr->Col()+1,myPlantPtr->PlantPtr->Type());}
void myShovel(myPlant* myPlantPtr){    
    myPlantPtr->shovel();
    AShovel(myPlantPtr->PlantPtr->Row()+1,myPlantPtr->PlantPtr->Col()+1,myPlantPtr->PlantPtr->Type());  
    myLogger("auto shovel");
}
void mySmartS(){
    if(!is_SmartS)return;
    mySetColor(2);
    myBar(0,30,580,600);
    vector<myPair> PV;
    PV.clear();
    for(auto& z:aAliveZombieFilter)
    if((z.Type()==ABY_23 || z.Type()==AHY_32) && z.State()==70){
        float completing = (AGetAnimationArray()+z.MRef<uint16_t>(0x118))->CirculationRate();
        if(0.64242 <= completing && completing<= 0.64485){
            if(z.FixationCountdown() || z.FreezeCountdown())    continue;
            PV.push_back(myPair(&z));
        }
    }
    if(!PV.size())    return;
    for(auto& p:aAlivePlantFilter){
        if(p.Type()==AYMJNP_47)continue;
        bool is_used=0;
        myPlant* mp=new myPlant(&p);
        for(int i=0;i<PV.size();i++){
            AZombie& z = *(PV[i].ZombiePtr);
            int zl=z.Abscissa()-30,zr=zl+89;
            int pl,pr;
            switch (p.Type()){
            case ANGT_30:   pl=p.Abscissa()+20,pr=pl+60; break;
            case AGJG_23:   pl=p.Abscissa()+30,pr=pl+40; break;
            default:        pl=p.Abscissa()+30,pr=pl+20; break;
            }
            if(p.Row()==z.Row() && pr>=zl && zr>=pl){
                is_used=1;
                PV[i].pushPlant(mp);
            }
        }
        if(is_used)continue;
        delete mp;
    }
    for(int i=0;i<PV.size();i++){
        myPair pr=PV[i];
        vector<myPlant*> v1,v2;
        v1.clear();v2.clear();
        for(int j=0;j<pr.getSize();j++){
            myPlant* tmp=pr.getMyPlant(j);
            if(tmp==nullptr)continue;
            if(v1.empty()){ v1.push_back(tmp);continue;}
            if(tmp->PlantPtr->Col()==v1[0]->PlantPtr->Col()){   v1.push_back(tmp);continue; }
            v2.push_back(tmp);
        }
        if(v1.empty())continue;
        if(v2.empty()){
            for(int j=0;j<v1.size();j++){
                for(int k=j+1;k<v1.size();k++){//k>j
                    if(v1[k]->PlantPtr->Type()==ANGT_30){myPlant* tmp=v1[j];v1[j]=v1[k];v1[k]=tmp;continue;}
                    if(v1[j]->PlantPtr->Type()==AHP_33){myPlant* tmp=v1[j];v1[j]=v1[k];v1[k]=tmp;continue;}
                }
            }
            if(v1[0]->PlantPtr->Type()==ANGT_30 && v1.size()==1)    myShovel(v1[0]);
            else if((v1[0]->PlantPtr->Type()==AXPG_8 || v1[0]->PlantPtr->Type()==AYGG_9) && v1.size()==1 
            &&  v1[0]->PlantPtr->Abscissa() > (v1[0]->PlantPtr->Col())*80+40)
                myShovel(v1[0]);          
            continue;
        }
        if(v1[0]->PlantPtr->Col()>v2[0]->PlantPtr->Col()){  vector<myPlant*> tmp;tmp=v1;v1=v2;v2=tmp;    }
        int max1=-1,min2=10000;
        for(int j=0;j<v1.size();j++){
            for(int k=j+1;k<v1.size();k++){//k>j
                if(v1[k]->PlantPtr->Type()==ANGT_30){myPlant* tmp=v1[j];v1[j]=v1[k];v1[k]=tmp;continue;}
                if(v1[j]->PlantPtr->Type()==AHP_33){myPlant* tmp=v1[j];v1[j]=v1[k];v1[k]=tmp;continue;}
            }
            int tmp=myGetPlantId(v1[j]);
            max1=tmp>max1?tmp:max1;
        }
        for(int j=0;j<v2.size();j++){
            for(int k=j+1;k<v2.size();k++){//k>j
                if(v2[k]->PlantPtr->Type()==ANGT_30){myPlant* tmp=v2[j];v2[j]=v2[k];v2[k]=tmp;continue;}
                if(v2[j]->PlantPtr->Type()==AHP_33){myPlant* tmp=v2[j];v2[j]=v2[k];v2[k]=tmp;continue;}
            }
            int tmp=myGetPlantId(v2[j]);
            min2=tmp<min2?tmp:min2;
        }     
        if(max1<min2){
            if(v1[0]->PlantPtr->Type()==ANGT_30 && v1.size()==1)    myShovel(v1[0]);
            else if((v1[0]->PlantPtr->Type()==AXPG_8 || v1[0]->PlantPtr->Type()==AYGG_9) && v1.size()==1 
            &&  v1[0]->PlantPtr->Abscissa() > (v1[0]->PlantPtr->Col())*80+40)
                myShovel(v1[0]); 
            else continue;
        }
        for(int li=0;li<v1.size();li++){
            int nl=myGetPlantId(v1[li]);
            if(nl<min2)
                myShovel(v1[li]);
            else{
                for(int ri=0;ri<v2.size();ri++)
                    if(nl<myGetPlantId(v2[ri]))     
                        myShovel(v2[ri]);   
            }
        }
    }
}

ATickRunner SmartS2Runner;
void mySmartS2(){
    if(!is_SmartS)return;
    APlant* myP[6]={nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    for(auto& p:aAlivePlantFilter){
        if(p.Type()!=ANGT_30) continue;
        int Ri=p.Row();
        if(myGetRowType(Ri)!=1) continue;
        if(!myP[Ri]){   myP[Ri]=&p;continue;    }
        if(myP[Ri]->Col()<p.Col()){   myP[Ri]=&p;continue;    }
    }
    AZombie* myZ[6]={nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    for(auto& z:aAliveZombieFilter){
        if(z.Type()!=ABC_12 && z.Type()!=ATL_22) continue;
        int Ri=z.Row();
        if(myGetRowType(Ri)!=1) continue;
        if(!myZ[Ri]){   myZ[Ri]=&z;continue;    }
        if(myZ[Ri]->Abscissa()>z.Abscissa()){   myZ[Ri]=&z;continue;}
    }
    for(int i=0;i<6;i++){
        if(!myP[i])continue;
        if(!myZ[i])continue;
        if(int(myZ[i]->Abscissa()+10-myZ[i]->Speed()) <= myP[i]->Abscissa()+80){
            AShovel(myP[i]->Row()+1,myP[i]->Col()+1,ANGT_30);
            myLogger("auto shovel");
        }
    }
}
*/
void myShovel(APlant* PlantPtr){    
    AShovel(PlantPtr->Row()+1,PlantPtr->Col()+1,PlantPtr->Type());  
    myLogger("auto shovel");
}
void mySortPlantPtrVector(vector<APlant*>& v){
    int v_size = v.size();
    for(int j=0;j<v_size;j++){
        for(int k=j+1;k<v_size;k++){//k>j
            if(v[k]->Type()==ANGT_30){APlant* tmp=v[j];v[j]=v[k];v[k]=tmp;continue;}
            if(v[j]->Type()==AHP_33){APlant* tmp=v[j];v[j]=v[k];v[k]=tmp;continue;}
        }
    }
}
void mySmartS(){
    if(!is_SmartS)return;
    mySetColor(2);
    myBar(0,30,580,600);

    // 2 car
    if(!is_SmartS)return;
    APlant* myP[6]={nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    for(auto& p:aAlivePlantFilter){
        if(p.Type()!=ANGT_30) continue;
        int Ri=p.Row();
        if(myGetRowType(Ri)!=1) continue;
        if(!myP[Ri]){   myP[Ri]=&p;continue;    }
        if(myP[Ri]->Col()<p.Col()){   myP[Ri]=&p;continue;    }
    }
    AZombie* myZ[6]={nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    for(auto& z:aAliveZombieFilter){
        if(z.Type()!=ABC_12 && z.Type()!=ATL_22) continue;
        int Ri=z.Row();
        if(myGetRowType(Ri)!=1) continue;
        if(!myZ[Ri]){   myZ[Ri]=&z;continue;    }
        if(myZ[Ri]->Abscissa()>z.Abscissa()){   myZ[Ri]=&z;continue;}
    }
    for(int i=0;i<6;i++){
        if(!myP[i])continue;
        if(!myZ[i])continue;
        if(int(myZ[i]->Abscissa()+10-myZ[i]->Speed()) <= myP[i]->Abscissa()+80){
            AShovel(myP[i]->Row()+1,myP[i]->Col()+1,ANGT_30);
            myLogger("auto shovel");
        }
    }

    for(auto& z:aAliveZombieFilter)
    if((z.Type()==ABY_23 || z.Type()==AHY_32) && z.State()==70){
        float completing = (AGetAnimationArray()+z.MRef<uint16_t>(0x118))->CirculationRate();
        if(0.64242 <= completing && completing<= 0.64485){
            if(z.FixationCountdown() || z.FreezeCountdown())    continue;
            // z:giga considered

            vector<APlant*> PV;
            PV.clear();
            for(auto& p:aAlivePlantFilter){
                if(p.Type()==AYMJNP_47)continue;
                int zl=int(z.Abscissa())-30,zr=zl+89;
                int pl,pr;
                switch (p.Type()){
                    case ANGT_30:   pl=p.Abscissa()+20,pr=pl+60; break;
                    case AGJG_23:   pl=p.Abscissa()+30,pr=pl+40; break;
                    default:        pl=p.Abscissa()+30,pr=pl+20; break;
                }
                if(p.Row()==z.Row() && pr>=zl && zr>=pl){
                    PV.push_back(&p);
                    // myLogger(std::to_string(p.Col()));
                }
            }
            // get all p

            vector<APlant*> v1,v2;
            v1.clear();v2.clear();
            for(int j=0;j<PV.size();j++){
                APlant* tmp=PV[j];
                if(tmp==nullptr)continue;
                if(v1.empty()){ v1.push_back(tmp);continue;}
                if(tmp->Col()==v1[0]->Col()){   v1.push_back(tmp);continue; }
                v2.push_back(tmp);
            }

            // no plant
            if(v1.empty())continue;
            // single grid
            if(v2.empty()){ 
                //myLogger("all="+std::to_string(PV.size())+",vi="+std::to_string(v1.size())+",vii="+std::to_string(v2.size()));
                mySortPlantPtrVector(v1);
                if(v1[0]->Type()==ANGT_30 && v1.size()==1)    myShovel(v1[0]);  // single ngt
                else if((v1[0]->Type()==AXPG_8 || v1[0]->Type()==AYGG_9) && v1.size()==1 
                &&  v1[0]->Abscissa() > (v1[0]->Col())*80+40)
                    myShovel(v1[0]);         // single right xpg
                continue;
            }
            // swap if needed (v1,v2)
            if(v1[0]->Col()>v2[0]->Col()){  vector<APlant*> tmp;tmp=v1;v1=v2;v2=tmp;    }
            mySortPlantPtrVector(v1);mySortPlantPtrVector(v2);
            int max1=-1,min2=10000;
            for(int j=0;j<v1.size();j++){
                int tmp=myGetPlantIdx(v1[j]);
                max1=tmp>max1?tmp:max1;
            }
            for(int j=0;j<v2.size();j++){
                int tmp=myGetPlantIdx(v2[j]);
                min2=tmp<min2?tmp:min2;
            }     
            // cannot help but basic ngt and right xpg
            if(max1<min2){
                if(v1[0]->Type()==ANGT_30 && v1.size()==1){ 
                    myShovel(v1[0]);
                    //myLogger("ngt");
                }    
                else if((v1[0]->Type()==AXPG_8 || v1[0]->Type()==AYGG_9) && v1.size()==1 
                &&  v1[0]->Abscissa() > (v1[0]->Col())*80+40){                  
                    myShovel(v1[0]);
                    //myLogger("mushroom");     
                }
                else continue;
            }

            for(int li=0;li<v1.size();li++){
                int nl=myGetPlantIdx(v1[li]);
                // myLogger("l"+std::to_string(li)+":"+std::to_string(myGetPlantIdx(v1[li])));

                if(nl<min2) {
                    //myLogger("left");
                    myShovel(v1[li]);
                }
                else{
                    for(int ri=0;ri<v2.size();ri++){
                        // myLogger("r"+std::to_string(ri)+":"+std::to_string(myGetPlantIdx(v2[ri])));

                        if(nl<myGetPlantIdx(v2[ri]))  {
                            //myLogger("right");
                            myShovel(v2[ri]);

                        }   
                    }
                }
            }
        }
    }

}




bool qq_rmding=0,ngt_rmding=0,is_rmd=0;
ATickRunner rmdRunner;
bool QQdanger(){
    for(auto& z:aAliveZombieFilter)
        if(z.Type()==AQQ_16 && z.Abscissa()<=70)
            return 1;
    return 0;
}
bool NGTdanger(){
    for(auto& p:aAlivePlantFilter)
        if(p.Type()==ANGT_30 && p.Hp()<=500)
            return 1;
    return 0;
}
void rmd_qq(){
    if(!is_rmd)return;
    if(QQdanger()){if (!qq_rmding) {
        myLogger("eyes disability");
        //myPause();
        qq_rmding=1;
        return; 
    }}
    else    qq_rmding=0;  
}
void rmd_ngt(){
    if(!is_rmd)return;
    if(NGTdanger()){if(!ngt_rmding){
        myLogger("pay attention to pumpkin");
        ngt_rmding=1;
        return;
    }}
    else    ngt_rmding=0;
}
void rmd_xc(){
    if(!is_rmd)return;
    for (auto& z : aAliveZombieFilter) 
        if (z.Type() == AXC_15 && z.State() == 16){
            int cd = z.StateCountdown();
            int Xi=z.Abscissa(),Yi=z.Ordinate();
            int Xl = Xi+60-5,Xr = Xi+60+5;
            //小丑 x:58-62,y:60-27,60+27(50/55,100/110)
            if(cd>=100)
                myDrawBar_zombie(Xl,Xr,Yi+60-24,Yi+60+25,5*(cd-100),{},clr_green);
            else
                myDrawBar_zombie(Xl,Xr,Yi+60-49,Yi+60+50,100*cd/100,{},clr_yellow);
        }
}
void myRmd(){rmd_qq();rmd_ngt();rmd_xc();}


bool is_showTrig=0;
bool is_showTotalHp=0;
ATickRunner TrigRunner;
void myShowTrig(){
    if(is_showTrig){
        int t=AGetMainObject()->RefreshCountdown();
        int w=AGetMainObject()->Wave();
        if(w<=0){
            if(t<=200){
                int tmp=51*(200-t)/200;
                mySetColor(11);
                myBar(20,78,8,58-tmp);            
                mySetColor(0);
                myBar(20,78,58+1-tmp,58);
            }   
            else{
                mySetColor(11);
                myBar(20,78,8,58);
            }
            return;
        }       
        if((w==9 or w==19) and t<=6 and -ANowTime(ANowWave()+1)<=200){
            t=-ANowTime(ANowWave()+1);
            int tmp=51*(200-t)/200;
            mySetColor(11);
            myBar(20,78,8,58-tmp);            
            mySetColor(0);
            myBar(20,78,58+1-tmp,58);
            return;
        }
        int currentHp = AAsm::ZombieTotalHp(w - 1);
        int refreshHp = AGetMainObject()->ZombieRefreshHp();
        int totalHp = AGetMainObject()->MRef<int>(0x5598);
        int trig_pivot = 59*(refreshHp)/totalHp;
        int pivot = 59*currentHp/totalHp;
        if(t<=200){
            int tmp=51*(200-t)/200;
            mySetColor(11);
            myBar(20,20-1+pivot,8,58-tmp); 
            mySetColor(7);
            myBar(20+pivot,78,8,58-tmp); 
            mySetColor(0);
            myBar(20+trig_pivot, 20+trig_pivot, 8, 58-tmp);          

            mySetColor(0);
            myBar(20,78,58+1-tmp,58);
        }   
        else{
            mySetColor(11);
            myBar(20,20-1+pivot,8,58);
            mySetColor(7);
            myBar(20+pivot,78,8,58);

            mySetColor(0);
            myBar(20+trig_pivot, 20+trig_pivot, 8, 58);
        } 
    }
    else if(is_showTotalHp){
        int t=AGetMainObject()->RefreshCountdown();
        int w=AGetMainObject()->Wave();
        if(w<=0){
            if(t<=200){
                int tmp=51*(200-t)/200;
                mySetColor(11);
                myBar(20,78,8,58-tmp);            
                mySetColor(0);
                myBar(20,78,58+1-tmp,58);
            }   
            else{
                mySetColor(11);
                myBar(20,78,8,58);
            }
            return;
        }       
        if((w==9 or w==19) and t<=6 and -ANowTime(ANowWave()+1)<=200){
            t=-ANowTime(ANowWave()+1);
            int tmp=51*(200-t)/200;
            mySetColor(11);
            myBar(20,78,8,58-tmp);            
            mySetColor(0);
            myBar(20,78,58+1-tmp,58);
            return;
        }
        if(w <1) return;
        int currentHp = AAsm::ZombieTotalHp(w - 1);
        int totalHp = AGetMainObject()->MRef<int>(0x5598);
        int pivot = 59*currentHp/totalHp;
        mySetColor(11);
        myBar(20,20-1+pivot,8,58);
        mySetColor(7);
        myBar(20+pivot,78,8,58);

        mySetColor(0);
        myBar(20+29, 20+29, 8, 58);
        myBar(20+38, 20+38, 8, 58);
    }
}


bool is_drawing=0;
ATickRunner drawRunner;
void show_ice_road(int t){
    auto ob_ptr = AGetMainObject();
    std::string info = "ice trail\n";
    for (int i=0;i<6;i++){
        int xi = ob_ptr->MRef<int>(0x60C + 0x4*i),
            ti = ob_ptr->MRef<int>(0x624 + 0x4*i),
            co = 1+int((xi-30)/80),
            value = ti+((t<-10000 or t>10000)?0:t);
        if(co<10) info = info + std::to_string(i+1)+"-"+ std::to_string(co)+":"+std::to_string(value)+"\n";
    }
    myText(info, 900,400);
}
void show_crater(int t){
    std::vector<APlaceItem*> item_list;
    item_list.clear();
    for (auto& item:aAlivePlaceItemFilter)
        if (item.Type() == 0x2)
            item_list.push_back(&item);
    std::sort(item_list.begin(),item_list.end(),[](APlaceItem* a,APlaceItem* b){return a->Value()<b->Value();});
    std::string info = "crater\n";
    for(auto item_ptr:item_list){
        int value = item_ptr->Value()+((t<-10000 or t>10000)?0:t),
            co = item_ptr->Col(),
            ro = item_ptr->Row();
        info = info + std::to_string(ro+1)+"-"+ std::to_string(co+1)+":"+std::to_string(value)+"\n";
        /*
            int xi = AAsm::GridToAbscissa(ro, co),
                yi = AAsm::GridToOrdinate(ro, co);
            int tmp=61*item_ptr->Value()/18000,
                xl=xi+10,xr=xi+70;
            mySetColor(7);
            myBar(xl,xl-1+tmp,yi+75,yi+78);
            mySetColor(6);
            myBar(xl+tmp,xr,yi+75,yi+78);    
        */
    }
    myText(info, 900,200);
} 
void show_ballon(){
    std::string info = "ballon\n";
    int ans = 819+100;
    for(AZombie & z:aAliveZombieFilter){
        if(z.Type()!=AQQ_16)    continue;
        int tmp = int(z.Abscissa()+100);
        if(tmp<ans)
            ans = tmp;
    }
    info = info + std::to_string(ans);
    myText(info, 900,50);
}
void showWave(){
    /*
    auto tmp = aPainter.GetRectColor();
    aPainter.SetRectColor(0x00000000);
    int w=AGetMainObject()->Wave();
    if(w<10)    myText("W0"+std::to_string(w),33,10);
    else        myText("W" + std::to_string(w),33,10);
    aPainter.SetRectColor(tmp);    
    */
    int w=AGetMainObject()->Wave();
    if(w<10)    myPureText("W0"+std::to_string(w),33,10);
    else        myPureText("W" + std::to_string(w),33,10);
}
void showTime(){
    /*
    auto tmp = aPainter.GetRectColor();
    aPainter.SetRectColor(0x00000000);
    int w=AGetMainObject()->Wave();
    int t=ANowTime(ANowWave());
    int ct=AGetMainObject()->RefreshCountdown();
    int x=27,y=35;
    if((w==9 or w==19) and ct<=6)   t=ANowTime(ANowWave()+1);
    if(t<-10000 or t>10000)  myText("????",x,y);
    else if(t<=-100)     myText("-"+std::to_string(-t),x,y);
    else if(t<=-10) myText("-0"+std::to_string(-t),x,y);
    else if(t<0)    myText("-00"+std::to_string(-t),x,y);
    else if(t<10)   myText("000"+std::to_string(t),x,y);
    else if(t<100)  myText("00"+std::to_string(t),x,y);
    else if(t<1000) myText("0"+std::to_string(t),x,y);
    else            myText(std::to_string(t),x,y);
    aPainter.SetRectColor(tmp);
    */
    int w=AGetMainObject()->Wave();
    int t=ANowTime(ANowWave());
    int ct=AGetMainObject()->RefreshCountdown();
    int x=27,y=35;
    if((w==9 or w==19) and ct<=6)   t=ANowTime(ANowWave()+1);
    if(t<-10000 or t>10000)  myPureText("????",x,y);
    else if(t<=-100)     myPureText("-"+std::to_string(-t),x,y);
    else if(t<=-10) myPureText("-0"+std::to_string(-t),x,y);
    else if(t<0)    myPureText("-00"+std::to_string(-t),x,y);
    else if(t<10)   myPureText("000"+std::to_string(t),x,y);
    else if(t<100)  myPureText("00"+std::to_string(t),x,y);
    else if(t<1000) myPureText("0"+std::to_string(t),x,y);
    else            myPureText(std::to_string(t),x,y);

    show_crater(t);
    show_ice_road(t);
    show_ballon();
}
void myDrawer(){
    if(is_drawing){
        showWave();
        showTime();
        auto ob = AGetMainObject();
        fog_x = (ob->Scene() == 3)?(ob->MRef<float>(0x5D0) + 480):10000;

        //x:10-70,
        //植物 y:15-18
        //花盆 y:75-78
        //南瓜 y:65-68
        for(auto& p : aAlivePlantFilter)
            if( p.Hp()<p.HpMax() ){
                int Xi=p.Abscissa(),Yi=p.Ordinate(),tmp=61*p.Hp()/p.HpMax();
                int Xl=Xi+10,Xr=Xi+70;
                if(!CheckPlantVisable(&p))continue;

                if(p.Type()==ANGT_30)
                    myDrawBar_plant(Xl, Xr, Yi+60, Yi+68, tmp, clr_green, clr_white);
                else if(p.Type()==AHY_16 || p.Type()==AHP_33)
                    myDrawBar_plant(Xl, Xr, Yi+75, Yi+78, tmp, clr_yellow, clr_white);
                else
                    myDrawBar_plant(Xl, Xr, Yi+15, Yi+18, tmp, clr_green, clr_white);   
            }
        //白眼 x:40-50,y:31-100(35/42/70,1500/1800/3000)
        //红眼 x:40-50,y:21-110(27/45/72/90,1800/3000/4800/6000)
        //橄榄 x:42-48,y:21-113(5/93,90/1670)  
        for(auto& z:aAliveZombieFilter){
            if(!CheckZombieVisable(&z))continue;
            if(!is_seeing_through_fog and z.Abscissa()+40>fog_x)continue;
            if(z.Type()==ABY_23){
                int Xi=z.Abscissa(),Yi=z.Ordinate();
                int tmp=70*z.Hp()/3000;
                myDrawBar_zombie(Xi+40, Xi+50, Yi+31, Yi+100, tmp,{35,42},clr_purple_white);
            }        
            else if(z.Type()==AHY_32){
                int Xi=z.Abscissa(),Yi=z.Ordinate();
                int tmp=90*z.Hp()/6000;
                myDrawBar_zombie(Xi+40, Xi+50, Yi+21, Yi+110, tmp,{27,45,72},clr_red);
            }     
            else if(z.Type()==AGL_7){
                int Xi=z.Abscissa(),Yi=z.Ordinate();
                int tmp=93*(z.Hp()+z.OneHp())/1670;
                myDrawBar_zombie(Xi+42, Xi+48, Yi+21, Yi+113, tmp,{5},clr_chocolate);
            }        
        }
    }
}


bool is_Offset=0;
ATickRunner offsetRunner;
void myShowOffset(){
    if(!is_Offset)return;
    for(auto&& p:aAlivePlantFilter)
    if(p.Type()==AXPG_8 || p.Type()==AYGG_9){
        int Xi=p.Abscissa(),Yi=p.Ordinate(),Xstd=p.Col()*80+40;
        int x=Xstd+10,y=Yi+40;
        if(Xi>Xstd)
        myText("R",x,y);
        else if(Xi<Xstd)
        myText("L",x,y);
    }
}


bool is_showCD=0;
ATickRunner CDRunner;
ABasicFilter<ASeed> mySeedFilter;
void myShowCD(){
    
    if(is_showCD){
        int cnt=0;
        for(auto& s:mySeedFilter){
            int Xl=cnt*51+90,   Xr=Xl+46,
                Yl=10,          Yr=Yl-1+68;
            if(s.IsUsable()){
                mySetColor(10);
                myBar(Xl,Xr,Yl,Yr);
            }
            else{

                int tmp=68*(s.InitialCd()-s.Cd()+1)/(s.InitialCd()+1);
                mySetColor(9);
                myBar(Xl,Xr,Yl,Yl-1+tmp);
                mySetColor(8);
                myBar(Xl,Xr,Yl+tmp,Yr);
                
                // cd
                /*
                auto tmp_clr = aPainter.GetRectColor();
                aPainter.SetRectColor(0x00000000);
                std::string str_cd = std::to_string(s.Cd());
                while (str_cd.length() < 4)     str_cd.insert(0, "0"); 
                myText(str_cd,Xl+1,Yl);
                aPainter.SetRectColor(tmp_clr);
                */
                std::string str_cd = std::to_string(s.Cd());
                while (str_cd.length() < 4)     str_cd.insert(0, "0"); 
                myPureText(str_cd,Xl+1,Yl);

            }
            cnt++;
        }
    }
}


bool is_defending=0;
ATickRunner defendRunner;
void myDefend(){
    if(is_defending){
        APlant* myP[6]={nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
        for(auto& p : aAlivePlantFilter){
            int Ri=p.Row();
            if(myGetRowType(Ri)!=1) continue;
            if(!myP[Ri]){   myP[Ri]=&p;continue;    }
            int Ci=p.Col();
            if(myP[Ri]->Col()<Ci){   myP[Ri]=&p;continue;    }
            if(myP[Ri]->Col()==Ci && p.Type()==ANGT_30){   myP[Ri]=&p;continue;    }
        }
        for(int i=0;i<6;i++){
            if(!myP[i])continue;

            int pr=myP[i]->Abscissa();
            if(myP[i]->Type()==ANGT_30)     pr+=80;
            else if(myP[i]->Type()==AGJG_23)    pr+=70;
            else if(myP[i]->Type()==AYMJNP_47)  pr+=120;
            else    pr+=50;

            int yl=myP[i]->Ordinate()+30,yr=yl+40;
            int zgl=42,zjr=40;
            //jr +40[-30,58]  gl +42[50,69]
            int Gr=zgl+pr-50,
                Jr=zjr+pr+30;


            int idx_next = AGetMainObject()->MRef<int>(0xB8);
            int idx_now =  AGetPlantIndex(myP[i]->Row()+1,myP[i]->Col()+1,myP[i]->Type());
            mySetColor(idx_now>idx_next?8:clr_yellow_40);
            myBar(Gr,Jr,yl,yr);



            mySetColor(7);
            myBar(Jr,Jr,yl,yr);
            myBar(Gr,Gr,yl,yr);


        }
    }  
}


bool is_helping=0;
void myHelp(){
    is_helping=!is_helping;
    is_counting=is_rmd=is_drawing=is_showCD=is_showTotalHp=is_defending=is_helping;
}


AReplay myReplay;
void mySetReplay(){
    myReplay.SetShowInfo(allow_replay);
    myReplay.SetMaxSaveCnt(allow_replay?replay_max_ticks:(1000/back_tick));
    if(allow_replay)
        myReplay.SetInfoPos(30, 580);
    /*
    if(!allow_back)return;
    myReplay.SetSaveDirPath("D:\\AAA_LH_SL\\");
    myReplay.SetShowInfo(allow_replay);
    myReplay.SetMaxSaveCnt(allow_replay?replay_max_ticks:(1000/back_tick));
    if(allow_replay)
        myReplay.SetInfoPos(30, 580);
    myReplay.StartRecord(back_tick);
    */
}
AOnEnterFight( mySetReplay(); );


AOnEnterFight({
    ASetGameSpeed(1);
    mySetInitLogger();
    mySetTextColor();
    is_pausing=0;
    TrigRunner.Start(myShowTrig,myMode);
    drawRunner.Start(myDrawer,myMode);

    CDRunner.Start(myShowCD,myMode);
    SmartSRunner.Start(mySmartS,myMode);
    //SmartS2Runner.Start(mySmartS2,myMode);
    defendRunner.Start(myDefend,myMode);
    StealRunner.Start(mySteal,myMode);
    rmdRunner.Start(myRmd,myMode);
    offsetRunner.Start(myShowOffset,myMode);
    giga_total_num=0;
    CountGigaRunner.Start(myShowCount,myMode);
});
void myTAS(){
    ASetReloadMode(AReloadMode::MAIN_UI_OR_FIGHT_UI);

    AConnect('1',[=]{ASetGameSpeed(0.1);});
    AConnect('2',[=]{ASetGameSpeed(0.25);});
    AConnect('3',[=]{ASetGameSpeed(0.5);});
    AConnect('4',[=]{ASetGameSpeed(0.75);});
    AConnect('5',[=]{ASetGameSpeed(1);});
    AConnect('6',[=]{ASetGameSpeed(1.5);});
    AConnect('7',[=]{ASetGameSpeed(2);});
    AConnect('8',[=]{ASetGameSpeed(3);});
    AConnect('9',[=]{ASetGameSpeed(5);});
    AConnect('0',[=]{ASetGameSpeed(10);});

    AConnect('A',[=]{   is_drawing=!is_drawing; });

    AConnect('C',[=]{   is_showCD=!is_showCD;   });

    AConnect('X',[=]{   is_showTotalHp=!is_showTotalHp; }); 
    AConnect('T',[=]{   is_showTrig=!is_showTrig;   }); 

    AConnect('Z',[=]{   if(is_drawing)is_defending=!is_defending;   });

    AConnect('S',[=]{   is_Stealth=!is_Stealth; });

    AConnect('W', []{   myPause();  });

    AConnect('D',[=]{
        static bool isDancing = false;
        isDancing=!isDancing;
        if(isDancing)   AMaidCheats::Dancing();
        else            AMaidCheats::Stop();
    });
    
    AConnect('Q',[=]{   myQuickShovel();});
    AConnect('E',[=]{   myHelp();   });

    AConnect('F',[=]{   is_rmd=!is_rmd; });

    AConnect('R',[=]{   is_SmartS=!is_SmartS;   });

    AConnect('V',[=]{   is_Offset=!is_Offset;   });

    AConnect('G',[=]{   is_counting=!is_counting;   });

    AConnect('Y',[=]{   is_seeing_through_fog=!is_seeing_through_fog;   });
    AConnect(VK_CAPITAL,[]{myPauseAndTick();});



    AConnect(VK_TAB, [] {
        //myTickBack();
        if(!allow_back)return;
        if(!back_constraint)
            myReplay.ShowOneTick(myReplay.GetRecordIdx() - 1);    
        else{
            int t=ANowTime(ANowWave());
            int w=AGetMainObject()->Wave();
            if(w == 0)  t = t+599;
            //if(t>52)
            //    myReplay.ShowOneTick(myReplay.GetRecordIdx() - (50/back_tick));
            //else 
            if(t>back_tick+1)
                myReplay.ShowOneTick(myReplay.GetRecordIdx() - 1);
        }
    });
    AConnect('P',[]{
        if(!allow_replay) return;
        myReplay.Stop();
        myReplay.StartPlay();
    });
    AConnect(VK_RETURN,[]{
        if(!allow_replay) return;
        myReplay.SetSaveDirPath("D:\\AAA_LH_SL\\");
        myReplay.StartRecord(back_tick);

        //myReplay.StartRecord(back_tick);
    });
    AConnect(VK_BACK,[]{
        if(!allow_replay) return;
        myReplay.Stop();
    });




}


#endif