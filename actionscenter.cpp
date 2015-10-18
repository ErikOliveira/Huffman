#include "actionscenter.h"

bool ActionsCenter::HuffnodeAlessthanB(Huffnode *a, Huffnode *b)
{
    if(a->getFrequency() == b->getFrequency())
        return ((a->getCharacter()) < (b->getCharacter()));
    else
        return ((a->getFrequency()) < (b->getFrequency()));
}

void ActionsCenter::treeRecursiveBuildCodingHelper(QString &transversedString, QString &huffEncodedCharString, QHash<uchar, QBitArray> &hashOfEncodedChar, Huffnode *apontador, const Huffnode *raiz)
{
    //qDebug()<<"Start over:";
    if(apontador)
    {
        if(apontador->isLeaf())
        {
            uchar leafCharHolder = apontador->getCharacter();
            //qDebug()<<"Leaf node reached...";
            if(leafCharHolder == '*')
            {
                //qDebug()<<"... Its content is default char *, so it will be marked";
                transversedString.append("!*");
                if(apontador == raiz) huffEncodedCharString.append('0');
                hashOfEncodedChar.insert(leafCharHolder,turnStringInBitArray(huffEncodedCharString));
                return;
            }

            else if(leafCharHolder == '!')
            {
                //qDebug()<<"... Its content is default char !, so it will be marked";
                transversedString.append("!!");
                if(apontador == raiz) huffEncodedCharString.append('0');
                hashOfEncodedChar.insert(leafCharHolder,turnStringInBitArray(huffEncodedCharString));
                return;
            }

            else
            {
                //qDebug()<<"... Its content has no any restriction, so it will not be modified";
                transversedString.append(apontador->getCharacter());
                if(apontador == raiz) huffEncodedCharString.append('0');
                hashOfEncodedChar.insert(leafCharHolder,turnStringInBitArray(huffEncodedCharString));
                return;
            }
        }

        else
        {
            //qDebug()<<"Current node isn't a leaf. Default char * will be put in place of it into transversed string";
            transversedString.append('*');

            if(apontador->getLeft())
            {
                //qDebug()<<"Left node, it isn't Nullpointer. Going by left...";
                huffEncodedCharString.append('0');
                treeRecursiveBuildCodingHelper(transversedString,huffEncodedCharString,hashOfEncodedChar,apontador->getLeft(),raiz);
                huffEncodedCharString.chop(1);
            }

            if(apontador->getRight())
            {
                //qDebug()<<"Right node, it isn't Nullpointer. Going by right...";
                huffEncodedCharString.append('1');
                treeRecursiveBuildCodingHelper(transversedString,huffEncodedCharString,hashOfEncodedChar,apontador->getRight(),raiz);
                huffEncodedCharString.chop(1);
            }
        }
    }
}

void ActionsCenter::treeRecursiveRebuildNodeHandler(const QString &string, qint64 &currentCharIndex, Huffnode *&apontador)
{
    //qDebug()<< "Handle start over...";

    if(currentCharIndex < string.size())
    {
        uchar holderCurrentCharHere = string.at(currentCharIndex).unicode();
        if(holderCurrentCharHere == '!')
        {
            //qDebug()<< "handler w !";
            apontador->setFrequency(1);
            holderCurrentCharHere = string.at(currentCharIndex+1).unicode();
            apontador->setCharacter(holderCurrentCharHere);

            //qDebug() << "make it inside huffnode class perfoms...";
            apontador->makeItLeaf();

            currentCharIndex += 2;
            //qDebug()<< "all done in handler w !";
            return;
        }
        else if(holderCurrentCharHere == '*')
        {
            //qDebug()<< "handler w *";
            apontador->setFrequency(0);
            apontador->setCharacter(holderCurrentCharHere);
            apontador->setLeft((new Huffnode()));
            apontador->setRight((new Huffnode()));
            ++currentCharIndex;
            //qDebug()<< "all done in handler w *";
            return;
        }
        else
        {
            //qDebug()<< "handler any w" << qPrintable(holderCurrentCharHere);
            apontador->setFrequency(1);
            apontador->setCharacter(holderCurrentCharHere);

            //qDebug()<<"make it leaf inside huffnode class perfoms...";
            apontador->makeItLeaf();

            ++currentCharIndex;
            //qDebug()<< "all done in handler any w " << qPrintable(holderCurrentCharHere);
            return;
        }
    }
    else
        qDebug()<<"end of string nothing to handled";
}

