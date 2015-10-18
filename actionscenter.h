#ifndef ACTIONSCENTER_H
#define ACTIONSCENTER_H
#include <QBitArray>
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QList>
#include "huffnode.h"
#include "hufftree.h"
#include "bitarray.h"

class ActionsCenter
{
protected:
    static bool HuffnodeAlessthanB(Huffnode *a, Huffnode *b);
    void treeRecursiveBuildCodingHelper(QString &transversedString, QString &huffEncodedCharString, QHash<uchar, QBitArray> &hashOfEncodedChar, Huffnode *apontador, const Huffnode *raiz);
    void treeRecursiveRebuildNodeHandler(const QString &string, qint64 &currentCharIndex, Huffnode *&apontador);
    void treeRecursiveRebuildHelper(const QString &transversedString, qint64& currentCharIndex, Huffnode*& apontador);
    QBitArray turnStringInBitArray(QString &string);
    void completePseudoBinNumString(QString &string, int completeToThisSize);
    void treeRecursiveTransverse(QString &transversedString, Huffnode *apontador);
public:
    ActionsCenter();
    ~ActionsCenter();

    //Actions for build Hufftree - all actions is 'tree' prefixed. Except that one considered StandAlone

        //Step 1* - Count Bindata from file. * = It will userful many time, so must WORK standalone
    void makeCount(QFile *userInFile, quint8 *arrayOfOccurrences);

        //Step 2 - Creating list of nodes
    void treeGenNode(QList<Huffnode *> &listLikeNodeRepository, quint8 *arrayOfOccurrences);

        //Step 3 - It will create a list with one unique node at its zero index which it is the root of tree
    void treeOrdering(QList<Huffnode *> &listLikeTree);

        //Step 4 - Make coding for tree node
    void treeCoding(QString *transverseTree, QHash<uchar,QBitArray> *hashTree, Huffnode *pointer, Huffnode* root);

        //Step 5 - Wizzard builder caller. Use it instead of calling step by step
    void treeBuilder(HuffTree *&builtTreeHolder, quint8 *arrayOfOccurrences);

    //Actions for make compressed file - all actions is 'comp' prefixed. Except that one considered StandAlone

        //Step 1 - Calculate trash size. That's means, how many bits is necessary for turn a bitarray chunkable in bytes perfectly
    int compCalcBinTrashSize(QHash<uchar,QBitArray> &hashedNodes, quint8 *arrayOfOccurrences);

        //Step 2 - Calculate tree size. That's means, how long is its your preOrderedString representation
    quint64 compCalcTreeSize(QString &preOrderedTree);

        //Step 3 - Calculate Filename size. That's means, how long is your nameString representation
    int compCalcFilenameSize(QFile *userInFile);

        //Step 4 - Generate the header
    QFile *compGenHeader(QBitArray binTrashSize, QBitArray binTreeSize, QBitArray binFilenameSize);

        //Step 5 - Generate Huffman encoded file like
    QFile *compGenHuffFile(QFile *userInFile, QHash<uchar,QBitArray> &hashedNodes, int TrashSize);

        //Step 6 - Unite Header and File
    QFile *compUniteFile(QFile *head, QFile *huffFile);

        //Step 7 - Wizzard Compress Caller
    void compCompress(const QString &filePath, const QString customOutname);

    //Actions for retrieve tree - all actions are 'tree' prefixed. Except that one considered StandAlone

        Huffnode *treeRebuildFromString(const QString &PreOrderedTransversedTree);

        //Actions for make decompressing a file - all actions is 'decomp' prefixed. Except that one considered StandAlone

        bool decompReadingHeader(QFile *userInFile, QString &preOrderedTransversedTree, QString& fileNameString, qint8& trashNumSize, qint64 treeNumSize,qint64 fileNumSize);
        QFile* decompRetrieveOriginalData(QFile *userCompFile, HuffTree *&tree);
};

#endif // ACTIONSCENTER_H
