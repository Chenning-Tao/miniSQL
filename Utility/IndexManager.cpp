//
// Created by 陶辰宁 on 2021/6/2.
//

#include "IndexManager.h"

void IndexManager::insertKey(string tableName, int column, string key) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind != index.end()){
        indexFind->second.key.insert(key);
    }
}

bool IndexManager::findKey(string tableName, int column, string key) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind == index.end()){
        throw string("Index doesn't exist!");
    } else {
        return (indexFind->second.key.find(key) != indexFind->second.key.end());
    }
}

IndexManager::IndexManager(BufferManager *inBM, Table *inTB) {
    BM = inBM;
    TB = inTB;
    for(auto Table : TB->index){
        string tableName = Table.first;
        vector<bool> tableIndexColumn;
        TB->tableInfo[Table.second].getIndex(tableIndexColumn);
        pageInfo start;
        char *point;
        for(int i = 0; i < tableIndexColumn.size(); ++i){
            if(tableIndexColumn[i]){
                string fileName = tableName + to_string(i) + "_idx";
                short type;
                int totalNumber;
                start = BM->fetchPage(fileName, 0);
                point = start.content;
                charToOther(point, type);
                charToOther(point, totalNumber);
                createIndex(tableName, i, type);

                int recordPerPage;
                if(type <= 0) recordPerPage = PAGE_SIZE/4;
                else recordPerPage = PAGE_SIZE/type;
                int count = 0;
                int curPage = 0;

                for(int j = 0; j < totalNumber; ++j){
                    if(count == 0) {
                        count = recordPerPage;
                        BM->changeComplete(fileName, curPage);
                        ++curPage;
                        start = BM->fetchPage(fileName, curPage);
                        point = start.content;
                    }
                    if(type == -1) {
                        int temp;
                        charToOther(point, temp);
                        insertKey(tableName, i, to_string(temp));
                    }
                    else if(type == 0){
                        float temp;
                        charToOther(point, temp);
                        insertKey(tableName, i, to_string(temp));
                    }
                    else {
                        string temp;
                        charToOther(point, temp, type);
                        insertKey(tableName, i, temp);
                    }
                    --count;
                }
            }
        }
    }
}

void IndexManager::createIndex(string tableName, int column, short type) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind == index.end()) {
        indexNode newNode;
        newNode.type = type;
        index.emplace(tableName + to_string(column), newNode);
        BM->fetchPage(tableName + to_string(column) + "_idx", 0);
        BM->changeComplete(tableName + to_string(column) + "_idx", 0);
    }
}

void IndexManager::dropIndex(string tableName, int column) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind != index.end()){
        index.erase(indexFind);
        BM->deleteFile(tableName + to_string(column) + "_idx");
        BM->flush();
    }
}

IndexManager::~IndexManager() {
    for(auto indexFind : index){
        short type = indexFind.second.type;
        string fileName = indexFind.first + "_idx";
        pageInfo start = BM->fetchPage(fileName, 0);
        char *point = start.content;
        otherToChar(type, point);
        int size = indexFind.second.key.size();
        otherToChar(size, point);

        int recordPerPage;
        if(type <= 0) recordPerPage = PAGE_SIZE/4;
        else recordPerPage = PAGE_SIZE/type;
        int count = 0;
        int curPage = 0;

        BM->changeComplete(fileName, 0);

        for(auto value : indexFind.second.key){
            if(count == 0) {
                count = recordPerPage;
                BM->changeComplete(fileName, curPage);
                ++curPage;
                start = BM->fetchPage(fileName, curPage);
                point = start.content;
            }
            if(type == -1) {
                otherToChar(stoi(value), point);
            }
            else if(type == 0){
                otherToChar(stof(value), point);
            }
            else
                otherToChar(value, point, type);
            --count;
        }
        BM->changeComplete(fileName, curPage);
    }
}