void ActionsCenter::treeRecursiveRebuildHelper(const QString &transversedString, qint64 &currentCharIndex, Huffnode *&apontador)
{
    //qDebug()<<"Recursive Rebuild start over...";
    if(apontador)
    {

        this->treeRecursiveRebuildNodeHandler(transversedString,currentCharIndex,apontador);
        Huffnode *holderSubTree;
        if(apontador->getLeft())
        {
            //qDebug()<<"going by subLeftTree...";
            holderSubTree = apontador->getLeft();
            this->treeRecursiveRebuildHelper(transversedString,currentCharIndex,holderSubTree);
        }
        if(apontador->getRight())
        {
            //qDebug()<<"going by subRightTree...";
            holderSubTree = apontador->getRight();
            this->treeRecursiveRebuildHelper(transversedString,currentCharIndex,holderSubTree);
        }
    }
}

QBitArray ActionsCenter::turnStringInBitArray(QString &string)
{
    QBitArray bitsFromString = QBitArray(string.size());
    for(int i = 0; i<bitsFromString.size(); ++i)
    {
        if(string.at(i) == '1')
        {
            bitsFromString.setBit(i,true);
        }
    }
    return bitsFromString;
}

void ActionsCenter::completePseudoBinNumString(QString &string, int completeToThisSize)
{
    while(string.size() < completeToThisSize)
    {
        string.prepend('0');
    }
}

void ActionsCenter::treeRecursiveTransverse(QString &transversedString, Huffnode *apontador)
{
    if(apontador)
    {
        if(apontador->isLeaf())
        {
            transversedString.append(apontador->getCharacter());
            return;
        }
        else
        {
            transversedString.append(apontador->getCharacter());
            this->treeRecursiveTransverse(transversedString,apontador->getLeft());
            this->treeRecursiveTransverse(transversedString,apontador->getRight());
        }
    }
}

ActionsCenter::ActionsCenter()
{

}

ActionsCenter::~ActionsCenter()
{

}

void ActionsCenter::makeCount(QFile *userInFile, quint8 *arrayOfOccurrences)
{
    Q_ASSERT_X(userInFile->open(QIODevice::ReadOnly), "ActionsCenter::makeCount", "D'oh! Are you sure that provider a valid filename. Aren't you?\nIf, yes: sorry it can not be opened, Try verify if it is in use for other program and try again later\nIf, no: Verify you type and try again!");

    while(!userInFile->atEnd())
    {
        QByteArray lineFromFile = userInFile->read(1024);
        for(int i=0; i<lineFromFile.size(); ++i)
            arrayOfOccurrences[(uchar(lineFromFile[i]))]++;
    }

    userInFile->close();
}

void ActionsCenter::treeGenNode(QList<Huffnode *> &listLikeNodeRepository, quint8 *arrayOfOccurrences)
{
    Q_ASSERT_X(listLikeNodeRepository.empty(), "ActionsCenter::treeGenNode", "D'oh! It works better in a empty list, cause things could messed up into it. So avoiding your Headache, nothing was performed. *Except for 'is-empty-test' itself, of sure :D");
    Huffnode *auxNodeHolder = NULL;
    for(int i=0; i<256;++i)
    {
        if(arrayOfOccurrences[i])
        {
            auxNodeHolder = new Huffnode(arrayOfOccurrences[i],(uchar(i)),NULL,NULL);
            listLikeNodeRepository.append(auxNodeHolder);
        }
    }

    /*qDebug()<<"in treeGenNode list of nodes";
    for(int i = 0; i <listLikeNodeRepository.size(); ++i)
    {
        qDebug("List item n: %d | Huffman char %c\n", i, listLikeNodeRepository.at(i)->getCharacter());
    }*/

    auxNodeHolder = NULL;
}

