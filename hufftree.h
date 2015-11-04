#ifndef HUFFTREE_H
#define HUFFTREE_H
#include <QDebug>
#include <QHash>
#include <QVector>
#include <QBitArray>
#include <QFile>
#include "huffnode.h"


class HuffTree
{
    Huffnode *m_root;
    Huffnode *m_current;

    quint64 m_size;
    QByteArray m_treePreOrderTransversed;

    QHash<uchar,QString> m_hash;
    QVector<QString> m_vector;

    void recursiveTransverseRepresentationGen(QByteArray &preOrderTreeRepresentationHere, Huffnode *apontador); //Edit signature/prototype
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
        void setPreOrderTransversedRepresentation(QByteArray &representation); //Edit signature/prototype
        void setHash(QHash<uchar, QString> &hash);
        void setVector(QVector<QString> &vector);

        //Getters
        Huffnode *getRoot() const;
        Huffnode *getCurrent();
        quint64 getTreeSize() const;
        QByteArray getTransversedTreeRepresentation() const; //Edit signature/prototype
        QHash<uchar, QString> getHash() const;
        QVector<QString> getVector() const;

    //Miscellaneous
    void transverseToPreOrderRepresentationGen(); //Edit signature/prototype

    void toLeft();
    void toRight();
    void toRoot();

    bool retriveDataByTransversed(bool zeroIsLeftOneIsRight, QFile*& repOriginalData);

    qint64 countLeafs();
    qint64 countAllNodes();
};

#endif // HUFFTREE_H
