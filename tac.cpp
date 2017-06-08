#include "comands.h"

int tac(vector<string> arg)
{
    FILE* in=stdin;
    string sep="\n";
    bool b=false,s=false;
    if(arg.size()>1)
        for(vector<string>::iterator it=++arg.begin();
            it!=arg.end();++it)
        {
            if(*it=="-b")
            {
                if(!b)
                   b=true;
                else
                {
                    fprintf(stderr,"eroare:tac-b");
                    return -1;
                }
            }
            else if(*it=="-s")
            {
                if(!s)
                {
                    s=true;
                    ++it;
                    if(it==arg.end())
                    {
                        fprintf(stderr,"eroare:tac-s1");
                        return -1;
                    }
                    sep=(*it);
                }
                else
                {
                    fprintf(stderr,"eroare:tac-s");
                    return -1;
                }
            }
            else if((++it)==arg.end())
            {
                it--;
                in=fopen((*it).c_str(),"r");
            }
            else
            {
                fprintf(stderr,"eroare:tac+\n");
                return -1;
            }
        }
    char str[1024];
    stringstream sl;
    //string line="";
    while (fgets(str, sizeof(str), in))
        sl<<string(str);
    string l="";
    string continut=sl.str();
    size_t pozitie=0;
    pozitie=continut.find(sep);
    while(pozitie!=string::npos)
    {
       if(b)
           l=sep+continut.substr(0,pozitie)+l;
       else
           l=continut.substr(0,pozitie)+sep+l;
       continut.erase(0,pozitie+sep.length());
       pozitie=continut.find(sep);
    }
    if(b)
        l=continut+l;
    else
        l=continut+l;
    //if(l[l.length()]==sep)
      // l[l.length()]=0;
    fprintf(stdout,"%s",l.c_str());
    return 0;
}
