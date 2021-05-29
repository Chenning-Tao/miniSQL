//
// Created by 陶辰宁 on 2021/5/29.
//

#include "RecordManager.h"

bool RecordManager::insert(const string& tableName, vector<short> type, vector<string> content) {
    try {
        TB->checkTable(tableName, type);
    }
    catch (string error) {
        throw error;
    }
    vector<int> pageRecord;
    initialHead(tableName, pageRecord);

    // 计算一条记录所需要耗费的空间大小
    int recordSize = 0;
    for(short cur : type){
        if (cur == -1 || cur == 0) recordSize += 4;
        else recordSize += cur;
    }
    int recordPerPage = PAGE_SIZE / recordSize;

    int i = 0;
    for(; i < pageRecord.size(); ++i){
        if(recordPerPage > pageRecord[i]) break;
    }
    if(i == pageRecord.size()){
        // 需要添加新的一个page
        this->addNewPage(tableName, pageRecord.size());
    }
    else {
        // 让这个page的数量加1
    }

    // todo: 使用空闲指针，需要解决一条记录<4的问题
}

RecordManager::RecordManager(BufferManager *inBM, CatalogManager *inCM, Table *inTB){
    BM = inBM;
    CM = inCM;
    TB = inTB;
}

void RecordManager::initialHead(string tableName, vector<int> &pageRecord) {
    pageInfo info = BM->fetchPage(std::move(tableName), 0);
    char *point = info.content;
    int pageNum;
    charToOther(point, pageNum);

    int temp;
    while(pageNum > 0){
        for (int i = 1; i < min(pageNum, PAGE_SIZE/4); ++i){
            charToOther(point, temp);
            pageRecord.push_back(temp);
        }
        pageNum -= (PAGE_SIZE/4);
    }
}

void RecordManager::createTable(string tableName) {
    pageInfo info = BM->fetchPage(tableName, 0);
    char *point = info.content;
    int pageNum = 0;
    otherToChar(pageNum, point);
    BM->changeComplete(tableName, 0);
}

void RecordManager::dropTable(const string& tableName) {
    BM->deleteFile(tableName);
}

void RecordManager::addNewPage(const string& tableName, int pageNum) {
    pageInfo info = BM->fetchPage(tableName, 0);
    otherToChar(pageNum+1, info.content);
    BM->changeComplete(tableName, 0);
    BM->fetchPage(tableName, pageNum+INFO_BLOCK_NUM);
    BM->changeComplete(tableName, pageNum+INFO_BLOCK_NUM);

    // 理论上不需要更新，因为初始化时候已经是0了
//    // 第1023个属于第一个page
//    int blockID = pageNum / (PAGE_SIZE/4);
//    pageInfo update = BM->fetchPage(tableName, blockID);
//
//    pageNum = pageNum - blockID * (PAGE_SIZE/4);
//    char *point = update.content + pageNum * 4;
//    otherToChar(int(0), point);
}
