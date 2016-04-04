/*util.cpp*/
#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>

#include "util.hpp"

using std::cout;
using std::endl;

template <typename Type>
int write2bin(std::string filename, Type * arr, std::size_t n)
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
template int write2bin<int>(std::string, int *, std::size_t);
template int write2bin<float>(std::string, float *, std::size_t);
template int write2bin<double>(std::string, double *, std::size_t);

template int write2bin<const int>(std::string, const int *, std::size_t);
template int write2bin<const float>(std::string, const float *, std::size_t);
template int write2bin<const double>(std::string, const double *, std::size_t);

template int read_from_bin<int>(std::string, int *, std::size_t, long int, int);
template int read_from_bin<float>(std::string, float *, std::size_t, long int, int);
template int read_from_bin<double>(std::string, double *, std::size_t, long int, int);




#include <sys/types.h>
#include <sys/stat.h>

int dir_exist(const std::string path, bool verbose)
{
    struct stat info;

    if( stat( path.c_str(), &info ) != 0 )
    {
        if (verbose)
            std::cout << "cannot access " << path << std::endl;
        return -1;
    }
    else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows 
    {
        if (verbose)
            std::cout << path << " is a directory\n";
        return 0;
    }
    else
    { 
        if (verbose)
            std::cout << path << " is not directory\n";
        return -2;
    }
}


int prep_dir(const std::string path)
{
    if (dir_exist(path) != 0)
        return mkdir(path.c_str(), S_IRWXU);
    else
        return 0;
}