//
// Created by 陶辰宁 on 2021/5/2.
//

#include "API.h"
#include <utility>

bool API::createTable(const string& tableName, const Attribute& tableAttribute) {
    try {
        CM->createTable(tableName, tableAttribute);
        short primary = TB->getPrimary(tableName);
        vector<short> type = TB->getType(tableName);
        vector<string> columnName = TB->getColumn(tableName);
        if(primary != -1) IM->createIndex(tableName, primary, type[primary]);
        TB->setIndex(tableName, columnName[primary], "primary");
        printf("Success!\n");
    }
    catch (runtime_error &error) {
        printf("%s\n", error.what());
        return false;
    }
    RM->createTable(tableName, tableAttribute);
    return true;
}

void API::initialize() {}

API::API() {
    BM = new BufferManager;
    TB = new Table(BM);
    CM = new CatalogManager(BM, TB);
    IM = new IndexManager(BM, TB);
    RM = new RecordManager(BM, IM);
}

API::~API() {
    delete RM;
    delete CM;
    delete IM;
    delete BM;
}

bool API::dropTable(const string& tableName) {
    try {
        vector<bool> index = TB->getIndex(tableName);
        for(int i = 0; i < index.size(); ++i){
            if(index[i]){
                IM->dropIndex(tableName, i);
            }
        }
        CM->dropTable(tableName);
        printf("Success!\n");
    }
    catch (runtime_error &error){
        printf("%s\n", error.what());
        return false;
    }
    RM->dropTable(tableName);
    return true;
}

bool API::insert(const string& tableName, const vector<short>& type, vector<string> content) {
    clock_t start, end;
    try {
        start = clock();
        TB->checkTable(tableName, type);
    }
    catch (runtime_error &error){
        printf("%s\n", error.what());
        return false;
    }
    vector<short> trueType = TB->getType(tableName);
    vector<bool> Unique = TB->getUnique(tableName);
    vector<bool> Index = TB->getIndex(tableName);
    try {
        for(int i = 0; i < Index.size(); ++i){
            if(Index[i]){
                if(IM->findKey(tableName, i, content[i]) != -1) throw runtime_error("Duplicated key!");
                Unique[i] = false;
            }
        }
        int pageNum = RM->insert(tableName, trueType, content, Unique);
        for(int i = 0; i < Index.size(); ++i){
            if(Index[i]){
                IM->insertKey(tableName, i, content[i], pageNum);
                break;
            }
        }
        end = clock();
        printf("Success in %.3fs\n", double(end-start)/CLOCKS_PER_SEC);
    }
    catch (runtime_error &error){
        printf("%s\n", error.what());
        return false;
    }

    return true;
}
bool CD_CMP(const conditionPair& a, const conditionPair& b) {return a.order < b.order;};

