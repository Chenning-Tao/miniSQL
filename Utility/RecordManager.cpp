//
// Created by 陶辰宁 on 2021/5/29.
//

#include "RecordManager.h"

void RecordManager::insert(const string& tableName, vector<short> type, vector<string> content) {
    string infoFile = tableName + "_INFO";
    vector<int> pageRecord;
    int recordSize, recordPerPage;
    initialHead(tableName, pageRecord, recordSize, recordPerPage);

    int i = 0;
    for(; i < pageRecord.size(); ++i){
        if(recordPerPage > pageRecord[i]) break;
    }

    int updateNum;
    if(i == pageRecord.size()){
        // 需要添加新的一个page
        pageInfo infoHead = BM->fetchPage(infoFile, 0);
        char *point = infoHead.content;
        otherToChar(int(pageRecord.size() + 1), point);
        BM->changeComplete(infoFile, 0);
        updateNum = 1;
    }
    else updateNum = pageRecord[i] + 1;

    int blockID = i / (PAGE_SIZE / 4) + 1;
    int offset = i - ((blockID-1) * PAGE_SIZE / 4);
    pageInfo info = BM->fetchPage(infoFile, blockID);
    char *point = info.content;
    point += (offset * 4);
    otherToChar(updateNum, point);
    BM->changeComplete(infoFile, blockID);

    // todo: 使用空闲指针，需要解决一条记录<4的问题

    // 找到需要插入的位置 i是需要插入的page位置
    pageInfo insert = BM->fetchPage(tableName, i);
    point = insert.content;
    // 获取空闲指针
    int freePointer, freePointerNext, freePointerUpdate;
    charToOther(point, freePointer);
    point = insert.content;
    point += (freePointer+1)*recordSize;
    charToOther(point, freePointerNext);
    point -= 4;
    // 定位到要插入的位置初始
    for(int x = 0; x < content.size(); ++x){
        if(type[x] == -1)
            otherToChar(stoi(content[x]), point);
        else if(type[x] == 0)
            otherToChar(stof(content[x]), point);
        else
            otherToChar(content[x], point, type[x]);
    }
    freePointerUpdate = freePointer + freePointerNext + 1;
    point = insert.content;
    otherToChar(freePointerUpdate, point);
    BM->changeComplete(tableName, i);
}

RecordManager::RecordManager(BufferManager *inBM, CatalogManager *inCM, Table *inTB){
    BM = inBM;
    CM = inCM;
    TB = inTB;
}

void RecordManager::initialHead(const string& tableName, vector<int> &pageRecord, int &recordSize, int &recordPerPage) {
    string fileName = tableName + "_INFO";
    pageInfo info = BM->fetchPage(fileName, 0);
    char *point = info.content;
    int pageNum;
    charToOther(point, pageNum);
    charToOther(point, recordSize);
    charToOther(point, recordPerPage);

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

void RecordManager::createTable(const string& tableName, Attribute tableInfo) {
    string fileName = tableName + "_INFO";
    // 修改info
    pageInfo info = BM->fetchPage(fileName, 0);
    char *point = info.content;
    int pageNum = 0;
    otherToChar(pageNum, point);
    vector<short> type;
    tableInfo.getType(type);
    // 计算一条记录所需要耗费的空间大小
    int recordSize = 0;
    for(short cur : type){
        if (cur == -1 || cur == 0) recordSize += 4;
        else recordSize += cur;
    }
    int recordPerPage = PAGE_SIZE / recordSize - 1;
    otherToChar(recordSize, point);
    otherToChar(recordPerPage, point);
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
    BM->flush();
}

