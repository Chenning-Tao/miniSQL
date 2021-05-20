//
// Created by 陶辰宁 on 2021/5/2.
//

#include "API.h"

#include <utility>

bool API::createTable(string tableName, Attribute tableAttribute) {
    CM.createTable(std::move(tableName), std::move(tableAttribute));
}

void API::initialize() {

}

API::API() {
}
