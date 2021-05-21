//
// Created by 陶辰宁 on 2021/5/2.
//

#ifndef MINISQL_COMMONHEAD_H
#define MINISQL_COMMONHEAD_H
#define CatalogPath "./database/Catalog"
#define DatabasePath "./database"
#include <iostream>
#include <string>
#include <algorithm>
#include "regex"
#include <cstdio>
#include <unordered_map>
#include <vector>

typedef struct pageInfo{
    char *content;
    int usedSize;
}pageInfo;

#endif //MINISQL_COMMONHEAD_H
