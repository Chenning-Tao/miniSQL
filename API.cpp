//
// Created by 陶辰宁 on 2021/5/2.
//

#include "API.h"

#include <utility>

bool API::createTable(string tableName, Attribute tableAttribute) {
    try {
        CM->createTable(std::move(tableName), std::move(tableAttribute));
        printf("Success!\n");
    }
    catch (const char *error) {
        cout << error << endl;
        return false;
    }
    return true;
}

void API::initialize() {

}

API::API() {
    BM = new BufferManager;
    CM = new CatalogManager(BM);
    RM = new RecordManager(BM);
}

API::~API() {
    delete CM;
    delete RM;
}

bool API::dropTable(string tableName) {
    try {
        CM->dropTable(std::move(tableName));
        printf("Success!\n");
    }
    catch (const char *error) {
        printf("%s\n", error);
        return false;
    }
    return true;
}

bool API::insert(string tableName, vector<short> type, vector<string> content) {
    try {
        RM->insert(std::move(tableName), std::move(type), std::move(content));
        printf("Success!\n");
    }
    catch (const char *error) {
        printf("%s\n", error);
        return false;
    }
    return true;
}
