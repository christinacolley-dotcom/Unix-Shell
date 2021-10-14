#include <cstring>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <istream>
#include <sstream>
#include <fcntl.h>
#include <iterator>
#include <sys/wait.h>
#include <iostream>
#include <ctime>
#include <string>
#include <chrono>
#include <iomanip>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <string.h>
using namespace std;



string trim (string input){
    int i=0;
    while(i<input.size() && input[i]==' ')
        i++;
    if(i<input.size())
        input=input.substr(i);
    else{
        return "";
    }

    i=input.size()-1;
    while(i>=0 && input[i]==' ')
        i--;
    if(i>=0)
        input=input.substr(0,i+1);
    else   
        return "";
    return input;
}
    
char** vec_to_char_array(vector<string>& parts){
        char ** result = new char* [parts.size()+1];
        for(int i=0; i<parts.size(); i++){
            result[i]=(char*) parts [i].c_str();
        }
        result [parts.size()] = NULL;
        return result;
    }

vector<string> split(string line, char separator){

    vector<string>commands;
    string command;
    string trimmedLine=trim(line);
    int pos=0;
    int last=0;
    int curr=0;
    
    while(curr<line.size()){
        if(line[curr]=='\''){
            command=line.substr(curr+1, line.size());
            pos=command.find('\'');
            if(pos>=0){
                curr=curr+pos+1;
            }
        }
        else if(line[curr]==separator){
            command=line.substr(last, curr-last);
            if(command.size()>0){
                commands.push_back(command);
            }
            last=curr+1;
        }
        else if(line[curr]=='\"'){
            command=line.substr(curr+1, line.size());
            pos=command.find('\"');
            if(pos>=0){
                curr=currx+pos+1;
            }
        }
        curr++;
    }
    command=line.substr(last, curr-last);
    if(commands.size()>0){
        commands.push_back(command);
    }

    if(separator==' '){
        for(int i=0; i<commands.size(); i++){
            int j=0;
            while(j<commands[i].size()){
                if(commands[i][j]=='\''){
                    commands[i].erase(commands[i].begin()+j);
                } else{
                    j++;
                }
            }
        }
    }
    return commands;
}



int main(){

    vector <int> process;

    while(true){
        auto tm= *localtime(&t);
        
        char* username=getenv("USER");
        cout << username <<  ":" << put_time(&tm, "%d-%m-%Y %H-%M-%S") << "$ ";


        for(int i=0; i<process.size();i++){
            if(waitpid(process[i],0,WNOHANG)!=0){
                process.erase(process.begin()+i);
            }
        }

        string inputline;
        getline(cin,inputline);

        if(inputline==string("exit")){
            cerr << "Bye!!!" << endl;
            exit(1);
            break;
        }
        vector<string> c=split(inputline,'|');
        int fd[2];
        pipe(fd);

        int cid=fork();

        for(int i=0; i<c.size();i++){
            string temp=trim(c[i]);
            int process2=0;

            if(temp[temp.size()-1]=='&'){
                c[i]=temp.substr(0,temp.size()-2);
                process2=1;
            }           

            if(!cid){
                if(trim(inputline).find("cd")==0){
                    string dirname = trim(split(inputline,' ')[1]);
                    chdir(dirname.c_str());
                    continue;
                }
                if(i<c.size()-1){
                    dup2(fd[1],1);
                }              

            }
            
             else{
                if(process2==1){
                    process.push_back(cid);
                }else if(i==c.size()-1){
                    waitpid(cid,0,0);
                }
                dup2(fd[0],0);
                close(fd[1]);
            }
        }

    }


}