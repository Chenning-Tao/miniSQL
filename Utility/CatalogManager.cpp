//
// Created by 陶辰宁 on 2021/5/19.
//

#include "CatalogManager.h"

bool CatalogManager::createTable(string tableName, Attribute tableAttribute) {
//    pageInfo newPage = BM.fetchPage("Catalog", BM.blockNum(tableName));
//    newPage.content[0] = tableAttribute.attributeNum();
//    printf("%x", newPage.content[0]);
}

CatalogManager::CatalogManager(BufferManager *BM) {
    pageInfo startPage = BM->fetchPage("Catalog", 0);
    /*
     前2048K：存在表的个数 --> page
     后2048K：空闲位置指针个数 --> 空闲page
     */
    char *used = startPage.content;
    char *unused = startPage.content + PAGE_SIZE/2;
    short usedSize, unusedSize;
    // 如果还没有建立Catalog文件
    if(!BufferManager::blockNum("Catalog")){
        // todo: 添加overflow的判断
        // 初始化存在的
        usedSize = 0;
        otherToChar(usedSize, used);
        // 初始化空闲的
        freePointer.push_back(1);
        unusedSize = 1;
        otherToChar(unusedSize, unused);
        otherToChar(freePointer[0], unused);
        // 告诉BM写入完成
        BM->changeComplete("Catalog", 0);
    }else {
        // 获取存在的
        int temp;
        charToOther(used, usedSize);
        for(short i = 0; i < usedSize; ++i){
            charToOther(used, temp);
            tablePointer.push_back(temp);
        }
        // 获取不存在的
        charToOther(unused, unusedSize);
        for(short i = 0; i < unusedSize; ++i){
            charToOther(used, temp);
            freePointer.push_back(temp);
        }
    }
}

CatalogManager::~CatalogManager() {
    delete BM;
}


