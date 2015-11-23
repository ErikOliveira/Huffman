#include "hufftree.h"

void HuffTree::recursiveTrasverseRepresentationGen(QByteArray &preOrderTreeRepresentationHere, Huffnode *apontador)
{
    if(apontador)
    {
        if(apontador->isLeaf() & (apontador->getFrequency() == 1))
        {
            uchar leafCharHolder = apontador->getCharacter();

            if(leafCharHolder == '*')
            {
                preOrderTreeRepresentationHere.append("!*");
                return;
            }

            else if(leafCharHolder == '!')
            {
                preOrderTreeRepresentationHere.append("!!");
                return;
            }

            else
            {
                preOrderTreeRepresentationHere.append(leafCharHolder);
                return;
            }
        }

        else
        {
            preOrderTreeRepresentationHere.append('*');

            Huffnode *holderSubTree = apontador->getLeft(); //Pex tip cascade applied!
            if(holderSubTree)
            {
                recursiveTrasverseRepresentationGen(preOrderTreeRepresentationHere,holderSubTree);
            }

            holderSubTree = apontador->getRight(); //Pex tip cascade applied!
            if(holderSubTree)
            {
                recursiveTrasverseRepresentationGen(preOrderTreeRepresentationHere,holderSubTree);
            }
        }
    }
}

