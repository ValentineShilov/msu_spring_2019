//Valentine Shilov
#pragma once
#include <ostream>
#include <istream>
#include <sstream> 
enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
private:
    static constexpr char Separator = ' ';
    std::ostream &out;

public:
    explicit Serializer(std::ostream& out)
        : out(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <typename... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(args...);
    }
    

    Error save(uint64_t &v) 
    {
        out << v << Separator;
        return out ? Error::NoError : Error::CorruptedArchive;
    }

    Error save(bool &v) 
    {
        out << (v ? "true" : "false") << Separator;
        return out ? Error::NoError : Error::CorruptedArchive;
    }

private:
    template<typename T, typename... ArgsT>
    Error process(T &&v, ArgsT &&... args) 
    {
        Error e(save(v));
        if (e == Error::CorruptedArchive)
            return e;
        else
            return process(std::forward<ArgsT>(args)...);
    }

    template<typename T>
    Error process(T &&v) 
    {
        return save(v);
    }

};

class Deserializer
{
private:
    static constexpr char Separator = ' ';
    std::istream &in;

public:
    explicit Deserializer(std::istream& in)
        : in(in)
    {
    }

    template <class... ArgsT>
    Error operator()(ArgsT &&... args)
    {
        return process(args...);
    }

    template<typename T>
    Error load(T &v) 
    {
        return v.serialize(*this);
    }

private:
    template<typename T>
    Error process(T &&v) 
    {
        return load(v);
    }

    template<typename T, typename... ArgsT>
    Error process(T &&v, ArgsT &&... args) 
    {
        Error e(load(v));
        if (e == Error::CorruptedArchive)
        {
            return Error::CorruptedArchive;
        }
        else
        {
            return process(std::forward<ArgsT>(args)...);
        }
    }

public:
    Error load(bool& rv)
    {
        std::string text;
        in >> text;
        if(text == "true")
        {
            rv = true;
            return Error::NoError;
        }
        else if(text == "false")
        {
            rv = false;
            return Error::NoError;
        }
        return Error::CorruptedArchive;
    }

    Error load(uint64_t& rv)
    {
        uint64_t v;
        std::string str;
        in >> str;
        if(!in || str.length() == 0)
        {
            return Error::CorruptedArchive;
        }
        for(size_t i = 0; i < str.length(); i++)
        {
            if(!isdigit(str[i]))
            {
                return Error::CorruptedArchive;
            }
        }
        std::stringstream sst(str);
        sst >> v;
        if(sst)
        {
            rv = v;
            return Error::NoError;
        }
        else
        {
            return Error::CorruptedArchive;
        }
    }

};