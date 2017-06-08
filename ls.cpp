#include "comands.h"

struct stat buf;
namespace {
enum FLAGS
{
    ALL,
    LINES,
    CLASSIFY,
    SIZE
};

//functia returneaza marimea unui folder
int size(const char* folder_name){
    //verifica sa nu mearga ciclic(. daca e folderul curent si .. sa nu mearga la folderul precedent)
    if(strcmp(folder_name,".")==0||strcmp(folder_name,"..")==0)
        return 0;
    DIR *d;
    struct dirent *de;
    //struct stat buf;
    int exists;
    int total_size;
    //deschide folderul
    d = opendir(folder_name);
    // verifica daca exista
    if (d == NULL) {
        perror("prsize");
        return 0;
    }

    total_size = 0;
    //parcurge fiecare fisier si folder din folderul ales si calculeaza marimea totala
    for (de = readdir(d); de != NULL; de = readdir(d)) {
        //daca e folder adaugam la marimea totala marimea lui calculata cu ajutoul functiei recursive
        if(de->d_type==DT_DIR)
            total_size+=size(de->d_name);
        else{
            //daca nu o adaugam direct cu stat;
            exists = stat(de->d_name, &buf);
            if (exists < 0) {
                //fprintf(stderr, "Couldn't stat %s\n", de->d_name);
            } else {
                total_size += buf.st_size;
            }
        }
    }
    //inchidem folderul
    closedir(d);
    //returnam marimea folderului
    return total_size;
}
void checkFlags(vector<string>& args, bool flags[])
{
    for(auto i = args.begin(); i != args.end(); ++i)
        if(*i == "-a")
        {
            flags[ALL] = true;
            args.erase(i);
            --i;
        }
        else if(*i == "-l")
        {
            flags[LINES] = true;
            args.erase(i);
            --i;
        }
        else if(*i == "-F")
        {
            flags[CLASSIFY] = true;
            args.erase(i);
            --i;
        }
        else if(*i == "-s")
        {
            flags[SIZE] = true;
            args.erase(i);
            --i;
        }
}

string getLocation(vector<string>& args)
{
    string location;
    for(string& i : args)
        if( i != "ls")
        {
            location = i;
            break;
        }
    return location;
}

vector<string> getContent(string location)
{
    char buf[1024];
    if(location == string())
        location = getcwd(buf, sizeof(buf));
    DIR* dir = opendir(location.c_str());

    vector<string> content;
    while(struct dirent* direntry = readdir(dir))
        content.push_back(string(direntry->d_name));

    closedir(dir);
    return content;
}
}
//
vector< vector<string> > StructDir;
/*
 * drwx permission format
 *
 */
void drwx(mode_t i, char *buf) {
    *buf++ = (i & S_IFDIR) ? 'd' : '-';
    *buf++ = (i & S_IRUSR) ? 'r' : '-';
    *buf++ = (i & S_IWUSR) ? 'w' : '-';
    *buf++ = (i & S_IXUSR) ? 'x' : '-';
    *buf++ = (i & S_IRGRP) ? 'r' : '-';
    *buf++ = (i & S_IWGRP) ? 'w' : '-';
    *buf++ = (i & S_IXGRP) ? 'x' : '-';
    *buf++ = (i & S_IROTH) ? 'r' : '-';
    *buf++ = (i & S_IWOTH) ? 'w' : '-';
    *buf++ = (i & S_IXOTH) ? 'x' : '-';
    *buf = '\0';
}

/*
     * Create struct for print
     * Mode | UID | GID | Size | Modify time  | Name
     *
     */
void create_struct(vector<string> &lsdir) {
    struct stat objFile;
    char * dirnow;
    char /*mode[10],*/uid[5],gid[5],size[100];
    vector<string> head;

    head.push_back("Mode");
    head.push_back("UID");
    head.push_back("GID");
    head.push_back("Size");
    head.push_back("Modify time");
    head.push_back("Name");

    StructDir.push_back(head);

    for (unsigned int i=0; i<lsdir.size(); i++) {
        vector<string> line;
        dirnow = new char[lsdir[i].length() + 1];
        strcpy(dirnow, lsdir[i].c_str());

        char time[25];
        stat(dirnow,&objFile);
        struct tm * timeinfo;

        timeinfo = localtime(&objFile.st_ctime);
        strftime(time,sizeof(time),"%Y-%m-%d %H:%M",timeinfo);

        char mode[BUFSIZ];
        drwx(objFile.st_mode, mode);
        if(objFile.st_uid==0)
            sprintf(uid, "root");
        else
            sprintf(uid, "%d", objFile.st_uid);
        if(objFile.st_gid==0)
            sprintf(gid, "root");
        else
            sprintf(gid, "%d", objFile.st_gid);
        sprintf(size, "%d", (int)objFile.st_size);

        line.push_back(mode);
        line.push_back(uid);
        line.push_back(gid);
        line.push_back(size);
        line.push_back(time);
        line.push_back(dirnow);

        StructDir.push_back(line);
    }
};

//
/*
     * Split dir name
     *
     * maxlength_name - max length directory name
     * length_blank - count blank befor begin directory name
     */
