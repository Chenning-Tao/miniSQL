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
    string infoFile = tableName + "_INFO";
    vector<int> pageRecord;
    initialHead(tableName, pageRecord);

    // 计算一条记录所需要耗费的空间大小
    int recordSize = 0;
    for(short cur : type){
        if (cur == -1 || cur == 0) recordSize += 4;
        else recordSize += cur;
    }
    int recordPerPage = PAGE_SIZE / recordSize - 1;

    int i = 0;
    for(; i < pageRecord.size(); ++i){
        if(recordPerPage > pageRecord[i]) break;
    }

    int updateNum;
    if(i == pageRecord.size()){
        // 需要添加新的一个page
        pageInfo infoHead = BM->fetchPage(infoFile, 0);
        otherToChar(pageRecord.size() + 1, infoHead.content);
        BM->changeComplete(infoFile, 0);

        updateNum = 1;
    }
    else {
        updateNum = pageRecord[i] + 1;
    }

    int blockID = i / (PAGE_SIZE / 4) + 1;
    int offset = i - ((blockID-1) * PAGE_SIZE / 4);
    pageInfo info = BM->fetchPage(infoFile, blockID);
    char *point = info.content;
    point += (offset * 4);
    otherToChar(updateNum, point);
    BM->changeComplete(infoFile, blockID);

    // todo: 使用空闲指针，需要解决一条记录<4的问题
}

RecordManager::RecordManager(BufferManager *inBM, CatalogManager *inCM, Table *inTB){
    BM = inBM;
    CM = inCM;
    TB = inTB;
}

void RecordManager::initialHead(const string& tableName, vector<int> &pageRecord) {
    string fileName = tableName + "_INFO";
    pageInfo info = BM->fetchPage(fileName, 0);
    char *point = info.content;
    int pageNum;
    charToOther(point, pageNum);

    int temp;
    for(int x = 1; pageNum > 0; ++x, pageNum -= (PAGE_SIZE/4)){
        info = BM->fetchPage(fileName, x);
        point = info.content;
        for (int i = 0; i < min(pageNum, PAGE_SIZE/4); ++i){
            charToOther(point, temp);
            pageRecord.push_back(temp);
        }
    }
}

void RecordManager::createTable(const string& tableName) {
    string fileName = tableName + "_INFO";
    // 修改info
    pageInfo info = BM->fetchPage(fileName, 0);
    char *point = info.content;
    int pageNum = 0;
    otherToChar(pageNum, point);
    BM->changeComplete(fileName, 0);
    BM->fetchPage(fileName, 1);
    BM->changeComplete(fileName, 1);
    // 修改内容
    pageInfo file = BM->fetchPage(tableName, 0);
    BM->changeComplete(tableName, 0);
}

void RecordManager::dropTable(const string& tableName) {
    BM->deleteFile(tableName);
    string fileName = tableName + "_INFO";
    BM->deleteFile(fileName);
}

void RecordManager::addNewPage(const string& tableName, int pageNum) {
    string fileName = tableName + "_INFO";
    pageInfo info = BM->fetchPage(fileName, 0);
    // 更新总页数
    otherToChar(pageNum+1, info.content);
    BM->changeComplete(fileName, 0);

    // 获取新的page
    BM->fetchPage(tableName, pageNum-1);
    BM->changeComplete(tableName, pageNum-1);

    // 理论上不需要更新，因为初始化时候已经是0了
//    // 第1023个属于第一个page
//    int blockID = pageNum / (PAGE_SIZE/4);
//    pageInfo update = BM->fetchPage(tableName, blockID);
//
//    pageNum = pageNum - blockID * (PAGE_SIZE/4);
//    char *point = update.content + pageNum * 4;
//    otherToChar(int(0), point);
}
