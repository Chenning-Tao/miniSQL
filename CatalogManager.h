//
// Created by 陶辰宁 on 2021/5/19.
//

#ifndef MINISQL_CATALOGMANAGER_H
#define MINISQL_CATALOGMANAGER_H
#include "API.h"


class CatalogManager {
public:
    bool createTable(string tableName, Attribute tableAttribute);
private:
};


#endif //MINISQL_CATALOGMANAGER_H
