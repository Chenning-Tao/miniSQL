//
// Created by 陶辰宁 on 2021/5/20.
//

#ifndef MINISQL_TABLE_H
#define MINISQL_TABLE_H
#include "../CommonHead.h"

// 用来存储table信息
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
};


class Table {

};


#endif //MINISQL_TABLE_H
