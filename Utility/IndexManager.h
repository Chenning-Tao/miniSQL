

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
    void insertKey(const string& tableName, int column, string key);
    bool findKey(const string& tableName, int column, string key);
    void deleteKey(const string& tableName, int column, string key);
    void deleteAll(const string& tableName, int column);
    void createIndex(const string& tableName, int column, short type);
    void dropIndex(const string& tableName, int column);
    ~IndexManager();
private:
    unordered_map<string, BpNode> index;
    BufferManager *BM;
    Table *TB;
};


#endif //MINISQL_INDEXMANAGER_H
