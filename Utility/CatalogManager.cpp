//
// Created by 陶辰宁 on 2021/5/19.
//

#include "CatalogManager.h"

bool CatalogManager::createTable(string tableName, Attribute tableAttribute) {
    pageInfo newPage = BM.fetchPage("Catalog", BM.blockNum(tableName));
    cout << newPage.usedSize;
}
