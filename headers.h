//
//  headers.h
//  natsubf
//
//  Created by 夏欣萌 on 2020/4/3.
//  Copyright © 2020 夏欣萌. All rights reserved.
//

#ifndef headers_h
#define headers_h
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <curses.h>
#include <pthread.h>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <io.h>
#define isatty _isatty
#define STDIN_FILENO 0
#include <readline/readline.h>
#include <readline/history.h>
#else
#include <unistd.h>
#include <editline/readline.h>
#endif
using namespace std;
#endif /* headers_h */