vector<string> split_blank (char* &dir_name,unsigned int &maxlength_name){
    char *split;
    vector<string> dir_split;
    vector<string> new_dir;

    string temp_string,prev_string;

    split = strtok(dir_name, " ");
    while (split != NULL) {
        dir_split.push_back(split);
        split = strtok(NULL, " ");
    }

    for (unsigned int i=0; i<dir_split.size(); i++) {
        if (dir_split[i].length() > maxlength_name-1u) {
            new_dir.push_back(dir_split[i]);
        } else {
            prev_string = temp_string;
            temp_string += dir_split[i] + " ";
            if (temp_string.length() > maxlength_name) {
                new_dir.push_back(prev_string);
                temp_string.clear();
            }
        }
    }
    if (!temp_string.empty()) {
        new_dir.push_back(temp_string);
    }
    return new_dir;
}
void print(vector< vector<string> > &dir_struct){
    char * print_string;
    int width=0;
    width=width+0;
    vector<unsigned int> maxlen;
    vector<string> splitname;
    unsigned int maxlength_name = 20;
    char s_format[100];
    string blank(" "),print_name;

    for (unsigned int i =0; i<dir_struct[0].size(); i++){
        maxlen.push_back(0);
    }

    for (unsigned int i=0; i<dir_struct.size(); i++) {
        for (unsigned int j=0; j<dir_struct[i].size(); j++){
            if (dir_struct[i][j].length() > maxlen[j]){
                maxlen[j] = dir_struct[i][j].length();
            }
        }
    }

    for (unsigned int i=0; i<dir_struct.size(); i++) {
        int all_length = 0;
        for (unsigned int j=0; j<dir_struct[i].size(); j++){
            print_string = new char[dir_struct[i][j].length() + 1];
            strcpy(print_string, dir_struct[i][j].c_str());
            width = maxlen[j] - dir_struct[i][j].length();
            all_length += maxlen[j]+2;
            if (j!=0 && j!=dir_struct[i].size()-1)
                sprintf(s_format, "%c%d%c", '%', maxlen[j]+2, 's');
            else if (j!=dir_struct[i].size()-1)
                sprintf(s_format, "%c%d%c", '%', maxlen[j], 's');
            else {
                int length_blank = all_length - maxlen[j];
                splitname = split_blank(print_string,maxlength_name);
                for (unsigned int i=0; i<splitname.size(); i++) {
                    if (i == 0)
                        blank= "  ";
                    else{
                        blank= "\n";
                        for (int i=0; i<length_blank-2; i++) {
                            blank += ' ';
                        }
                    }
                    print_name = blank + splitname[i];
                    print_string = new char[print_name.length() + 1];
                    strcpy(print_string, print_name.c_str());
                    printf("%s",print_string);
                }
            }
            if (j != dir_struct[i].size()-1) {
                printf(s_format,print_string);
            }
        }
        printf("\n");
    }
}



//
int ls(vector<string> args)//ls
{
    bool flags[4] = {false, false, false, false};
    checkFlags(args, flags);//se verifica flag-urile
    vector<string> dirContent = getContent(getLocation(args));//se ia content-ul dirului din args
    if(flags[ALL] == false)//ls -a
        for(auto it = dirContent.begin(); it != dirContent.end(); ++it)
            if(it->at(0) == '.')
            {
                dirContent.erase(it);
                --it;
            }
    vector<int> sizeContent;
    for(vector<string>::iterator it = dirContent.begin(); it != dirContent.end();++it){
        if(strcmp((*it).c_str(),".")==0||strcmp((*it).c_str(),"..")==0)
            sizeContent.push_back(0);
        else{
            //verificam daca e folder sau director
            struct stat buf;
            int exists=stat((*it).c_str(),&buf);
            if (exists < 0) {
                fprintf(stderr, "Couldn't stat %s\n", (*it).c_str());
            } else {
                //aici verificam propriu zis
                /* if(S_ISDIR(buf.st_mode))
                    sizeContent.push_back(size((*it).c_str()));
                else*/
                sizeContent.push_back(buf.st_size==0?0:((
                                                            ((int)buf.st_size/4096)+1)*4));
            }
        }
    }
    if(flags[CLASSIFY])//ls -F
    {
        struct stat buf;

        for(string fis:dirContent)
        {
            stat(fis.c_str(),&buf);
            char end;
            if(S_ISDIR(buf.st_mode)) end='/';//file is a directory
            else
            {
                if(S_ISREG(buf.st_mode))
                    end=' '; //file is a regular file
                if (access(fis.c_str(), X_OK) == 0)
                    end='*';
                if(S_ISLNK(buf.st_mode))
                    end='@';//file is a symbolic link
                if(S_ISSOCK(buf.st_mode))
                    end='=';
                if(S_ISFIFO(buf.st_mode))
                    end='|';

            }
            printf ("%s%c\n ", fis.c_str(),end);
        }
        return 0;
    }
    if(flags[SIZE])//ls -s
    {
        int total=0;
        for(vector<int>::iterator ij = sizeContent.begin();ij!=sizeContent.end();++ij){total+=*ij;
        }
        cout<<"Total:"<<total<<endl;
        vector<int>::iterator ij = sizeContent.begin();
        for(vector<string>::iterator it = dirContent.begin(); it != dirContent.end()&&ij!=sizeContent.end();++it){
            cout<<*ij<<" "<<*it<<" \n";
            ++ij;
        }
        return 0;
    }

    sort(dirContent.begin(), dirContent.end(), [](string a, string b) {
        for(char& i : a)
            i = toupper(i);
        for(char& i : b)
            i = toupper(i);
        return a < b;
    });



    if(flags[LINES])
    {
        create_struct(dirContent);
        print(StructDir);
        return 0;
    }

    for(string i : dirContent)
    {
        cout<<i <<endl;
    }
    return 0;
}
