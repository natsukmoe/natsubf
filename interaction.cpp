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
    string Command;
    int curpos=0;
    vector<char> ram(30000,0);
    while(1){
        printf("Natsubf:%d(%d)> ",curpos,ram[curpos]);
        getline(cin,Command);
        int khpip=0;
        for(int i=0;i<(int)Command.size();i++){
            if(Command[i]=='('){
                khpip++;
            }else{
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
            printf("... ");
            string s;
            getline(cin,s);
            for(int i=0;i<(int)s.size();i++){
                if(s[i]=='('){
                    khpip++;
                }else{
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
    }
}
