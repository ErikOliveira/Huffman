#include <QCoreApplication>
#include "count.h"
#include <iostream>
using namespace std;
using std::cin;
using std::cout;
using std::getline;
using std::string;
using std::endl;

int main()
{

    string path;
    getline(cin,path);
    QFile *archive = new QFile(path.c_str());
    int *p = count(archive);
    if(p)
        for(int i=0; i<256; i++){
            if(p[i])
                printf("Ascii char: %c | Frequency: %d\n", i,p[i]);
    }
    return 0;
}
