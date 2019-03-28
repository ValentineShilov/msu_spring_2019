#include <iostream>

#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <exception>


template<typename T> 
T min(const T &a, const T &b)
{
    return (a > b) ? b : a;
}

//calculator interface
class Calc
{
    public:
        Calc()
        {
            
        }
        static int64_t calculate(const std::string &s)
        {
            return processPlusOperand(preprocess(s));   
        }
    private:
        static  int64_t processMultiplyOperand(std::string s, bool div=false)
        {
           
           return processPlusOperand(s);
 
        }
        static int64_t processPlusOperand(std::string s)
        {
            
            if(s.length()==0) return 0;
            for(size_t i = 0; i < s.length(); i++)
            {
                if(s[i]=='+')
                {
                    std::pair<std::string, std::string > p(split(s, i));
                    return processPlusOperand(p.first) + processPlusOperand(p.second);

                }
                else if(s[i]=='-'&&i!=0)
                {
                    std::pair<std::string, std::string > p(split(s, i));
                    p.second = std::string("-") + p.second;
                    return processPlusOperand(p.first) + processPlusOperand(p.second);
                }
            }
            //no pluses left
            for(size_t i = 0; i < s.length(); i++)
            {
                if(s[i]=='*')
                {
                    std::pair<std::string, std::string > p(split(s, i));
                    return processMultiplyOperand(p.first) *  processMultiplyOperand(p.second);
                }
                if(s[i]=='/')
                {
                    std::pair<std::string, std::string > p(split(s, i));
                    
           
                    int64_t b(strToInt64(p.second));
                    if(b==0) throw(calcException());
                    else return strToInt64(p.first) / strToInt64(p.second);
          
                    
                }

            }
            std::string digitBuffer;
            for(size_t i = 0; i < s.length(); i++)
            {
                if(isdigit(s[i])|| s[i]=='-')
                {
                    digitBuffer+=s[i];
                }

            }
            if(digitBuffer.length()==0) return 0;
            int64_t rv(strToInt64(digitBuffer));

            return rv;
                

        }

    //class for calculation exception in calculator interface
    class calcException: public std::exception
    {
    public:

        virtual const char* what() const throw()
        {
            return "error";
        }

    };

    private:

        static std::pair<std::string, std::string> split(const std::string &s, size_t pos)
        {
            std::string l;
            std::string r;
            for(size_t i=0; i<(s.length());++i)
            {
                if(i<pos)
                    l+=s[i];
                else if(i>pos)
                    r+=s[i];

            }
            return std::make_pair(l,r);
        }
        static std::string preprocess(const std::string &s)
        {
            std::string rv;
            bool MinusFlag=false;
            for(size_t i=0;i<s.length();++i)
            {
                if(s[i]!=' ')
                {
                    
                    if(MinusFlag) 
                        if(s[i]=='-')
                        {
                            rv+='+';
                            MinusFlag = false;
                        } 
                        else
                        {
                            rv+='-';
                            rv+=s[i];
                            
                            MinusFlag = false;
                        } 
                    else if(s[i]=='-') MinusFlag = true;
                    else 
                    {  
                        if(! (isdigit(s[i]) || s[i]=='+' || s[i]=='-' || s[i]=='/'|| s[i]=='*'))
                            throw(calcException());
                        rv+=s[i];
                    }
                }
            }
           
            return rv;
        }
        static int64_t strToInt64(const std::string &s)
        {
            if(s.length()==0) return 0;
            int64_t rv;
            std::stringstream ss(s);
            ss >> rv;
            if(!ss) 
                throw(calcException());
            return rv;
        }
       

};

int main(int argc, char *argv[] )
{
    try
    {
        if(argc!=2)
        {
            std::cout <<"error";
            return 1;
        }
        std::string s(argv[1]);
        std::cout <<  Calc::calculate(s);
    }
    catch (std::exception &e)
    {
        std::cout <<e.what();
        return 1;
    }
    
    return 0;
 
}