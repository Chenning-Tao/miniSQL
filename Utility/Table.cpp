//
// Created by 陶辰宁 on 2021/5/20.
//

#include "Table.h"
using namespace std;

void Attribute::setInt(string inName, bool Unique) {
    this->count++;
    int temp = this->count;
    this->name[temp] = std::move(inName);
    this->unique[temp] = Unique;
    this->type[temp] = -1;
    this->index[temp] = false;
}

void Attribute::setChar(string inName, short length, bool Unique) {
    this->count++;
    int temp = this->count;
    this->name[temp] = std::move(inName);
    this->unique[temp] = Unique;
    this->type[temp] = length;
    this->index[temp] = false;
}

Attribute::Attribute() {
    this->count = -1;
    this->primary = -1;
}

void Attribute::setFloat(string inName, bool Unique) {
    this->count++;
    int temp = this->count;
    this->name[temp] = std::move(inName);
    this->unique[temp] = Unique;
    this->type[temp] = 0;
    this->index[temp] = false;
}

bool Attribute::setPrimary(const string& inName) {
    if(this->primary != -1){
        cout << "This table already have primary key!" << endl;
        return false;
    }
    short temp = this->count;
    bool find = false;
    for(short i = 0; i < temp; ++i){
        if(inName == this->name[i]){
            this->primary = i;
            find = true;
        }
    }
    if(!find) cout << "Cannot find this key in this table!" << endl;
    return find;
}

short Attribute::attributeNum() const {
    return short(count+1);
}

void Attribute::readIn(char *content) {
    charToOther(content, count);
    charToOther(content, primary);
    for(int i = 0; i < count; ++i){
        charToOther(content, name[i], 64);
        charToOther(content, type[i]);
        charToOther(content, unique[i]);
        charToOther(content, index[i]);
    }
    count -= 1;
}

void Attribute::writeOut(char *&content) {
    otherToChar(count+1, content);
    otherToChar(primary, content);
    for(int i = 0; i <= count; ++i){
        otherToChar(name[i], content, 64);
        otherToChar(type[i], content);
        otherToChar(unique[i], content);
        otherToChar(index[i], content);
    }
}

Table::Table(BufferManager *inBM) {
    this->BM = inBM;
}

void Table::readIn(pageInfo inTable) {
    /*
     * table的存储方式:
     * tableName 256位
     * count(告诉有多少个) primary跟在后面(获得位置)
     * 64位存name，4位存type，2位存unique和index
     */
    char *cur = inTable.content;
    // 读入数据表名字
    string inTableName;
    charToOther(cur, inTableName, 256);
    tableName.push_back(inTableName);
    index.emplace(inTableName, tableName.size()-1);
    // 读入数据表信息
    Attribute inTableInfo;
    inTableInfo.readIn(cur);
    tableInfo.push_back(inTableInfo);
    tableBlock.push_back(inTable.blockID);
}

int Table::isExist(const std::string& inTableName){
    auto tableFind = index.find(inTableName);
    if(tableFind == index.end()) return -1;
    else return tableBlock[tableFind->second];
}

void Table::addNew(const std::string& inTableName, const Attribute& inTableInfo, int inBlockID) {
    tableName.push_back(inTableName);
    tableInfo.push_back(inTableInfo);
    tableBlock.push_back(inBlockID);
    index.emplace(inTableName, tableName.size() - 1);
}

void Table::deleteTable(std::string deleteTableName) {
    index.erase(deleteTableName);
}
