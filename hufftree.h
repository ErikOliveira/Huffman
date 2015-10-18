#ifndef HUFFTREE_H
#define HUFFTREE_H
#include <QDebug>
#include <QHash>
#include <QBitArray>
#include <QFile>
#include "huffnode.h"


class HuffTree
{
    Huffnode *m_root;
    Huffnode *m_current;

    quint64 m_size;
    QString *m_treePreOrderTransversed;

    QHash<uchar,QBitArray> *m_hash;

    void recursiveTransverseStringGen(QString &preOrderTreeLikeStringHere, Huffnode *apontador);
    void countingNodesHelper(qint64& nNodes, Huffnode* apontador,bool toggleJustLeafsOrAny);
    bool recursiveTransverseToRetrieve(QFile*& storageFile);
public:

    //Constructor & Desconstructors
    HuffTree();
    ~HuffTree();

    //Settlers & Getters
        //Settlers
        void setCurrent(Huffnode *current);
        void setRoot(Huffnode *root);
        void setTreeSize(quint64 &size);
        void setPreOrderTransversedString(QString *string);
        void setTemp(const QString &string);
        void setHash(QHash<uchar,QBitArray> *hash);

        //Getters
        Huffnode *getRoot() const;
        Huffnode *getCurrent();
        quint64 getTreeSize() const;
        QString *getTransversedTreeString() const;
        QHash<uchar, QBitArray> *getHash() const;

    //Miscellaneous
    void transverseToPreOrderStringGen();

    void toLeft();
    void toRight();
    void toRoot();

    bool retriveDataByTransversed(bool zeroIsLeftOneIsRight, QFile*& repOriginalData);

    qint64 countLeafs();
    qint64 countAllNodes();
};

#endif // HUFFTREE_H
