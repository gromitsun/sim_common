/*util.hpp*/
#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <cstdio>


template <typename Type>
int write2bin(std::string filename, Type * arr, std::size_t n);

template <typename Type>
int read_from_bin(std::string filename, Type * arr, std::size_t n, long int offset=0, int origin=SEEK_SET);

std::string time2fname(const std::string & prefix, const unsigned int & t, const std::string & suffix = ".bin");

int dir_exist(const std::string path, bool verbose=false);

int prep_dir(const std::string path);

#endif