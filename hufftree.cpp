#include "hufftree.h"


void HuffTree::recursiveTransverseStringGen(QString &preOrderTreeLikeStringHere, Huffnode *apontador)
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
                preOrderTreeLikeStringHere.append("!*");
                return;
            }

            else if(leafCharHolder == '!')
            {
                //qDebug()<<"... Its content is default char !, so it will be marked";
                preOrderTreeLikeStringHere.append("!!");
                return;
            }

            else
            {
                //qDebug()<<"... Its content has no any restriction, so it will not be modified";
                preOrderTreeLikeStringHere.append(apontador->getCharacter());
                return;
            }
        }

        else
        {
            //qDebug()<<"Current node isn't a leaf. Default char * will be put in place of it into transversed string";
            preOrderTreeLikeStringHere.append('*');

            if(apontador->getLeft())
            {
                //qDebug()<<"Left node, it isn't Nullpointer. Going by left...";
                recursiveTransverseStringGen(preOrderTreeLikeStringHere,apontador->getLeft());
            }

            if(apontador->getRight())
            {
                //qDebug()<<"Right node, it isn't Nullpointer. Going by right...";
                recursiveTransverseStringGen(preOrderTreeLikeStringHere,apontador->getRight());
            }
        }
    }
}

void HuffTree::countingNodesHelper(qint64 &nNodes, Huffnode *apontador, bool toggleJustLeafsOrAny)
{
    if(apontador)
    {
        if(apontador->isLeaf())
        {
            nNodes++;
        }
        else
        {
            if(toggleJustLeafsOrAny) nNodes++;
            countingNodesHelper(nNodes,apontador->getLeft(),toggleJustLeafsOrAny);
            countingNodesHelper(nNodes,apontador->getRight(),toggleJustLeafsOrAny);
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
            qDebug() << "it's leaf...";
            if(m_current->getFrequency() == 1)
            {
                QString data = "";
                data += m_current->getCharacter();
                qDebug()<<"Its leaf, jesus |" << "it's content:    "<<data;
                storageFile->write(data.toUtf8(),data.size());
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
            qDebug()<<"non-leaf reach, ok trying move again...";
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
    m_treePreOrderTransversed = new QString();
    m_hash = new QHash<uchar,QBitArray>();
}

HuffTree::~HuffTree()
{
    delete m_treePreOrderTransversed;
    delete m_hash;
}

void HuffTree::setCurrent(Huffnode *current)
{
    m_current = current;
}

void HuffTree::setRoot(Huffnode *root)
{
    if(!m_root) m_root = m_current = root;
    else m_root = root;
}

void HuffTree::setTreeSize(quint64 &size)
{
    m_size = size;
}

void HuffTree::setPreOrderTransversedString(QString *string)
{
    delete m_treePreOrderTransversed;
    m_treePreOrderTransversed = string;
    m_size = string->size();
}

void HuffTree::setTemp(const QString &string)
{
    m_treePreOrderTransversed->clear();
    m_treePreOrderTransversed->append(string);
    m_size = m_treePreOrderTransversed->size();
}

void HuffTree::setHash(QHash<uchar, QBitArray> *hash)
{
    delete m_hash;
    m_hash = hash;
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

QString *HuffTree::getTransversedTreeString() const
{
    return m_treePreOrderTransversed;
}

QHash<uchar, QBitArray> *HuffTree::getHash() const
{
    return m_hash;
}

void HuffTree::transverseToPreOrderStringGen()
{
    if(m_treePreOrderTransversed->isEmpty())
    {
        this->recursiveTransverseStringGen(*m_treePreOrderTransversed,m_root);
    }
    else
    {
        m_treePreOrderTransversed->clear();
        this->recursiveTransverseStringGen(*m_treePreOrderTransversed,m_root);
    }
}

void HuffTree::toLeft()
{
    if(m_current) m_current = m_current->getLeft();
}

void HuffTree::toRight()
{
    if(m_current) m_current = m_current->getRight();
}

void HuffTree::toRoot()
{
    m_current = m_root;
}

bool HuffTree::retriveDataByTransversed(bool zeroIsLeftOneIsRight, QFile *&repOriginalData)
{
    qDebug() << "retrieve caller w: " << zeroIsLeftOneIsRight;

    if(!m_root->isLeaf())
    {
        if(zeroIsLeftOneIsRight) this->toRight();
        else this->toLeft();
    }
    else this->toRoot();

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
