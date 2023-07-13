#include "memory_pool.h"
#include <iostream>
#include <vector>
class Test {
    public:
        int x;
        char y;
        float z;
        Test(int _x,char _y,float _z):x(_x),y(_y),z(_z){}
        ~Test() {
            std::cout << "Destory" << std::endl;
        }
};


int main() {
    MemoryPool<Test>pool(10000);
    std::vector<Test*>vec;
    for(int i = 0 ; i < 100001 ;i++){
        auto t1 = pool.Allocate(1,'a',1.0);
        vec.push_back(t1);
    }
    for(int i = 0 ;i < 100001;i++) {
        pool.Deallocate(vec[vec.size()-1]);
        vec.pop_back();
    }
    for(int i = 0 ; i < 100001 ;i++){
        auto t1 = pool.Allocate(1,'a',1.0);
        vec.push_back(t1);
    }
    for(int i = 0 ;i < 100001 ;i++) {
        pool.Deallocate(vec[vec.size()-1]);
        vec.pop_back();
    }
}