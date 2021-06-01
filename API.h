//
// Created by 陶辰宁 on 2021/5/2.
//

#ifndef MINISQL_API_H
#define MINISQL_API_H
#include "CommonHead.h"
#include "Utility/CatalogManager.h"
#include "Utility/RecordManager.h"
#include "Utility/Table.h"
using namespace std;

class API {
private:
    BufferManager *BM;
    CatalogManager *CM;
    RecordManager *RM;
    Table *TB;
public:
    API();
    ~API();
    bool createTable(const string& tableName, const Attribute& tableAttribute);
    void initialize();
    bool dropTable(string tableName);
    bool insert(const string& tableName, const vector<short>& type, vector<string> content);
    bool select(const vector<string>& column, string tableName, vector<conditionPair> &CD);
};

#endif //MINISQL_API_H
