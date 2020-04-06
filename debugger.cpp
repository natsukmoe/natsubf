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
vector<int> ddid;
vector<pair<int,pair<int,int>>> ddpos;
int dds;
}

void CheckParenthesis(const vector<char> &);

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
    CheckParenthesis(Full);
    puts("Natsubf Brainfuck Debugger by Natsu Kinmoe");
    puts("Program used curses engine (pdcurses on Windows and ncurses on macOS)");
    initscr();
    noecho();
    clear();
    WINDOW *title=newwin(1,COLS,0,0);
    string Title="Natsubf Brainfuck Debugger";
    mvwprintw(title,0,COLS/2-(int)Title.size()/2,Title.c_str());
    wrefresh(title);
    WINDOW *code=newwin(3,COLS-1,2,0);
    start_color();
    init_pair(1,COLOR_WHITE,COLOR_RED);
    wattron(code,COLOR_PAIR(1));
    for(int i=0;i<3;i++){
        for(int j=0;j<COLS-1;j++){
            mvwaddch(code,i,j,' ');
        }
    }
    WINDOW *input=newwin(LINES-15,COLS/2-1,6,0);
    init_pair(2,COLOR_BLACK,COLOR_CYAN);
    wattron(input,COLOR_PAIR(2));
    for(int i=0;i<LINES-15;i++){
        for(int j=0;j<COLS/2-1;j++){
            mvwaddch(input,i,j,' ');
        }
    }
    WINDOW *output=newwin(LINES-15,COLS-COLS/2-2,6,COLS/2+1);
    init_pair(3,COLOR_BLACK,COLOR_GREEN);
    wattron(output,COLOR_PAIR(3));
    for(int i=0;i<LINES-15;i++){
        for(int j=0;j<COLS-COLS/2-2;j++){
            mvwaddch(output,i,j,' ');
        }
    }
    WINDOW *ramwatch=newwin(5,COLS-1,LINES-8,0);
    init_pair(4,COLOR_BLACK,COLOR_YELLOW);
    wattron(ramwatch,COLOR_PAIR(4));
    for(int i=0;i<5;i++){
        for(int j=0;j<COLS-1;j++){
            mvwaddch(ramwatch,i,j,' ');
        }
    }
    WINDOW *status=newwin(2,COLS-1,LINES-2,0);
    init_pair(5,COLOR_BLACK,COLOR_WHITE);
    wattron(status,COLOR_PAIR(5));
    for(int i=0;i<2;i++){
        for(int j=0;j<COLS-1;j++){
            mvwaddch(status,i,j,' ');
        }
    }
    wrefresh(code);
    wrefresh(input);
    wrefresh(output);
    wrefresh(ramwatch);
    wrefresh(status);
    wgetch(input);
    endwin();
}