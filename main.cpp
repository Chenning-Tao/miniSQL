#include "CommonHead.h"
#include "Interpreter.h"
using namespace std;

int main() {
    Interpreter *c;
    c = new Interpreter;
    string SQL, line;
    printf("-------------miniSQL interface-------------\n");
    printf("miniSQL>  ");
    while (true){
        getline(cin, line);
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        if(line == "exit" || line == "quit" || line == "exit;" || line == "quit;"){
            delete c;
            printf("Bye!\n");
            break;
        }
        // 如果没有;号
        SQL += line;
        if(line.find(';') != std::string::npos){
            c->interpret(SQL);
            printf("miniSQL>  ");
            SQL = "";
        }
    }
}
