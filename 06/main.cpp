//Valentine Shilov
#include <iostream>
#include <fstream>
#include <algorithm>  
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <array>
#include <thread> 
#include <stdexcept>
#include <sstream>
#include <stdio.h>
#include <sys/stat.h>
const size_t BUFFER_SIZE(100000);

void genFile(const char* filename, size_t n)
{
    srand(time(NULL));
    FILE *f = fopen(filename, "wb");
    if(f!=nullptr)
    {
        for(size_t i=0; i<n; ++i)
        {
            uint64_t a;
            a = rand();
            fwrite(reinterpret_cast<char*>(&a), 1, sizeof(a), f);
        }
        fclose(f);
    }
   
}
void merge(const char *fn1, const char *fn2, const char *outn)
{
    std::ifstream f1(fn1, std::ios::in | std::ios::binary);
    std::ifstream f2(fn2, std::ios::in | std::ios::binary);
    std::ofstream out(outn, std::ios::out | std::ios::binary);
    //std::cout <<"Merging: " << fn1 <<" " <<fn2 <<" " << outn <<std::endl; 
    uint64_t a(0);
    uint64_t b(0);
    bool f1f(true), f2f(true), f1_opened(false), f2_opened(false);
    if(f1)
    {
        f1_opened = true;
        f1f = !f1.read(reinterpret_cast<char*>(&a), sizeof(uint64_t)).eof();
    }
    else
        f1f = false;
    if(f2)
    {
        f2_opened = false;
        f2f = !f2.read(reinterpret_cast<char*>(&b), sizeof(uint64_t)).eof();
    }
    else
        f2f = false;
    //std::cout <<a <<" " <<b <<std::endl;
    while(f1f || f2f)
    {

        if(a>b && f2f && f1f)
        {
            
            out.write(reinterpret_cast<char*>(&b), sizeof(uint64_t));
            f2f = !f2.read(reinterpret_cast<char*>(&b), sizeof(uint64_t)).eof();
        }
        if(a<=b && f1f && f2f)
        {
           
            out.write(reinterpret_cast<char*>(&a), sizeof(uint64_t));
            f1f = !f1.read(reinterpret_cast<char*>(&a), sizeof(uint64_t)).eof();
        }
        if(f1f && !f2f)
        {
            
            out.write(reinterpret_cast<char*>(&a), sizeof(uint64_t));
            f1f = !f1.read(reinterpret_cast<char*>(&a), sizeof(uint64_t)).eof();
        }
        if(f2f && !f1f)
        {
            
            out.write(reinterpret_cast<char*>(&b), sizeof(uint64_t));
            f2f = !f2.read(reinterpret_cast<char*>(&b), sizeof(uint64_t)).eof();
        }

    }
    if(f1_opened) f1.close();
    if(f2_opened) f2.close();
    remove(fn1);
    remove(fn2);
} 
void recursiveMergeSort(const std::pair< size_t, size_t> a,  const char* filename, size_t depth, const std::string &out)
{
    auto len(a.second - a.first);
    //std::cout << "recursiveMergeSort " <<a.first << " " <<  a.second  <<" "  << filename <<std::endl;
    if(len > BUFFER_SIZE)
    {
        auto p1(a);
        auto p2(a);
        p1.second = p1.second - len/2;
        p2.first = p1.second;
        //std::string out1, out2; 
        std::stringstream o1, o2;
        o1 << "w/o1_"  << depth <<"_" << a.first <<".tmp";
        o2 << "w/o2_"  << depth <<"_" << a.first <<".tmp";
        
        recursiveMergeSort(p1,  filename, depth + 1, o1.str());
        recursiveMergeSort(p2,  filename, depth + 1, o2.str());
        
        merge(o1.str().c_str(), o2.str().c_str(), out.c_str());
    }
    if(len <= BUFFER_SIZE)
    {
        std::array<uint64_t, BUFFER_SIZE> arr;
        std::ifstream f(filename, std::ios::in | std::ios::binary);
        if(!f)
            throw(std::runtime_error("Error"));
        f.seekg(a.first*sizeof(uint64_t), std::ios::beg);
        if(f)
        {
            bool ff = !f.read(reinterpret_cast<char*>(arr.begin()), sizeof(uint64_t) * (len) ).eof();
           
            std::sort(arr.begin(), arr.begin() + len);
            std::ofstream of(out.c_str(), std::ios::out | std::ios::binary);
            
            of.write(reinterpret_cast<char*>(arr.begin()), sizeof(uint64_t)* (len));
            of.close();
            
            f.close();
        }
        else
        {
            std::cout <<"fopen failed: " << filename << std::endl;
            throw(std::runtime_error("Invalid input file!!!"));
        }
        
        
    }
    
}

void mergeSort(const char* filename, size_t nthreads=2)
{
    std::ifstream f(filename, std::ios::in | std::ios::binary);
    std::vector< std::pair<size_t, size_t> > blocks;
    std::vector< std::thread > threads;
    mkdir("w", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if(f)
    {
        f.seekg( 0, std::ios::end);
        size_t fsize(f.tellg());
        //std::cout <<"fsize: " <<fsize <<" "  << filename << std::endl;
        f.seekg(0, std::ios::beg);
        auto bs(fsize/nthreads);
        size_t i;
        if(fsize % sizeof(uint64_t) != 0)
        {
            throw(std::runtime_error("Invalid input file!!!"));
        }
        for(i = 0; i < nthreads - 1; ++i)
        {
            
            blocks.push_back(std::make_pair(bs * i / sizeof(uint64_t), bs * (i + 1)/ sizeof(uint64_t)));
        }
        blocks.push_back(std::make_pair(bs * (i) / sizeof(uint64_t), fsize / sizeof(uint64_t)));
        
        size_t j(0);
        std::vector<std::string> res_files;
        for(auto p : blocks)
        {
            std::stringstream o1;
            o1 << "w/of_"  << 0 <<"_" << "a" <<".." <<j <<".tmp";
            res_files.push_back(o1.str());
            threads.push_back( std::move(std::thread(recursiveMergeSort, p, filename, 0, o1.str())));
 
            j++;
        }
        for(auto i = threads.begin(); i != threads.end(); ++i)
        {
            if(i->joinable())
                i->join();
        }

        //final merge
        std::string final_out("result.dat");
        remove(final_out.c_str());
        std::string final_tmp(final_out + ".tmp");
        remove(final_tmp.c_str());
        for(auto rf : res_files)
        {
            rename(final_out.c_str(), final_tmp.c_str());
            merge(rf.c_str(), final_tmp.c_str(), final_out.c_str());
            remove(final_tmp.c_str());
        }
    }
    else
    {
        throw(std::runtime_error("Error"));
    }
}
int main()
{
    //genFile("in.dat", 10500000);
    mergeSort("in.dat", 4);
    return 0;
}