void ActionsCenter::treeOrdering(QList<Huffnode *> &listLikeTree)
{
    while(((listLikeTree.size())>1))
    {
        qSort(listLikeTree.begin(),listLikeTree.end(),this->HuffnodeAlessthanB);
        /*for(int i =0; i<listLikeTree.size(); ++i)
            qDebug("--------------------------------------------\nAscii char: %c | Frequency: %d\n\n--------------------------------------------\n", listLikeTree.at(i)->getCharacter(), listLikeTree.at(i)->getFrequency());*/

        Huffnode *auxNodeHolder = new Huffnode(((listLikeTree.at(0)->getFrequency()) + (listLikeTree.at(1)->getFrequency())), '*',  listLikeTree.at(0), listLikeTree.at(1));
        listLikeTree.removeFirst();
        listLikeTree.removeFirst();
        listLikeTree.prepend(auxNodeHolder);
        //qDebug("\n###############################\n");
    }

}

void ActionsCenter::treeCoding(QString *transverseTree, QHash<uchar, QBitArray> *hashTree, Huffnode *pointer, Huffnode *root)
{
    QString *auxCharCodingHolder = new QString();
    this->treeRecursiveBuildCodingHelper(*transverseTree,*auxCharCodingHolder,*hashTree,pointer,root);
    delete auxCharCodingHolder;
}

void ActionsCenter::treeBuilder(HuffTree *&builtTreeHolder, quint8 *arrayOfOccurrences)
{
    qDebug()<<"TreeBuilder - Tasks Done (1/4)";
    QList<Huffnode*> *auxListLikeTreeHolder = new QList<Huffnode*>();
    this->treeGenNode(*auxListLikeTreeHolder,arrayOfOccurrences);
    qDebug()<<"TreeBuilder - Tasks Done (2/4)";

    this->treeOrdering(*auxListLikeTreeHolder);
    builtTreeHolder->setRoot(auxListLikeTreeHolder->at(0));
    delete auxListLikeTreeHolder;
    qDebug()<<"TreeBuilder - Tasks Done(3/4)";

    this->treeCoding(builtTreeHolder->getTransversedTreeString(),builtTreeHolder->getHash(),builtTreeHolder->getCurrent(),builtTreeHolder->getRoot());
    qDebug()<<"TreeBuilder - Tasks Done(4/4)\nTree Built!";
}

int ActionsCenter::compCalcBinTrashSize(QHash<uchar, QBitArray> &hashedNodes, quint8 *arrayOfOccurrences)
{
    quint64 preCalc = 0;

    for(int i=0; i<256; ++i)
    {
        if(arrayOfOccurrences[i])
        {
            preCalc += (arrayOfOccurrences[i]) * (hashedNodes.value((uchar(i))).size());
        }
    }

    preCalc = 8 - (preCalc%8);
    if(preCalc == 8)
        preCalc = 0;

    return preCalc;
}

quint64 ActionsCenter::compCalcTreeSize(QString &preOrderedTree)
{
    return preOrderedTree.size();
}

int ActionsCenter::compCalcFilenameSize(QFile *userInFile)
{
    Q_ASSERT_X(userInFile->open(QIODevice::ReadOnly), "ActionsCenter::compCalcFilenameSize", "D'Oh! I can't open the input file. For security nothing will be done. Dhuffman is dead (x.X)");
    int computedFileNameSize = (userInFile->fileName().size()-1);
    while((userInFile->fileName().at(computedFileNameSize) != '/') & (userInFile->fileName().at(computedFileNameSize) != '/'))
    {
        --computedFileNameSize;
    }

    computedFileNameSize = ((userInFile->fileName().size() - computedFileNameSize)-1);
    qDebug()<<"Size of file in compCalcFileSize:    "<<QString::number(computedFileNameSize);
    userInFile->close();
    return computedFileNameSize;
}

QFile* ActionsCenter::compGenHeader(QBitArray binTrashSize, QBitArray binTreeSize, QBitArray binFilenameSize)
{
    QFile *headerTmp = new QFile("header.tmp");
    BitArray *helper = new BitArray(24);

    Q_ASSERT_X(headerTmp->open(QIODevice::WriteOnly), "ActionsCenter::compGenHeader", "Temp header can't be create. Not know how to handle it");

    helper->addBitArray(binTrashSize);
    helper->addBitArray(binTreeSize);
    helper->addBitArray(binFilenameSize);

    QList<QBitArray> listOfChunks = QList<QBitArray>();
    helper->makeItListOfChunks(listOfChunks);

    for(int i = 0; i<listOfChunks.size(); ++i)
        headerTmp->write(helper->turnChunkOfBitsToByte(listOfChunks.at(i)),1);

    delete helper;

    headerTmp->close();
    return headerTmp;
}

