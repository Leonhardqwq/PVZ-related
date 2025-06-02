#include<cstdio>
#include <iostream>

#include <cstdint>
#include <string>
#include <vector>
#include <queue>

#include <algorithm>

#include <thread>
#include <mutex>

#include <fstream>


unsigned int MAX_THREAD = std::thread::hardware_concurrency();
const uint32_t SEED_STEP = 0x65;
std::mutex mtx;

enum Zombie {
    ALZ_2 = 0,    // 路障
    ACG_3,        // 撑杆
    ATT_4,        // 铁桶
    ADB_5,        // 读报

    ATM_6,        // 铁门
    AGL_7,        // 橄榄
    AWW_8,        // 舞王
    AQS_11,       // 潜水

    ABC_12,       // 冰车
    AHT_14,       // 海豚
    AXC_15,       // 小丑
    AQQ_16,       // 气球

    AKG_17,       // 矿工
    ATT_18,       // 跳跳
    ABJ_20,       // 蹦极
    AFT_21,       // 扶梯

    ATL_22,       // 投篮
    ABY_23,       // 白眼
    AHY_32        // 红眼
};
enum Scene {
    DAY,
    NIGHT,
    POOL,
    ROOF
};
const std::string ZombieName[19] = {
    "路障",   "撑杆",  "铁桶",  "读报",  "铁门", 
    "橄榄",   "舞王",  "潜水",  "冰车",  "海豚", 
    "小丑", "气球", "矿工", "跳跳", "蹦极", 
    "扶梯", "投篮", "白眼", "红眼"
};

class ZombieTypeList{
private:
    // rng
    uint32_t buf[0x270];
    uint32_t idx;
    void rng_init(uint32_t seed){
        buf[0] = seed;
        for (int i = 1; i < 0x18e; ++i) 
            buf[i] = (buf[i - 1] ^ (buf[i - 1] >> 30)) * 0x6C078965 + i;
        idx = 0;
    }
    uint32_t rng_gen(int max){
        uint32_t tmp;

        tmp = (buf[idx] & 0x80000000) | (buf[idx + 1] & 0x7FFFFFFF);
        buf[idx] = (tmp >> 1) ^ buf[(idx + 0x18D) % 0x270];
        if (tmp & 1)    buf[idx] ^= 0x9908B0DF;
        
        buf[idx + 0x18D + 1] = (buf[idx + 0x18D] ^ (buf[idx + 0x18D] >> 30)) * 0x6C078965 + idx + 0x18D + 1;

        tmp = buf[idx];
        tmp ^= (tmp >> 11);
        tmp ^= ((tmp & 0xFF3A58AD) << 7);
        tmp ^= ((tmp & 0xFFFFDF8C) << 15);
        tmp ^= (tmp >> 18);
        idx = idx + 1;
        return (tmp & 0x7FFFFFFF) % max;
    }

    //scene allow
    const uint32_t allow_mask[4] = {
        0x307fd7f,
        0x307fc7f,
        0x307ffff,
        0x307ed3f
    };

    //gen_type->bit_num
    const uint32_t conv_id[33] = {
        31, 24, 
        0, 1, 2,
        3, 4, 5, 6, 
        31,31, 
        7,8, 
        31, 
        9,10, 11, 12, 13,
        31,
        14, 15, 16, 17, 
        31, 25,
        31, 31, 31, 31, 31, 31,
        18
    };
    Scene scene=DAY;

public:
    ZombieTypeList(){}
    ZombieTypeList(Scene initialScene) : scene(initialScene) {}
    void set_scene(Scene sc){   scene = sc; }
    uint32_t get_list(uint32_t rng_seed){
        if (rng_seed == 0) rng_seed = 0x1105;
        rng_init(rng_seed);
        uint32_t has = 0;
        if (rng_gen(5) == 0) 
            has |= (1 << ADB_5);
        else 
            has |= (1 << ALZ_2);

        int j = 0;
        while (j < 9) {
            uint32_t type = rng_gen(33);
            uint32_t nid = conv_id[type];
            if (has & (1 << nid)) continue;
            if (allow_mask[scene] & (1 << nid)) {
                has |= (1 << nid);
                j++;
            }
        }
        return has;
    }
};
struct SeedInfo {
    uint16_t metric = 0;       
    std::vector<uint32_t> seeds;        
    uint64_t seed_count = 0;                 

