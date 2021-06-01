//
// Created by 陶辰宁 on 2021/5/2.
//

#ifndef MINISQL_COMMONHEAD_H
#define MINISQL_COMMONHEAD_H
#define DatabasePath "./database/"
#include <iostream>
#include <string>
#include <algorithm>
#include "regex"
#include <cstdio>
#include <unordered_map>
#include <vector>
#include <queue>
#include <utility>
#include <ctime>
#include <fstream>
#include "./Utility/tabulate.hpp"

typedef struct pageInfo{
    char *content;
    int blockID;
}pageInfo;

enum operatorSet {Greater, Less, LessEqual, GreaterEqual, Equal, NotEqual};

inline void addRowInit(tabulate::Table &output, const int& column){
    switch (column) {
        case 1: output.add_row({""}); break;
        case 2: output.add_row({"",""}); break;
    }
};

#endif //MINISQL_COMMONHEAD_H
