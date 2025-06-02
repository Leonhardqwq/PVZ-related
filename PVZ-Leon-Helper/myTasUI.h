//#pragma once
#ifndef __MYTASUI_H__
#define __MYTASUI_H__

#include <win32gui/main.h>
#include <string>
#include <myTas.h>
#include <filesystem>
#include <iomanip>
#include <tlhelp32.h>

// win初始化

// 页面距离
constexpr int SPACE = 10;
constexpr int WIDTH = 100;
constexpr int HEIGHT = 30;
// 主页面
constexpr int MAIN_WIDTH = (SPACE + WIDTH) * 5 + SPACE + 2*SPACE;
constexpr int MAIN_HEIGHT =(SPACE + HEIGHT)*15 + SPACE;

// 生成主页面
AMainWindow mainWindow("Leon Helper",MAIN_WIDTH, MAIN_HEIGHT);
// 提示信息
ALabel* infoLabel = nullptr;
void Info(const std::string& tip){  if(infoLabel != nullptr)    infoLabel->SetText(tip);    }
void Warning(const std::string& tip){   Info(tip);  MessageBeep(MB_ICONWARNING);    }
// edit
std::pair<AEdit*, APushButton*> CreateEditButton(AWindow* window, const std::string& labelText,
    const std::string& editText, const std::string& btnText, int x, int y)
{
    auto label = window->AddLabel(labelText, x, y);
    x += SPACE + label->GetWidth();
    auto edit = window->AddEdit(editText, x, y, WIDTH, HEIGHT);
    x += SPACE + edit->GetWidth();
    auto btn = window->AddPushButton(btnText, x, y);
    return {edit, btn};
}
template <typename Func>
void CreateKeyButton(AWindow* window, const std::string& labelText,
    AKey& key, const std::string& btnText, int x, int y, AConnectHandle& handle, Func&& func)
{
    std::string str;
    str.push_back(key);
    auto [edit, btn] = CreateEditButton(window, labelText, str, btnText, x, y);
    AConnect(btn, [&key, &handle, edit = edit, func = std::forward<Func>(func)] mutable {
        auto str = edit->GetText();
        if ((str[0] > 'Z' || str[0] < 'A') && (str[0] > '9' || str[0] < '0')) {
            Warning("绑定的按键只能为大写字母或数字");
            return;
        }
        handle.Stop();
        key = str[0];
        handle = AConnect(str[0], std::move(func));
        Info(std::string("按键") + str[0] + "绑定成功");
    });
}
// comboBox
template <typename Func>
void CreateComboBoxButton(AWindow* window, const std::string& labelText,const std::string& btnText, 
    int x, int y, const std::vector<const std::string>& comboText_list,std::vector<Func> func_list)
{
    auto label = window->AddLabel(labelText, x, y);
    x += SPACE + label->GetWidth();
    auto box = window->AddComboBox(x, y);
    x += SPACE + box->GetWidth();
    auto btn = window->AddPushButton(btnText, x, y);
    for(std::string text:comboText_list)
        box->AddString(text);

    AConnect(btn, [=] {
        auto str = box->GetString();
        auto it = std::find(comboText_list.begin(), comboText_list.end(), str);
        if(it == comboText_list.end()){
            Warning("无效的选项！");
        }
        else{
            int idx = std::distance(comboText_list.begin(), it);
            func_list[idx]();
            Info(labelText+"("+str+")"+ btnText+ "成功");
        }
    });
}
// checkBox
template <typename Func1,typename Func2>
void CreateCheckBox(AWindow* window, const std::string& boxText,int x, int y,ACheckBox** box, 
    ACtrlConnHandle& handle,Func1&& funcTrue,Func2&& funcFalse)
{
    *box = window->AddCheckBox(boxText, x, y);
    handle = AConnect((*box), [=] {
        if ((*box)->GetCheck())    funcTrue();
        else                    funcFalse();
    });
}
// set checkbox
void mySetCheckBox(ACheckBox* box,bool status){
    if(box==nullptr)return;
    box->SetCheck(status);
}

