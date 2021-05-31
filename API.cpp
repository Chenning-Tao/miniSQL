//
// Created by 陶辰宁 on 2021/5/2.
//

#include "API.h"

#include <utility>

bool API::createTable(const string& tableName, const Attribute& tableAttribute) {
    try {
        CM->createTable(tableName, tableAttribute);
        printf("Success!\n");
    }
    catch (const char *error) {
        cout << error << endl;
        return false;
    }
    RM->createTable(tableName, tableAttribute);
    return true;
}

void API::initialize() {

}

API::API() {
    BM = new BufferManager;
    TB = new Table(BM);
    CM = new CatalogManager(BM, TB);
    RM = new RecordManager(BM, CM, TB);
}

API::~API() {
    delete RM;
    delete CM;
}

bool API::dropTable(string tableName) {
    try {
        CM->dropTable(tableName);
        printf("Success");
    }
    catch (const char *error) {
        printf("%s\n", error);
        return false;
    }
    RM->dropTable(tableName);
    return true;
}

bool API::insert(const string& tableName, const vector<short>& type, vector<string> content) {
    try {
        TB->checkTable(tableName, type);
    }
    catch (string error) {
        printf("%s\n", error.data());
        return false;
    }
    vector<short> trueType = TB->getType(tableName);
    try {
        RM->insert(tableName, trueType, content);
        printf("Success");
    }
    catch (string error) {
        printf("%s\n", error.data());
        return false;
    }

    return true;
}

bool API::select(const vector<string> &column, string tableName) {
    // 无条件选择
    // todo: 选择某几列
    if(TB->isExist(tableName) == -1) {
        printf("Table %s doesn't exist!\n", tableName.data());
        return false;
    }
    if(column[0] != "*") {
        try {TB->checkColumn(column, tableName);}
        catch (string error) {
            printf("%s\n", error.data());
        }
    }
    int separation;
    vector<short> type =  TB->getType(tableName);
    vector<string> columnName = TB->getColumn(tableName);
    RM->printTitle(columnName, type, separation);
    RM->select(tableName, type, separation);
    printf("Success");
}
