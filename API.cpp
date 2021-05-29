//
// Created by 陶辰宁 on 2021/5/2.
//

#include "API.h"

#include <utility>

bool API::createTable(const string& tableName, Attribute tableAttribute) {
    try {
        CM->createTable(tableName, std::move(tableAttribute));
        printf("Success!\n");
    }
    catch (const char *error) {
        cout << error << endl;
        return false;
    }
    RM->createTable(tableName);
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
        printf("Success!\n");
    }
    catch (const char *error) {
        printf("%s\n", error);
        return false;
    }
    RM->dropTable(tableName);
    return true;
}

bool API::insert(string tableName, vector<short> type, vector<string> content) {
    try {
        RM->insert(std::move(tableName), std::move(type), std::move(content));
        printf("Success!\n");
    }
    catch (string error) {
        printf("%s\n", error.data());
        return false;
    }
    return true;
}
