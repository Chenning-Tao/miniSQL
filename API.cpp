//
// Created by 陶辰宁 on 2021/5/2.
//

#include "API.h"

#include <utility>

bool API::createTable(string tableName, Attribute tableAttribute) {
    try {
        CM->createTable(std::move(tableName), std::move(tableAttribute));
    }
    catch (const char *error) {
        cout << error << endl;
        return false;
    }

}

void API::initialize() {

}

API::API() {
    BM = new BufferManager;
    CM = new CatalogManager(BM);
}

API::~API() {
    delete CM;
}
