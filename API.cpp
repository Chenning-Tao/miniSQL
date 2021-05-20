//
// Created by 陶辰宁 on 2021/5/2.
//

#include "API.h"

#include <utility>

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

bool API::createTable(string tableName, Attribute tableAttribute) {
    return false;
}

void API::initialize() {

}