    SeedInfo(uint16_t m, const std::vector<uint32_t> &s, uint64_t cnt) : metric(m), seeds(s), seed_count(cnt) {}

    bool operator<(const SeedInfo &other) const{
        return (metric < other.metric);
    }
};
std::vector<SeedInfo> final_results;

class BasicSeedFinder{
public:
    uint32_t get_real_seed(uint32_t rng_seed,   uint32_t id,uint32_t mode,uint32_t level){
        return uint32_t(rng_seed-id-mode-level*SEED_STEP);
    }
    uint32_t get_rng_seed(uint32_t real_seed,   uint32_t id,uint32_t mode,uint32_t level){
        return uint32_t(real_seed+id+mode+level*SEED_STEP);
    }
    bool check_has(uint32_t has,uint32_t mask){return (has & mask)==mask;}
    std::string str_has(uint32_t has){
        std::string info="普僵,";
        int cnt=1;
        for(int i=0;i<19;i++)
            if(has & (1<<i)){
                info += ZombieName[i]+",";
                cnt++;
            }
        return info+std::to_string(cnt)+" types\n";
    }
    void show_list(uint32_t real_seed,uint32_t id,uint32_t mode,Scene sc,uint32_t level,uint32_t level_num){
        ZombieTypeList ztl(sc);
        printf("\n种子'0x%08X'在%d~%df出怪情况为:\n",
            real_seed , level * 2 + 1,(level + level_num) * 2);
        for (uint32_t i=0;i<level_num;i++){
            printf("%d~%df:",2 *( level + i)+1,2 * (level +1+ i));
            std::string info=str_has(ztl.get_list(get_rng_seed(
                real_seed,  id,  mode,  level+i)));
            printf("%s",info.data());
        }
    }
    void show_list(uint32_t seed,uint32_t id,uint32_t mode,Scene sc,uint32_t level,uint32_t level_num,bool use_rng){
        if(use_rng){
            uint32_t real_seed = get_real_seed( seed,  id,  mode,  level);
            show_list(real_seed, id, mode, sc, level, level_num);
        }
        else show_list(seed, id, mode, sc, level, level_num);
    }

    void show_results( std::vector<SeedInfo>& resu,uint32_t id,uint32_t mode,Scene sc,uint32_t level,uint32_t level_num){
        std::sort(resu.begin(), resu.end());
        auto tmp = resu.end()-1;
        std::cout<<tmp->metric<<std::endl;
        uint32_t seed = get_real_seed(tmp->seeds.at(0),  id,  mode,  level);
        show_list(seed, id,  mode,  sc,  level,  level_num);
    }
    void output_csv(std::string filename,std::vector<SeedInfo>& resu){
        std::ofstream file(filename);
        std::sort(resu.begin(), resu.end());
        auto iter = resu.end()-1;
        for(int i=0;i<=5;i++){
            file<<iter->metric;
            file<<",";
            auto vec = iter->seeds;
            for (int j = 0; j < vec.size(); ++j) {
                file << vec[j];
                if (j < vec.size() - 1)
                    file << ",";  
            }
            file<<'\n';    
            iter--;
        }
        file.close();
    }
};

class ContinueSeedFinder : public BasicSeedFinder {
private:
    const uint64_t MAX_COUNT = 100;
    const uint16_t LOWEST_STORE = 5;
    ZombieTypeList _zlt;
    std::vector<uint32_t> masks;

    Scene scene;
    uint32_t cur_seed;
    uint64_t num_total;
    uint64_t num_cur;
public:
    std::vector<SeedInfo> results;

