#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <thread>
#include "ThreadPool.hpp"
#include <omp.h>
using namespace std;

void pcalc(vector<float> &v){
    //omp_set_num_threads(4);  //open this you will get 4097 threads in your progama
    #pragma omp parallel for
    for(int i = 0 ; i < v.size(); i++){
        v[i] = v[i] * v[i];
    }
}

vector<float> make_v(){
    vector<float> v(10000);
    for(int i = 0; i < 10000; i++){
        v[i] = rand();
    }
    return v;
}

int func(){
    auto t = make_v();
    pcalc(t);
    float sum = 0;
    for(auto &x : t){
        sum += x;
    }
    printf("%lf\n", sum);
    return (int)sum;
}

int main(void){
    ThreadPool tp(1024);
    while(true){
        tp.enqueue(func);
    }
    return 0;
}