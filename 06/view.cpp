#include <iostream>
void printFile(const char* filename)
{
    FILE *f = fopen(filename, "rb");
    if(f!=nullptr)
    {
        uint64_t a(0), b(0);
        size_t i;
        bool sorted(true);

        for(i=0;  fread(reinterpret_cast<char*>(&a), 1, sizeof(a), f) > 0; ++i)
        {
            std::cout <<a <<" ";
            if(a<b)  sorted=false;
            b=a;
            if(i%5==0) std::cout <<std::endl;
        }
         std::cout <<std::endl <<"len: " <<i << " sorted:" <<sorted <<std::endl;
        fclose(f);
    }
    else
    {
        std::cout <<"Fopen failed" <<std::endl;
    }
   
}

int main(int argc, char* argv[] )
{
    if(argc>1)
    {
        printFile(argv[1]);
    }
}