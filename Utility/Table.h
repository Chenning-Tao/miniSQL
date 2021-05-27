//
// Created by 陶辰宁 on 2021/5/20.
//

#ifndef MINISQL_TABLE_H
#define MINISQL_TABLE_H
#include "../CommonHead.h"
#include "BufferManager.h"

// 用来存储table信息
// 存储一个table所需的最大空间：2 + 64*32 + 32*2 + 2 + 32 + 32
// 每列名字最长64字节
class Attribute
{
private:
    short count;
    std::string name[32];   // 用来存储数据列名字
    short type[32];     // 存储数据类型，-1表示是int，0表示为float，1-255表示char长度
    short primary;      // 如果有，表示位置，没有设置为-1
    bool unique[32];       // 判断此数据列是否是唯一
    bool index[32];     // 是否存在index
public:
    Attribute();
    void setInt(std::string inName, bool Unique);
    void setChar(std::string inName, short length, bool Unique);
    void setFloat(std::string inName, bool Unique);
    bool setPrimary(const std::string& inName);
    short attributeNum() const;
    void readIn(char *content);
    void writeOut(char *&content);
};

class Table {
private:
    std::vector<Attribute> tableInfo;
    std::vector<std::string> tableName;
    std::vector<int> tableBlock;
    std::unordered_map<std::string, int> index;
    BufferManager *BM;
public:
    Table(BufferManager *inBM);
    void readIn(pageInfo inTable);
    int isExist(const std::string& inTableName);
    void addNew(const std::string& inTableName, const Attribute& inTableInfo, int inBlockID);
    void deleteTable(std::string deleteTableName);
};


#endif //MINISQL_TABLE_H
