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
/*
 * 需要两个文件用来存储内容
 * NAME_INFO
 * 第0块存有多少页
 * 第1块开始后面开始存每一页有多少内容
 * NAME
 * 每一页开头第一个是空闲指针，后面存内容
 */

class RecordManager {
private:
    BufferManager *BM;
    CatalogManager *CM;
    Table *TB;
    void initialHead(const string& tableName, vector<int> &pageRecord, int &recordSize, int &recordPerPage);
public:
    RecordManager(BufferManager *inBM, CatalogManager *inCM, Table *inTB);
    void insert(const string& tableName, vector<short> type, vector<string> content);
    void createTable(const string& tableName, Attribute tableInfo);
    void dropTable(const string& tableName);
    void select(const string& tableName, const vector<short>& type, int separation);
    void print(const vector<string> &result);
    static void print(int count);
    void printTitle(const vector<string> &column, const vector<short> &type, int &separation);
};


#endif //MINISQL_RECORDMANAGER_H
