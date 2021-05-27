//
// Created by 陶辰宁 on 2021/5/19.
//

#ifndef MINISQL_CATALOGMANAGER_H
#define MINISQL_CATALOGMANAGER_H
#include "../CommonHead.h"
#include "BufferManager.h"
#include "Table.h"
using namespace std;

class CatalogManager {
public:
    CatalogManager(BufferManager *inBM);
    ~CatalogManager();
    void createTable(string tableName, Attribute tableAttribute);
private:
    BufferManager *BM;
    Table *TB;
    vector<int> tablePointer;
    queue<int> freePointer;
    int last;
};


#endif //MINISQL_CATALOGMANAGER_H
