
#include "IndexManager.h"

void IndexManager::insertKey(const string& tableName, int column, const string& key, int pageNum) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind != index.end()){
        if(indexFind->second.type == -1){
            indexFind->second.intKey.emplace(stoi(key), pageNum);
        }else if(indexFind->second.type == 0){
            indexFind->second.floatKey.emplace(stof(key), pageNum);
        }else indexFind->second.stringKey.emplace(key, pageNum);
    }
}

int IndexManager::findKey(const string& tableName, int column, const string& key) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind == index.end()){
        throw runtime_error("Index doesn't exist!");
    } else {
        if(indexFind->second.type == -1){
            if(indexFind->second.intKey.find(stoi(key)) == indexFind->second.intKey.end()) return -1;
            else return indexFind->second.intKey.find(stoi(key))->second;
        }else if(indexFind->second.type == 0){
            if(indexFind->second.floatKey.find(stof(key)) == indexFind->second.floatKey.end()) return -1;
            else return indexFind->second.floatKey.find(stof(key))->second;
        }else {
            if(indexFind->second.stringKey.find(key) == indexFind->second.stringKey.end()) return -1;
            else return indexFind->second.stringKey.find(key)->second;
        }

    }
}

IndexManager::IndexManager(BufferManager *inBM, Table *inTB) {
    BM = inBM;
    TB = inTB;
    for(const auto& Table : TB->index){
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
                if(type <= 0) recordPerPage = PAGE_SIZE/8;
                else recordPerPage = PAGE_SIZE/(type+4);
                int count = 0;
                int curPage = 0;
                int num;

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
                        charToOther(point, num);
                        insertKey(tableName, i, to_string(temp), num);
                    }
                    else if(type == 0){
                        float temp;
                        charToOther(point, temp);
                        insertKey(tableName, i, to_string(temp), num);
                    }
                    else {
                        string temp;
                        charToOther(point, temp, type);
                        insertKey(tableName, i, temp, num);
                    }
                    --count;
                }
            }
        }
    }
}

void IndexManager::createIndex(const string& tableName, int column, short type) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind == index.end()) {
        BpNode newNode;
        newNode.type = type;
        index.emplace(tableName + to_string(column), newNode);
        BM->fetchPage(tableName + to_string(column) + "_idx", 0);
        BM->changeComplete(tableName + to_string(column) + "_idx", 0);
    }
}

void IndexManager::dropIndex(const string& tableName, int column) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind != index.end()){
        index.erase(indexFind);
        BM->deleteFile(tableName + to_string(column) + "_idx");
        BM->flush();
    }
}

IndexManager::~IndexManager() {
    for(const auto& indexFind : index){
        short type = indexFind.second.type;
        string fileName = indexFind.first + "_idx";
        pageInfo start = BM->fetchPage(fileName, 0);
        char *point = start.content;
        otherToChar(type, point);
        int size;
        if(indexFind.second.type == -1) size = indexFind.second.intKey.size();
        else if(indexFind.second.type == 0) size = indexFind.second.floatKey.size();
        else size = indexFind.second.stringKey.size();
        otherToChar(size, point);

        int recordPerPage;
        if(type <= 0) recordPerPage = PAGE_SIZE/8;
        else recordPerPage = PAGE_SIZE/(type+4);
        int count = 0;
        int curPage = 0;

        BM->changeComplete(fileName, 0);

        if(type == -1) {
            for (const auto &value : indexFind.second.intKey) {
                if (count == 0) {
                    count = recordPerPage;
                    BM->changeComplete(fileName, curPage);
                    ++curPage;
                    start = BM->fetchPage(fileName, curPage);
                    point = start.content;
                }
                otherToChar(value.first, point);
                otherToChar(value.second, point);
                --count;
            }
        }
        else if(type == 0){
            for (const auto &value : indexFind.second.floatKey) {
                if (count == 0) {
                    count = recordPerPage;
                    BM->changeComplete(fileName, curPage);
                    ++curPage;
                    start = BM->fetchPage(fileName, curPage);
                    point = start.content;
                }
                    otherToChar(value.first, point);
                    otherToChar(value.second, point);
                --count;
            }
        }
        else {
            for (const auto &value : indexFind.second.stringKey) {
                if (count == 0) {
                    count = recordPerPage;
                    BM->changeComplete(fileName, curPage);
                    ++curPage;
                    start = BM->fetchPage(fileName, curPage);
                    point = start.content;
                }
                string temp = value.first;
                otherToChar(temp, point, type);
                otherToChar(value.second, point);
                --count;
            }
        }
        BM->changeComplete(fileName, curPage);
    }
}

void IndexManager::deleteKey(const string& tableName, int column, const string& key) {
    auto indexFind = index.find(tableName + to_string(column));
    if(indexFind != index.end()){
        if(indexFind->second.type == -1){
            indexFind->second.intKey.erase(stoi(key));
        }else if(indexFind->second.type == 0){
            indexFind->second.floatKey.erase(stof(key));
        }else indexFind->second.stringKey.erase(key);
    }
}

void IndexManager::deleteAll(const string &tableName, int column) {
    auto indexFind = index.find(tableName + to_string(column));
    short type = indexFind->second.type;
    dropIndex(tableName, column);
    createIndex(tableName, column, type);
}

void IndexManager::keyRange(const string &tableName, int column, const string& lowerBound, const string& upperBound, vector<int> &page) {
    auto tableFind = index.find(tableName+to_string(column));
    if(tableFind->second.type == -1){
        auto LBound = tableFind->second.intKey.lower_bound(stoi(lowerBound));
        auto UBound = tableFind->second.intKey.upper_bound(stoi(upperBound));
        auto End = tableFind->second.intKey.end();
        for(auto it = LBound; it != UBound && it != End; ++it){
            auto result = find(page.begin(), page.end(), it->second);
            if(result == page.end()){
                page.emplace_back(it->second);
            }
        }
    }else if(tableFind->second.type == 0){
        auto LBound = tableFind->second.floatKey.lower_bound(stof(lowerBound));
        auto UBound = tableFind->second.floatKey.upper_bound(stof(upperBound));
        auto End = tableFind->second.floatKey.end();
        for(auto it = LBound; it != UBound && it != End; ++it){
            auto result = find(page.begin(), page.end(), it->second);
            if(result == page.end()){
                page.emplace_back(it->second);
            }
        }
    }else {
        auto LBound = tableFind->second.stringKey.lower_bound(lowerBound);
        auto UBound = tableFind->second.stringKey.upper_bound(upperBound);
        auto End = tableFind->second.stringKey.end();
        for(auto it = LBound; it != UBound && it != End; ++it){
            auto result = find(page.begin(), page.end(), it->second);
            if(result == page.end()){
                page.emplace_back(it->second);
            }
        }
    }


}
