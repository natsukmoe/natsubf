//
//  runner.cpp
//  natsubf
//
//  Created by 夏欣萌 on 2020/4/3.
//  Copyright © 2020 夏欣萌. All rights reserved.
//

#include "headers.h"

namespace {
vector<char> Program,Full;
}

bool CheckParenthesis(const vector<char> &,bool);

void RunProgram(){
    CheckParenthesis(Full,1);
    int cur=0;
    vector<char> ram(30000,0);
    int Size=(int)Program.size(),pos=0;
    vector<int> reps;
    while(cur<Size){
        if(Program[cur]=='+'){
            ram[pos]++;
        }else if(Program[cur]=='-'){
            ram[pos]--;
        }else if(Program[cur]=='<'){
            if(pos==0){
                fprintf(stderr,"Error: Memory out of bounds!\nYou executed a '<' at memory position 0!\n");
                exit(1);
            }
            pos--;
        }else if(Program[cur]=='>'){
            if(pos==29999){
                fprintf(stderr,"Error: Memory out of bounds!\nYou executed a '>' at memory position 29999!\n");
                exit(2);
            }
            pos++;
        }else if(Program[cur]==','){
            ram[pos]=getchar();
        }else if(Program[cur]=='.'){
            putchar(ram[pos]);
        }else if(Program[cur]=='['){
            if(ram[pos]){
                reps.push_back(cur);
            }else{
                int ceng=1;
                while(cur<Size&&ceng){
                    cur++;
                    if(Program[cur]=='['){
                        ceng++;
                    }
                    if(Program[cur]==']'){
                        ceng--;
                    }
                }
            }
        }else if(Program[cur]==']'){
            if(ram[pos]){
                cur=reps.back();
            }else{
                reps.pop_back();
            }
        }
        cur++;
    }
}

void StartRun(const vector<string> &files){
    for(const string &s:files){
        ifstream Input(s);
        if(!Input){
            fprintf(stderr,"Error: Unable to read file %s\n",s.c_str());
            exit(3);
        }
        char ch;
        while(Input.get(ch)){
            if(ch=='+'||ch=='-'||ch=='<'||ch=='>'||ch=='.'||ch==','||ch=='['||ch==']'){
                Program.push_back(ch);
            }
            Full.push_back(ch);
        }
    }
    RunProgram();
}
