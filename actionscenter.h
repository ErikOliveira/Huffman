#ifndef ACTIONSCENTER_H
#define ACTIONSCENTER_H
#include <QBitArray>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QList>
#include <QPair>
#include "huffnode.h"
#include "hufftree.h"
#include "bitarray.h"

class ActionsCenter
{
protected:
    static bool HuffnodeAlessthanB(Huffnode *a, Huffnode *b);
    void treeRecursiveBuildCodingHelper(QByteArray &transversedRepresentation, QString &huffCodePatternString, QVector<QString> &huffCodingPatternsRepository, Huffnode *apontador, const Huffnode *raiz);
    void treeRecursiveRebuildNodeHandler(QByteArray &transversedRepresentation, Huffnode *&apontador);
    void treeRecursiveRebuildHelper(QByteArray &transversedRepresentation, Huffnode*& apontador);
    QBitArray turnStringInBitArray(QString &string);
    void completePseudoBinNumString(QString &string, int completeToThisSize);
public:
    ActionsCenter();
    ~ActionsCenter();

    //Actions for build Hufftree - all actions is 'tree' prefixed. Except that one considered StandAlone

        //Step 0 - Count Bindata from file. It will userful many time, so must WORK standalone
        void makeCount(QFile *userInFile, quint8 *arrayOfOccurrences);

        //Step 1 - Creating list of nodes
        void treeGenNode(QList<Huffnode *> &listLikeNodeRepository, quint8 *arrayOfOccurrences);

        //Step 2 - It will create a list with one unique node at its zero index which it is the root of tree
        void treeOrdering(QList<Huffnode *> &listLikeTree);

        //Step 3 - Make coding for tree node
        void treeCoding(QByteArray &transversedTree, QVector<QString> &huffCodingPatternsRepository, Huffnode *pointer, Huffnode* root);

        //Step 4 - Wizzard builder caller. Use it instead of calling step by step
        void treeBuilder(HuffTree *&holderBuiltTree, quint8 *arrayOfOccurrences);

    //Actions for make compressed file - all actions is 'comp' prefixed. Except that one considered StandAlone

        //Step 1 - Calculate trash size. That's means, how many bits is necessary for turn a bitarray chunkable in bytes perfectly
        int compCalcBinTrashSize(QVector<QString> &huffCodingPatternsRepository, quint8 *arrayOfOccurrences);

        //Step 2 - Calculate tree size. That's means, how long is its your preOrderedString representation
        quint64 compCalcTreeSize(QString &preOrderedTree);

        //Step 3 - Calculate Filename size. That's means, how long is your nameString representation
        QByteArray compCalcFilenameSize(QFile *&userInFile);

        //Step 4 - Generate the header
        QFile *compGenHeader(QString outCustomName, QString binTrashSize, QString binTreeSize, QString binFilenameSize, const QByteArray &originalName, const QByteArray &treeRep);

        //Step 5 - Generate Huffman encoded file like
        void compGenHuffFile(QFile *userInFile, QFile *outHuffFile, QVector<QString> &huffCodingPatternsRepository, int trashSize);

        //Step 6 - Unite Header and File
        QFile *compUniteFile(QFile *head, QFile *huffFile);

        //Step 7 - Wizard Compress Caller
        void compCompress(const QString &filePath, const QString customOutname);

    //Actions for retrieve tree - all actions are 'tree' prefixed. Except that one considered StandAlone

        //Step 1 - Wizzard rebuild caller.
        Huffnode *treeRebuildFromString(QByteArray &preOrderedTransversedTree);

    //Actions for make decompressing a file - all actions is 'decomp' prefixed. Except that one considered StandAlone

        //Step 1 - read the huff header
        bool decompReadingHeader(QFile *userInFile, QByteArray &preOrderedTransversedTree, QByteArray &fileNameString, qint8& trashNumSize, qint64& treeNumSize, qint64& fileNumSize);

        //Step 2 - retrieve original data
        bool decompRetrieveOriginalData(QByteArray customOutName , QFile *userCompFile, HuffTree *&tree, const qint64 treeSize, const qint64 filenameSize, const qint8 trash);

        //Step 3 - Wizard DesCompress Caller.
        int decompDecompress(const QString &filepath);

    //In & out strings paths handler
        QPair<bool,QString> compIOPaths(const QString &inPath, const QString &outPath); //returns true if provided paths are usable
        bool decompIOPaths(const QString &inPath, const QString &outDirPath); //returns true if provided paths are usable
};

#endif // ACTIONSCENTER_H
