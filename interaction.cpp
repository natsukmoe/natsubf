//
//  interaction.cpp
//  natsubf
//
//  Created by 夏欣萌 on 2020/4/10.
//  Copyright © 2020 夏欣萌. All rights reserved.
//

#include "headers.h"

void StartInteract(){
    puts("Natsubf Brainfuck Interaction by Natsu Kinmoe");
    puts("Type '*' to reset the state, type '$' to exit.");
    puts("");
    rl_bind_key('\t',rl_insert);
    stifle_history(50);
    string Command;
    int curpos=0;
    vector<char> ram(30000,0);
    vector<int> cycpos;
    char prompt[25];
    while(1){
        int Val=ram[curpos];
        if(Val<0){
            Val+=256;
        }
        sprintf(prompt,"Natsubf:%d(%d)> ",curpos,Val);
        //printf("Natsubf:%d(%d)> ",curpos,Val);
        char *line=readline(prompt);
        if(!line){
            puts("");
            break;
        }
        Command=line;
        int khpip=0;
        for(int i=0;i<(int)Command.size();i++){
            if(Command[i]=='['){
                khpip++;
            }else if(Command[i]==']'){
                khpip--;
                if(khpip<0){
                    break;
                }
            }
        }
        if(khpip){
            puts("Execution failed! Unmatched right parenthesis found!");
            continue;
        }
        bool tbc=0;
        while(khpip){
            line=readline("... ");
            if(!line){
                puts("");
                goto ENDWIN;
            }
            string s=line;
            for(int i=0;i<(int)s.size();i++){
                if(s[i]=='['){
                    khpip++;
                }else if(s[i]==']'){
                    khpip--;
                    if(khpip<0){
                        break;
                    }
                }
            }
            if(khpip<0){
                tbc=1;
                break;
            }
            Command+=s;
        }
        if(tbc){
            puts("Execution failed! Unmatched right parenthesis found!");
            continue;
        }
        bool isinp=0;
        for(int i=0;i<(int)Command.size();i++){
            if(Command[i]=='<'){
                if(curpos==0){
                    puts("Runtime Error! You executed a '<' at position 0!");
                    break;
                }
                curpos--;
            }else if(Command[i]=='>'){
                if(curpos==29999){
                    puts("Runtime Error! You executed a '>' at position 29999!");
                    break;
                }
                curpos++;
            }else if(Command[i]==','){
                ram[curpos]=getchar();
                isinp=1;
            }else if(Command[i]=='.'){
                putchar(ram[curpos]);
            }else if(Command[i]=='+'){
                ram[curpos]++;
            }else if(Command[i]=='-'){
                ram[curpos]--;
            }else if(Command[i]=='['){
                if(!ram[curpos]){
                    khpip=1;
                    while(khpip){
                        i++;
                        if(Command[i]=='['){
                            khpip++;
                        }else if(Command[i]==']'){
                            khpip--;
                        }
                    }
                }else{
                    cycpos.push_back(i);
                }
            }else if(Command[i]==']'){
                if(!ram[curpos]){
                    cycpos.pop_back();
                }else{
                    i=cycpos.back();
                }
            }else if(Command[i]=='*'){
                ram.assign(30000,0);
                curpos=0;
            }else if(Command[i]=='$'){
                exit(0);
            }
        }
        if(isinp){
            while(getchar()!='\n');
        }
    }
ENDWIN:;
}
