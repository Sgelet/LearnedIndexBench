#include <vector>
#include <random>
#include <chrono>
#include <cstring>
#include <unordered_set>
#include "DynamicLearnedIndex/learned_index/LearnedIndex.h"
#include "PGM-index/include/pgm/pgm_index_dynamic.hpp"
#include "PGM-index/include/pgm/pgm_index.hpp"
#include "iostream"

#ifndef MODEL
#define MODEL 1
#endif

using hrc = std::chrono::high_resolution_clock;

enum Data { RANDOM, LINES, ADVERS};

template<typename T>
std::vector<T> readData(){
    std::vector<T> res;
    res.reserve(1e9);
    T e;
    std::cout << "Reading data... ";
    auto t0 = hrc::now();
    while(std::cin>>e){
        res.emplace_back(e);
    }
    auto t1 = hrc::now();
    std::cout<<res.size() << " elements read in " << (t1-t0).count()*1e-9 << std::endl;
    return res;
}

// TODO: Variadic function parameters?
// args[0] always contains the number of elements to generate
template<typename T>
std::vector<T> generateData(Data type, const bool output, const std::vector<T>& args){
    std::mt19937 engine(42);
    std::uniform_int_distribution<T> uniform(1);

    T n = args[0];
    std::vector<T> data;
    std::unordered_set<T> set;
    if(type == RANDOM){
        set.reserve(n);
    }
    //data.reserve(n);

    switch (type) {
        case RANDOM:
            // Floyds sampling algorithm
            for(T j = args[1]-n+1; j<args[1]; ++j){
                T t = uniform(engine)%j;
                if(set.count(t)) t = j;
                set.insert(t);
                std::cout << t << std::endl;
            }
            break;
        case LINES:
            T lines = args[1];
            T base = args[2];
            T offset = 0;
            for(T k = 0; k < lines; ++k){
                for(T i = 1; i <= n; ++i){
                    offset += base<<k;
                    std::cout << (offset + (base<<k)) << std::endl;
                }
            }
            break;
    }

    //if(output) for(const T& e: data) std::cout << "e" << std::endl;
    return data;
}

template<typename T, int epsilon = EPSILON>
void lineCountTest(const std::vector<T>& data, int interval){
    auto index = LearnedIndex<T,epsilon>();
    auto pgm = pgm::DynamicPGMIndex<T,T, pgm::PGMIndex<T,epsilon>>();

    int max_index = 0;
    int count = 0;
    int curr_seg_idx = 0;
    for(T i : data){
        count++;
        if constexpr (MODEL == 0) pgm.insert_or_assign(i,i);
        else index.insert(i);

        curr_seg_idx = MODEL ? index.segments_count() : pgm.segments_count();

        if(curr_seg_idx > max_index) max_index = curr_seg_idx;

        if(count % interval == 0){
            std::cout << count << " " << " " << curr_seg_idx << std::endl;
        }
    }

    std::cout << "max: " << max_index << std::endl;
    std::cout << "size: " << (MODEL ? index.size_in_bytes() : pgm.size_in_bytes()) << std::endl;
}

template<typename T>
std::vector<std::pair<T,T>> generateQueries(const std::vector<T>& data, const uint op_count, const double q_ratio, const double ins_ratio){
    std::vector<std::pair<T,T>> queries;
    queries.reserve(op_count);

    std::mt19937 engine(9001);
    std::uniform_int_distribution<T> key_dist(1,1e11);
    std::uniform_int_distribution<T> id_dist(1,1e8-op_count);

    uint num_queries = (q_ratio*op_count);
    uint inserts = (op_count - num_queries)*ins_ratio;
    uint deletes = op_count - inserts - num_queries;

    T lo;
    T hi;

    for(uint i=0; i<num_queries; ++i){
        lo = key_dist(engine);
        hi = lo+1000000;//key_dist(engine);
        queries.emplace_back(lo,hi);
    }
    for(uint i=0; i<inserts; ++i){
        queries.emplace_back(-1,data[data.size()-op_count+i]);
    }
    for(uint i=0; i<deletes; ++i){
        queries.emplace_back(-2,data[id_dist(engine)]);
    }

    std::shuffle(queries.begin(),queries.end(),engine);

    return queries;
}