QFile* ActionsCenter::compGenHuffFile(QFile *userInFile, QHash<uchar, QBitArray> &hashedNodes, int TrashSize)
{
    QFile * HuffmanTmp = new QFile("huff.tmp");
    bool perfectFileFlag = true;
    Q_ASSERT_X(HuffmanTmp->open(QIODevice::WriteOnly), "ActionsCenter::compGenHuffFile", "Temp out file can't be created. Not know how to handle it");
    Q_ASSERT_X(userInFile->open(QIODevice::ReadOnly), "ActionsCenter::compGenHuffFile", "I can't open the provided Input File, verify if its a valid filename and try again from the top, sorry it hard to say. Dhuffman is gets fuzzy (o.O)");

    if(TrashSize) perfectFileFlag = false;

    BitArray *binDataHelper = new BitArray(1024*8);
    QByteArray avaliableDataInByte;

    qDebug()<<"genHuff";
    while(!userInFile->atEnd())
    {
        qDebug()<<"Reading...";
        QByteArray lineOfData = userInFile->readLine(1024);
        for(int i = 0; i<lineOfData.size(); i++)
        {
            qDebug()<<"Calling addbits na gen huff";
            binDataHelper->addBitArray(hashedNodes.value((uchar(lineOfData[i]))));
            qDebug() << "bitarray before write job: " << binDataHelper->getReadableBitArray();

            while(binDataHelper->theresOneChunkAvaliable())
            {
                avaliableDataInByte = binDataHelper->turnChunkOfBitsToByte(binDataHelper->breakOneChunk());

                qDebug()<<"theres a chunk of data avalaible to be write down. It's... "<< binDataHelper->getReadableBitArray();
                if(!avaliableDataInByte.isEmpty())
                    HuffmanTmp->write(avaliableDataInByte.data_ptr()->data(),1);
                else
                {
                    qDebug()<<"error empty byteArray returned";
                    break;
                }
            }
        }
    }

    qDebug()<<"File data bitarray na genHufffile:   "<<binDataHelper->getReadableBitArray();
    qDebug()<<"Trash dentro da GenHuff: "<<QString::number(TrashSize);

    if(!perfectFileFlag)
    {
        binDataHelper->setPseudoEndIndex(binDataHelper->getPseudoEndIndex()+TrashSize);
        avaliableDataInByte = binDataHelper->turnChunkOfBitsToByte(binDataHelper->breakOneChunk());

        qDebug()<<"After complete:  "<<binDataHelper->getReadableBitArray();
        HuffmanTmp->write(avaliableDataInByte.data_ptr()->data(),1);
    }

    qDebug()<<"After huffed:    "<<binDataHelper->getReadableBitArray();

    userInFile->close();
    HuffmanTmp->close();
    return HuffmanTmp;
}

QFile *ActionsCenter::compUniteFile(QFile *head, QFile *huffFile)
{
    Q_ASSERT_X(head->open(QIODevice::Append), "ActionsCenter::compUniteFile", "D'Oh! I cant open header file. Something did much wrong, not know how to handle it. Dhuffman is dead (x.X)");
    Q_ASSERT_X(huffFile->open(QIODevice::ReadOnly), "ActionsCenter::compUniteFile", "D'Oh! I can't open huffFile. Something did much wrong, not know how to handle it. Dhuffman is dead (x.X");

    while(!huffFile->atEnd())
    {
        head->write(huffFile->readLine(1024));
    }

    huffFile->close();
    head->close();
    return head;
}