// tas 组件初始化
// key init
static AConnectHandle quickShovel_handle;
static AConnectHandle pause_handle;
static AConnectHandle pauseAndTick_handle;
static AConnectHandle bigPauseAndTick_handle;
static AConnectHandle hugePauseAndTick_handle;
static AConnectHandle speedSwitch_handle;

AKey quickShovel_key = 'Q';
AKey pause_key = 'W';
AKey pauseAndTick_key = 'D';
AKey speedSwitch_key = 'S';

// checkbox init
ACheckBox* drawInfo_box = nullptr;
ACheckBox* smartShovel_box = nullptr;
ACheckBox* rmd_box = nullptr;
ACheckBox* count_box = nullptr;
ACheckBox* defend_box = nullptr;
ACheckBox* showCD_box = nullptr;

ACheckBox* trig_box = nullptr;
ACheckBox* seeingFog_box = nullptr;
ACheckBox* steal_box = nullptr;
ACheckBox* showOffset_box = nullptr;
ACheckBox* quickMode_box = nullptr;

ACheckBox* usual_box = nullptr;
ACheckBox* all_box = nullptr;

ACheckBox* backLim_box = nullptr;
ACheckBox* autoRecord_box = nullptr;
ACheckBox* allowPlay_box = nullptr;

// 等待维护
ACtrlConnHandle drawInfo_handle;
ACtrlConnHandle smartShovel_handle;
ACtrlConnHandle rmd_handle;
ACtrlConnHandle count_handle;
ACtrlConnHandle defend_handle;
ACtrlConnHandle showCD_handle ;

ACtrlConnHandle trig_handle ;
ACtrlConnHandle seeingFog_handle ;
ACtrlConnHandle steal_handle ;
ACtrlConnHandle showOffset_handle ;
ACtrlConnHandle quickMode_handle ;

ACtrlConnHandle usual_handle ;
ACtrlConnHandle all_handle ;

ACtrlConnHandle backLim_handle ;
ACtrlConnHandle autoRecord_handle ;
ACtrlConnHandle allowPlay_handle ;

// comboBox init
float myGameSpeed = 1;
float nowGameSpeed = 1;

// allow_back = true;
// allow_replay = true;
// replay_max_ticks=7000;
// back_tick = 10;

#define FightUiCheck()                            \
    if (AGetPvzBase()->GameUi() != 3) {           \
        Warning("只有在战斗界面才能使用此功能!"); \
        return;                                   \
    }
#define FightOrCardUiCheck()                                            \
    if (AGetPvzBase()->GameUi() != 3 && AGetPvzBase()->GameUi() != 2) { \
        Warning("只有在战斗界面或者选卡界面才能使用此功能!");           \
        return;                                                         \
    }
 
#define __CheckASCII(path, info, ret) \
    for (auto c : path) {             \
        if (uint8_t(c) > 127) {       \
            info;                     \
            return ret;               \
        }                             \
    }
std::string GetCurTimeStr(){
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d-%H-%M-%S");
    return ss.str();
}
char savePath[1000];
char playPath[1000];
void replay_startRecord(){
    FightUiCheck();
    if(myReplay.GetState() != AReplay::REST){Warning("请先退出录制");return;}
    myReplay.SetSaveDirPath(savePath+std::string("/") + GetCurTimeStr() + "/");
    myReplay.StartRecord(back_tick);
    Info("Replay : 开始录制");
};
void replay_stop(){myReplay.Stop();};
void replay_startPlay(){
    FightUiCheck();
    if(!allow_replay){Warning("不允许播放");return;}
    if (myReplay.GetState() != AReplay::REST) {Warning("请先退出录制");return;}
    std::string fileName = playPath;
    if (fileName.empty()) {return;}
    myReplay.SetSaveDirPath(fileName);
    myReplay.StartPlay();
    Info("Replay : 开始播放");
};
void replay_pause(){
    FightUiCheck();
    if (myReplay.GetState() != AReplay::PLAYING) {Warning("只能在播放状态使用");return;}
    if (myReplay.IsPaused()) {
        if (myReplay.GetState() == myReplay.RECORDING) myPause();
        myReplay.GoOn();
    } 
    else    myReplay.Pause();
};
void replay_preTick(){
    FightUiCheck();
    if (myReplay.GetState() != AReplay::RECORDING) {Warning("只能在录制状态使用");return;}
    if(!back_constraint)
        myReplay.ShowOneTick(myReplay.GetRecordIdx() - 1);    
    else{
        int t=ANowTime(ANowWave());
        int w=AGetMainObject()->Wave();
        if(w == 0)  t = t+599;
        if(t>back_tick+1)
            myReplay.ShowOneTick(myReplay.GetRecordIdx() - 1);
    }
};

