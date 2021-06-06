//
// Created by 陶辰宁 on 2021/5/19.
//

#include "CatalogManager.h"

void CatalogManager::createTable(string tableName, Attribute tableAttribute) {
    if(TB->isExist(tableName) != -1) throw runtime_error("Table already existed!");
    // 添加到table到数据结构中
    int newBlockID = freePointer.front();
    // 首先获取一个空闲的page
    TB->addNew(tableName, tableAttribute, newBlockID);
    freePointer.pop();
    if(newBlockID == last){
        ++last;
        freePointer.push(last);
    }
    pageInfo newPage = BM->fetchPage("Catalog", newBlockID);
    tablePointer.push_back(newBlockID);
    /*
    * table的存储方式:
    * tableName 256位
    * count(告诉有多少个) primary跟在后面(获得位置)
    * 32位存name，4位存type，2位存unique和index, 32位存indexName
    */
    char *cur = newPage.content;
    otherToChar(tableName, cur, 256);
    tableAttribute.writeOut(cur);
    BM->changeComplete("Catalog", newBlockID);
}

CatalogManager::CatalogManager(BufferManager *inBM, Table *inTB) {
    TB = inTB;
    this->BM = inBM;
    pageInfo startPage = BM->fetchPage("Catalog", 0);
    /*
     * 前2048K：存在表的个数 --> page
     * 后2048K：空闲位置指针个数+最后一个 --> 空闲page
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
        last = 1;
        freePointer.push(1);
        unusedSize = 1;
        otherToChar(unusedSize, unused);
        otherToChar(last ,unused);
        otherToChar(freePointer.front(), unused);
        // 告诉BM写入完成
        BM->changeComplete("Catalog", 0);
    }else {
        // 获取存在的
        int temp;
        charToOther(used, usedSize);
        for(short i = 0; i < usedSize; ++i){
            charToOther(used, temp);
            tablePointer.push_back(temp);
            // 读入数据表
            pageInfo inTable = BM->fetchPage("Catalog", temp);
            TB->readIn(inTable);
        }
        // 获取不存在的
        charToOther(unused, unusedSize);
        charToOther(unused, last);
        for(short i = 0; i < unusedSize; ++i){
            charToOther(unused, temp);
            freePointer.push(temp);
        }
    }
}

CatalogManager::~CatalogManager() {
    pageInfo startPage = BM->fetchPage("Catalog", 0);
    char *used = startPage.content;
    char *unused = startPage.content + PAGE_SIZE/2;
    short usedSize, unusedSize;

    usedSize = short(tablePointer.size());
    unusedSize = short(freePointer.size());

    // 存在的
    otherToChar(usedSize, used);
    for(int i = 0; i < usedSize; ++i) {
        pageInfo newPage = BM->fetchPage("Catalog", tablePointer[i]);
        /*
        * table的存储方式:
        * tableName 256位
        * count(告诉有多少个) primary跟在后面(获得位置)
        * 32位存name，4位存type，2位存unique和index, 32位存indexName
        */
        char *cur = newPage.content;
        string tableName;
        charToOther(cur, tableName, 256);
        auto tableFind = TB->index.find(tableName);
        TB->tableInfo[tableFind->second].writeOut(cur);
        BM->changeComplete("Catalog", tablePointer[i]);
        otherToChar(tablePointer[i], used);
    }
    // 空闲的
    otherToChar(unusedSize, unused);
    otherToChar(last, unused);
    for(int i = 0; i < unusedSize; ++i){
        otherToChar(freePointer.front(), unused);
        freePointer.pop();
    }
    BM->changeComplete("Catalog", 0);
    delete TB;
}

void CatalogManager::dropTable(const string& tableName) {
    int blockID = TB->isExist(tableName);
    if(blockID == -1) throw runtime_error("This table doesn't exists!");
    BM->deletePage("Catalog", blockID);
    TB->deleteTable(tableName);
    // 在已有list中删除
    for(auto it=tablePointer.begin(); it!= tablePointer.end(); ) {
        if (*it == blockID)
            it = tablePointer.erase(it);
        else
            ++it;
    }
    // 在空闲中加上
    freePointer.push(blockID);
}
