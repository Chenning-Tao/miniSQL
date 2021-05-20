//
// Created by 陶辰宁 on 2021/5/2.
//

#ifndef MINISQL_API_H
#define MINISQL_API_H
#include "CommonHead.h"
using namespace std;

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
    void setInt(string inName, bool Unique);
    void setChar(string inName, short length, bool Unique);
    void setFloat(string inName, bool Unique);
    bool setPrimary(const string& inName);
};

class API {
private:
public:
    bool createTable(string tableName, Attribute tableAttribute);
    void initialize();
};



#endif //MINISQL_API_H
