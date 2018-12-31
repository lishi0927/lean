#ifndef Data_Process
#define Data_Process

#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <cassert>
#include <functional>
#include <vector>
#include <string>
using namespace std;

vector<float> read_file_doubles(const string &filename);

vector<vector<float>> read_file_doubles_mipmap(const string &filename, int width, int height, int bit, int mipmap);

#endif
