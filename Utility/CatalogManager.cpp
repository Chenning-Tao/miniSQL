//
// Created by 陶辰宁 on 2021/5/19.
//

#include "CatalogManager.h"

bool CatalogManager::createTable(string tableName, Attribute tableAttribute) {
//    pageInfo newPage = BM.fetchPage("Catalog", BM.blockNum(tableName));
//    newPage.content[0] = tableAttribute.attributeNum();
//    printf("%x", newPage.content[0]);
}

CatalogManager::CatalogManager() {
    // 如果还没有建立Catalog文件
    if(!BufferManager::blockNum("Catalog")){
        freePointer.push_back(1);
        pageInfo startPage = BM.fetchPage("Catalog", 0);
        char *cur = startPage.content;
        short catalogSize = 1;
        otherToChar(catalogSize, cur);
        otherToChar(freePointer[0], cur);
        startPage.usedSize = cur - startPage.content;
    }
    //pageInfo startPage = BM.fetchPage("Catalog");
}


