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
vector<char> ram;
int Progsz;
vector<int> ddid;
vector<pair<int,pair<int,int>>> ddpos;
int dds;
int Curpos,Ramptr;
WINDOW *title,*code,*input,*output,*ramwatch,*status;
vector<char> inputs,outputs;
int inppos;
bool isRunning=0;
char Hexcode[]="0123456789abcdef";
}

bool CheckParenthesis(const vector<char> &,bool);

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
    for(int i=0;i<5;i++){
        for(int j=0;j<Ramcols;j++){
            mvwaddch(ramwatch,i,j,' ');
        }
    }
    vector<char> ln1,ln2,ln3;
    int Ls=0;
    int DisplayArea=(Ramcols+20)/12;
    for(int i=pos-DisplayArea;i<=pos+DisplayArea;i++){
        if(i==pos){
            Ls=(int)ln1.size();
        }
        if(i<0||i>29999){
            ln1.push_back(' ');ln1.push_back(' ');ln1.push_back(' ');
            ln1.push_back(' ');ln1.push_back(' ');ln1.push_back(' ');
            ln2.push_back(' ');ln2.push_back(' ');ln2.push_back(' ');
            ln2.push_back(' ');ln2.push_back(' ');ln2.push_back(' ');
            ln3.push_back(' ');ln3.push_back(' ');ln3.push_back(' ');
            ln3.push_back(' ');ln3.push_back(' ');ln3.push_back(' ');
        }else{
            vector<char> temp;
            int Temp=i;
            while(Temp){
                temp.push_back(Temp%10+'0');
                Temp/=10;
            }
            if(!temp.size()){
                temp.push_back('0');
            }
            reverse(temp.begin(),temp.end());
            for(int j=0;j<6;j++){
                if(j<(int)temp.size()){
                    ln1.push_back(temp[j]);
                }else{
                    ln1.push_back(' ');
                }
            }
            int R=ram[i];
            if(R<0){
                R+=256;
            }
            if(R>=32&&R<=126){
                ln2.push_back(ram[i]);ln2.push_back(' ');ln2.push_back(' ');
                ln2.push_back(' ');ln2.push_back(' ');ln2.push_back(' ');
            }else{
                bool isSpecial=0;
                char SpecialCode;
                if(R==7){
                    isSpecial=1;
                    SpecialCode='a';
                }else if(R==8){
                    isSpecial=1;
                    SpecialCode='b';
                }else if(R==9){
                    isSpecial=1;
                    SpecialCode='t';
                }else if(R==10){
                    isSpecial=1;
                    SpecialCode='n';
                }else if(R==11){
                    isSpecial=1;
                    SpecialCode='v';
                }else if(R==12){
                    isSpecial=1;
                    SpecialCode='f';
                }else if(R==13){
                    isSpecial=1;
                    SpecialCode='r';
                }else if(R==0){
                    isSpecial=1;
                    SpecialCode='0';
                }
                if(isSpecial){
                    ln2.push_back('\\');ln2.push_back(SpecialCode);ln2.push_back(' ');
                    ln2.push_back(' ');ln2.push_back(' ');ln2.push_back(' ');
                }else{
                    ln2.push_back('\\');ln2.push_back('x');ln2.push_back(Hexcode[R/16]);
                    ln2.push_back(Hexcode[R%16]);ln2.push_back(' ');ln2.push_back(' ');
                }
            }
            temp.clear();
            Temp=R;
            while(Temp){
                temp.push_back(Temp%10+'0');
                Temp/=10;
            }
            if(!temp.size()){
                temp.push_back('0');
            }
            reverse(temp.begin(),temp.end());
            for(int j=0;j<6;j++){
                if(j<temp.size()){
                    ln3.push_back(temp[j]);
                }else{
                    ln3.push_back(' ');
                }
            }
        }
    }
    int curMidPoint=Ramcols/2-3;
    for(int i=0;i<curMidPoint;i++){
        mvwaddch(ramwatch,1,i,ln1[Ls-curMidPoint+i]);
    }
    wattron(ramwatch,COLOR_PAIR(9));
    for(int i=0;i<5;i++){
        mvwaddch(ramwatch,1,i+curMidPoint,ln1[Ls+i]);
    }
    wattron(ramwatch,COLOR_PAIR(4));
    for(int i=curMidPoint+5;i<Ramcols;i++){
        mvwaddch(ramwatch,1,i,ln1[Ls-curMidPoint+i]);
    }
    for(int i=0;i<curMidPoint;i++){
        mvwaddch(ramwatch,2,i,ln2[Ls-curMidPoint+i]);
    }
    wattron(ramwatch,COLOR_PAIR(9));
    for(int i=0;i<5;i++){
        mvwaddch(ramwatch,2,i+curMidPoint,ln2[Ls+i]);
    }
    wattron(ramwatch,COLOR_PAIR(4));
    for(int i=curMidPoint+5;i<Ramcols;i++){
        mvwaddch(ramwatch,2,i,ln2[Ls-curMidPoint+i]);
    }
    for(int i=0;i<curMidPoint;i++){
        mvwaddch(ramwatch,3,i,ln3[Ls-curMidPoint+i]);
    }
    wattron(ramwatch,COLOR_PAIR(9));
    for(int i=0;i<5;i++){
        mvwaddch(ramwatch,3,i+curMidPoint,ln3[Ls+i]);
    }
    wattron(ramwatch,COLOR_PAIR(4));
    for(int i=curMidPoint+5;i<Ramcols;i++){
        mvwaddch(ramwatch,3,i,ln3[Ls-curMidPoint+i]);
    }
}