void ActionsCenter::compCompress(const QString &filePath, const QString customOutname)
{
    QFile *inputedFile = new QFile(filePath);
    quint8 countedBinData[256] = {0};

    qDebug()<<"Compress (1/3) - Counting (1)";
    this->makeCount(inputedFile,countedBinData);
    qDebug()<<"Counted!\nCompress - Done (1/3)";

    HuffTree *fileTree = new HuffTree();

    qDebug()<<"Compress (2/3) - Tree Jobs (4)";
    this->treeBuilder(fileTree,countedBinData);
    qDebug()<<"Compress - Done (2/3)";

    qDebug()<<"Compress (3/3) - Compress Jobs (6)";
    int bitsTrash = this->compCalcBinTrashSize(*fileTree->getHash(), countedBinData);
    QString TrashBits = QString::number((bitsTrash),2);
    completePseudoBinNumString(TrashBits,3);
    qDebug()<<"After Completing job, Trash string:   "<<TrashBits<<"\nCompress Jobs - Done (1/6)";

    QString TreeSize = QString::number(compCalcTreeSize(*fileTree->getTransversedTreeString()),2);
    completePseudoBinNumString(TreeSize,13);
    qDebug()<<"After Completing job, Tree String:   "<<TreeSize<<"\nCompress Jobs - Done (2/6)";

    qint64 FileSize = compCalcFilenameSize(inputedFile);
    QString FileNSize = QString::number(FileSize,2);
    completePseudoBinNumString(FileNSize,8);
    qDebug()<<"After completing job, File name String:  "<<FileNSize;

    qDebug()<<"Compress Jobs - Done (3/6)";

    QFile *huffOutFilelikeHeader = compGenHeader(turnStringInBitArray(TrashBits),turnStringInBitArray(TreeSize),turnStringInBitArray(FileNSize));

    Q_ASSERT_X(huffOutFilelikeHeader->open(QIODevice::Append), "ActionsCenter::compCompress", "D'Oh! I can't open GenHeader");
    Q_ASSERT_X(inputedFile->open(QIODevice::ReadOnly), "ActionsCenter::compCompress", "D'Oh! I can't open inputedFile again. That's no make any sense, Dhuffman is Dead (x.X)");

    qDebug()<<"File size number:     "<<QString::number(FileSize);
    QString OriginalFilenameHolder = "";

    for(int j = (inputedFile->fileName().size()) - FileSize; j<inputedFile->fileName().size(); ++j)
    {
        OriginalFilenameHolder += inputedFile->fileName().at(j);
    }

    qDebug()<<"Retrived file name:  "<<OriginalFilenameHolder;

    inputedFile->close();

    qDebug()<<"Filename after obtain file:   "<<FileNSize;

    huffOutFilelikeHeader->write(OriginalFilenameHolder.toUtf8());
    huffOutFilelikeHeader->write(fileTree->getTransversedTreeString()->toUtf8());

    huffOutFilelikeHeader->close();

    qDebug()<<"Compress Jobs - Done (4/6)";

    huffOutFilelikeHeader = compUniteFile(huffOutFilelikeHeader,compGenHuffFile(inputedFile,*fileTree->getHash(),bitsTrash));

    qDebug()<<"Compress Jobs - Done (5/6)";

    Q_ASSERT_X(huffOutFilelikeHeader->open(QIODevice::ReadWrite), "ActionsCenter::compCompress", "D'Oh! I can't open gen out File");
    Q_ASSERT_X(inputedFile->open(QIODevice::ReadOnly), "ActionsCenter::comCompress", "D'Oh! I can't open input file again, something change. Is it still there? isn't it? dhuffman is dead (x.X)");

    FileNSize = inputedFile->fileName();
    FileNSize.chop(FileSize);

    FileNSize.append(customOutname+".huff");
    inputedFile->close();

    qDebug()<<"Custom out file name: "<<FileNSize;
    huffOutFilelikeHeader->close();
    huffOutFilelikeHeader->setFileName(FileNSize);
    huffOutFilelikeHeader->close();
    qDebug()<<"Compress Jobs - Done (6/6)";

    qDebug()<<"Compress - Done (3/3)";
}

Huffnode *ActionsCenter::treeRebuildFromString(const QString &PreOrderedTransversedTree)
{
    Q_ASSERT_X(!PreOrderedTransversedTree.isEmpty(), "ActionsCenter::treeRebuildFromString", "D'Oh! I can't handle with a empty string. Did you has been already created it tree, dind't you?");

    Huffnode* pseudoRoot = NULL;
    qint64 charIndex = 0;

    qDebug()<<"Ok Rebuild will start";

    pseudoRoot = new Huffnode();
    pseudoRoot->setLeft((new Huffnode));
    pseudoRoot->setRight((new Huffnode));

    this->treeRecursiveRebuildHelper(PreOrderedTransversedTree,charIndex,pseudoRoot);

    qDebug()<<"All done in RebuildFromString";

    return pseudoRoot;
}

