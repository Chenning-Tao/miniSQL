//
// Created by 陶辰宁 on 2021/5/2.
//

#include "Interpreter.h"
#include <utility>

int Interpreter::interpret(string SQL) {
    regex Word("[a-z]+");
    smatch firstWord;
    if(regex_search(SQL, firstWord, Word)){
        // 用于判断是哪类操作
        string type = firstWord.str(0);
        if(type == "insert"){
            smatch attribute;
            regex insertBody(R"(insert[\s]into[\s]([\w]+)[\s]values[\s]?[\(](.*)[\)][\s]?;)");
            if(regex_match(SQL, attribute, insertBody)){
                return this->insert(attribute[1], attribute[2]);
            }
            else printf("Syntax error near insert!\n");
            return 0;
        }else if(type == "drop"){
            smatch dropWord;
            regex dropTable("drop[\\s]table[\\s]");
            regex dropIndex("drop[\\s]index[\\s]");
            // drop table
            if(regex_search(SQL, dropWord, dropTable))
                return this->dropTable(SQL.substr(dropWord[0].length()));
        }else if(type == "create"){
            smatch dropWord;    // 用来将前面这一段去掉
            regex createTable("create[\\s]table[\\s]");
            regex createIndex("create[\\s]index[\\s]");
            if(regex_search(SQL, dropWord, createIndex))
                return this->createIndex(SQL.substr(dropWord[0].length()));
            else if(regex_search(SQL, dropWord, createTable))
                return this->createTable(SQL.substr(dropWord[0].length()));
            else
                cout << "Syntax error near create!" << endl;
        }else if(type == "delete"){

        }else if(type == "select") {
            smatch word;
            regex unconditional(R"(select[\s](.*)[\s]from[\s]([\w]+)[\s]?;)");
            regex conditional(R"(select[\s](.*)[\s]from[\s]([\w]+)[\s]where[\s](.*);)");
            if (regex_match(SQL, word, unconditional)) {
                select(word[1], word[2], "");
            } else if(regex_match(SQL, word, conditional)) {
                select(word[1], word[2], word[3]);
            }
            else printf("Syntax error near select!\n");
        }else if(type == "execfile"){
            smatch filename;
            regex filenameRule(R"(execfile[\s]([\w.]+)[\s]?;)");
            if (regex_match(SQL, filename, filenameRule)){
                execfile(filename[1]);
            }else printf("Syntax error near execfile!\n");
        }else
            printf("Syntax error!\n");
    } else printf("Syntax error!\n");
}

int Interpreter::createTable(string SQL) {
    regex Word("[\\w]+");
    smatch tableNameMatch;
    // 获得table的名字
    regex_search(SQL, tableNameMatch, Word);
    string tableName = tableNameMatch[0];
    SQL = SQL.substr(tableNameMatch[0].length());

    regex bracket(R"(\((.+?)\)[\s]?;[\s]?)");
    regex intRule(R"(^[\s]?[\s]?([\w]+)[\s]int[\s]?(unique)?,[\s]?)");
    regex charRule(R"(^[\s]?[\s]?([\w]+)[\s]char[\s]?[\(]([0-9]+)[\)][\s]?(unique)?,[\s]?)");
    regex floatRule(R"(^[\s]?[\s]?([\w]+)[\s]float[\s]?(unique)?,[\s]?)");
    regex primaryRule(R"(^[\s]?[\s]?primary[\s]key[\s]?[\(]([\w]+)[\)][\s]?,[\s]?)");
    smatch attributesS;
    smatch intS;
    smatch charS;
    smatch floatS;
    smatch primaryS;

    regex_search(SQL, attributesS, bracket);
    string attributes = attributesS.str(1);
    attributes.push_back(',');
    Attribute newTable;
    while(!attributes.empty()){
        if(regex_search(attributes, charS, charRule)){
            newTable.setChar(charS[1], short(stoi(charS[2])), charS[3].length());
            attributes = attributes.substr(charS[0].length());
        }else if(regex_search(attributes, intS, intRule)){
            newTable.setInt(intS[1], intS[2].length());
            attributes = attributes.substr(intS[0].length());
        }else if(regex_search(attributes, floatS, floatRule)){
            newTable.setFloat(floatS[1], floatS[2].length());
            attributes = attributes.substr(floatS[0].length());
        }else if(regex_search(attributes, primaryS, primaryRule)){
            if(!newTable.setPrimary(primaryS[1])) return 0;
            attributes = attributes.substr(primaryS[0].length());
        }else{
            cout << "Error near ' " << attributes.substr(0, 10) << " '" <<endl;
            return 0;
        }
    }

    api.createTable(tableName, newTable);
    return 1;
}

