//
//  optimizer.cpp
//  natsubf
//
//  Created by 夏欣萌 on 2020/4/17.
//  Copyright © 2020 夏欣萌. All rights reserved.
//

#include "headers.h"

namespace{
vector<short> cmds;
vector<char> Full;
}

bool CheckParenthesis(const vector<char> &,bool);

void optimizeProgram(const vector<string> &files){
    for(const string &s:files){
        ifstream Input(s);
        if(!Input){
            fprintf(stderr,"Error: Unable to read file %s\n",s.c_str());
            exit(3);
        }
        char ch;
        while(Input.get(ch)){
            Full.push_back(ch);
        }
    }
    CheckParenthesis(Full,1);
    for(int i=0;i<Full.size();i++){
        if(Full[i]=='+'||Full[i]=='-'){
            cmds.push_back(1);
            int cnt=0;
            while(Full[i]=='+'||Full[i]=='-'){
                if(Full[i]=='+'){
                    cnt++;
                    if(cnt>=256){
                        cnt-=256;
                    }
                }else{
                    cnt--;
                    if(cnt<0){
                        cnt+=256;
                    }
                }
                i++;
            }
            cmds.push_back(cnt);
        }else if(Full[i]=='<'||Full[i]=='>'){
            cmds.push_back(2);
            int cnt=0;
            while(Full[i]=='<'||Full[i]=='>'){
                if(Full[i]=='>'){
                    cnt++;
                    if(cnt>=30000){
                        fprintf(stderr,"Error: Memory out of bounds!\nYou executed a '>' at memory position 29999!\n");
                        exit(2);
                    }
                }else{
                    cnt--;
                    if(cnt<=-30000){
                        fprintf(stderr,"Error: Memory out of bounds!\nYou executed a '<' at memory position 0!\n");
                        exit(1);
                    }
                }
                i++;
            }
            cmds.push_back(cnt);
        }else if(Full[i]==','){
            cmds.push_back(3);
        }else if(Full[i]=='.'){
            cmds.push_back(4);
        }else if(Full[i]=='['){
            cmds.push_back(5);
        }else if(Full[i]==']'){
            cmds.push_back(6);
        }
    }
    int cur=0;
    vector<char> ram(30000,0),stk;
    int Size=(int)cmds.size(),pos=0;
    while(cur<Size){
        if(cmds[cur]==1){
            cur++;
            ram[pos]+=cmds[cur];
        }else if(cmds[cur]==2){
            cur++;
            pos+=cmds[cur];
            if(pos<0){
                fprintf(stderr,"Error: Memory out of bounds!\nYou executed a '<' at memory position 0!\n");
                exit(1);
            }
            if(pos>29999){
                fprintf(stderr,"Error: Memory out of bounds!\nYou executed a '>' at memory position 29999!\n");
                exit(2);
            }
        }else if(cmds[cur]==3){
            ram[pos]=getchar();
        }else if(cmds[cur]==4){
            putchar(ram[pos]);
        }else if(cmds[cur]==5){
            if(!ram[pos]){
                int ceng=1;
                while(ceng){
                    cur++;
                    if(cmds[cur]==5){
                        ceng++;
                    }else if(cmds[cur]==6){
                        ceng--;
                    }else if(cmds[cur]==1||cmds[cur]==2){
                        cur++;
                    }
                }
            }else{
                stk.push_back(cur);
            }
        }else if(cmds[cur]==6){
            if(!ram[pos]){
                stk.pop_back();
            }else{
                cur=stk.back();
            }
        }
        cur++;
    }
}
