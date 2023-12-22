#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sys/types.h>
#include <dirent.h>

#define DIREC 0
#define FILE 1
#define NOT_FOUND -1

bool check_location(const std::string &path);
int get_resource_type(std::string &path);