void PrintStatus(const char a[],const char b[]){
    wattron(status,COLOR_PAIR(5));
    int Statcols=COLS-1;
    int cur=0;
    for(;a[cur];cur++){
        mvwaddch(status,0,cur,a[cur]);
    }
    for(;cur<Statcols;cur++){
        mvwaddch(status,0,cur,' ');
    }
    cur=0;
    for(;b[cur];cur++){
        mvwaddch(status,1,cur,b[cur]);
    }
    for(;cur<Statcols;cur++){
        mvwaddch(status,1,cur,' ');
    }
}

void PrintOutput(){
    wattron(output,COLOR_PAIR(3));
    for(int i=0;i<LINES-15;i++){
        for(int j=0;j<COLS-COLS/2-2;j++){
            mvwaddch(output,i,j,' ');
        }
    }
    int Outlines=LINES-17,Outcols=COLS-COLS/2-6;
    int Szout=(int)outputs.size();
    int ln=0,col=0;
    for(int i=0;i<Szout;i++){
        if(outputs[i]=='\n'){
            ln++;
            col=0;
        }else{
            col++;
            if(col==Outcols){
                col=0;
                ln++;
            }
        }
    }
    if(ln<Outlines){
        ln=0;
        col=0;
        for(int i=0;i<Szout;i++){
            if(outputs[i]=='\n'){
                ln++;
                col=0;
            }else{
                mvwaddch(output,ln+1,col+2,outputs[i]);
                col++;
                if(col==Outcols){
                    col=0;
                    ln++;
                }
            }
        }
    }else{
        int curl=ln;
        ln=0;
        col=0;
        for(int i=0;i<Szout;i++){
            if(outputs[i]=='\n'){
                ln++;
                col=0;
            }else{
                if(ln>=curl-Outlines+1){
                    mvwaddch(output,ln-curl+Outlines,col+2,outputs[i]);
                }
                col++;
                if(col==Outcols){
                    col=0;
                    ln++;
                }
            }
        }
    }
}

