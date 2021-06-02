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
    int createIndex(const string& SQL);
    bool dropTable(const string& SQL);
    bool dropIndex(const string& SQL);
    bool insert(const string& tableName, string tableAttribute);
    bool SelectDelete(string column, string tableName, string condition, int mode);
    bool execfile(const string &fileName);
public:
    API api;
    int interpret(string SQL);
};



#endif //MINISQL_INTERPRETER_H
