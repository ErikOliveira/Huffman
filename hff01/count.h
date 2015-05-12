#ifndef COUNT_H
#define COUNT_H
#include <QDebug>
#include <QFile>

int* count(QFile* archive){
    if(!(archive->open(QIODevice::ReadOnly))){
        qDebug()<<"Error. It can't be open :(";
        return NULL;
    }
    else{

        int *p= new int[256];
        for(int i =0; i<256; i++)
            p[i] = 0;

        QByteArray file = archive->readAll();
        for(int i=0; i<file.size(); i++){
            p[((uchar)file[i])]++;
        }
        return p;
    }
}

#endif // COUNT_H