template<typename T, int epsilon = EPSILON>
void runtimeTest(const std::vector<T>& data, const uint op_count = 10000000, const double q_ratio = 0.5, double ins_ratio = 0.5){
    auto index = LearnedIndex<T,epsilon>();
    auto pgm = pgm::DynamicPGMIndex<T,T, pgm::PGMIndex<T,epsilon>>();

    std::cout << "Inserting...";

    auto t0 = hrc::now();

    for(uint i=0; i< data.size() - op_count; ++i){
        if constexpr (MODEL == 0) pgm.insert_or_assign(data[i],data[i]);
        else index.insert(data[i]);
    }

    auto t1 = hrc::now();

    std::cout << " done in: " << (t1-t0).count() * 1e-9 << std::endl;

    auto queries = generateQueries<T>(data,op_count,q_ratio,ins_ratio);

    std::cout << "Simulating dynamic batch of " << op_count << " operations, with query ratio "<< q_ratio << std::endl;

    t0 = hrc::now();
    volatile uint t = 0;
    for(auto& q: queries){
        if(q.first == -1){
            if constexpr (MODEL == 0) pgm.insert_or_assign(q.second,q.second);
            else index.insert(q.second);
        } else if(q.first == -2){
            if constexpr (MODEL == 0) pgm.erase(q.second);
            else index.remove(q.second);
        } else {
            if constexpr (MODEL == 0) t += pgm.range(q.first,q.second).size();
            else t += index.range(q.first,q.second).size();
        }
    }

    t1 = hrc::now();
    std::cout << "Updates done in: " << (t1-t0).count() * 1e-9 << std::endl;
    std::cout << "size: " << (MODEL ? index.size_in_bytes() : pgm.size_in_bytes()) << std::endl;
}

template<typename T, int epsilon = EPSILON>
void adversarialTest(const std::vector<T>& data, const uint op_count = 10000000){
    auto index = LearnedIndex<T,epsilon>();
    auto pgm = pgm::DynamicPGMIndex<T,T, pgm::PGMIndex<T,epsilon>>();

    std::cout << "Inserting...";

    auto t0 = hrc::now();

    for(uint i=0; i< data.size(); ++i){
        if constexpr (MODEL == 0) pgm.insert_or_assign(data[i],data[i]);
        else index.insert(data[i]);
    }

    auto t1 = hrc::now();

    std::cout << " done in: " << (t1-t0).count() * 1e-9 << std::endl;

    if(MODEL == 0) std::cout << "PGM max level before deletes " << pgm.get_max_level() << std::endl;

    std::cout << "Deleting... ";
    t0 = hrc::now();
    for(uint i=0; i< data.size() - 1000; ++i){
        if constexpr (MODEL ==0) pgm.erase(data[i]);
        else index.remove(data[i]);
    }
    t1 = hrc::now();
    std::cout << " done in: " << (t1-t0).count() * 1e-9 << std::endl;

    if(MODEL == 0) std::cout << "PGM max level after deletes " << pgm.get_max_level() << std::endl;

    auto queries = generateQueries<T>(data,op_count,1.0,0.0);

    std::cout << "Performing " << op_count << " range queries" << std::endl;

    t0 = hrc::now();
    volatile uint t = 0;
    for(auto& q: queries){
        if constexpr (MODEL == 0) t += pgm.range(q.first,q.second).size();
        else t += index.range(q.first,q.second).size();
    }

    t1 = hrc::now();
    std::cout << "Updates done in: " << (t1-t0).count() * 1e-9 << std::endl;

    std::cout << "size: " << (MODEL ? index.size_in_bytes() : pgm.size_in_bytes()) << std::endl;
}

#define numtype int64_t

int main(int argc, char* argv[]){
    if(argc < 3) return -1;

    if(!std::strcmp(argv[1],"GEN")){
        if(!std::strcmp(argv[2],"RAN")){
            if(argc==5) generateData<numtype>(RANDOM, true, {std::stol(argv[3]), std::stol(argv[4])});
            else std::cout << "Must supply number of elements and size of universe for random data" << std::endl;
        } else if(!std::strcmp(argv[2],"LINES")){
            if(argc==6) generateData<numtype>(LINES, true, {std::stol(argv[3]),std::stol(argv[4]),std::stol(argv[5])});
            else std::cout << "Musty supply number of elements, number of lines, and base slope" << std::endl;
        }
    } else if(!std::strcmp(argv[1],"COUNT")){
        auto data = readData<numtype>();
        lineCountTest<numtype>(data,std::stol(argv[2]));
    } else if(!std::strcmp(argv[1],"RUN")){
        auto data = readData<numtype>();
        runtimeTest(data,std::stol(argv[2]),std::stod(argv[3]),std::stod(argv[4]));
    } else if(!std::strcmp(argv[1],"ADV")){
        auto data = readData<numtype>();
        adversarialTest(data,std::stol(argv[2]));
    }
}