void PrintInputatPos(int pos,bool cur=0){
    wattron(input,COLOR_PAIR(2));
    for(int i=0;i<LINES-15;i++){
        for(int j=0;j<COLS/2-1;j++){
            mvwaddch(input,i,j,' ');
        }
    }
    int Inlines=LINES-17,Incols=COLS/2-5;
    int Szin=(int)inputs.size();
    int ln=0,col=0,fln=0,flh=Inlines/2;
    for(int i=0;i<Szin;i++){
        if(pos==i){
            fln=ln;
        }
        if(inputs[i]=='\n'){
            ln++;
            col=0;
        }else{
            col++;
            if(col==Incols){
                col=0;
                ln++;
            }
        }
    }
    if(pos==Szin){
        fln=ln;
    }
    if(ln<Inlines){
        wattron(input,COLOR_PAIR(7));
        ln=0;
        col=0;
        for(int i=0;i<Szin;i++){
            if(pos>=inppos){
                wattron(input,COLOR_PAIR(2));
            }
            if(inputs[i]=='\n'){
                ln++;
                col=0;
            }else{
                mvwaddch(input,ln+1,col+2,inputs[i]);
                col++;
                if(col==Incols){
                    col=0;
                    ln++;
                }
            }
        }
        if(cur){
            wattron(input,COLOR_PAIR(3));
            mvwaddch(input,ln+1,col+2,' ');
            wmove(input,ln+1,col+2);
        }
    }else if(Inlines-flh>ln-fln){
        int Startln=ln-Inlines+1;
        wattron(input,COLOR_PAIR(7));
        ln=0;
        col=0;
        for(int i=0;i<Szin;i++){
            if(pos>=inppos){
                wattron(input,COLOR_PAIR(2));
            }
            if(inputs[i]=='\n'){
                ln++;
                col=0;
            }else{
                if(ln>=Startln){
                    mvwaddch(input,ln-Startln+1,col+2,inputs[i]);
                }
                col++;
                if(col==Incols){
                    col=0;
                    ln++;
                }
            }
        }
        if(cur){
            wattron(input,COLOR_PAIR(3));
            mvwaddch(input,Inlines,col+2,' ');
            wmove(input,Inlines,col+2);
        }
    }else{
        int Startln=fln-flh,Endln=Inlines-flh+fln-1;
        wattron(input,COLOR_PAIR(7));
        ln=0;
        col=0;
        for(int i=0;i<Szin;i++){
            if(pos>=inppos){
                wattron(input,COLOR_PAIR(2));
            }
            if(inputs[i]=='\n'){
                ln++;
                col=0;
            }else{
                if(ln>=Startln&&ln<=Endln){
                    mvwaddch(input,ln-Startln+1,col+2,inputs[i]);
                }
                col++;
                if(col==Incols){
                    col=0;
                    ln++;
                }
            }
        }
        if(cur){
            wattron(input,COLOR_PAIR(3));
            mvwaddch(input,Inlines,col+2,' ');
            wmove(input,Inlines,col+2);
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
    ram.assign(30000,0);
    CheckParenthesis(Full,0);
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
    init_pair(7,COLOR_CYAN,COLOR_BLACK);
    init_pair(9,COLOR_YELLOW,COLOR_BLACK);
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
    PrintStatus("Status: Not started","[S: Next Step][B: Run to breakpoint][Tab: Input][L: Reload][R: Reset][Q: Quit]");
    wrefresh(title);
    wrefresh(code);
    wrefresh(input);
    wrefresh(output);
    wrefresh(ramwatch);
    wrefresh(status);
    char ch;
    bool isInputing=0;
    while((ch=wgetch(title))){
        if(isInputing){
            if(ch=='\t'){
                isInputing=0;
                PrintStatus("Status: Finished inputting","[S: Next Step][B: Run to breakpoint][Tab: Input][L: Reload][R: Reset][Q: Quit]");
                wrefresh(status);
            }else if(ch!=8&&ch!=127){
                if(ch==13){
                    ch=10;
                }
                inputs.push_back(ch);
            }else{
                if((int)inputs.size()!=inppos){
                    inputs.pop_back();
                }
            }
            PrintInputatPos(isInputing?(int)inputs.size():inppos,isInputing);
            wrefresh(input);
        }else{
            if(!isRunning){
                if(ch=='l'){
                    auto _prog=Program;
                    auto _dd=ddid;
                    auto _ddp=ddpos;
                    auto _full=Full;
                    Program.clear();
                    ddid.clear();
                    ddpos.clear();
                    Full.clear();
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
                    if(!CheckParenthesis(Full,0)){
                        Program=_prog;
                        ddid=_dd;
                        ddpos=_ddp;
                        Full=_full;
                        PrintStatus("Status: Reload failed! Unmatched parenthesis found!","[S: Next Step][B: Run to breakpoint][Tab: Input][L: Reload][R: Reset][Q: Quit]");
                        wrefresh(status);
                    }else{
                        Progsz=(int)Program.size();
                        ram.assign(30000,0);
                        inppos=0;
                        PrintInputatPos(inppos);
                        Curpos=0;
                        PrintCodeatPos(Curpos);
                        Ramptr=0;
                        PrintRamatPos(Ramptr);
                        PrintStatus("Status: Reloaded","[S: Next Step][B: Run to breakpoint][Tab: Input][L: Reload][R: Reset][Q: Quit]");
                        outputs.clear();
                        PrintOutput();
                        wrefresh(code);
                        wrefresh(input);
                        wrefresh(output);
                        wrefresh(ramwatch);
                        wrefresh(status);
                    }
                }else if(ch=='r'){
                    ram.assign(30000,0);
                    inppos=0;
                    PrintInputatPos(inppos);
                    Curpos=0;
                    PrintCodeatPos(Curpos);
                    Ramptr=0;
                    PrintRamatPos(Ramptr);
                    PrintStatus("Status: Reset","[S: Next Step][B: Run to breakpoint][Tab: Input][L: Reload][R: Reset][Q: Quit]");
                    outputs.clear();
                    PrintOutput();
                    wrefresh(code);
                    wrefresh(input);
                    wrefresh(output);
                    wrefresh(ramwatch);
                    wrefresh(status);
                }else if(ch=='\t'){
                    isInputing=1;
                    PrintInputatPos((int)inputs.size(),1);
                    PrintStatus("Status: Inputting","[Tab: Finish inputting]");
                    wrefresh(input);
                    wrefresh(status);
                }else if(ch=='q'){
                    break;
                }
            }
        }
    }
    endwin();
}
