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

class ActionsCenter: public QObject
{
    Q_OBJECT
protected:
    void completePseudoBinNumString(QString &string, int completeToThisSize);
public:
    ActionsCenter();
    ~ActionsCenter();

    //Actions for make compressed file - all actions is 'comp' prefixed. Except that one considered StandAlone

        //Step 0 - Count Bindata from file. It will userful many time, so must WORK standalone
        bool makeCount(QFile *userInFile, quint64 *arrayOfOccurrences);

        //Step 1 - Calculate trash size. That's means, how many bits is necessary for turn a bitarray chunkable in bytes perfectly
        int compCalcBinTrashSize(QVector<QString> &huffCodingPatternsRepository, quint64 *arrayOfOccurrences);

        //Step 2 - Calculate Filename size. That's means, how long is your nameString representation
        QByteArray compCalcFilenameSize(QFile *&userInFile);

        //Step 3 - Generate the header
        QFile *compGenHeader(QString outCustomName, QString binTrashSize, QString binTreeSize, QString binFilenameSize, const QByteArray &originalName, const QByteArray &treeRep);

        //Step 4 - Generate Huffman encoded file like
        bool compGenHuffFile(QFile *userInFile, QFile *outHuffFile, QVector<QString> &huffCodingPatternsRepository, int trashSize);

        //Step 5 - Wizard Compress Caller
        int compCompress(const QString &filePath, const QString &customOutname);

    //Actions for make decompressing a file - all actions is 'decomp' prefixed. Except that one considered StandAlone

        //Step 1 - read the huff header
        bool decompReadingHeader(QFile *userInFile, QByteArray &preOrderedTrasversedTree, QByteArray &fileNameString, qint8& trashNumSize, qint64& treeNumSize, qint64& fileNumSize);

        //Step 2 - retrieve original data
        bool decompRetrieveOriginalData(QByteArray customOutName , QFile *userCompFile, HuffTree *&tree, const qint64 treeSize, const qint64 filenameSize, const qint8 trash);

        //Step 3 - Wizard DesCompress Caller.
        int decompDecompress(const QString &filepath);

    //In & out strings paths handler
        QPair<bool,QString> compIOPaths(const QString &inPath, const QString &outPath); //returns true if provided paths are usable
        QPair<bool, QString> decompIOPaths(const QString &inPath, const QString &outDirPath); //returns true if provided paths are usable
};

#endif // ACTIONSCENTER_H
