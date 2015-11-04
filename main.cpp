#include <QCoreApplication>
#include "actionscenter.h"
#define CompInFileName ("C:/users/Erik/documents/test1") //E:/Windows 7 Ultimate x32 e x64.iso //C:/users/Erik/documents/test1
#define CompOutFilename ("C:/users/Erik/documents/test1-2.huff")
#define DecompFileName ("C:/users/Erik/documents/test1.huff") //C:/Users/Erik/Documents/GitHub/build-Huffman-Desktop_Qt_5_4_1_MinGW_32bit-Debug/header.tmp
#define DecompOutDirName ("C:/users/Erik/downloads") //

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ActionsCenter *huffman = new ActionsCenter();
    int rCode = 0xFF;

    QPair<bool, QString> programFlag = huffman->compIOPaths(CompInFileName,CompOutFilename);

    if(programFlag.first)
        huffman->compCompress(CompInFileName,programFlag.second);


    /*bool pgmFlag = huffman->decompIOPaths(DecompFileName,DecompOutDirName);
    if(pgmFlag)
       rCode = huffman->decompDecompress(DecompFileName);*/

    return rCode;
}
