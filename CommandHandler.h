#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "comands.h"

using namespace std;

class CommandHandler
{
    enum class commandType{
        NATIVE,
        CUSTOM
    };

public:
    CommandHandler();
    void playpipe(string &imput);
    void setCommand(const string& command);
    void execute();
    void executeNative();
private:
    vector<string> commandList;
    commandType type;
    bool redirect(string &imput );
    int fileDstdin;
    int fileDstdout;


};

#endif // COMMANDHANDLER_H
