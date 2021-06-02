//
// Created by 陶辰宁 on 2021/5/29.
//

#ifndef MINISQL_RECORDMANAGER_H
#define MINISQL_RECORDMANAGER_H

#include "../CommonHead.h"
#include "BufferManager.h"
#include "CatalogManager.h"
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
    void initialHead(const string& tableName, vector<int> &pageRecord, int &recordSize, int &recordPerPage);
    inline bool condition(const conditionPair& CD, const string& value){
        if(CD.type > 0){
            switch (CD.OP) {
                case Greater: return value > CD.condition;
                case Less: return value < CD.condition;
                case Equal: return value == CD.condition;
                case NotEqual: return value != CD.condition;
                case GreaterEqual: return value >= CD.condition;
                case LessEqual: return value <= CD.condition;
            }
        }else if(CD.type == -1){
            int a = stoi(value);
            int b = stoi(CD.condition);
            switch (CD.OP) {
                case Greater: return a > b;
                case Less: return a < b;
                case Equal: return a == b;
                case NotEqual: return a != b;
                case GreaterEqual: return a >= b;
                case LessEqual: return a <= b;
            }
        }
        else if(CD.type == 0){
            float a = stof(value);
            float b = stof(CD.condition);
            switch (CD.OP) {
                case Greater: return a > b;
                case Less: return a < b;
                case Equal: return a == b;
                case NotEqual: return a != b;
                case GreaterEqual: return a >= b;
                case LessEqual: return a <= b;
            }
        }
    };
public:
    RecordManager(BufferManager *inBM);
    void insert(const string& tableName, const vector<short> &type, vector<string>& content, const vector<bool>& unique);
    void Delete(const string& tableName, const vector<short> &type, const vector<conditionPair> &CD);
    void createTable(const string& tableName, Attribute tableInfo);
    void dropTable(const string& tableName);
    int select(const string& tableName, const vector<short>& type, tabulate::Table &output, const vector<conditionPair> &CD);
    void print(const vector<string> &result);
    static void print(int count);
    void printTitle(const vector<string> &column, const vector<short> &type, int &separation);
};


#endif //MINISQL_RECORDMANAGER_H
