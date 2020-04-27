//
//  main.cpp
//  natsubf
//
//  Created by 夏欣萌 on 2020/4/3.
//  Copyright © 2020 夏欣萌. All rights reserved.
//

#include "headers.h"

void StartRun(const vector<string> &);
void StartDebug(const vector<string> &);
void StartInteract();
void optimizeProgram(const vector<string> &);
void CompileProgram(const vector<string> &,int);

bool CheckParenthesis(const vector<char> &Full,bool ExitwhenFail=1){
    int ceng=0;
    int FirstPos=-1;
    int Size=(int)Full.size();
    for(int i=0;i<Size;i++){
        if(Full[i]=='['){
            ceng++;
            if(ceng==1){
                FirstPos=i;
            }
        }
        if(Full[i]==']'){
            ceng--;
            if(ceng<0){
                if(ExitwhenFail){
                    fprintf(stderr,"Error: Unable to match parenthesis at position %d!\n",i);
                    exit(4);
                }else{
                    return 0;
                }
            }
        }
    }
    if(ceng>0){
        if(ExitwhenFail){
            fprintf(stderr,"Error: Unable to match parenthesis at position %d!\n",FirstPos);
            exit(4);
        }else{
            return 0;
        }
    }
    return 1;
}

void printHelp(){
    puts("Natsubf Brainfuck Runner and Debugger by Natsu Kinmoe");
    puts("Version: 1.2.0.0");
    puts("Usage: natsubf [-d|-o] [file...]");
    puts("       natsubf [-i]");
    puts("       natsubf [-c(c)] [file...]");
    puts("\t-d\t\tStart a debugger.");
    puts("\t-i\t\tStart interaction.");
    puts("\t-o\t\tRun the program in optimize mode.");
    puts("\t-c(c)\t\tCompile the program to other programming languages, or to an executable file if there's only -c .");
    puts("\t\tc\tCompile to C++");
}

int main(int argc,const char *argv[]) {
    if(argc==1){
        printHelp();
    }else{
        bool isDebug=false,isInter=false,isopt=false,isccpp=false,iscexe=false,isoption=true;
        if(!strcmp(argv[1],"-d")){
            isDebug=true;
        }else if(!strcmp(argv[1],"-i")){
            isInter=true;
        }else if(!strcmp(argv[1],"-o")){
            isopt=true;
        }else if(!strcmp(argv[1],"-cc")){
            isccpp=true;
        }else if(!strcmp(argv[1],"-c")){
            iscexe=true;
        }else{
            isoption=false;
        }
        vector<string> filenames;
        for(int i=1+isoption;i<argc;i++){
            filenames.push_back(argv[i]);
        }
        if(isDebug){
            StartDebug(filenames);
        }else if(isInter){
            StartInteract();
        }else if(isopt){
            optimizeProgram(filenames);
        }else if(isccpp){
            CompileProgram(filenames,1);
        }else if(iscexe){
            CompileProgram(filenames,0);
        }else{
            StartRun(filenames);
        }
    }
    return 0;
}
