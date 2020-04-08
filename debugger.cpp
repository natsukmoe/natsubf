//
//  debugger.cpp
//  natsubf
//
//  Created by 夏欣萌 on 2020/4/4.
//  Copyright © 2020 夏欣萌. All rights reserved.
//

#include "headers.h"

namespace {
vector<char> Program,Full;
int Progsz;
vector<int> ddid;
vector<pair<int,pair<int,int>>> ddpos;
int dds;
int Curpos,Ramptr;
WINDOW *title,*code,*input,*output,*ramwatch,*status;
vector<char> inputs,outputs;
}

void CheckParenthesis(const vector<char> &);

void PrintCodeatPos(int pos){
    wattron(code,COLOR_PAIR(1));
    int Codecols=COLS-1;
    for(int i=0;i<3;i++){
        for(int j=0;j<Codecols;j++){
            mvwaddch(code,i,j,' ');
        }
    }
    int curMidPoint=Codecols/2;
    int frontChars=curMidPoint;
    if(frontChars<=pos){
        int nowPrintedPos=pos-frontChars;
        for(int i=0;i<frontChars;i++){
            mvwaddch(code,1,i,Program[nowPrintedPos]);
            nowPrintedPos++;
        }
    }else{
        int nowCursorPos=frontChars-pos;
        for(int i=0;i<pos;i++){
            mvwaddch(code,1,nowCursorPos,Program[i]);
            nowCursorPos++;
        }
    }
    wattron(code,COLOR_PAIR(6));
    mvwaddch(code,1,curMidPoint,Program[pos]);
    wattron(code,COLOR_PAIR(1));
    int curSorPos=curMidPoint+1,nowPrPos=pos+1;
    while(curSorPos<Codecols&&nowPrPos<Progsz){
        mvwaddch(code,1,curSorPos,Program[nowPrPos]);
        nowPrPos++;
        curSorPos++;
    }
}

void PrintRamatPos(int pos){
    wattron(ramwatch,COLOR_PAIR(4));
    int Ramcols=COLS-1;
    for(int i=0;i<3;i++){
        for(int j=0;j<Ramcols;j++){
            mvwaddch(ramwatch,i,j,' ');
        }
    }
}

void StartDebug(const vector<string> &files){
    int Fileid=1,Lineid,Colid;
    for(const string &s:files){
        Lineid=1;
        Colid=1;
        ifstream Input(s);
        if(!Input){
            fprintf(stderr,"Error: Unable to read file %s\n",s.c_str());
            exit(1);
        }
        char ch;
        while(Input.get(ch)){
            if(ch=='+'||ch=='-'||ch=='<'||ch=='>'||ch=='.'||ch==','||ch=='['||ch==']'||ch=='#'){
                Program.push_back(ch);
                if(ch=='#'){
                    ddid.push_back(dds++);
                    ddpos.emplace_back(Fileid,make_pair(Lineid,Colid));
                }else{
                    ddid.push_back(0);
                }
            }
            Full.push_back(ch);
            if(ch=='\n'){
                Lineid++;
                Colid=1;
            }else{
                Colid++;
            }
        }
        Fileid++;
    }
    Progsz=(int)Program.size();
    CheckParenthesis(Full);
    puts("Natsubf Brainfuck Debugger by Natsu Kinmoe");
    puts("Program used curses engine (pdcurses on Windows and ncurses on macOS)");
    initscr();
    noecho();
    clear();
    title=newwin(2,COLS,0,0);
    string Title="Natsubf Brainfuck Debugger";
    mvwprintw(title,0,COLS/2-(int)Title.size()/2,Title.c_str());
    Title="(Note: use a '#' to make a breakpoint)";
    mvwprintw(title,1,COLS/2-(int)Title.size()/2,Title.c_str());
    code=newwin(3,COLS-1,2,0);
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_RED);
    init_pair(2,COLOR_BLACK,COLOR_CYAN);
    init_pair(3,COLOR_BLACK,COLOR_GREEN);
    init_pair(4,COLOR_BLACK,COLOR_YELLOW);
    init_pair(5,COLOR_BLACK,COLOR_WHITE);
    init_pair(6,COLOR_RED,COLOR_WHITE);
    wattron(code,COLOR_PAIR(1));
    Curpos=0;
    PrintCodeatPos(Curpos);
    input=newwin(LINES-15,COLS/2-1,6,0);
    wattron(input,COLOR_PAIR(2));
    for(int i=0;i<LINES-15;i++){
        for(int j=0;j<COLS/2-1;j++){
            mvwaddch(input,i,j,' ');
        }
    }
    output=newwin(LINES-15,COLS-COLS/2-2,6,COLS/2+1);
    wattron(output,COLOR_PAIR(3));
    for(int i=0;i<LINES-15;i++){
        for(int j=0;j<COLS-COLS/2-2;j++){
            mvwaddch(output,i,j,' ');
        }
    }
    ramwatch=newwin(5,COLS-1,LINES-8,0);
    PrintRamatPos(Ramptr);
    status=newwin(2,COLS-1,LINES-2,0);
    wattron(status,COLOR_PAIR(5));
    for(int i=0;i<2;i++){
        for(int j=0;j<COLS-1;j++){
            mvwaddch(status,i,j,' ');
        }
    }
    wrefresh(title);
    wrefresh(code);
    wrefresh(input);
    wrefresh(output);
    wrefresh(ramwatch);
    wrefresh(status);
    wgetch(input);
    endwin();
}
