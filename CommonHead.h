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
#include <set>
#include <unordered_set>
#include <map>
#include "./Utility/tabulate.hpp"
#define SELECT 1
#define DELETE 2

typedef struct pageInfo{
    char *content;
    int blockID;
}pageInfo;

typedef struct RecordLocation{
    int page;
    int offset;
}RecordLocation;

enum operatorSet {Greater, Less, LessEqual, GreaterEqual, Equal, NotEqual};

inline void addRowInit(tabulate::Table &output, const int& column){
    switch (column) {
        case 1: output.add_row({""}); break;
        case 2: output.add_row({"",""}); break;
        case 3: output.add_row({"","",""}); break;
        case 4: output.add_row({"","","",""}); break;
        case 5: output.add_row({"","","","",""}); break;
        case 6: output.add_row({"","","","","",""}); break;
        case 7: output.add_row({"","","","","","",""}); break;
        case 8: output.add_row({"","","","","","","",""}); break;
        case 9: output.add_row({"","","","","","","","",""}); break;
        case 10: output.add_row({"","","","","","","","","",""}); break;
        case 11: output.add_row({"","","","","","","","","","",""}); break;
        case 12: output.add_row({"","","","","","","","","","","",""}); break;
        case 13: output.add_row({"","","","","","","","","","","","",""}); break;
        case 14: output.add_row({"","","","","","","","","","","","","",""}); break;
        case 15: output.add_row({"","","","","","","","","","","","","","",""}); break;
        case 16: output.add_row({"","","","","","","","","","","","","","","",""}); break;
        case 17: output.add_row({"","","","","","","","","","","","","","","","",""}); break;
        case 18: output.add_row({"","","","","","","","","","","","","","","","","",""}); break;
        case 19: output.add_row({"","","","","","","","","","","","","","","","","","",""}); break;
        case 20: output.add_row({"","","","","","","","","","","","","","","","","","","",""}); break;
        case 21: output.add_row({"","","","","","","","","","","","","","","","","","","","",""}); break;
        case 22: output.add_row({"","","","","","","","","","","","","","","","","","","","","",""}); break;
        case 23: output.add_row({"","","","","","","","","","","","","","","","","","","","","","",""}); break;
        case 24: output.add_row({"","","","","","","","","","","","","","","","","","","","","","","",""}); break;
        case 25: output.add_row({"","","","","","","","","","","","","","","","","","","","","","","","",""}); break;
        case 26: output.add_row({"","","","","","","","","","","","","","","","","","","","","","","","","",""}); break;
        case 27: output.add_row({"","","","","","","","","","","","","","","","","","","","","","","","","","",""}); break;
        case 28: output.add_row({"","","","","","","","","","","","","","","","","","","","","","","","","","","",""}); break;
        case 29: output.add_row({"","","","","","","","","","","","","","","","","","","","","","","","","","","","",""}); break;
        case 30: output.add_row({"","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""}); break;
        case 31: output.add_row({"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""}); break;
        case 32: output.add_row({"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",""}); break;
    }
};

typedef struct conditionPair{
    std::string columnName;
    operatorSet OP;
    std::string condition;
    short type;
    int order; // 用来判断在第几个位置
}conditionPair;

#endif //MINISQL_COMMONHEAD_H
