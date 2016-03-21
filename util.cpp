/*util.cpp*/
#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>

#include "util.hpp"

using std::cout;
using std::endl;

template <typename Type>
int write2bin(std::string filename, Type * arr, unsigned int n)
{
	std::FILE * pFile;
    pFile = std::fopen(filename.c_str(), "wb");
    if (pFile==NULL)
    {
        cout << "Error opening file " << filename << "!" << endl;
        return -1;
    }
    cout << "Writing output to file " << filename << endl;
    std::fwrite(arr, sizeof(Type), n, pFile);
    std::fclose(pFile);
    cout << "Done\n";
    return 0;
}


template <typename Type>
int read_from_bin(std::string filename, Type * arr, std::size_t n, long int offset, int origin)
{
    std::FILE * pFile;
    pFile = std::fopen(filename.c_str(), "rb");
    if (pFile==NULL)
    {
        std::cout << "Error opening file " << filename << "!" << std::endl;
        return -1;
    }
    std::cout << "Reading data from file " << filename << std::endl;
    std::fseek(pFile, offset, origin);
    std::fread(arr, sizeof(Type), n, pFile);
    std::fclose(pFile);
    std::cout << "Done\n";
    return 0;
}


std::string time2fname(const std::string & prefix, const unsigned int & t, const std::string & suffix)
{
	std::ostringstream current_t;
    current_t << t;
    return prefix + current_t.str() + suffix;
}


/* Explicit instantiation */
template int write2bin<int>(std::string, int *, unsigned int);
template int write2bin<float>(std::string, float *, unsigned int);
template int write2bin<double>(std::string, double *, unsigned int);

template int write2bin<const int>(std::string, const int *, unsigned int);
template int write2bin<const float>(std::string, const float *, unsigned int);
template int write2bin<const double>(std::string, const double *, unsigned int);

template int read_from_bin<int>(std::string, int *, std::size_t, long int, int);
template int read_from_bin<float>(std::string, float *, std::size_t, long int, int);
template int read_from_bin<double>(std::string, double *, std::size_t, long int, int);