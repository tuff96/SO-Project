#ifndef COMANDS_H
#define COMANDS_H

#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <locale>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

using namespace std;

int ls(vector<string> args);
int tac(vector<string> args);
int dir(vector<string> args);

#endif