struct {
    bool checkTypes[20] = {
        0,0,0,1,0,
        1,0,0,1,0,
        1,1,0,0,0,
        1,0,0,1,1};
    bool checkAverageRowSpawn = false;
    bool checkRandomType = false;
} settings;

static AConnectHandle preTick_handle;
AKey preTick_key = 'A';
void CreateReplayGroup(AWindow* window, int x, int y){
    window->AddLabel("回放功能", x, y, (SPACE + WIDTH) * 5 + SPACE,(SPACE + HEIGHT) * 4+SPACE);
    x = SPACE + WIDTH+SPACE;
    
    CreateCheckBox(window,"回溯限制", x, y, &backLim_box, backLim_handle,
        [=]{back_constraint=1;},
        [=]{back_constraint=0;}
    );
    backLim_box->SetCheck(back_constraint);
    x += WIDTH+SPACE;

    CreateCheckBox(window,"自动录制", x, y, &autoRecord_box, autoRecord_handle,
        [=]{                                 
            if (AGetPvzBase()->GameUi() == 3 ) { 
                Warning("战斗界面无法更改此设置!"); 
                autoRecord_box->SetCheck(0);
                return;
            }  
            allow_autoRecording=1;},
        [=]{
            if (AGetPvzBase()->GameUi() == 3 ) { 
                Warning("战斗界面无法更改此设置!"); 
                autoRecord_box->SetCheck(1);
                return;
            }  
            allow_autoRecording=0;}
    );
    autoRecord_box->SetCheck(allow_autoRecording);
    x += WIDTH+SPACE;

    CreateCheckBox(window,"允许播放", x, y, &allowPlay_box, allowPlay_handle,
        [=]{
            if (AGetPvzBase()->GameUi() == 3 ) { 
                Warning("战斗界面无法更改此设置!"); 
                allowPlay_box->SetCheck(0);
                return;
            }  
            allow_replay=1; },
        [=]{
            if (AGetPvzBase()->GameUi() == 3 ) { 
                Warning("战斗界面无法更改此设置!"); 
                allowPlay_box->SetCheck(1);
                return;
            }  
            allow_replay=0; }
    );
    allowPlay_box->SetCheck(allow_replay);
    x += WIDTH+SPACE;

    x = SPACE;
    y += SPACE + HEIGHT;
    auto startRecordBtn = window->AddPushButton("开始录制", x, y);
    AConnect(startRecordBtn,[=]{replay_startRecord();});
    x += SPACE + WIDTH;
    auto stopBtn = window->AddPushButton("退出录制/播放", x, y);
    AConnect(stopBtn, [=]{replay_stop();});
    x += SPACE + WIDTH;
    auto startPlayBtn = window->AddPushButton("开始播放", x, y);
    AConnect(startPlayBtn, [=]{replay_startPlay();});
    x += SPACE + WIDTH;
    auto pauseBtn = window->AddPushButton("暂停播放", x, y);
    AConnect(pauseBtn, [=]{replay_pause();});
    x += SPACE + WIDTH;
    auto preTickBtn = window->AddPushButton("上一帧", x, y);
    AConnect(preTickBtn, [=]{replay_preTick();});

    // 录制路径
    y += SPACE + HEIGHT;
    x = SPACE;
    window->AddLabel("录制路径:", x, y);
    x += SPACE + WIDTH;
    auto savePathEdit = window->AddEdit(savePath, x, y, WIDTH * 3 + SPACE, 30, ES_AUTOHSCROLL);
    savePathEdit->SetText(savePath);
    x += (SPACE + WIDTH) * 3;
    auto btn = window->AddPushButton("设置", x, y);
    AConnect(btn, [=] {
        auto path = savePathEdit->GetText();
        __CheckASCII(path, Warning("您设置的保存路径: [" + path + "] 中含有非 ASCII 字符, 请将其设置为纯英文路径再次尝试");
                     savePathEdit->SetText(savePath), );
        if (!std::filesystem::exists(path)) {
            Warning("设置的路径: [" + path + "] 不存在");
            savePathEdit->SetText(savePath);
        } else {
            std::strcpy(savePath, path.c_str());
            Info("设置路径: [" + path + "] 成功");
        }
    });

    // 播放路径
    y += SPACE + HEIGHT;
    x = SPACE;
    window->AddLabel("播放路径:", x, y);
    x += SPACE + WIDTH;
    auto playPathEdit = window->AddEdit(playPath, x, y, WIDTH * 3 + SPACE, 30, ES_AUTOHSCROLL);
    playPathEdit->SetText(playPath);
    x += (SPACE + WIDTH) * 3;
    auto btn_play = window->AddPushButton("设置", x, y);
    AConnect(btn_play, [=] {
        auto path = playPathEdit->GetText();
        __CheckASCII(path, Warning("您设置的保存路径: [" + path + "] 中含有非 ASCII 字符, 请将其设置为纯英文路径再次尝试");
                     playPathEdit->SetText(playPath), );
        if (!std::filesystem::exists(path)) {
            Warning("设置的路径: [" + path + "] 不存在");
            playPathEdit->SetText(playPath);
        } else {
            std::strcpy(playPath, path.c_str());
            Info("设置路径: [" + path + "] 成功");
        }
    });



}
// 子页面1
AWindow* CreatePage1(AWindow* parent, int pageX, int pageY){
    auto window = parent->AddWindow(pageX, pageY);
    window->AddLabel("", 0, 0, (SPACE + WIDTH) * 5 + SPACE, (SPACE + HEIGHT) * 7+SPACE);

    int x = SPACE;
    int y = SPACE;

    // row 1~3
    CreateKeyButton(window, "快捷铲子:", quickShovel_key, "设置", x, y,
        quickShovel_handle,myQuickShovel);
    y += SPACE + HEIGHT;
    CreateKeyButton(window, "暂停:", pause_key, "设置", x, y,
        pause_handle,myPause);
    y += SPACE + HEIGHT;
    CreateKeyButton(window, "下一刻:", pauseAndTick_key, "设置", x, y,
        pauseAndTick_handle,myPauseAndTick);
    y += SPACE + HEIGHT;
    CreateKeyButton(window, "上一帧:", preTick_key, "设置", x, y,
        preTick_handle,[=]{  replay_preTick();});
    y += SPACE + HEIGHT;    
    CreateKeyButton(window, "倍速切换:", speedSwitch_key, "设置", x, y,
        speedSwitch_handle,[=]{  
            if(myGameSpeed == nowGameSpeed){    ASetGameSpeed(1);   nowGameSpeed = 1;   }
            else{   ASetGameSpeed(myGameSpeed);   nowGameSpeed = myGameSpeed;  }

        });
    y += SPACE + HEIGHT;

    std::vector<std::function<void()>> maid_funcs = {
        [=]{AMaidCheats::Stop(); },
        [=]{AMaidCheats::Dancing(); },
        [=]{AMaidCheats::Move();  },
        [=]{AMaidCheats::CallPartner(); }
    };
    CreateComboBoxButton(window, "女仆模式", "修改", x, y, {"停用","跳舞","前进","召唤"}, maid_funcs);
    y += SPACE + HEIGHT;

    std::vector<std::function<void()>> speed_funcs = {
        [=]{ nowGameSpeed=myGameSpeed = 1; ASetGameSpeed(myGameSpeed); },
        [=]{ nowGameSpeed=myGameSpeed = 10; ASetGameSpeed(myGameSpeed); },
        [=]{ nowGameSpeed=myGameSpeed = 5; ASetGameSpeed(myGameSpeed); },
        [=]{ nowGameSpeed=myGameSpeed = 3; ASetGameSpeed(myGameSpeed); },
        [=]{ nowGameSpeed=myGameSpeed = 2; ASetGameSpeed(myGameSpeed); },
        [=]{ nowGameSpeed=myGameSpeed = 1.5; ASetGameSpeed(myGameSpeed); },
        [=]{ nowGameSpeed=myGameSpeed = 0.75; ASetGameSpeed(myGameSpeed); },
        [=]{ nowGameSpeed=myGameSpeed = 0.5; ASetGameSpeed(myGameSpeed); },
        [=]{ nowGameSpeed=myGameSpeed = 0.25; ASetGameSpeed(myGameSpeed); },
        [=]{ nowGameSpeed=myGameSpeed = 0.1; ASetGameSpeed(myGameSpeed); },
    };
    CreateComboBoxButton(window, "倍速", "修改", x, y, 
    {"1","10","5","3","2","1.5","0.75","0.5","0.25","0.1"}, 
    speed_funcs);
    y += SPACE + HEIGHT;

    // row 4
    x = (SPACE + WIDTH) * 3 + SPACE;
    y = SPACE;

    CreateCheckBox(window,"显示信息", x, y, &drawInfo_box, drawInfo_handle,
        [=]{is_drawing=is_showTotalHp=1;},
        [=]{is_drawing=is_showTotalHp=0;}
    );
    y += SPACE + HEIGHT;
    CreateCheckBox(window,"智能铲子", x, y, &smartShovel_box, smartShovel_handle,
        [=]{is_SmartS=1;},
        [=]{is_SmartS=0;}
    );
    y += SPACE + HEIGHT;
    CreateCheckBox(window,"开启提示", x, y, &rmd_box, rmd_handle,
        [=]{is_rmd=1;},
        [=]{is_rmd=0;}
    );
    y += SPACE + HEIGHT;
    CreateCheckBox(window,"红橄计数", x, y, &count_box, count_handle,
        [=]{is_counting=1;},
        [=]{is_counting=0;}
    );
    y += SPACE + HEIGHT;
    CreateCheckBox(window,"卡槽高亮", x, y, &showCD_box, showCD_handle,
        [=]{is_showCD=1;},
        [=]{is_showCD=0;}
    );
    y += SPACE + HEIGHT;
    CreateCheckBox(window,"显示防御域", x, y, &defend_box, defend_handle,
        [=]{is_defending=1;},
        [=]{is_defending=0;}
    );
    y += SPACE + HEIGHT;
    CreateCheckBox(window,"一键常用", x, y, &usual_box, usual_handle,
        [=]{
            // Info(std::to_string(smartShovel_handle));
            mySetCheckBox(all_box, 0);

            mySetCheckBox(smartShovel_box, 1);
            mySetCheckBox(rmd_box, 1);
            mySetCheckBox(count_box, 1);
            mySetCheckBox(defend_box, 1);
            mySetCheckBox(showCD_box, 1);
            mySetCheckBox(drawInfo_box, 1);

            mySetCheckBox(trig_box, 0);
            mySetCheckBox(seeingFog_box, 0);
            mySetCheckBox(steal_box, 0);
            mySetCheckBox(showOffset_box, 0);
            is_drawing=is_showTotalHp=is_SmartS = is_rmd = is_counting = is_defending = is_showCD = 1;
            is_showTrig = is_seeing_through_fog = is_Stealth = is_Offset = 0;
        },
        [=]{
            mySetCheckBox(all_box, 0);

            mySetCheckBox(smartShovel_box, 0);
            mySetCheckBox(rmd_box, 0);
            mySetCheckBox(count_box, 0);
            mySetCheckBox(defend_box, 0);
            mySetCheckBox(showCD_box, 0);
            mySetCheckBox(drawInfo_box, 0);

            mySetCheckBox(trig_box, 0);
            mySetCheckBox(seeingFog_box, 0);
            mySetCheckBox(steal_box, 0);
            mySetCheckBox(showOffset_box, 0);
            is_drawing=is_showTotalHp=is_SmartS = is_rmd = is_counting = is_defending = is_showCD = 0;
            is_showTrig = is_seeing_through_fog = is_Stealth = is_Offset = 0;
        }
    );

    //row 5
    x = (SPACE + WIDTH) * 4 + SPACE;
    y = SPACE;
    CreateCheckBox(window,"读取刷新", x, y, &trig_box, trig_handle,
        [=]{is_showTrig=1;},
        [=]{is_showTrig=0;}
    );
    y += SPACE + HEIGHT;
    CreateCheckBox(window,"蘑菇偏移", x, y, &showOffset_box, showOffset_handle,
        [=]{is_Offset=1;},
        [=]{is_Offset=0;}
    );
    y += SPACE + HEIGHT;
    CreateCheckBox(window,"显血透雾", x, y, &seeingFog_box, seeingFog_handle,
        [=]{is_seeing_through_fog=1;},
        [=]{is_seeing_through_fog=0;}
    );
    y += SPACE + HEIGHT;
    CreateCheckBox(window,"普僵系隐身", x, y, &steal_box, steal_handle,
        [=]{is_Stealth=1;},
        [=]{is_Stealth=0;}
    );
    y += SPACE + HEIGHT;
    CreateCheckBox(window,"快捷模式", x, y, &quickMode_box, quickMode_handle,
        [=]{AGetPvzBase()->MRef<bool>(0x7F5) = 1;},
        [=]{AGetPvzBase()->MRef<bool>(0x7F5) = 0;}
    );

    y += SPACE + HEIGHT;
    y += SPACE + HEIGHT;
    CreateCheckBox(window,"一键全选", x, y, &all_box, all_handle,
        [=]{
            mySetCheckBox(usual_box, 0);

            mySetCheckBox(smartShovel_box, 1);
            mySetCheckBox(rmd_box, 1);
            mySetCheckBox(count_box, 1);
            mySetCheckBox(defend_box, 1);
            mySetCheckBox(showCD_box, 1);
            mySetCheckBox(drawInfo_box, 1);

            mySetCheckBox(trig_box, 1);
            mySetCheckBox(seeingFog_box, 1);
            mySetCheckBox(steal_box, 1);
            mySetCheckBox(showOffset_box, 1);
            is_drawing=is_showTotalHp=is_SmartS = is_rmd = is_counting = is_defending = is_showCD = 1;
            is_showTrig = is_seeing_through_fog = is_Stealth = is_Offset = 1;
        },
        [=]{
            mySetCheckBox(usual_box, 0);

            mySetCheckBox(smartShovel_box, 0);
            mySetCheckBox(rmd_box, 0);
            mySetCheckBox(count_box, 0);
            mySetCheckBox(defend_box, 0);
            mySetCheckBox(showCD_box, 0);
            mySetCheckBox(drawInfo_box, 0);

            mySetCheckBox(trig_box, 0);
            mySetCheckBox(seeingFog_box, 0);
            mySetCheckBox(steal_box, 0);
            mySetCheckBox(showOffset_box, 0);
            is_drawing=is_showTotalHp=is_SmartS = is_rmd = is_counting = is_defending = is_showCD = 0;
            is_showTrig = is_seeing_through_fog = is_Stealth = is_Offset = 0;
        }
    );
    y += SPACE + HEIGHT;

    y+=SPACE;

    CreateReplayGroup(window, 0, y);

    return window;
}

