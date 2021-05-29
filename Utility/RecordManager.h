//
// Created by 陶辰宁 on 2021/5/29.
//

#ifndef MINISQL_RECORDMANAGER_H
#define MINISQL_RECORDMANAGER_H

#include "../CommonHead.h"
#include "BufferManager.h"
#include "CatalogManager.h"
#define INFO_BLOCK_NUM 2
using namespace std;

class RecordManager {
private:
    BufferManager *BM;
    CatalogManager *CM;
    Table *TB;
    void initialHead(string tableName, vector<int> &pageRecord);
    void addNewPage(const string& tableName, int pageNum);
public:
    RecordManager(BufferManager *inBM, CatalogManager *inCM, Table *inTB);
    bool insert(const string& tableName, vector<short> type, vector<string> content);
    void createTable(string tableName);
    void dropTable(const string& tableName);
};


#endif //MINISQL_RECORDMANAGER_H
