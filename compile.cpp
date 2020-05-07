//
//  compile.cpp
//  natsubf
//
//  Created by 夏欣萌 on 2020/4/26.
//  Copyright © 2020 夏欣萌. All rights reserved.
//

#include "headers.h"

namespace{
vector<char> Full;
vector<short> cmds;
}

bool CheckParenthesis(const vector<char> &,bool);

void CompileCpp(const string &filename){
    ofstream fout(filename);
    puts("Writing header ...");
    fout<<"/*\n * Program compiled with natsubf by Natsu Kinmoe\n * Natsubf is a multifunctional brainfuck tool\n * https://github.com/natsukmoe/natsubf\n */\n";
    fout<<"#include <cstdio>\n\nchar ram[30000];\nint ptr;\n\n";
    fout<<"int main(){\n";
    int cmdptr=0;
    int Siz=(int)cmds.size();
    int Tabs=1;
    puts("Writing program ...");
    printf("Progress: 0%%\r");
    int lstpct=0;
    while(cmdptr<Siz){
        if(cmds[cmdptr]==1){
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            cmdptr++;
            fout<<"ram[ptr]+="<<cmds[cmdptr]<<";\n";
        }else if(cmds[cmdptr]==2){
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            cmdptr++;
            if(cmds[cmdptr]>0){
                fout<<"ptr+="<<cmds[cmdptr]<<";\n";
            }else{
                fout<<"ptr-="<<-cmds[cmdptr]<<";\n";
            }
        }else if(cmds[cmdptr]==3){
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            fout<<"ram[ptr]=getchar();\n";
        }else if(cmds[cmdptr]==4){
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            fout<<"putchar(ram[ptr]);\n";
        }else if(cmds[cmdptr]==5){
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            Tabs++;
            fout<<"while(ram[ptr]){\n";
        }else if(cmds[cmdptr]==6){
            Tabs--;
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            fout<<"}\n";
        }
        cmdptr++;
        if(cmdptr*100/Siz>lstpct){
            lstpct=cmdptr*100/Siz;
            printf("Progress: %d%%\r",lstpct);
        }
    }
    fout<<"\treturn 0;\n";
    fout<<"}\n";
    puts("Compile to C++ finished successfully.");
}

void CompileJava(const string &filename){
    ofstream fout(filename);
    puts("Writing header ...");
    fout<<"/*\n * Program compiled with natsubf by Natsu Kinmoe\n * Natsubf is a multifunctional brainfuck tool\n * https://github.com/natsukmoe/natsubf\n */\n";
    fout<<"import java.io.*;\npublic class Main{\n\tstatic byte[] ram=new byte[30000];\n\tstatic int ptr;\n\n";
    fout<<"\tpublic static byte getchar(){\n\t\tbyte ch=-1;\n\t\ttry{\n\t\t\tch=(byte)System.in.read();\n\t\t}catch(IOException e){}\n\t\treturn ch;\n\t}\n\n";
    fout<<"\tpublic static void main(String[] args){\n";
    int cmdptr=0;
    int Siz=(int)cmds.size();
    int Tabs=2;
    puts("Writing program ...");
    printf("Progress: 0%%\r");
    int lstpct=0;
    while(cmdptr<Siz){
        if(cmds[cmdptr]==1){
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            cmdptr++;
            fout<<"ram[ptr]+="<<cmds[cmdptr]<<";\n";
        }else if(cmds[cmdptr]==2){
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            cmdptr++;
            if(cmds[cmdptr]>0){
                fout<<"ptr+="<<cmds[cmdptr]<<";\n";
            }else{
                fout<<"ptr-="<<-cmds[cmdptr]<<";\n";
            }
        }else if(cmds[cmdptr]==3){
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            fout<<"ram[ptr]=getchar();\n";
        }else if(cmds[cmdptr]==4){
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            fout<<"System.out.print((char)ram[ptr]);\n";
        }else if(cmds[cmdptr]==5){
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            Tabs++;
            fout<<"while(ram[ptr]!='\\0'){\n";
        }else if(cmds[cmdptr]==6){
            Tabs--;
            for(int i=0;i<Tabs;i++){
                fout<<"\t";
            }
            fout<<"}\n";
        }
        cmdptr++;
        if(cmdptr*100/Siz>lstpct){
            lstpct=cmdptr*100/Siz;
            printf("Progress: %d%%\r",lstpct);
        }
    }
    fout<<"\t}\n}\n";
    puts("Compile to Java finished successfully.");
}

void CompileProgram(const vector<string> &files,int lx){
    if(!lx){
        puts("Checking g++ ...");
        if(system("g++ -v")){
            puts("g++ not found! Please install C++ compiler and try again!");
            exit(10);
        }
    }
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
    int newsiz=0;
    for(int i=0;i<Full.size();i++){
        if(Full[i]==','||Full[i]=='.'||Full[i]=='+'||Full[i]=='-'||Full[i]=='<'||Full[i]=='>'||Full[i]=='['||Full[i]==']'){
            Full[newsiz++]=Full[i];
        }
    }
    Full.erase(Full.begin()+newsiz,Full.end());
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
            i--;
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
            i--;
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
    string filename=files[0];
    if(files.size()>1){
        filename+=" and more files";
    }
    if(lx==1){
        filename+=".cpp";
        CompileCpp(filename);
    }else if(lx==2){
        if(filename[filename.size()-1]!='/'&&filename[filename.size()-1]!='\\'){
            filename.pop_back();
        }
        filename+="_javacode";
        system(("mkdir \""+filename+"\"").c_str());
        filename+="/Main.java";
        CompileJava(filename);
    }else if(!lx){
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        string outfilename=filename+".exe";
        #else
        string outfilename=filename+".out";
        #endif
        filename+=".~tmp~.cpp";
        CompileCpp(filename);
        puts("Compiling to executable ...");
        system(("g++ \""+filename+"\" -o \""+outfilename+"\"").c_str());
        #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        system(("del \""+filename+"\"").c_str());
        #else
        system(("rm \""+filename+"\"").c_str());
        #endif
        puts("Compile to executable finished successfully.");
    }
}