bool API::SelectDelete(const vector<string> &column, string tableName, vector<conditionPair> &CD, int mode) {
    // 无条件选择
    // todo: 选择某几列
    clock_t start, end;
    start = clock();
    if(TB->isExist(tableName) == -1) {
        printf("Table %s doesn't exist!\n", tableName.data());
        return false;
    }
    if(column[0] != "*") {
        try {TB->checkColumn(column, tableName);}
        catch (runtime_error &error){
            printf("%s\n", error.what());
            return false;
        }
    }
//    int separation;
    vector<short> type = TB->getType(tableName);
    vector<string> columnName = TB->getColumn(tableName);
    // 判断条件是否正确
    if(!CD.empty()){
        for(int i = 0; i < CD.size(); ++i){
            int pos = -1;
            for(int j = 0; j < columnName.size(); ++j){
                if(CD[i].columnName == columnName[j]) pos = j;
            }
            if(pos == -1){
                printf("Column %s doesn't exist!\n", CD[i].columnName.data());
                return false;
            }
            //if((type[pos] > 0 && type[pos] != CD[i].type)){
            if(type[pos] != CD[i].type){
                printf("Condition %d doesn't match!\n", i+1);
                return false;
            }
            // 是第几个位置
            CD[i].order = pos;
        }
    }
    sort(CD.begin(), CD.end(), CD_CMP);

    vector<bool> Index = TB->getIndex(tableName);
    vector<int> indexColumn;
    int CDCur = 0;
    vector<int> page;
    bool findFlag = false;

    for(int i = 0; i < Index.size(); ++i){
        if(Index[i]) {
            if(CDCur < CD.size()){
                if(CD[CDCur].order == i){
                    if(CD[CDCur].OP == Equal){
                        int temp = IM->findKey(tableName, i, CD[CDCur].condition);
                        if(temp != -1) page.emplace_back(temp);
                        findFlag = true;
                    }
                    else if(CDCur + 1 < CD.size()){
                        if(CD[CDCur].order == CD[CDCur+1].order){
                            if((CD[CDCur].OP == GreaterEqual || CD[CDCur].OP ==  Greater)
                               && (CD[CDCur+1].OP == LessEqual || CD[CDCur+1].OP == Less)){
                                IM->keyRange(tableName, i, CD[CDCur].condition, CD[CDCur+1].condition, page);
                                findFlag = true;
                                CDCur += 2;
                            }
                            else if((CD[CDCur+1].OP == GreaterEqual || CD[CDCur].OP ==  Greater)
                                    && (CD[CDCur].OP == LessEqual || CD[CDCur+1].OP == Less)){
                                IM->keyRange(tableName, i, CD[CDCur+1].condition, CD[CDCur].condition, page);
                                findFlag = true;
                                CDCur += 2;
                            }
                        }
                    }
                }else ++CDCur;
            }
            indexColumn.emplace_back(i);
        }
    }

    if(mode == SELECT) {
        // 打印表格标题
        tabulate::Table output;
        output.format().font_align(tabulate::FontAlign::center);
        addRowInit(output, columnName.size());
        for (int i = 0; i < columnName.size(); ++i) {
            output[0][i].set_text(columnName[i]);
        }
//    RM->printTitle(columnName, type, separation);
        // 进行选择
        int total;
        if(!findFlag) total = RM->select(tableName, type, output, CD);
        else total = RM->select(tableName, type, output, CD, page);
        end = clock();
        printf("%d rows in the set!\n", total);
        printf("Success in %.3fs\n", (double) (end - start) / CLOCKS_PER_SEC);
        if(total == 0) return true;
        printf("Start output formatting!\n");
        // 调整内容显示
        output[0].format()
                .font_color(tabulate::Color::yellow)
                .font_align(tabulate::FontAlign::center)
                .font_style({tabulate::FontStyle::bold});
        if (total > 1) {
            for (int i = 2; i < total; ++i) {
                output[i].format()
                        .hide_border_bottom()
                        .hide_border_top();
            }
            output[1].format().hide_border_bottom();
            output[total].format().hide_border_top();
        }
        cout << output << endl;
        printf("%d rows in the set!\n", total);
    }
    else if(mode == DELETE){
        if(!findFlag) RM->Delete(tableName, type, CD, indexColumn);
        else RM->Delete(tableName, type, CD, indexColumn, page);
        end = clock();
        printf("Success in %.3fs\n", (double)(end - start)/CLOCKS_PER_SEC);
    }

    return true;
}

bool API::createIndex(const string& indexName, const string& tableName, const string& columnName) {
    int i;
    try {
        i = TB->setIndex(tableName, columnName, indexName);
        vector<short> Type = TB->getType(tableName);
        IM->createIndex(tableName, i, Type[i]);
    }
    catch (runtime_error &error){
        printf("%s\n", error.what());
        return false;
    }
    return true;
}

bool API::dropIndex(const string &indexName, const string& tableName) {
    int i;
    try {
        i = TB->deleteIndex(tableName, indexName);
        IM->dropIndex(tableName, i);
        printf("Success!\n");
    }
    catch (runtime_error &error){
        printf("%s\n", error.what());
        return false;
    }
    return true;
}
