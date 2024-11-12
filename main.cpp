#include <vector>
#include <random>
#include <chrono>
#include "DynamicLearnedIndex/learned_index/LearnedIndex.h"
#include "PGM-index/include/pgm/pgm_index_dynamic.hpp"
#include "iostream"

[[gnu::noinline]]
bool insertPGM(pgm::DynamicPGMIndex<uint32_t, uint32_t>& index, uint x){
    index.insert_or_assign(x,x);
    return true;
}

[[gnu::noinline]]
bool removePGM(pgm::DynamicPGMIndex<uint32_t, uint32_t>& index, uint x){
    index.erase(x);
    return true;
}

[[gnu::noinline]]
bool insertDLI(LearnedIndex<uint,64>& index, uint x){
    return index.insert(x);
}
[[gnu::noinline]]
bool removeDLI(LearnedIndex<uint,64>& index, uint x){
    return index.remove(x);
}

using hrc = std::chrono::high_resolution_clock;
void runtimeTest(uint size){
    // Read data
    std::mt19937 engine(42);
    std::vector<uint> data;
    data.reserve(size);
    for(uint i = 1; i<=size; ++i){
        data.push_back(i);
    }

    std::shuffle(data.begin(),data.end(),engine);

    auto index = LearnedIndex<uint,64>();
    pgm::DynamicPGMIndex<uint32_t, uint32_t> dynamic_pgm;

    std::cout << "Inserting"<<std::endl;

    auto t0 = hrc::now();
    volatile bool b;
    for(unsigned int i : data){
        b = insertDLI(index,i);
    }
    auto t1 = hrc::now();

    std::cout << "DLI: Insertion done after "<< (t1 - t0).count() * 1e-9 << "s and b is " << b << std::endl;

    t0 = hrc::now();
    for(unsigned int i : data){
        b ^= insertPGM(dynamic_pgm,i);
    }
    t1 = hrc::now();

    std::cout << "PGM: Insertion done after "<< (t1 - t0).count() * 1e-9 << "s and b is " << b << std::endl;

    std::shuffle(data.begin(),data.end(),engine);

    std::cout << "Removing"<<std::endl;
    t0 = hrc::now();
    for(unsigned int i : data){
        b ^= removeDLI(index,i);
    }
    t1 = hrc::now();
    std::cout << "DLI: Removal done after "<< (t1 - t0).count() * 1e-9 << "s and b is " << b << std::endl;


    t0 = hrc::now();
    for(unsigned int i : data){
        b ^= removePGM(dynamic_pgm,i);
    }
    t1 = hrc::now();
    std::cout << "PGM: Removal done after "<< (t1 - t0).count() * 1e-9 << "s and b is " << b << std::endl;

}

int main(int argc, char* argv[]){
    runtimeTest(10000000u);
}