// 出怪列表label
ALabel* zombieListName_label = nullptr;
ALabel* zombieListInfo_label[20] = {};
ALabel* zombieListSum_label = nullptr;

void zombieListInfo_update(){
    std::string name_list[33] = {
        "普僵",
        "旗帜",
        "路障",
        "撑杆",
        "铁桶",
        "读报",
        "铁门",
        "橄榄",
        "舞王",
        "伴舞",
        "鸭子",
        "潜水",
        "冰车",
        "雪橇",
        "海豚",
        "小丑",
        "气球",
        "矿工",
        "跳跳",
        "雪人",
        "蹦极",
        "扶梯",
        "投篮",
        "白眼",
        "小鬼",
        "僵博",
        "豌豆",
        "坚果",
        "辣椒",
        "机枪",
        "倭瓜",
        "高坚",
        "红眼"
    };
    int zombie_list[33][20]={};
    int zombie_sum[33]={};
    auto list = AGetMainObject()->ZombieList();
    for(int w=1;w<=20;w++)
        for(int i = 50*(w-1);i<50*w;i++){
            auto tmp = *( list+i);
            if(tmp == 0xffffffff)   break;
            else if(tmp>=0 and tmp<=32)
                zombie_list[tmp][w-1]++;
        }
    for(int i=0;i<=32;i++){
        if(i==1)continue;
        for(int w=0;w<20;w++)
            zombie_sum[i]+=zombie_list[i][w];
    }

    std::string name = "wave\n";
    std::string info_w[20];
    for(int i=0;i<20;i++)   info_w[i]= (i+1>=10?"":" ")+std::to_string(i+1)+"\n";
    std::string sum = "sum\n";
    for(int i=0;i<=32;i++){
        if(i==1)continue;
        if(zombie_sum[i]==0) continue;
        name += name_list[i]+"\n";
        sum +=  std::to_string(zombie_sum[i])+"\n";
        for(int w=0;w<20;w++)
            info_w[w] += (zombie_list[i][w]>=10?"":" ") + std::to_string(zombie_list[i][w])+"\n";
    }
    
    if(zombieListName_label != nullptr)    zombieListName_label->SetText(name);   
    for(int w=0;w<20;w++)   if(zombieListInfo_label[w] != nullptr)    zombieListInfo_label[w]->SetText(info_w[w]);   
    if(zombieListSum_label != nullptr)    zombieListSum_label->SetText(sum);   
}

