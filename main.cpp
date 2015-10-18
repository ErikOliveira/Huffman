#include <QCoreApplication>
#include "actionscenter.h"
#define CompInFileName ("C:/users/Erik/documents/tst.txt")
#define CompOutFilename ("compTest")
#define DecompFileName ("C:/Users/Erik/Documents/GitHub/Huffman/build-Huffman-Desktop_Qt_5_4_1_MinGW_32bit-Debug/header.tmp")
#define TreeString ("**!**!!A*b**_ca")

/* treeStrings...

//  *a**cb**fed - Huffsample.txt
//  **!**!!A*b**_ca - test1
//  *E**UD*L*C**ZKM - tst.txt

*/

void MakeaTest(HuffTree *&p)
{
    /*qDebug()<<"QList size test. E: 1 | R: "<<QString::number(p->getListLikeRoot()->size())<<"QList print its contain test.\n";
    for(int i =0; i<p->getListLikeRoot()->size(); ++i)
    {
        qDebug("List item idex %d:\n        Ascii node char %c | Ascii node freq %d\n##################\n", i, p->getListLikeRoot()->at(i)->getCharacter(), p->getListLikeRoot()->at(i)->getFrequency());
    }/**/
    /*qDebug()<<"Forced leaf search test...\nE: !* | R:  "<<qPrintable(p->getRoot()->getLeft()->getLeft()->getCharacter())
           <<"\nE: !! | R: "<<qPrintable(p->getRoot()->getLeft()->getRight()->getLeft()->getCharacter())<<"\nE: A | R: "
          <<qPrintable(p->getCurrent()->getLeft()->getRight()->getRight()->getCharacter())<<"\nE: b | R: "
         <<qPrintable(p->getRoot()->getRight()->getLeft()->getCharacter())<<"\nE: _ | R: "
        <<qPrintable(p->getRoot()->getRight()->getRight()->getLeft()->getLeft()->getCharacter())<<"\nE: c | R: "
       <<qPrintable(p->getRoot()->getRight()->getRight()->getLeft()->getRight()->getCharacter())<<"\nE: a | R: "
      <<qPrintable(p->getRoot()->getRight()->getRight()->getRight()->getCharacter());/**/

    /*qDebug()<<"Forced leaf search test 2\nE: E | R: "<<qPrintable(p->getRoot()->getLeft()->getCharacter())<<"\nE: U | R: "
           <<qPrintable(p->getRoot()->getRight()->getLeft()->getLeft()->getCharacter())<<"\nE: D | R: "
          <<qPrintable(p->getRoot()->getRight()->getLeft()->getRight()->getCharacter())<<"\nE: L | R: "
         <<qPrintable(p->getRoot()->getRight()->getRight()->getLeft()->getCharacter())<<"\nE: C | R: "
        <<qPrintable(p->getRoot()->getRight()->getRight()->getRight()->getLeft()->getCharacter())<<"\nE: Z | R: "
       <<qPrintable(p->getRoot()->getRight()->getRight()->getRight()->getRight()->getLeft()->getLeft()->getCharacter())<<"\nE: K | R: "
      <<qPrintable(p->getRoot()->getRight()->getRight()->getRight()->getRight()->getLeft()->getRight()->getCharacter())<<"\nE: M | R: "
     <<qPrintable(p->getRoot()->getRight()->getRight()->getRight()->getRight()->getRight()->getCharacter());/**/

    QList<uchar> list= p->getHash()->keys();
    BitArray helper = BitArray(1024);
    qDebug()<<"list size:   "<<QString::number(list.size());
    for(int i = 0; i<list.size(); ++i)
    {
        helper.addBitArray(p->getHash()->value(list.at(i)));
        qDebug()<<"After rebuild tree:\nKey value:  "<<qPrintable(list.at(i))
               <<"  CODING: "<<helper.getReadableBitArray();
        helper.erase();
    }

}

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

    ActionsCenter *huffman = new ActionsCenter();

    huffman->compCompress(CompInFileName,CompOutFilename);

    qDebug() << "CompressDone!";

    QFile *mytest = new QFile(DecompFileName);
    QString data;
    QString filename;
    qint8 trash = 0;
    qint64 file = 0;
    qint64 tree = 0;
//    huffman->decompReadingHeader(mytest,data,trash);

    //    delete mytest;
    //    mytest = NULL;
    //    mytest = new QFile("C:/users/Erik/documents/test1");
    //    quint8 mtest[256] = {0};
    //    huffman->makeCount(mytest,mtest);
    //    HuffTree *mytree = new HuffTree();
    //    huffman->treeBuilder(mytree,mtest);

/*//    data = TreeString;

    //    mytree->getTransversedTreeString();

    //    qDebug()<< "tree from string" << data;

//    HuffTree* mytree2 = new HuffTree();
//    mytree2->setRoot(huffman->treeRebuildFromString(data));
//    qDebug()<<"after rebuild done!"
//           <<"\nTree nodes quant.: "<<QString::number(mytree2->countAllNodes())
//          <<"\nTree leaf nodes quant.:"<<QString::number(mytree2->countLeafs());
//    mytree2->transverseToPreOrderStringGen();
//    huffman->treeCoding(mytree2->getTransversedTreeString(),mytree2->getHash(),mytree2->getCurrent(),mytree2->getRoot());
//    MakeaTest(mytree2);


    //    delete mytest;
    //    mytest = NULL;
    //    mytest = new QFile("C:/Users/Erik/Documents/GitHub/Huffman/build-Huffman-Desktop_Qt_5_4_1_MinGW_32bit-Debug/huff.tmp");
    huffman->decompRetrieveOriginalData(mytest,mytree2);/*a*/

    huffman->decompReadingHeader(mytest,data,filename,trash,tree,file);
    return 0;
}
