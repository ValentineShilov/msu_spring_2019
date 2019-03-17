//Valentine Shilov
#include <iostream>
#include "numbers.dat"
#include <vector>
#include <algorithm>
#include <math.h>


int find(int left, int right, const std::vector<int> &primeNums, const int Data[], const int Size)
{

    if(left > right) return 0;
    
    auto li = std::lower_bound(Data, Data + Size, left);
    auto ri = std::upper_bound(Data, Data + Size, right);
  
    //check if both values are in the array
    if( left != *li) return 0;
    if( right != *(ri - 1) ) return 0;
  
    int cnt(0);

    auto ll = std::lower_bound(primeNums.begin(), primeNums.end(), left);
    auto rr = std::upper_bound(primeNums.begin(), primeNums.end(), right);
    //search for each prime number in the part of the array and count
    auto j = li;
    for(auto i = ll; i != rr; ++i)
    {
        for(; j != ri; ++j)
        {
            if(*j == *i) ++cnt;
            else if(*j > *i) break;
        }
    }
   return cnt;
}
std::vector<int> genPrime(int MAXN)
{
    std::vector<int> primeNums;
    
    //optimization: there are 9592 prime numbers from 0 to 99991
    if(MAXN >= 99991) primeNums.reserve(9592);

    //special cases:
    if(MAXN < 2) return primeNums;
   
    primeNums.push_back(2);
    if(MAXN <= 2) return primeNums;
   
    primeNums.push_back(3);
    if(MAXN <= 3) return primeNums;
    

    for(int i = 5; i <= MAXN; i += 2)
    {
        bool flag = false;

        for(auto& n : primeNums)
        {
            if(i % n == 0)
            {
                flag=true;
                break;
            } 
            else if(n * n > i) break;
        }
        if(!flag) 
        {
            primeNums.push_back(i); 
           // std::cout <<i <<" ";
        }
    }
    return primeNums;
}
int main(int argc, char* argv[])
{
 
    //array is sorted => max number - the last
    int MAXN = Data[Size-1];
    
    if( (argc < 3) || (argc % 2 != 1) ) return -1;

    //generate prime numbers table
    std::vector<int> primeNums(genPrime(MAXN));

    //read args
    for (int i = 1; i < argc - 1; i += 2)
    {
        int left = std::atoi(argv[i]);
        int right = std::atoi(argv[i+1]);
      
        if(left <= right)  
            std::cout << find(left, right, primeNums, Data, Size) << std::endl;
        else  
            std::cout << 0 << std::endl;
    }
   
    
    return 0;
}
