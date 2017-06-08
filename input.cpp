#include "input.h"

Input::Input()
{
    rl_bind_key('\t', rl_complete);
}

void Input::getInput()
{
    while(true) {
        //reading stuff
        char b[1024];
        getcwd(b,sizeof(b));
        char*p=strtok(b,"//"),*f=p;
        while((p=strtok(NULL,"//"))!=NULL)f=p;
        string c=f;c=c+"$ ";
        input = readline(c.c_str());

        if(input.empty())
            continue;
        add_history(input.c_str());
        //parsing sTuff
        com.playpipe(input);
        //separator();


    }
}

void Input::separator()
{
    stringstream stream(input);
    string sub;
    while(getline(stream, sub, '|'))
        if(sub.empty() == false)
            commands.push_back(sub);
}
