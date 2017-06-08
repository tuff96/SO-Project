#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>
#include <cstdlib>
#include <dirent.h>
#include "CommandHandler.h"
#include "comands.h"
//#include "ls.cpp"

using namespace std;

class Input{
public:
    Input();

    void getInput();

private:
    string input;
    string prompt;
    vector<string> commands;
    CommandHandler com;
    void separator();
};

#endif // INPUT_H
