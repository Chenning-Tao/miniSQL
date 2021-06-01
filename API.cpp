//
// Created by 陶辰宁 on 2021/5/2.
//

#include "API.h"

#include <utility>

bool API::createTable(const string& tableName, const Attribute& tableAttribute) {
    try {
        CM->createTable(tableName, tableAttribute);
        printf("Success!\n");
    }
    catch (const char *error) {
        cout << error << endl;
        return false;
    }
    RM->createTable(tableName, tableAttribute);
    return true;
}

void API::initialize() {

}

API::API() {
    BM = new BufferManager;
    TB = new Table(BM);
    CM = new CatalogManager(BM, TB);
    RM = new RecordManager(BM, CM, TB);
}

API::~API() {
    delete RM;
    delete CM;
}

bool API::dropTable(string tableName) {
    try {
        CM->dropTable(tableName);
        printf("Success");
    }
    catch (const char *error) {
        printf("%s\n", error);
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
    catch (string error) {
        printf("%s\n", error.data());
        return false;
    }
    vector<short> trueType = TB->getType(tableName);
    try {
        RM->insert(tableName, trueType, content);
        end = clock();
        printf("Success in %.3fs\n", double(end-start)/CLOCKS_PER_SEC);
    }
    catch (string error) {
        printf("%s\n", error.data());
        return false;
    }

    return true;
}

bool API::select(const vector<string> &column, string tableName) {
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
        catch (string error) {
            printf("%s\n", error.data());
        }
    }
//    int separation;
    vector<short> type = TB->getType(tableName);
    vector<string> columnName = TB->getColumn(tableName);
    // 打印表格标题
    tabulate::Table output;
    output.format().font_align(tabulate::FontAlign::center);
    addRowInit(output, columnName.size());
    for(int i = 0; i < columnName.size(); ++i){
        output[0][i].set_text(columnName[i]);
    }
//    RM->printTitle(columnName, type, separation);
    int total = RM->select(tableName, type, output);
    end = clock();
    printf("%d rows in the set!\n", total);
    printf("Success in %.3fs\n", (double )(end-start)/CLOCKS_PER_SEC);
    printf("Start output formatting!\n");
    // 调整内容显示
    output[0].format()
            .font_color(tabulate::Color::yellow)
            .font_align(tabulate::FontAlign::center)
            .font_style({tabulate::FontStyle::bold});
    if(total > 1) {
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
