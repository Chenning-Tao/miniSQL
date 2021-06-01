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

int RecordManager::select(const string& tableName, const vector<short>& type,
                          tabulate::Table &output, const vector<conditionPair>& CD) {
    vector<int> pageRecord;
    int recordSize, recordPerPage;
    initialHead(tableName, pageRecord, recordSize, recordPerPage);

    pageInfo record;
    int count;
    int resultCount = 0;
    for(int i = 0; i < pageRecord.size(); ++i){
        count = pageRecord[i];
        record = BM->fetchPage(tableName, i);
        char *point = record.content;
        int freePointer = 0;
        int tempInt;
        float tempFloat;
        string tempString;

        while(true){
            if(freePointer == 0){
                charToOther(point, freePointer);
                point -= 4;
                point += recordSize;
            }
            else{
                --count;
                --freePointer;
                bool flag = true;
                vector<string> tempResult;
                for(int j = 0; j < type.size(); ++j){
                    if(type[j] == -1){
                        charToOther(point, tempInt);
                        tempResult.emplace_back(to_string(tempInt));
                    }
                    else if(type[j] == 0){
                        charToOther(point, tempFloat);
                        tempResult.emplace_back(to_string(tempFloat));
                    }
                    else {
                        charToOther(point, tempString, type[j]);
                        tempResult.emplace_back(tempString);
                        tempString.clear();
                    }
                }
                for(const auto & j : CD){
                    flag = condition(j, tempResult[j.order]);
                    if(!flag) break;
                }

                if(flag) {
                    ++resultCount;
                    addRowInit(output, type.size());
                    for(int j = 0; j < tempResult.size(); ++j){
                        output[resultCount][j].set_text(tempResult[j]);
                    }
                }
//                print(tempResult);
            }
            if(count == 0) break;
        }
    }
//    print(separation);
    return resultCount;
}

void RecordManager::print(const vector<string> &result) {
    for(int j = 0; j < result.size(); ++j) {
        if (j + 1 == result.size())
            printf(" %s", result[j].data());
        else printf(" %s|", result[j].data());
    }
    printf("\n");
}

void RecordManager::printTitle(const vector<string> &column, const vector<short> &type, int &separation) {
    separation = 0;
    for(int i = 0; i < column.size(); ++i){
        if (type[i] == -1 || type[i] == 0)
            separation += 13;
        else
            separation += (type[i]+3);
    }
    print(separation);
    for(int i = 0; i < column.size(); ++i){
        if (type[i] == -1 || type[i] == 0){
            if(i+1 != column.size())
                printf(" %-10s |", column[i].data());
            else
                printf(" %-10s", column[i].data());;
        }
        else
            printf(" %-*s |", type[i], column[i].data());
    }
    printf("\n");
    print(separation);
}

void RecordManager::print(int count) {
    for(int i = 0; i < count; ++i)
        printf("-");
    printf("\n");
}