// 出怪页面
AWindow* CreateSpawnPage(AWindow* parent, int pageX, int pageY){
    auto window = parent->AddWindow(pageX, pageY);
    struct Info {
        std::string name;
        int type;
        ACheckBox* box;
    };
    std::vector<Info> infos = {
        {"路障", ALZ_2},
        {"撑杆", ACG_3},
        {"铁桶", ATT_4},
        {"读报", ADB_5},
        {"铁门", ATM_6},
        {"橄榄", AGL_7},
        {"舞王", AWW_8},
        {"潜水", AQS_11},
        {"冰车", ABC_12},
        {"海豚", AHT_14},
        {"小丑", AXC_15},
        {"气球", AQQ_16},
        {"矿工", AKG_17},
        {"跳跳", ATT_18},
        {"雪人", AXR_19},
        {"蹦极", ABJ_20},
        {"扶梯", AFT_21},
        {"投篮", ATL_22},
        {"白眼", ABY_23},
        {"红眼", AHY_32},
    };

    constexpr int ROW_CNT = 4;
    constexpr int COL_CNT = 5;
    constexpr int HEIGHT = 30;

    window->AddLabel("", 0, 0, (WIDTH + SPACE) * 5+SPACE, (HEIGHT + SPACE) * 5+SPACE);

    for (int row = 0; row < ROW_CNT; ++row) {
        for (int col = 0; col < COL_CNT; ++col) {
            int x = col * (WIDTH + SPACE) + SPACE;
            int y = row * (HEIGHT + SPACE) + SPACE ;
            int idx = row * COL_CNT + col;
            infos[idx].box = window->AddCheckBox(infos[idx].name, x, y);
            infos[idx].box->SetCheck(settings.checkTypes[idx]);
        }
    }

    int x = SPACE;
    int y = (HEIGHT + SPACE) * 4 + SPACE * 1;
    auto internalBtn = window->AddPushButton("自然出怪", x, y);
    x += SPACE + WIDTH;
    auto updateListBtn = window->AddPushButton("显示列表", x, y);
    x += SPACE + WIDTH;
    auto randomTypeBox = window->AddCheckBox("随机添加种类", x, y, 120);
    randomTypeBox->SetCheck(settings.checkRandomType);

    x += SPACE + WIDTH;
    x += SPACE + WIDTH;
    auto averageBtn = window->AddPushButton("极限出怪", x, y);

    auto spawnFunc = [infos = std::move(infos)](ASetZombieMode mode) {
        std::vector<int> types;
        types.push_back(APJ_0);
        for (int i = 0; i < infos.size(); ++i) {
            settings.checkTypes[i] = infos[i].box->GetCheck();
            if (settings.checkTypes[i]) {
                types.push_back(infos[i].type);
            }
        }
        FightOrCardUiCheck();
        if (settings.checkRandomType) {
            try {
                types = ACreateRandomTypeList(types);
            } catch (AException& exce) {
                Warning(std::string("捕获到 AException:") + exce.what());
                return;
            }
        }
        ASetZombies(types, mode);
        ::Info("出怪设置成功");
        zombieListInfo_update();
    };
    AConnect(averageBtn, [=] {  spawnFunc(ASetZombieMode::AVERAGE); });
    AConnect(internalBtn, [spawnFunc = std::move(spawnFunc)] {  spawnFunc(ASetZombieMode::INTERNAL);    });
    AConnect(randomTypeBox, [randomTypeBox] {   settings.checkRandomType = randomTypeBox->GetCheck();   });
    AConnect(updateListBtn, [=] {  zombieListInfo_update();  });

    y += HEIGHT + SPACE;
    y += SPACE;
    x = SPACE;

    window->AddLabel("", 0, y,  (SPACE + WIDTH) * 5 + SPACE, (SPACE + HEIGHT) * 6+SPACE);

    int tmp_height = (SPACE + HEIGHT) * 6+SPACE;
    zombieListName_label = window->AddLabel("", x, y,40,tmp_height);
    x +=45;
    for(int i =0;i<20;i++){
        zombieListInfo_label[i] = window->AddLabel("", x, y,20,tmp_height);
        x+=23;
    }
    x+=10;
    zombieListSum_label = window->AddLabel("", x, y, (SPACE + WIDTH) * 5 + SPACE - x,tmp_height);

    return window;
}