    ContinueSeedFinder(uint32_t start_seed,uint64_t start_offset,uint64_t num,
        Scene sc,std::vector<std::vector<uint32_t>> true_constraints){
        scene = sc;
        masks.clear();
        for(auto cons:true_constraints){
            uint32_t tmp_mask=0;
            for(auto z:cons)
                tmp_mask |= (1<<z);
            masks.push_back(tmp_mask);
        }
        cur_seed = uint32_t(start_seed+start_offset*SEED_STEP);
        num_total = num;
        num_cur = 0;
        results.clear();
        _zlt.~ZombieTypeList(); 
        new(&_zlt) ZombieTypeList(scene); 
    }
    bool check_constraints(uint32_t has){
        for (auto mask:masks)
            if(check_has( has,  mask))
                return true;
        return false;
    }
    void find(){
        while(num_cur<num_total){
            uint16_t len = 0;
            while(num_cur<num_total){
                if(!check_constraints(_zlt.get_list(cur_seed)))
                    break;
                cur_seed+=SEED_STEP;
                num_cur++;
                len++;
            }
            bool found=false;

            if(len>LOWEST_STORE){
                for (auto &info : results) 
                if (info.metric == len) {
                    found = true;
                    ++info.seed_count;
                    if (int(info.seed_count) <= MAX_COUNT)
                        info.seeds.push_back(uint32_t(cur_seed-SEED_STEP*len));
                    break;
                }
                if (!found) 
                    results.push_back(SeedInfo(len, {uint32_t(cur_seed-SEED_STEP*len)}, 1));
            }

            if((num_cur)%(num_total/100)==0){
                printf("%lf%% %d %lld %u\n",(100.0*num_cur/num_total),len,num_cur,uint32_t(cur_seed-SEED_STEP*len));
            }

            num_cur++;
            cur_seed+=SEED_STEP;
        }
    }

    void show_tmp(){
        for(auto tmp:results){
            std::cout<<tmp.metric<<std::endl;
        }
        printf("end");
    }

};
class MultiContinueSeedFinder: public BasicSeedFinder{
private:
    std::vector<std::thread> pool;

    const uint32_t interval = 50;
    uint32_t start_seed;
    uint64_t num_total;
    Scene scene;
    std::vector<std::vector<uint32_t>> true_constraints;

public:
    MultiContinueSeedFinder(uint32_t start_s,uint64_t num_t,Scene sc,std::vector<std::vector<uint32_t>> true_cons)
    :start_seed(start_s),num_total(num_t),scene(sc),true_constraints(true_cons) {pool.clear();}
    void single_thread_find_continue(uint64_t st,uint64_t num_assign){
        ContinueSeedFinder csf(this->start_seed,st,num_assign,this->scene,this->true_constraints);
        csf.find();
        mtx.lock();
        //printf("\n%lld,%lld\n\n",st,num_assign);
        for (auto now_info : csf.results) {
            bool found = false;
            for(auto &info:final_results)
            if (info.metric == now_info.metric) {
                found = true;
                for(auto s:now_info.seeds)
                    info.seeds.push_back(s);
                std::sort(info.seeds.begin(), info.seeds.end());
                auto last = std::unique(info.seeds.begin(), info.seeds.end());
                info.seeds.erase(last, info.seeds.end());
                break;
            }
            if (!found)     final_results.push_back(now_info);
        }
        mtx.unlock();
    }
    void multi_thread_find_continue(){
        uint64_t block = 1 + num_total/MAX_THREAD, num_assign = block + interval;
        for (int i=0;i<MAX_THREAD;i++){
            uint64_t st = uint64_t(i*block);
            std::thread t([this,st,num_assign]{
                single_thread_find_continue(st,num_assign);
            });
            pool.push_back(std::move(t));
        }
        for (auto &t: pool) t.join();
    }
};


class ScoreSeedFinder : public BasicSeedFinder {
private:
    const uint64_t MAX_COUNT = 100;
    const uint16_t LOWEST_STORE = 5;
    ZombieTypeList _zlt;
    std::vector<uint32_t> constraints;
    uint16_t weight[19]={};

    Scene scene;
    uint32_t cur_seed;
    
    uint64_t num_total;
    uint64_t num_cur;

    uint64_t num_level;

public:
    std::vector<SeedInfo> results;

