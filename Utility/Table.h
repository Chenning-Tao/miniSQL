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
private:     // 判断此数据列是否是唯一
public:
    bool index[32];     // 是否存在index
    short primary;      // 如果有，表示位置，没有设置为-1
    std::string indexName[32];
    short count;
    std::string name[32];   // 用来存储数据列名字
    short type[32];     // 存储数据类型，-1表示是int，0表示为float，1-255表示char长度
    bool unique[32];
    Attribute();
    void setInt(std::string inName, bool Unique);
    void setChar(std::string inName, short length, bool Unique);
    void setFloat(std::string inName, bool Unique);
    bool setPrimary(const std::string& inName);
    void readIn(char *&content);
    void writeOut(char *&content);
    void checkInfo(std::vector<short> inType);
    void getType(std::vector<short> &inType);
    void getColumn(std::vector<std::string> &Column);
    void getUnique(std::vector<bool> &Unique);
    void getIndex(std::vector<bool> &Index);
    void getIndexName(std::vector<std::string> &IndexName);
    void checkColumn(const std::vector<std::string>& Column);
};

class Table {
private:
    BufferManager *BM;
public:
    std::vector<Attribute> tableInfo;
    std::vector<std::string> tableName;
    std::vector<int> tableBlock;
    std::unordered_map<std::string, int> index;
    Table(BufferManager *inBM);
    void readIn(pageInfo inTable);
    int isExist(const std::string& inTableName);
    std::vector<short> getType(const std::string& inTableName);
    std::vector<std::string> getColumn(const std::string& inTableName);
    std::vector<bool> getUnique(const std::string& inTableName);
    std::vector<bool> getIndex(const std::string& inTableName);
    std::vector<std::string> getIndexName(const std::string& inTableName);
    short getPrimary(const std::string& inTableName);
    void addNew(const std::string& inTableName, const Attribute& inTableInfo, int inBlockID);
    void deleteTable(const std::string& deleteTableName);
    void checkTable(const std::string& inTableName, std::vector<short> type);
    void checkColumn(const std::vector<std::string>& column, std::string& inTableName);
    int setIndex(const std::string& inTableName, const std::string& columnName, const std::string& indexName);
    int deleteIndex(const std::string& inTableName, const std::string& indexName);
};


#endif //MINISQL_TABLE_H
