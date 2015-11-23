#ifndef HUFFTREE_H
#define HUFFTREE_H
#include <QDebug>
#include <QHash>
#include <QVector>
#include <QBitArray>
#include <QFile>
#include "huffnode.h"
#include "bitarray.h"


class HuffTree
{
    Huffnode *m_root;
    Huffnode *m_current;

    quint64 m_size;
    QByteArray m_treePreOrderTrasversed;
    QVector<QString> m_vector;


    //Built-in helpers
    void recursiveTrasverseRepresentationGen(QByteArray &preOrderTreeRepresentationHere, Huffnode *apontador); //Edit signature/prototype
    bool recursiveTrasverseToClear(Huffnode *&apontador);
    void loopClearHelper();
    void countingNodesHelper(qint64& nNodes, Huffnode* apontador,bool toggleJustLeafsOrAny);
    bool recursiveTrasverseToRetrieve(QByteArray &pseudoStorageFile);

    //Moved from actions center
    static bool HuffnodeAlessThanB(Huffnode *a, Huffnode *b);
    void recursiveBuildCodingHelper(QByteArray &trasversedRepresentation, QString &huffCodePatternString, QVector<QString> &huffCodingPatternsRepository, Huffnode *apontador, const Huffnode *raiz);
    void recursiveRebuildNodeHandler(QByteArray &trasversedRepresentation, Huffnode *&apontador);
    void recursiveRebuildHelper(QByteArray &trasversedRepresentation, Huffnode*& apontador);

public:

    //Constructor & Desconstructors
    HuffTree();
    ~HuffTree();

    //Settlers & Getters
        //Settlers
        void setCurrent(Huffnode *current);
        void setRoot(Huffnode *&root);
        void setTreeSize(quint64 &size);
        void setPreOrderTrasversedRepresentation(QByteArray &representation); //Edit signature/prototype
        void setVector(QVector<QString> &vector);

        //Getters
        Huffnode *getRoot() const;
        Huffnode *getCurrent();
        quint64 getTreeSize() const;
        QByteArray getTrasversedTreeRepresentation() const; //Edit signature/prototype
        QVector<QString> getVector() const;

    //Miscellaneous
    void trasverseToPreOrderRepresentationGen(); //Edit signature/prototype
    void clear();

    void toLeft();
    void toRight();
    void toRoot();
    bool retriveDataByTrasversed(qint8 trash, BitArray *& bitarray, QByteArray &repOriginalData);

    qint64 countLeafs();
    qint64 countAllNodes();

    //Wizards
        //Builder Steps set
            //Step 1 - Creating list of nodes
            void genNode(QList<Huffnode*> &listLikeNodeRepository, quint64 *arrayOfOccurrences);

            //Step 2 - It will create a list with one unique node at its zero index which it is the root of tree
            void ordering(QList<Huffnode *> &listLikeTree);

            //Step 3 - Make coding for tree node
            void coding();

            //Step 4 - Wizard builder caller. Use it instead of calling step by step
            bool builder(quint64 *arrayOfOccurrences);

        //Rebuilder Steps set
            //Step 1 - Wizard rebuild caller
            bool rebuilder(QByteArray &retrievedRepresentation);
};

#endif // HUFFTREE_H
