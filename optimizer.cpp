//
//  optimizer.cpp
//  natsubf
//
//  Created by 夏欣萌 on 2020/4/17.
//  Copyright © 2020 夏欣萌. All rights reserved.
//

#include "headers.h"

namespace{
vector<int> cmds;
vector<char> Full;
}

bool CheckParenthesis(const vector<char> &,bool);

void optimizeProgram(vector<string> &files){
    for(const string &s:files){
        ifstream Input(s);
        if(!Input){
            fprintf(stderr,"Error: Unable to read file %s\n",s.c_str());
            exit(1);
        }
        char ch;
        while(Input.get(ch)){
            Full.push_back(ch);
        }
    }
    CheckParenthesis(Full,1);
}