int Interpreter::createIndex(const string& SQL){
    return 0;
}

bool Interpreter::dropTable(const string& SQL) {
    regex Word("([\\w]+)[\\s]?;");
    smatch tableNameMatch;
    // 获得table的名字
    if(regex_match(SQL, tableNameMatch, Word)) {
        string tableName = tableNameMatch[1];
        api.dropTable(tableName);
    }
    else{
        cout << "Syntax error near delete!" << endl;
        return false;
    }
}

bool Interpreter::insert(const string& tableName, string tableAttribute) {
    tableAttribute.push_back(',');
    regex charRule("[\\s]?'(.*)'[\\s]?");
    regex intRule("[\\s]?([0-9]+)[\\s]?");
    regex floatRule("[\\s]?([0-9]+.[0-9]+)[\\s]?");
    smatch word;
    vector<short> type;
    vector<string> content;
    char *split = strtok(tableAttribute.data(), ",");
    while(split != nullptr){
        string temp(split);
        if(regex_match(temp, word, charRule)){
            content.emplace_back(word[1]);
            type.push_back(word[1].length());
        }
        else if(regex_match(temp, word, intRule)){
            content.emplace_back(word[1]);
            type.push_back(-1);
        }
        else if(regex_match(temp, word, floatRule)){
            content.emplace_back(word[1]);
            type.push_back(0);
        }
        else {
            printf("Syntax error near %s!", split);
            return false;
        }
        split = strtok(nullptr, ",");
    }
    api.insert(tableName, type, content);
}

bool Interpreter::select(string column, string tableName, string condition) {
    column.push_back(',');
    char *split = strtok(column.data(), ",");

    smatch word;
    regex wordRule(R"([\s]?([\w*]+)[\s]?)");
    vector<string> Column;
    while(split != nullptr){
        string temp(split);
        if(regex_match(temp, word, wordRule))
            Column.emplace_back(word[1]);
        split = strtok(nullptr, ",");
    }

    vector<conditionPair> CD;
    if(!condition.empty()){
        condition += " and";
        smatch conditionS;
        regex operatorRule(R"([\s]?([\w]+)[\s]?([><=]+)[\s]?([\w'.]+)[\s]?)");
        while(condition.find("and") != string::npos){
            int find = condition.find("and");
            string temp = condition.substr(0, find);
            if(regex_match(temp, conditionS, operatorRule)){
                conditionPair in;
                in.columnName = conditionS[1];
                if(conditionS[2] == ">") in.OP = Greater;
                else if(conditionS[2] == "<") in.OP = Less;
                else if(conditionS[2] == "=") in.OP = Equal;
                else if(conditionS[2] == "<>") in.OP = NotEqual;
                else if(conditionS[2] == ">=") in.OP = GreaterEqual;
                else if(conditionS[2] == "<=") in.OP = LessEqual;
                else {
                    cout << "Invalid operator " << conditionS[2] << endl;
                    return false;
                }
                regex charRule(R"([']([\w]+)['])");
                regex intRule("[0-9]+");
                regex floatRule("[0-9.]+");
                smatch tempS;
                string conditionTest = conditionS[3];
                if(regex_match(conditionTest, tempS, charRule)) {
                    in.type = tempS[1].length();
                    in.condition = tempS[1];
                }
                else if(regex_match(conditionTest, intRule)) {
                    in.type = -1;
                    in.condition = conditionTest;
                }
                else if(regex_match(conditionTest, floatRule)) {
                    in.type = 0;
                    in.condition = conditionTest;
                }
                CD.emplace_back(in);
            }
            condition = condition.substr(find+3);
        }
    }
    return api.select(Column, std::move(tableName), CD);
}

bool Interpreter::execfile(const string &fileName) {
    ifstream file;
    file.open(fileName, ios::in);
    if(!file.is_open()) {
        printf("File doesn't exist!\n");
        return false;
    }
    clock_t start, end;
    start = clock();
    string SQL, line;
    while (!file.eof()) {
        getline(file, line);
        SQL += line;
        if(line.find(';') != std::string::npos){
            printf("miniSQL>  %s\n", SQL.data());
            interpret(SQL);
            SQL = "";
        }
    }
    end = clock();
    printf("Success in %.3fs.\n", double (end - start)/CLOCKS_PER_SEC);
    file.close();
}
