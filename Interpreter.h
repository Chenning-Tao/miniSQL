//
// Created by 陶辰宁 on 2021/5/2.
//

#ifndef MINISQL_INTERPRETER_H
#define MINISQL_INTERPRETER_H
#include "CommonHead.h"
#include "API.h"
using namespace std;

class Interpreter {
private:
    int createTable(string SQL);
    int createIndex(string SQL);
public:
    API *api;
    int interpret(string SQL);
};



#endif //MINISQL_INTERPRETER_H
