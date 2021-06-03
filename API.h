//
// Created by 陶辰宁 on 2021/5/2.
//

#ifndef MINISQL_API_H
#define MINISQL_API_H
#include "CommonHead.h"
#include "Utility/CatalogManager.h"
#include "Utility/RecordManager.h"
#include "Utility/IndexManager.h"
#include "Utility/Table.h"
using namespace std;

class API {
private:
    BufferManager *BM;
    CatalogManager *CM;
    RecordManager *RM;
    Table *TB;
    IndexManager *IM;
public:
    API();
    ~API();
    bool createTable(const string& tableName, const Attribute& tableAttribute);
    void initialize();
    bool dropTable(const string& tableName);
    bool insert(const string& tableName, const vector<short>& type, vector<string> content);
    bool SelectDelete(const vector<string>& column, string tableName, vector<conditionPair> &CD, int mode);
    bool createIndex(const string& indexName, const string& tableName, const string& columnName);
    bool dropIndex(const string& indexName, const string& tableName);
};

#endif //MINISQL_API_H