AOnAfterInject({
    strcpy(savePath, "D:\\AAA_LH_SL");
    // strcpy(savePath, "C:\\ProgramData\\PopCap Games\\PlantsVsZombies");

    int x = SPACE;
    int y = SPACE;

    auto page1Btn = mainWindow.AddPushButton("功能", x, y);
    auto page1 = CreatePage1(&mainWindow, SPACE, y + HEIGHT + SPACE);

    x += SPACE + WIDTH;

    auto spawnBtn = mainWindow.AddPushButton("出怪", x, y);
    auto spawnPage = CreateSpawnPage(&mainWindow, SPACE, y + HEIGHT + SPACE);

    mainWindow.AddLabel("信息:", SPACE, MAIN_HEIGHT - SPACE*2  - HEIGHT * 2, MAIN_WIDTH-2*SPACE, 2*HEIGHT);
    infoLabel = mainWindow.AddLabel("", SPACE + WIDTH, MAIN_HEIGHT - SPACE*2  - HEIGHT * 2, MAIN_WIDTH-2*SPACE - WIDTH,  2*HEIGHT);
    
    AConnect(page1Btn, [=] {
        spawnPage->Hide();
        page1->Show();
    });
    AConnect(spawnBtn, [=] {
        spawnPage->Show();
        page1->Hide();
    });
    spawnPage->Hide();
    page1->Show();
});
AOnEnterFight(
    //myText(std::to_string(zombieListInfo_label != nullptr), 100 , 100);
    if(allow_autoRecording)
        replay_startRecord();
    zombieListInfo_update();
);


void myTAS_ui(){
    ASetReloadMode(AReloadMode::MAIN_UI_OR_FIGHT_UI);
    mySetInitLogger();
    mySetTextColor();

    is_pausing=0;

    quickShovel_handle = AConnect(quickShovel_key,myQuickShovel);
    pause_handle = AConnect(pause_key,myPause);
    pauseAndTick_handle = AConnect(pauseAndTick_key,myPauseAndTick);
    bigPauseAndTick_handle = AConnect('E',myBigPauseAndTick);
    hugePauseAndTick_handle = AConnect('F',myHugePauseAndTick);
    speedSwitch_handle = AConnect(speedSwitch_key,[=]{  
        if(myGameSpeed == nowGameSpeed){    ASetGameSpeed(1);   nowGameSpeed = 1;   }
        else{   ASetGameSpeed(myGameSpeed);   nowGameSpeed = myGameSpeed;  }
    });
    preTick_handle = AConnect(preTick_key,[=]{replay_preTick();});

}

#endif