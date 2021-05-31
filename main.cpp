#include "CommonHead.h"
#include "Interpreter.h"
using namespace std;

int main() {
    Interpreter *c;
    c = new Interpreter;
    string SQL, line;
    printf("-------------miniSQL interface-------------\n");
    printf("miniSQL>");
    while (true){
        getline(cin, line);
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        if(line == "exit" || line == "quit" || line == "exit;" || line == "quit;"){
            delete c;
            cout << "Bye!" << endl;
            break;
        }
        // 如果没有;号
        SQL += line;
        if(line.find(';') != std::string::npos){
            clock_t start, end;
            start = clock();
            c->interpret(SQL);
            end = clock();
            printf(" in %.3fs.\n", double (end-start)/CLOCKS_PER_SEC);
            printf("miniSQL>");
            SQL = "";
        }
    }
}
