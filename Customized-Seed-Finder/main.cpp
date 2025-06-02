# include "inc/seed_finder.h"
#include <cstdint>


Scene my_scene = DAY;
uint64_t find_len = 0xffffffff;
void test_score(){
	final_results.clear();
    MultiScoreSeedFinder finder(0,uint64_t(find_len),50,my_scene,
    {AHY_32,AQQ_16,ABC_12},
       {4,1,1});
    finder.multi_thread_find_score();
    finder.show_results(final_results, 1, 13, my_scene, 1000, 50);
    finder.output_csv("output.csv", final_results);
}
void test_continue(){
	final_results.clear();
    MultiContinueSeedFinder finder(0,uint64_t(find_len),my_scene,{{ABC_12}});
    finder.multi_thread_find_continue();
    finder.show_results(final_results, 1, 13, my_scene, 1000, 50);
    finder.output_csv("output.csv", final_results);
}
void show(){
    BasicSeedFinder tmp;
    tmp.show_list(435501496,1,13,my_scene,1000,25,true);
    // printf("%u\n",tmp.get_real_seed(1764367071, 1,15,1000));
    // printf("%u\n",tmp.get_rng_seed( 0xa + 0X69289047 - 0xa, 1, 15, 1000));
}
int main(){
    //test_continue();
    test_score();  
    //show();

    return 0;
}
