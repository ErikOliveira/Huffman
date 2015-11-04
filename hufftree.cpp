#include "hufftree.h"


void HuffTree::recursiveTransverseRepresentationGen(QByteArray &preOrderTreeRepresentationHere, Huffnode *apontador)
{
    //qDebug()<<"Start over:";
    if(apontador)
    {
        if(apontador->isLeaf() & (apontador->getFrequency() == 1))
        {
            uchar leafCharHolder = apontador->getCharacter();
            //qDebug()<<"Leaf node reached...";

            if(leafCharHolder == '*')
            {
                //qDebug()<<"... Its content is default char *, so it will be marked";
                preOrderTreeRepresentationHere.append("!*");
                return;
            }

            else if(leafCharHolder == '!')
            {
                //qDebug()<<"... Its content is default char !, so it will be marked";
                preOrderTreeRepresentationHere.append("!!");
                return;
            }

            else
            {
                //qDebug()<<"... Its content has no any restriction, so it will not be modified";
                preOrderTreeRepresentationHere.append(leafCharHolder);
                return;
            }
        }

        else
        {
            //qDebug()<<"Current node isn't a leaf. Default char * will be put in place of it into transversed string";
            preOrderTreeRepresentationHere.append('*');

            Huffnode *holderSubTree = apontador->getLeft(); //Pex tip cascate applied!
            if(holderSubTree)
            {
                //qDebug()<<"Left node, it isn't Nullpointer. Going by left...";
                recursiveTransverseRepresentationGen(preOrderTreeRepresentationHere,holderSubTree);
            }

            holderSubTree = apontador->getRight(); //Pex tip cascate applied!
            if(holderSubTree)
            {
                //qDebug()<<"Right node, it isn't Nullpointer. Going by right...";
                recursiveTransverseRepresentationGen(preOrderTreeRepresentationHere,holderSubTree);
            }
        }
    }
}

void HuffTree::countingNodesHelper(qint64 &nNodes, Huffnode *apontador, bool toggleJustLeafsOrAny)
{
    if(apontador)
    {
        if(apontador->isLeaf())
            nNodes++;

        else
        {
            if(toggleJustLeafsOrAny)
                nNodes++;

            countingNodesHelper(nNodes, apontador->getLeft(), toggleJustLeafsOrAny);
            countingNodesHelper(nNodes, apontador->getRight(), toggleJustLeafsOrAny);
        }
    }
}

bool HuffTree::recursiveTransverseToRetrieve(QFile *&storageFile)
{
    qDebug() << "transversing";
    if(m_current)
    {
        if(m_current->isLeaf())
        {
            //qDebug() << "it's leaf...";
            if(m_current->getFrequency() == 1)
            {
                QByteArray data = QByteArray();
                data.insert(0,m_current->getCharacter());
                qDebug()<< "it's content:    "<<data.toHex();
                storageFile->write(data,data.size());
                this->toRoot();
                return true;
            }
            else
            {
                qDebug()<< "Did it was made by me? 'Cause retrieve tree leafs all has char freq one";
                this->toRoot();
                return false;
            }
        }
        else
        {
            //qDebug()<<"non-leaf reach, ok trying move again...";
            return true;
        }
    }
    else
    {
        qDebug() << "D'Oh! Null pointered node reach, that's mean that something did much wrong. Not know how to dealing with it";
        storageFile->seek(0);
        storageFile->write("Something did wrong in retrieved data, sorry this is not what you looking for");
        this->toRoot();
        return false;
    }
}

HuffTree::HuffTree()
{
    m_root = NULL;
    m_current = NULL;
    m_size = 0;
    m_treePreOrderTransversed = QByteArray();
    m_hash = QHash<uchar,QString>();
    m_vector = QVector<QString>();
}

HuffTree::~HuffTree()
{
    m_treePreOrderTransversed.clear();
    m_hash.clear();
}

void HuffTree::setCurrent(Huffnode *current)
{
    m_current = current;
}

void HuffTree::setRoot(Huffnode *root)
{
    if(!m_root)
        m_root = m_current = root;
    else
        m_root = root;
}

void HuffTree::setTreeSize(quint64 &size)
{
    m_size = size;
}

void HuffTree::setPreOrderTransversedRepresentation(QByteArray &representation)
{
    if(m_treePreOrderTransversed.operator !=(representation)){
        m_treePreOrderTransversed.clear();
        m_treePreOrderTransversed = representation;
        m_size = representation.size();
    }
}

void HuffTree::setHash(QHash<uchar, QString> &hash)
{
    m_hash.clear();
    m_hash = hash;
}

void HuffTree::setVector(QVector<QString> &vector)
{
    m_vector.clear();
    m_vector = vector;
}

Huffnode *HuffTree::getRoot() const
{
    return m_root;
}

Huffnode *HuffTree::getCurrent()
{
    return m_current;
}

quint64 HuffTree::getTreeSize() const
{
    return m_size;
}

QByteArray HuffTree::getTransversedTreeRepresentation() const
{
    return m_treePreOrderTransversed;
}

QHash<uchar, QString> HuffTree::getHash() const
{
    return m_hash;
}

QVector<QString> HuffTree::getVector() const
{
    return m_vector;
}

void HuffTree::transverseToPreOrderRepresentationGen()
{
    QByteArray nwRepresentation = QByteArray();
    this->recursiveTransverseRepresentationGen(nwRepresentation,m_root);
    this->setPreOrderTransversedRepresentation(nwRepresentation);
}

void HuffTree::toLeft()
{
    if(m_current)
        m_current = m_current->getLeft();
}

void HuffTree::toRight()
{
    if(m_current)
        m_current = m_current->getRight();
}

void HuffTree::toRoot()
{
    m_current = m_root;
}

bool HuffTree::retriveDataByTransversed(bool zeroIsLeftOneIsRight, QFile *&repOriginalData)
{
    //qDebug() << "retrieve caller w: " << zeroIsLeftOneIsRight;

    if(m_root->isLeaf())
        this->toRoot();

    else
    {
        if(zeroIsLeftOneIsRight)
            this->toRight();
        else
            this->toLeft();
    }

    return this->recursiveTransverseToRetrieve(repOriginalData);
}

qint64 HuffTree::countLeafs()
{
    qint64 countedLeafs = 0;
    this->countingNodesHelper(countedLeafs,m_root,false);
    return countedLeafs;
}

qint64 HuffTree::countAllNodes()
{
    qint64 countedNodes = 0;
    this->countingNodesHelper(countedNodes,m_root,true);
    return countedNodes;
}
