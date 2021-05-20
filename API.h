//
// Created by 陶辰宁 on 2021/5/2.
//

#ifndef MINISQL_API_H
#define MINISQL_API_H
#include "CommonHead.h"
#include "Utility/CatalogManager.h"
using namespace std;

class API {
private:
public:
    API();
    bool createTable(string tableName, Attribute tableAttribute);
    void initialize();
    CatalogManager CM;
};

#endif //MINISQL_API_H
