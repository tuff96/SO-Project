#include "CommandHandler.h"

CommandHandler::CommandHandler() :
    commandList(),
    type(commandType::NATIVE) // se initializeaza cu NATIVE
{

}

void CommandHandler::playpipe(string &imput)
{
    fileDstdin=dup(1);
    fileDstdout=dup(0);
    redirect(imput);
    //
    stringstream stream2(imput);
    string primu;
    stream2>>primu;
    if(primu=="exit"||primu=="cd")
    {
        setCommand(imput);
        return ;
    }
    stringstream stream(imput);
    string sub;
    vector<string> command;
    command.clear();
    while(getline(stream, sub, '|'))
        if(sub.empty() == false)
            command.push_back(sub);
    int fd[2];
    pid_t pid;
    int copy;

    for(size_t i=0;i<command.size();i++)
    {
        pipe(fd);

        if((pid=fork())==0)
        {
            if(i==0)
            {
                dup2(fileDstdin,0);
            }
            else
            {
                dup2(copy,0);
                close(copy);
            }
            close(fd[0]);
            if(i<command.size()-1)
            {
                dup2(fd[1],1);
            }
            else
            {
                dup2(fileDstdout,1);
            }
            setCommand(command.at(i));
            exit(0);
        }
        else
        {
            waitpid(pid,NULL,0);
            kill(pid,9);
            close(fd[1]);
            copy=fd[0];
        }

    }

}

void CommandHandler::setCommand(const string &command) // se seteaza si verifica ce tip de comanda e
{
    stringstream stream(command); // se face split la comanda dupa spatii
    string sub;
    commandList.erase(commandList.begin(),commandList.end());
    while(getline(stream, sub, ' '))
        if(sub.empty() == false)
            commandList.push_back(sub); // se introduce in vector bucatile din comanda

    auto it=commandList.begin();

    if(*it == "ls"       ||
            *it == "tac" ||
            *it == "dir" ||
            *it == "exit"||
            *it == "cd"  )

        type = commandType::CUSTOM;
    else
        type = commandType::NATIVE;
    execute();

}
void CommandHandler::execute() // se verifica tipul comenzii, dupa care se ruleaza
{
    if(type == commandType::NATIVE)
        executeNative();
    else //se verifica care comanda este, din cele custom
    {
        auto it = commandList.begin();
        if(*it == "ls")
            ls(commandList);
        else if(*it == "tac")
            tac(commandList);
        else if(*it == "dir")
            dir(commandList);
        else if(*it == "cd")
        {
            char b[1024];
            getcwd(b,sizeof(b));
            string c = b;
            string dir=*(++commandList.begin());
            if(dir[0]!=47)
                dir=c+"//"+dir;
            chdir(dir.c_str());
        }
        else if(*it=="exit")
            exit(0);
    }
}

void CommandHandler::executeNative() // executa comenzi native
{
    char* ar[commandList.size()+1];
    for(unsigned int i = 0; i < commandList.size(); i++)
    {
        ar[i] = new char[commandList.at(i).length()];
        strcpy(ar[i],(commandList.at(i)).c_str());
    }

    ar[commandList.size()] = NULL;
    pid_t pid = fork();
    if(pid == 0) // child process
    {
        execvp(ar[0], ar);
        exit(0);
    }
    else // parent process
    {
        wait(NULL); // wait for the child to finish
        kill(pid,9); // then kill it.
    }
}

bool CommandHandler::redirect(string &imput)
{

    size_t pozitieinitiala=imput.length();
    size_t pozitieout=imput.find(">");
    size_t pozitieinp=imput.find("<");
    string substring;
    if(pozitieout!=string::npos)
    {
        substring=imput.substr(pozitieout+1,imput.length()-pozitieout-1);
        stringstream s(substring);
        if(pozitieout<pozitieinitiala)
        {
            pozitieinitiala=pozitieout;

        }
        string stringstdout;
        s>>stringstdout;
        fileDstdout=open(stringstdout.c_str(),O_WRONLY|O_CREAT  );
        //dup2(fileDstdout,1);
    }
    if(pozitieinp!=string::npos)
    {
        substring=imput.substr(pozitieinp+1,imput.length()-pozitieinp-1);
        stringstream s(substring);
        if(pozitieinp<pozitieinitiala)
        {
            pozitieinitiala=pozitieinp;

        }
        string stringstdin;
        s>>stringstdin;
        fileDstdin=open(stringstdin.c_str(),O_RDONLY);
        //dup2(fileDstdin,0);
    }
    imput.erase(pozitieinitiala,imput.length());
    return true;
}