bool HuffTree::recursiveTrasverseToClear(Huffnode *&apontador)
{
    if(apontador)
    {
        if(apontador->isLeaf())
        {
            delete apontador;
            return true;
        }

        else
        {
            Huffnode *holderSubTree = apontador->getLeft(); //Pex tip cascade applied!
            bool flag;

            flag = recursiveTrasverseToClear(holderSubTree);
            if(flag)
                    apontador->setLeft(NULL);

            holderSubTree = apontador->getRight(); //Pex tip cascade applied!

            flag = recursiveTrasverseToClear(holderSubTree);
            if(flag)
                apontador->setRight(NULL);
        }
    }
    else
        return false;
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

bool HuffTree::recursiveTrasverseToRetrieve(QByteArray &pseudoStorageFile)
{
    if(m_current)
    {
        if(m_current->isLeaf())
        {
            if(m_current->getFrequency() == 1)
            {
                pseudoStorageFile.append((quint8(m_current->getCharacter())));
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
        pseudoStorageFile.clear();
        this->toRoot();
        return false;
    }
}


bool HuffTree::HuffnodeAlessThanB(Huffnode *a, Huffnode *b)
{
    if(a->getFrequency() == b->getFrequency())
        return ((a->getCharacter()) < (b->getCharacter()));
    else
        return ((a->getFrequency()) < (b->getFrequency()));
}

void HuffTree::recursiveBuildCodingHelper(QByteArray &trasversedRepresentation, QString &huffCodePatternString, QVector<QString> &huffCodingPatternsRepository, Huffnode *apontador, const Huffnode *raiz)
{
    if(apontador)
    {
        if(apontador->isLeaf())
        {
            uchar leafCharHolder = apontador->getCharacter();

            if(leafCharHolder == '*')
            {
                trasversedRepresentation.append("!*");

                if(apontador == raiz)
                    huffCodePatternString.append('0');

                huffCodingPatternsRepository[leafCharHolder] = huffCodePatternString;
                return;
            }

            else if(leafCharHolder == '!')
            {
                trasversedRepresentation.append("!!");

                if(apontador == raiz)
                    huffCodePatternString.append('0');

                huffCodingPatternsRepository[leafCharHolder] = huffCodePatternString;
                return;
            }

            else
            {
                trasversedRepresentation.append(leafCharHolder);

                if(apontador == raiz)
                    huffCodePatternString.append('0');

                huffCodingPatternsRepository[leafCharHolder] = huffCodePatternString;
                return;
            }
        }

        else
        {
            trasversedRepresentation.append('*');

            Huffnode *holderSubTree = apontador->getLeft(); //Pex tip cascade applied!
            if(holderSubTree)
            {
                huffCodePatternString.append('0');
                recursiveBuildCodingHelper(trasversedRepresentation,huffCodePatternString,huffCodingPatternsRepository,holderSubTree,raiz);
                huffCodePatternString.chop(1);
            }

            holderSubTree = apontador->getRight(); //Pex tip cascade applied!

            if(holderSubTree)
            {
                huffCodePatternString.append('1');
                recursiveBuildCodingHelper(trasversedRepresentation,huffCodePatternString,huffCodingPatternsRepository,holderSubTree,raiz);
                huffCodePatternString.chop(1);
            }
        }
    }
}

void HuffTree::recursiveRebuildNodeHandler(QByteArray &trasversedRepresentation, Huffnode *&apontador)
{
    if(trasversedRepresentation.isEmpty())
    {
        qDebug()<<"End of representation. No remain data to  be processed";
    }
    else
    {
        if((uchar(trasversedRepresentation[0])) == 0x21)
        {
            apontador->setFrequency(1);
            apontador->setCharacter((uchar(trasversedRepresentation[1])));
            apontador->makeItLeaf();

            trasversedRepresentation.remove(0,2);
            return;
        }
        else if((uchar (trasversedRepresentation[0])) == 0x2A)
        {
            apontador->setFrequency(0);
            apontador->setCharacter((uchar(trasversedRepresentation[0])));
            apontador->setLeft((new Huffnode()));
            apontador->setRight((new Huffnode()));

            trasversedRepresentation.remove(0,1);
            return;
        }
        else
        {
            apontador->setFrequency(1);
            apontador->setCharacter((uchar(trasversedRepresentation[0])));
            apontador->makeItLeaf();

            trasversedRepresentation.remove(0,1);
            return;
        }
    }
}

void HuffTree::recursiveRebuildHelper(QByteArray &trasversedRepresentation, Huffnode *&apontador)
{
    if(apontador)
    {

        this->recursiveRebuildNodeHandler(trasversedRepresentation,apontador);

        Huffnode *holderSubTree = apontador->getLeft(); //Pex tip cascade applied!
        if(holderSubTree)
            this->recursiveRebuildHelper(trasversedRepresentation,holderSubTree);

        holderSubTree = apontador->getRight(); //Pex tip cascade applied!
        if(holderSubTree)
            this->recursiveRebuildHelper(trasversedRepresentation,holderSubTree);
    }
}

HuffTree::HuffTree()
{
    m_root = NULL;
    m_current = NULL;
    m_size = 0;
    m_treePreOrderTrasversed = QByteArray();
    m_vector = QVector<QString>();
}

HuffTree::~HuffTree()
{
    m_treePreOrderTrasversed.clear();
    m_vector.clear();
    this->clear();
}

void HuffTree::setCurrent(Huffnode *current)
{
    m_current = current;
}

void HuffTree::setRoot(Huffnode *&root)
{
    if(!m_root)
    {
        m_root = m_current = root;
        m_treePreOrderTrasversed.clear();
    }

    else
    {
        this->clear();
        this->setRoot(root);
    }
}

void HuffTree::setTreeSize(quint64 &size)
{
    m_size = size;
}

void HuffTree::setPreOrderTrasversedRepresentation(QByteArray &representation)
{
    if(m_treePreOrderTrasversed.operator !=(representation)){
        m_treePreOrderTrasversed.clear();
        m_treePreOrderTrasversed = representation;
        m_size = representation.size();
    }
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

QByteArray HuffTree::getTrasversedTreeRepresentation() const
{
    return m_treePreOrderTrasversed;
}

QVector<QString> HuffTree::getVector() const
{
    return m_vector;
}

void HuffTree::trasverseToPreOrderRepresentationGen()
{
    QByteArray nwRepresentation = QByteArray();
    this->recursiveTrasverseRepresentationGen(nwRepresentation,m_root);
    this->setPreOrderTrasversedRepresentation(nwRepresentation);
}

void HuffTree::clear()
{
    this->recursiveTrasverseToClear(m_root);

    if(m_root->isLeaf())
    {
        delete m_root;
        m_root = m_current = NULL;
        m_treePreOrderTrasversed.clear();
    }
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

bool HuffTree::retriveDataByTrasversed(qint8 trash, BitArray *&bitarray, QByteArray &repOriginalData)
{
    bool flag = true;

    for(qint64 i = bitarray->pseudoSize(); (i>trash) & flag; --i)
    {
        if(!m_root->isLeaf())
        {
            if(bitarray->pseudoBeginValue())
                this->toRight();
            else
                this->toLeft();

            bitarray->pseudoFF();
        }
        else
        {
            this->toRoot();
            bitarray->pseudoFF();
        }

        flag = this->recursiveTrasverseToRetrieve(repOriginalData);
    }

    if(bitarray->pseudoEmpty())
        bitarray->erase();
    else
        bitarray->pseudoErase();

    return flag;
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

void HuffTree::genNode(QList<Huffnode *> &listLikeNodeRepository, quint64 *arrayOfOccurrences)
{
    Q_ASSERT_X(listLikeNodeRepository.empty(), Q_FUNC_INFO, "D'oh! It works better in a empty list, cause things could messed up into it. So avoiding your Headache, nothing was performed. *Except for 'is-empty-test' itself, of sure :D");

    Huffnode *auxNodeHolder = NULL;

    for(qint64 i=0; i<256; ++i)
    {
        if(arrayOfOccurrences[i])
        {
            auxNodeHolder = new Huffnode(arrayOfOccurrences[i],(uchar(i)),NULL,NULL);
            listLikeNodeRepository.append(auxNodeHolder);
        }
    }

    auxNodeHolder = NULL;
}

void HuffTree::ordering(QList<Huffnode *> &listLikeTree)
{
    qint64 nmListSize = listLikeTree.size(); //Pex tip cascade applied!
    while(nmListSize > 1)
    {
        qSort(listLikeTree.begin(),listLikeTree.end(),this->HuffnodeAlessThanB);

        Huffnode *auxNodeHolder = new Huffnode(((listLikeTree.at(0)->getFrequency()) + (listLikeTree.at(1)->getFrequency())), '*',  listLikeTree.at(0), listLikeTree.at(1));
        listLikeTree.removeFirst(); // -1
        listLikeTree.removeFirst(); // -1
        listLikeTree.prepend(auxNodeHolder); //+1
        --nmListSize; // nmlistsize -1 -1 +1 = numlistsize-1
    }
}

void HuffTree::coding()
{
    QString auxCharCodingHolder = "";
    QByteArray preOrderedRepresentation = QByteArray();
    QVector<QString> codedPatternsRepository = QVector<QString>(256,"");

    this->recursiveBuildCodingHelper(preOrderedRepresentation,auxCharCodingHolder,codedPatternsRepository,m_current,m_root);
    this->setPreOrderTrasversedRepresentation(preOrderedRepresentation);
    this->setVector(codedPatternsRepository);
}

bool HuffTree::builder(quint64 *arrayOfOccurrences)
{
    qDebug()<<"\t\t\t--- Task (1/3): Gen Leafs Nodes ---";

    QList<Huffnode*> auxListLikeTreeHolder = QList<Huffnode*>(); //removed new operator
    this->genNode(auxListLikeTreeHolder, arrayOfOccurrences);
    qDebug()<<"\t\t\t\t\tGenered!\n\t\t\t-----------------------------------\n\n";

    qDebug()<<"\t\t\t--- Task (2/3): Ordering Nodes\t---";

    this->ordering(auxListLikeTreeHolder); //removed desref. pointer
    Huffnode *aux = auxListLikeTreeHolder.at(0);
    this->setRoot(aux); //Edit
    qDebug()<<"\t\t\t\t\tOrdered!\n\t\t\t-----------------------------------\n\n";

    if(m_root != NULL)
    {

        qDebug()<<"\t\t\t--- Task (3/3): Rep. & Coding\t---";
        this->coding(); //Edit

        qDebug()<<"\t\t\t\t\tCoded!\n\t\t\t-----------------------------------\n\n\t\t\t\tTree Built!";

        return true;
    }

    else
    {
        return false;
    }
}

bool HuffTree::rebuilder(QByteArray &retrievedRepresentation)
{
    Q_ASSERT_X(!retrievedRepresentation.isEmpty(), Q_FUNC_INFO, "D'Oh! I can't handle with a empty string. Did you has been already created it tree, dind't you?");

    Huffnode* pseudoRoot = new Huffnode();
    this->recursiveRebuildHelper(retrievedRepresentation,pseudoRoot);
    this->setRoot(pseudoRoot);

    if(m_root != NULL)
        return true;

    else
        return false;
}
