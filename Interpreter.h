//
// Created by 陶辰宁 on 2021/5/2.
//

#ifndef MINISQL_INTERPRETER_H
#define MINISQL_INTERPRETER_H
#include "CommonHead.h"
#include "API.h"
#include "Utility/Table.h"
using namespace std;

class Interpreter {
private:
    int createTable(string SQL);
    int createIndex(string SQL);
    bool dropTable(string SQL);
    bool dropIndex(string SQL);
    bool insert(const string& tableName, string tableAttribute);
    bool select(string column, string tableName, string condition);
    bool execfile(const string &fileName);
public:
    API api;
    int interpret(string SQL);
};



#endif //MINISQL_INTERPRETER_H