    ScoreSeedFinder(uint32_t start_seed,uint64_t start_offset,uint64_t num,uint64_t num_l,
        Scene sc,std::vector<uint32_t> true_constraints,std::vector<uint16_t> true_weight){
        scene = sc;
        constraints = true_constraints;
        for(int i=0;i<constraints.size();i++)
            weight[constraints[i]] = true_weight[i];
        cur_seed = uint32_t(start_seed+start_offset*SEED_STEP);
        num_total = num;
        num_cur = 0;
        num_level = num_l;
        results.clear();
        _zlt.~ZombieTypeList(); 
        new(&_zlt) ZombieTypeList(scene); 
    }
    uint16_t get_score(uint32_t has){
        uint16_t ans_score = 0;
        for(int i=0;i<constraints.size();i++){
            auto z = constraints[i];
            if(has&(1<<z))
                ans_score+=weight[z];
        }
        return ans_score;
    }
    void find(){
        std::queue<uint16_t> q_score;
        uint16_t sum_score = 0;
        for(int i=0;i<num_level;i++){
            uint16_t tmp_score = get_score(_zlt.get_list(cur_seed));
            q_score.push(tmp_score);
            sum_score+=tmp_score;
            cur_seed+=SEED_STEP;
        }
        while(num_cur<num_total){
            uint16_t tmp_score = get_score(_zlt.get_list(cur_seed));
            uint16_t minus_score = q_score.front();
            q_score.push(tmp_score);q_score.pop();
            sum_score+= tmp_score-minus_score;

            cur_seed+=SEED_STEP;
            num_cur++;

            bool found=false;
            if(sum_score>LOWEST_STORE){
                for (auto &info : results) 
                if (info.metric == sum_score) {
                    found = true;
                    ++info.seed_count;
                    if (int(info.seed_count) <= MAX_COUNT)
                        info.seeds.push_back(uint32_t(cur_seed-SEED_STEP*num_level));
                    break;
                }
                if (!found) 
                    results.push_back(SeedInfo(sum_score, {uint32_t(cur_seed-SEED_STEP*num_level)}, 1));
            }
            if((num_cur)%(num_total/100)==0){
                printf("%lf%% %d %lld %u\n",(100.0*num_cur/num_total),sum_score,num_cur,uint32_t(cur_seed-SEED_STEP*num_level));
            }
        }
    }
    void show_tmp(){
        for(auto tmp:results){
            std::cout<<tmp.metric<<std::endl;
        }
        printf("end");
    }
};
class MultiScoreSeedFinder: public BasicSeedFinder{
private:
    std::vector<std::thread> pool;

    const uint32_t interval = 5;
    uint32_t start_seed;
    uint64_t num_total;
    uint64_t num_level;
    Scene scene;
    std::vector<uint32_t> constraints;
    std::vector<uint16_t> weights;

public:
    MultiScoreSeedFinder(uint32_t start_s,uint64_t num_t,uint64_t num_l,Scene sc,
        std::vector<uint32_t> true_cons,std::vector<uint16_t> true_w)
    :   start_seed(start_s),num_total(num_t),num_level(num_l),scene(sc),
        constraints(true_cons),weights(true_w) {pool.clear();}
    void single_thread_find_score(uint64_t st,uint64_t num_assign){
        ScoreSeedFinder ssf(this->start_seed,st,num_assign,this->num_level,this->scene,
            this->constraints,this->weights);
        ssf.find();
        mtx.lock();
        for (auto now_info : ssf.results) {
            bool found = false;
            for(auto &info:final_results)
            if (info.metric == now_info.metric) {
                found = true;
                for(auto s:now_info.seeds)
                    info.seeds.push_back(s);
                std::sort(info.seeds.begin(), info.seeds.end());
                auto last = std::unique(info.seeds.begin(), info.seeds.end());
                info.seeds.erase(last, info.seeds.end());
                break;
            }
            if (!found)     final_results.push_back(now_info);
        }
        mtx.unlock();
    }
    void multi_thread_find_score(){
        uint64_t block = 1 + num_total/MAX_THREAD, num_assign = block + num_level + interval;
        for (int i=0;i<MAX_THREAD;i++){
            uint64_t st = uint64_t(i*block);
            std::thread t([this,st,num_assign]{
                single_thread_find_score(st,num_assign);
            });
            pool.push_back(std::move(t));
        }
        for (auto &t: pool) t.join();
    }
};