bool ActionsCenter::decompReadingHeader(QFile *userInFile, QString &preOrderedTransversedTree, QString &fileNameString, qint8 &trashNumSize, qint64 treeNumSize, qint64 fileNumSize)
{
    Q_ASSERT_X(userInFile->open(QIODevice::ReadOnly), "ActionsCenter::decompReadingHeader", "File can't be opened");

    if(userInFile->bytesAvailable() > 3)
    {
        BitArray *helper = new BitArray(24);
        QByteArray headerHolder = (userInFile->readLine(4));

        helper->addBitArray(helper->turnByteToChunkOfBits(headerHolder));
        qDebug()<<"After handler:   "<<helper->getReadableBitArray();

        headerHolder.clear();
        headerHolder = helper->turnChunkOfBitsToByte(helper->getBitArray());

        qDebug() << "byte array:    " << headerHolder.toHex();
        trashNumSize = (((uchar(headerHolder[0])) &0x70)>>5);
        qDebug() << "Trash size in int: " << QString::number(trashNumSize);

        qDebug() << "byte array:    " << headerHolder.toHex();
        treeNumSize = ((uchar(headerHolder[0])) &0x1F) | ((uchar(headerHolder[1])) &0xFF);
        qDebug() << "tree size in int:  " << QString::number(treeNumSize);

        qDebug() << "byte array:    " << headerHolder.toHex();
        fileNumSize = ((uchar(headerHolder[2])) &0xFF);
        qDebug() << "file size in int:  " << QString::number(fileNumSize);

        qint64 availabledatasize = userInFile->bytesAvailable();
        qint64 pseudoavailabledatasizeneeded = (treeNumSize+fileNumSize);

        if(availabledatasize <= pseudoavailabledatasizeneeded)
        {
            qDebug() << "there's no enough data to retrieve from it's header...\n file availavle bytes: "
                     << QString::number(availabledatasize) << "\n pseudo available bytes needed:    "
                     << QString::number(pseudoavailabledatasizeneeded);
            return false;
        }

        headerHolder.clear();
        headerHolder = userInFile->readLine(fileNumSize+1);
        qDebug() << "byte array:    " << headerHolder.toHex();
        fileNameString = headerHolder;
        qDebug() << "retrived file name:    " << fileNameString;

        headerHolder.clear();
        headerHolder = userInFile->readLine(treeNumSize+1);
        qDebug() << "byte array: " << headerHolder.toHex();
        preOrderedTransversedTree = headerHolder;
        qDebug() << "retrieved tree:    " << preOrderedTransversedTree;

        return true;
    }
    else
    {
        qDebug()<<"There's no data enough for retrieve header. Not know how to handle, Nothing to do with it. File is conrupted";
        return false;
    }
}

QFile* ActionsCenter::decompRetrieveOriginalData(QFile *userCompFile, HuffTree *&tree)
{
    QFile *retrievedFile = new QFile("rtrvr.tmp");

    Q_ASSERT_X(retrievedFile->open(QIODevice::WriteOnly),"ActionsCenter::desRetriveOriginalData", "Sorry, I can't open or even create a file to retrive your data. Check with your adm if you had rights for use this directory, and so on try again later. Huffman dead (x_X)");
    Q_ASSERT_X(userCompFile->open(QIODevice::ReadOnly),"ActionsCenter::desRetriveOriginalData","Sorry, I can't open your provided huffman file. Check you typing and try again later, Huffman is dead (x_X)");

    BitArray *helper = new BitArray(1024*8);
    bool flag = true;

    tree->transverseToPreOrderStringGen();
    qDebug()<< "tree: "<< *tree->getTransversedTreeString();
    while(!userCompFile->atEnd() & flag)
    {
        QByteArray holderData = userCompFile->readLine(1024);
        helper->addReversedBitArray(helper->turnByteToChunkOfBits(holderData));
        helper->pseudoRemoveFromEnd(2);
        qDebug()<<"retrieved bits" << helper->getReadableBitArray();
        //qint64 holder = helper->pseudoSize();
        while(!helper->pseudoEmpty() & flag)
        {
            flag = tree->retriveDataByTransversed(helper->pseudoBeginValue(),retrievedFile);

            if(flag) helper->pseudoFF();
            else
            {
                qDebug() << "Theres something wrong, may this tree wasn't build from it file?";
                helper->setPseudoBeginIndex(0);
                break;
            }
        }
    }

    retrievedFile->close();
    return retrievedFile;
}
