#include "comands.h"


int dir(vector<string> args)
{
    char buf[1024];
    //verifica daca are argumente :daca nu are ,ia ca director pe cel curent daca are argumente il ia ca director pe cel dat  ca argument
    if(args.size()==1)
    {
        getcwd(buf,1024);
    }
    else
    {
        strcpy(buf,args.at(args.size()-1).c_str());
    }
    //deschidem directorul
    DIR* cd=opendir(buf);
    //string parent=args[1]+"/";
    //vectoru e ca sa stocam continutu directorului
    vector<string>continut;
    //parcurgem toate fisierele din director
    while(struct dirent* e=readdir(cd)){
        //luam numele fiecarui fisier
        string name= e->d_name;
        //il verificam sa nu fie: . =directorul curent ; .. =directorul precedent ; incepe cu . =ascuns;
        if(name=="."||name==".."||name[0]=='.')
            continue;
        //daca nu e unul din directoarele de mai sus il adaugam in vector
        continut.push_back(name);
    }
    //inchidem directorul
    closedir(cd);
    //sortam continutul
    sort(continut.begin(),continut.end());
    //afisam continutul pe ecran
    for(unsigned int i=0;i<continut.size();i++) {
        printf("%s \t",continut.at(i).c_str());
    }
    printf("\n");
    return 0;
}
