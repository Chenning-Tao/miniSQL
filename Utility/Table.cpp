//
// Created by 陶辰宁 on 2021/5/20.
//

#include "Table.h"

#include <utility>
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

void Attribute::readIn(char *&content) {
    charToOther(content, count);
    charToOther(content, primary);
    cout << "Primary key:" << primary << endl;
    for(int i = 0; i < count; ++i){
        charToOther(content, name[i], 64);
        charToOther(content, type[i]);
        charToOther(content, unique[i]);
        charToOther(content, index[i]);
        cout << name[i] << " " << type[i] << " " << unique[i] << " " << index[i] << endl;
    }
    count -= 1;
}

void Attribute::writeOut(char *&content) {
    otherToChar(short(count+1), content);
    otherToChar(primary, content);
    for(int i = 0; i <= count; ++i){
        otherToChar(name[i], content, 64);
        otherToChar(type[i], content);
        otherToChar(unique[i], content);
        otherToChar(index[i], content);
    }
}

void Attribute::checkInfo(std::vector<short> inType) {
    if (inType.size() != count + 1) throw string("Size doesn't match!");
    for(int i = 0; i < inType.size(); ++i){
        if(inType[i] == -1 && type[i] == 0) continue;
        if(inType[i] != type[i]) throw string("Column " + to_string(i+1) + " doesn't match!");
    }
}

void Attribute::getType(vector<short> &inType) {
    for(int i = 0; i <= count; ++i){
        inType.push_back(type[i]);
    }
}

void Attribute::checkColumn(std::vector<std::string> Column) {
    // todo: 完成
    bool flag = false;
    for(auto &i : Column){
        flag = false;
        for(int j = 0; j <= count; ++j){
            if(name[j] == i){
                flag = true;

            }
        }
    }
}

void Attribute::getColumn(vector<std::string> &Column) {
    for(int i = 0; i <= count; ++i){
        Column.push_back(name[i]);
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
    cout << "Table Name: " << inTableName << endl;
    tableName.push_back(inTableName);
    index.emplace(inTableName, tableName.size()-1);
    // 读入数据表信息
    Attribute inTableInfo;
    inTableInfo.readIn(cur);
    tableInfo.push_back(inTableInfo);
    tableBlock.push_back(inTable.blockID);
}

int Table::isExist(const std::string& inTableName){
    // 如果没找到返回-1 找到返回是在第几个block
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

void Table::checkTable(const std::string& inTableName, std::vector<short> type) {
    auto tableFind = index.find(inTableName);
    if(tableFind == index.end()) throw string("Table " + inTableName + " doesn't exist!");
    try{
        tableInfo[tableFind->second].checkInfo(std::move(type));
    }
    catch (string error) {
        throw error;
    }
}

void Table::checkColumn(const vector<std::string> &column, string &inTableName) {
    auto tableFind = index.find(inTableName);
    if(tableFind == index.end()) throw string("Table " + inTableName + " doesn't exist!");
    try{
        tableInfo[tableFind->second].checkColumn(column);
    }
    catch (string error){
        throw error;
    }
}

vector<short> Table::getType(const string &inTableName) {
    auto tableFind = index.find(inTableName);
    if(tableFind == index.end()) throw string("Table " + inTableName + " doesn't exist!");
    vector<short> result;
    tableInfo[tableFind->second].getType(result);
    return result;
}

vector<string> Table::getColumn(const string &inTableName) {
    auto tableFind = index.find(inTableName);
    if(tableFind == index.end()) throw string("Table " + inTableName + " doesn't exist!");
    vector<string> result;
    tableInfo[tableFind->second].getColumn(result);
    return result;
}
