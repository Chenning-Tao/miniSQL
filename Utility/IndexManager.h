//
// Created by 陶辰宁 on 2021/6/2.
//

#ifndef MINISQL_INDEXMANAGER_H
#define MINISQL_INDEXMANAGER_H
#include "BpTree.h"
#include "BufferManager.h"
#include "../CommonHead.h"
#include "Table.h"
using namespace std;

class IndexManager {
public:
    IndexManager(BufferManager *inBM, Table *inTB);
    void insertKey(string tableName, int column, string key);
    bool findKey(string tableName, int column, string key);
    void createIndex(string tableName, int column, short type);
    void dropIndex(string tableName, int column);
    ~IndexManager();
private:
    unordered_map<string, indexNode> index;

    BufferManager *BM;
    Table *TB;
};


#endif //MINISQL_INDEXMANAGER_H
