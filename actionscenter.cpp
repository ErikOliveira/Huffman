#include "actionscenter.h"

bool ActionsCenter::HuffnodeAlessthanB(Huffnode *a, Huffnode *b)
{
    if(a->getFrequency() == b->getFrequency())
        return ((a->getCharacter()) < (b->getCharacter()));
    else
        return ((a->getFrequency()) < (b->getFrequency()));
}

void ActionsCenter::treeRecursiveBuildCodingHelper(QByteArray &transversedRepresentation, QString &huffCodePatternString, QVector<QString> &huffCodingPatternsRepository, Huffnode *apontador, const Huffnode *raiz)
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
                transversedRepresentation.append("!*");
                if(apontador == raiz)
                    huffCodePatternString.append('0');
                huffCodingPatternsRepository[leafCharHolder] = huffCodePatternString;
                return;
            }

            else if(leafCharHolder == '!')
            {
                //qDebug()<<"... Its content is default char !, so it will be marked";
                transversedRepresentation.append("!!");
                if(apontador == raiz)
                    huffCodePatternString.append('0');
                huffCodingPatternsRepository[leafCharHolder] = huffCodePatternString;
                return;
            }

            else
            {
                /*qDebug()<<"... Its content has no any restriction, so it will not be modified"
                       <<"\ncodification: " << huffEncodedCharString
                      <<'\n';*/
                transversedRepresentation.append(leafCharHolder);
                if(apontador == raiz)
                    huffCodePatternString.append('0');
                huffCodingPatternsRepository[leafCharHolder] = huffCodePatternString;
                return;
            }
        }

        else
        {
            //qDebug()<<"Current node isn't a leaf. Default char * will be put in place of it into transversed string";
            transversedRepresentation.append('*');

            Huffnode *holderSubTree = apontador->getLeft(); //Pex tip cascate applied!
            if(holderSubTree)
            {
                //qDebug()<<"Left node, it isn't Nullpointer. Going by left...";
                huffCodePatternString.append('0');
                treeRecursiveBuildCodingHelper(transversedRepresentation,huffCodePatternString,huffCodingPatternsRepository,holderSubTree,raiz);
                huffCodePatternString.chop(1);
            }

            holderSubTree = apontador->getRight(); //Pex tip cascate applied!
            if(holderSubTree)
            {
                //qDebug()<<"Right node, it isn't Nullpointer. Going by right...";
                huffCodePatternString.append('1');
                treeRecursiveBuildCodingHelper(transversedRepresentation,huffCodePatternString,huffCodingPatternsRepository,holderSubTree,raiz);
                huffCodePatternString.chop(1);
            }
        }
    }
}

void ActionsCenter::treeRecursiveRebuildNodeHandler(QByteArray &transversedRepresentation, Huffnode *&apontador)
{
    //qDebug()<< "Handle start over...";

    if(!transversedRepresentation.isEmpty())
    {
        if((uchar(transversedRepresentation[0])) == 0x21)
        {
            //qDebug()<< "handler w !";
            apontador->setFrequency(1);
            //holderCurrentCharHere = string.at(currentCharIndex+1).unicode();
            apontador->setCharacter((uchar(transversedRepresentation[1])));

            //qDebug() << "make it inside huffnode class perfoms...";
            apontador->makeItLeaf();

            //currentCharIndex += 2;
            //qDebug()<< "all done in handler w !";

            transversedRepresentation.remove(0,2);
            return;
        }
        else if((uchar (transversedRepresentation[0])) == 0x2A)
        {
            //qDebug()<< "handler w *";
            apontador->setFrequency(0);
            apontador->setCharacter((uchar(transversedRepresentation[0])));
            apontador->setLeft((new Huffnode()));
            apontador->setRight((new Huffnode()));

            transversedRepresentation.remove(0,1);
            return;
        }
        else
        {
            qDebug()<< "handler any w" << qPrintable(((uchar (transversedRepresentation.at(0))) &0xFF));
            apontador->setFrequency(1);
            apontador->setCharacter((uchar(transversedRepresentation[0])));

            //qDebug()<<"make it leaf inside huffnode class perfoms...";
            apontador->makeItLeaf();

            //++currentCharIndex;
            //qDebug()<< "all done in handler any w " << qPrintable(holderCurrentCharHere);
            transversedRepresentation.remove(0,1);
            return;
        }
    }
    else
        qDebug()<<"end of string nothing to handled";
}

void ActionsCenter::treeRecursiveRebuildHelper(QByteArray &transversedRepresentation, Huffnode *&apontador)
{
    //qDebug()<<"Recursive Rebuild start over...";
    if(apontador)
    {

        this->treeRecursiveRebuildNodeHandler(transversedRepresentation,apontador);

        Huffnode *holderSubTree = apontador->getLeft(); //Pex tip cascate applied!
        if(holderSubTree)
        {
            //qDebug()<<"going by subLeftTree...";
            this->treeRecursiveRebuildHelper(transversedRepresentation,holderSubTree);
        }

        holderSubTree = apontador->getRight(); //Pex tip cascate applied!
        if(holderSubTree)
        {
            //qDebug()<<"going by subRightTree...";
            this->treeRecursiveRebuildHelper(transversedRepresentation,holderSubTree);
        }
    }
}

QBitArray ActionsCenter::turnStringInBitArray(QString &string)
{
    qint64 nmStringSize = string.size(); //Pex tip cascate applied!
    QBitArray bitsFromString = QBitArray(nmStringSize);
    for(int i = 0; i<nmStringSize; ++i)
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
    qint64 nmStringSize = string.size(); //Pex tip cascate applied!

    for(qint64 i = nmStringSize; i<completeToThisSize; ++i)
    {
        string.prepend('0');
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

    QByteArray fileData = userInFile->readAll(); //Pex tip!
    userInFile->close();

    qint64 readDataSize = fileData.size(); //Pex tip!
    for(int i=0;  i<readDataSize; ++i)
        arrayOfOccurrences[(uchar(fileData[i]))]++;

    qDebug()<<"\t\t\t\t\tCounted!";
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
    qint64 nmListSize = listLikeTree.size(); //Pex tip cascate applied!
    while(nmListSize > 1) //((listLikeTree.size())>1)
    {
        qSort(listLikeTree.begin(),listLikeTree.end(),this->HuffnodeAlessthanB);
        /*for(int i =0; i<listLikeTree.size(); ++i)
            qDebug("--------------------------------------------\nAscii char: %c | Frequency: %d\n\n--------------------------------------------\n", listLikeTree.at(i)->getCharacter(), listLikeTree.at(i)->getFrequency());/*a*/

        Huffnode *auxNodeHolder = new Huffnode(((listLikeTree.at(0)->getFrequency()) + (listLikeTree.at(1)->getFrequency())), '*',  listLikeTree.at(0), listLikeTree.at(1));
        listLikeTree.removeFirst(); // -1
        listLikeTree.removeFirst(); // -1
        listLikeTree.prepend(auxNodeHolder); // +1
        --nmListSize; // nmlistsize = numlistSize -1 -1 +1 = numlistsize-1
        //qDebug("\n###############################\n");
    }

}

void ActionsCenter::treeCoding(QByteArray &transversedTree, QVector<QString> &huffCodingPatternsRepository, Huffnode *pointer, Huffnode *root)
{
    QString auxCharCodingHolder = "";
    this->treeRecursiveBuildCodingHelper(transversedTree,auxCharCodingHolder,huffCodingPatternsRepository,pointer,root);
}

void ActionsCenter::treeBuilder(HuffTree *&holderBuiltTree, quint8 *arrayOfOccurrences)
{
    qDebug()<<"\t\t\t--- Task (1/3): Gen Leafs Nodes ---";

    QList<Huffnode*> auxListLikeTreeHolder = QList<Huffnode*>(); //removed new operator
    this->treeGenNode(auxListLikeTreeHolder, arrayOfOccurrences);
    qDebug()<<"\t\t\t\t\tGenered!\n\t\t\t-----------------------------------\n\n";

    qDebug()<<"\t\t\t--- Task (2/3): Ordering Nodes\t---";

    this->treeOrdering(auxListLikeTreeHolder); //removed desref. pointer
    holderBuiltTree->setRoot(auxListLikeTreeHolder.at(0)); //Edit
    qDebug()<<"\t\t\t\t\tOrdered!\n\t\t\t-----------------------------------\n\n";

    qDebug()<<"\t\t\t--- Task (3/3): Rep. & Coding\t---";

    QByteArray preOrderedRepresentation = QByteArray(); //Edit
    QVector<QString> codedPatternsRepository = QVector<QString>(256,""); //Edit size of it
    this->treeCoding(preOrderedRepresentation, codedPatternsRepository, holderBuiltTree->getCurrent(), holderBuiltTree->getRoot()); //Edit
    holderBuiltTree->setVector(codedPatternsRepository); //Edit
    holderBuiltTree->setPreOrderTransversedRepresentation(preOrderedRepresentation); //Edit
    qDebug()<<"\t\t\t\t\tCoded!\n\t\t\t-----------------------------------\n\n\t\t\t\tTree Built!";
}

int ActionsCenter::compCalcBinTrashSize(QVector<QString> &huffCodingPatternsRepository, quint8 *arrayOfOccurrences)
{
    quint64 preCalc = 0;

    for(int i=0; i<256; ++i) //Edit size of it
    {
        if(arrayOfOccurrences[i])
        {
            preCalc += (arrayOfOccurrences[i]) * (huffCodingPatternsRepository[i].size());
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

QByteArray ActionsCenter::compCalcFilenameSize(QFile *&userInFile)
{
    return QFileInfo(*userInFile).fileName().toUtf8();
}

QFile* ActionsCenter::compGenHeader(QString outCustomName, QString binTrashSize, QString binTreeSize, QString binFilenameSize, const QByteArray &originalName, const QByteArray &treeRep)
{
    QFile *headerTmp = new QFile(outCustomName);
    BitArray *helper = new BitArray(24);

    Q_ASSERT_X(headerTmp->open(QIODevice::WriteOnly), "ActionsCenter::compGenHeader", "Temp header can't be create. Not know how to handle it");

    helper->addBitArray(binTrashSize);
    helper->addBitArray(binTreeSize);
    helper->addBitArray(binFilenameSize);

    headerTmp->write(helper->turnChunkOfBitsToByte());

    headerTmp->write(originalName);
    headerTmp->write(treeRep);

    helper->~BitArray();
    headerTmp->close();
    return headerTmp;
}

void ActionsCenter::compGenHuffFile(QFile *userInFile, QFile* outHuffFile, QVector<QString> &huffCodingPatternsRepository, int trashSize)
{
    //QFile * HuffmanTmp = new QFile("huff.tmp");
    bool perfectFileFlag = true;

    Q_ASSERT_X(outHuffFile->open(QIODevice::WriteOnly | QIODevice::Append), "ActionsCenter::compGenHuffFile", "Temp out file can't be created. Not know how to handle it");
    Q_ASSERT_X(userInFile->open(QIODevice::ReadOnly), "ActionsCenter::compGenHuffFile", "I can't open the provided Input File, verify if its a valid filename and try again from the top, sorry it hard to say. Dhuffman is gets fuzzy (o.O)");

    if(trashSize)
        perfectFileFlag = false;

    BitArray *binDataHelper = new BitArray(1024*9);
    QByteArray avaliableDataInByte;

    qDebug()<<"genHuff";
    while(!userInFile->atEnd())
    {
        qDebug()<<"Reading...";
        QByteArray lineOfData = userInFile->read(1024);
        qint64 realDataSize = lineOfData.size(); //Pex tip cascate applied

        for(int i = 0; i<realDataSize; i++)
        {
            if((binDataHelper->getPseudoEndIndex()+1) >= binDataHelper->getMaximumSize())
            {
                qDebug() << "avoid loss data...";
                avaliableDataInByte = binDataHelper->turnChunkOfBitsToByte();
                //qDebug() << "available data to write down...  " << avaliableDataInByte.toHex();
                //qDebug() << "readable bitarray...   " << binDataHelper->getReadableBitArray();

                if(avaliableDataInByte.isEmpty())
                {
                    qDebug() << "error empty byteArray returned";
                    break;
                }
                else
                    outHuffFile->write(avaliableDataInByte, avaliableDataInByte.size());
            }

            binDataHelper->addBitArray(huffCodingPatternsRepository.at((uchar(lineOfData[i]))));
        }

        avaliableDataInByte = binDataHelper->turnChunkOfBitsToByte();
        /*qDebug()<< "available data to write down in Byte array...   "<< avaliableDataInByte.toHex()
                << "remain data:    " << binDataHelper->getReadableBitArray();*/

        if(avaliableDataInByte.isEmpty())
        {
            qDebug() << "error empty byteArray returned";
            break;
        }
        else
            outHuffFile->write(avaliableDataInByte, avaliableDataInByte.size());
    }

    qDebug()<<"File data bitarray na genHufffile:   "<<binDataHelper->getReadableBitArray();
    qDebug()<<"Trash dentro da GenHuff: "<<QString::number(trashSize);

    if(!perfectFileFlag)
    {
        //binDataHelper->setPseudoEndIndex(binDataHelper->getPseudoEndIndex()+TrashSize);
        binDataHelper->completeMe(trashSize);
        qDebug()<<"After complete:  "<<binDataHelper->getReadableBitArray();
        avaliableDataInByte = binDataHelper->turnChunkOfBitsToByte(); //binDataHelper->turnChunkOfBitsToByte(binDataHelper->breakOneChunk());
        qDebug() << "Available data in byte:    " << avaliableDataInByte.toHex();
        outHuffFile->write(avaliableDataInByte,avaliableDataInByte.size());
    }

    qDebug()<<"After huffed:    "<<binDataHelper->getReadableBitArray();

    binDataHelper->~BitArray();
    userInFile->close();
    outHuffFile->close();
    return;
}

QFile *ActionsCenter::compUniteFile(QFile *head, QFile *huffFile)
{
    Q_ASSERT_X(head->open(QIODevice::Append), "ActionsCenter::compUniteFile", "D'Oh! I cant open header file. Something did much wrong, not know how to handle it. Dhuffman is dead (x.X)");
    Q_ASSERT_X(huffFile->open(QIODevice::ReadOnly), "ActionsCenter::compUniteFile", "D'Oh! I can't open huffFile. Something did much wrong, not know how to handle it. Dhuffman is dead (x.X");

    while(!huffFile->atEnd())
    {
        head->write(huffFile->read(1024));
    }

    huffFile->close();
    head->close();
    return head;
}

void ActionsCenter::compCompress(const QString &filePath, const QString customOutname)
{
    QFile *inputedFile = new QFile(filePath);
    quint8 countedBinData[256] = {0};

    qDebug("%s\n\t\t%s","Compress (1/3)","--- \t\tCounting Jobs (1/1)\t\t ---");

    this->makeCount(inputedFile,countedBinData);
    qDebug()<<"\t\t----------------------------------------------------\n\nCompress (1/3): Done!\n";

    HuffTree *fileTree = new HuffTree();

    qDebug("%s\n\t\t%s", "Compress (2/3)", "--- \t\tTree Jobs (3)\t\t ---");

    this->treeBuilder(fileTree,countedBinData);
    qDebug()<<"\t\t--------------------------------------------\n\nCompress (2/3): Done!\n";
    qDebug("%s\n\t\t%s", "Compress (3/3)", "--- \t\t\tHuff Jobs (5)\t\t\t ---");

    QVector<QString> codedPatternsRepository = fileTree->getVector();
    int bitsTrash = this->compCalcBinTrashSize(codedPatternsRepository, countedBinData);
    QString trashBits = QString::number((bitsTrash),2);
    completePseudoBinNumString(trashBits,3);
    qDebug()<<"\n\t\t\t---\tTask (1/5): Get Trash Info\t\t---\n\t\t\t\tTrash Binary string:\n\t\t\t\t\t"<< qPrintable(trashBits)
           <<"\n\n\t\t\t-------------------------------------------\n";

    QString treeSize = QString::number((fileTree->getTreeSize()),2);
    this->completePseudoBinNumString(treeSize,13);
    qDebug()<< "\n\t\t\t---\tTask (2/5): Get Tree Info\t\t---\n\t\t\t\tTree Binary string:\n\t\t\t\t\t"<< qPrintable(treeSize)
           <<"\n\n\t\t\t-------------------------------------------\n";

    QByteArray fileNameString = this->compCalcFilenameSize(inputedFile);
    qDebug()<< "\t\t\tRetrived file name:\t"<< fileNameString;

    QString fileNSize = QString::number(fileNameString.size(),2);
    this->completePseudoBinNumString(fileNSize,8);
    qDebug()<< "\n\t\t\t---\tTask (3/5): Get Filename Info\t---\n\t\t\t\tFile name Binary string:\n\t\t\t\t\t"<< fileNSize
            << "\n\t\t\t-------------------------------------------\n" <<"\n\t\t\t--- Task (4/5) OutFileName Handler\t---\n";

    QFile *huffOutFile = compGenHeader(customOutname, trashBits, treeSize, fileNSize, fileNameString, fileTree->getTransversedTreeRepresentation());
    qDebug("\n\t\t\t%s\n","-------------------------------------------");

    qDebug("\n\t\t\t%s\n", "--- Task (5/5): Apply Huff on file\t---");

    compGenHuffFile(inputedFile, huffOutFile, codedPatternsRepository, bitsTrash);
    qDebug()<<"\n\t\t\t\t\t\tHuffed!\n\t\t\t----------------------------------------------------\n\nCompress (3/3): Done!";
}

Huffnode *ActionsCenter::treeRebuildFromString(QByteArray &preOrderedTransversedTree)
{
    Q_ASSERT_X(!preOrderedTransversedTree.isEmpty(), "ActionsCenter::treeRebuildFromString", "D'Oh! I can't handle with a empty string. Did you has been already created it tree, dind't you?");

    Huffnode* pseudoRoot = new Huffnode();
    qDebug()<<"Ok Rebuild will start";
    this->treeRecursiveRebuildHelper(preOrderedTransversedTree,pseudoRoot);
    qDebug()<<"All done in RebuildFromString";

    return pseudoRoot;
}

bool ActionsCenter::decompReadingHeader(QFile *userInFile, QByteArray &preOrderedTransversedTree, QByteArray &fileNameString, qint8 &trashNumSize, qint64 &treeNumSize, qint64 &fileNumSize)
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
        trashNumSize = (((uchar(headerHolder[0])) &0xE0)>>5);
        qDebug() << "Trash size in int: " << QString::number(trashNumSize);

        qDebug() << "byte array:    " << headerHolder.toHex();
        treeNumSize = (((quint16(headerHolder[0])) &0x1F)<<8) | ((quint16(headerHolder[1])) &0xFF);
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
            userInFile->close();
            return false;
        }

        headerHolder.clear();
        headerHolder = userInFile->readLine(fileNumSize+1);
        qDebug() << "byte array:    " << headerHolder.toHex();
        fileNameString = headerHolder;
        qDebug() << "retrived file name:    " << fileNameString;

        headerHolder.clear();
        headerHolder = userInFile->read(treeNumSize);
        qDebug() << "byte array: " << headerHolder.toHex();
        qDebug() << "byte array size:   " << QString::number(headerHolder.size());
        preOrderedTransversedTree = headerHolder;
        qDebug() << "retrieved tree:    " << preOrderedTransversedTree.toHex();
        userInFile->close();
        return true;
    }
    else
    {
        qDebug()<<"There's no data enough for retrieve header. Not know how to handle, Nothing to do with it. File is conrupted";
        userInFile->close();
        return false;
    }
}

bool ActionsCenter::decompRetrieveOriginalData(QByteArray customOutName, QFile *userCompFile, HuffTree *&tree, const qint64 treeSize, const qint64 filenameSize, const qint8 trash)
{
    QFile *retrievedFile = new QFile(customOutName);

    Q_ASSERT_X(retrievedFile->open(QIODevice::WriteOnly),"ActionsCenter::decompRetriveOriginalData", "Sorry, I can't open or even create a file to retrive your data. Check with your adm if you had rights for use this directory, and so on try again later. Huffman dead (x_X)");
    Q_ASSERT_X(userCompFile->open(QIODevice::ReadOnly),"ActionsCenter::decompRetriveOriginalData","Sorry, I can't open your provided huffman file. Check you typing and try again later, Huffman is dead (x_X)");

    BitArray *helper = new BitArray(1024*9);
    qDebug() << "pseudo pos:    " << qPrintable(QString::number(treeSize+filenameSize+4))
             << "available bytes:   " << qPrintable(QString::number(userCompFile->bytesAvailable()));

    bool flag = userCompFile->seek(3+treeSize+filenameSize);
    qDebug() << "flag:  " << flag;

    tree->transverseToPreOrderRepresentationGen();
    qDebug()<< "tree: "<< *tree->getTransversedTreeRepresentation();

    while(!userCompFile->atEnd() & flag)
    {
        QByteArray holderData = userCompFile->read(1024);
        //qDebug() << "byteArray: " << holderData.toHex();
        helper->addReversedBitArray(helper->turnByteToChunkOfBits(holderData));
        //qDebug()<<"retrieved bits" << helper->getReadableBitArray();
        qDebug()<<"trash size:  " << QString::number(trash) << "\n pseudosize:  " << QString::number(helper->pseudoSize());

        for(qint64 i = helper->pseudoSize(); ((i>trash) & (!helper->pseudoEmpty())) & flag; --i)
        {
            flag = tree->retriveDataByTransversed(helper->pseudoBeginValue(),retrievedFile);
            qDebug() << "flag:  " << flag;
            if(flag) helper->pseudoFF();
            else
            {
                qDebug() << "There's something wrong, may this tree wasn't built from it file?";
                helper->pseudoRW();
                break;
            }
        }

        if(helper->pseudoEmpty())
        {
            qDebug() << "Should no enter here, except if was trash size is zero";
            helper->erase();
        }
        else
        {
            qDebug() << "pseudo size:   " << helper->pseudoSize()
                    << "readable bitarray should be "+QString::number(trash)+": "<< helper->getReadableBitArray();
            helper->pseudoErase();
        }
    }

    retrievedFile->close();
    return flag;
}

int ActionsCenter::decompDecompress(const QString &filepath)
{
    bool status = false;

    HuffTree *tree = new HuffTree();
    QFile *huffedFile = new QFile(filepath);
    QByteArray stringTree = "";
    QByteArray stringFile = "";
    qint8 numTrash = -1;
    qint64 numFilename = -1;
    qint64 numTreeSize = -1;

    qDebug() << "Descompress jobs - (1/3)";
    status = decompReadingHeader(huffedFile,stringTree,stringFile,numTrash,numTreeSize,numFilename);

    if(status)
        qDebug() << "Descompress jobs - Done (1/3)";
    else
    {
        qDebug() << "Error in jobs (1/3) - Error in read file's header\nHuffman is Dead(x.X)";
        return 0xD0;
    }

    QString outHandler = QDir::currentPath()+'/'+stringFile;
    status = QFileInfo::exists(outHandler);

    if(status)
    {
        qDebug()<< "\n\nOut huff file:\n\t\t\t" << qPrintable(outHandler)
                << "\nD'Oh! output File already exist.\nSorry, but Huffman was designed to not allowed overwritten files.\nCheck the output info above and solves it conflicts by your own and give it another try!\nHuffman is dead (x.X)\n";
        return 0xD1;
    }
    else
    {
        qDebug() << "Descompress jobs - (2/3)";
        tree->setRoot(treeRebuildFromString(stringTree));
        tree->transverseToPreOrderRepresentationGen();

        if(stringTree.isEmpty())
        {
            status = true;
            qDebug() << "Descompress jobs - Done (2/3)";
        }

        else
        {
            qDebug() << "Error in jobs (2/3) - Error in rebuild tree\n"
                     << "remain representation hints (it should be zero): " << QString::number(stringTree.size()) << ". That's means some nodes are not placed into the tree, something fails so bad! Not know how to handle it\n Huffman is Dead(x.X)\n\n";
            return 0xD2;
        }

        if(status)
        {
            status = decompRetrieveOriginalData(stringFile, huffedFile, tree, numTreeSize, numFilename, numTrash);
            if(status)
                qDebug() << "Descompress jobs - Done (3/3)";
            else
            {
                qDebug() << "Error in jobs (3/3) - Error in retrieved original data\n"
                         << "Sorry, Not know how to handle with it.\nHuffman is Dead (x.X)\n";
                return 0xD3;
            }
        }
    }

    return 0x00;
}

QPair<bool, QString> ActionsCenter::compIOPaths(const QString &inPath, const QString &outPath)
{
    QPair<bool, QString> callCompWiz;
    callCompWiz.first = false;
    QFileInfo inputedFile = QFileInfo(inPath);
    callCompWiz.first = inputedFile.exists();

    if(callCompWiz.first)
    {
        QString outName = "";

        if(outPath.isEmpty())
        {
            qDebug()<< "Huffman says: Empty String to out huffed file\n";

            outName = inputedFile.canonicalFilePath();
            qDebug()<<"User file's input path:\n\t\t\t"<< qPrintable(outName) <<"\n\n";

            qint64 extensionRemove = outName.lastIndexOf('.');
            if(extensionRemove > 0)
            {
                outName.chop((outName.size()) - extensionRemove);
            }

            outName += ".huff";
            qDebug()<< "Huffman gen output path:\n\t\t\t" << qPrintable(outName) <<"\n\n";

            QFileInfo huffOutFile = QFile(outName);
            callCompWiz.first = !huffOutFile.exists();
            if(callCompWiz.first)
                callCompWiz.second = outName;
            else
                qDebug() << "D'Oh! Output File already exist.\nSorry, but Huffman, It was designed to not allowed overwritten files.\nDid you know you can typing a out name, didn't you?\nNow you can check the in/output information above and give it another try after solves this conflicts by your own!\nHuffman is dead (x.X)\n";
        }

        else
        {
            QFileInfo huffOutFile = QFileInfo(outPath);
            callCompWiz.first = !huffOutFile.exists();
            if(callCompWiz.first)
                callCompWiz.second = outPath;
            else
                qDebug() << "D'Oh! output File already exist.\nSorry, but Huffman was designed to not allowed overwritten existent files.\nCheck your typing and give it another try!\nHuffman is dead (x.X)";
        }
    }
    else
        qDebug() << "System says: Input file doesn't exist.\nCheck your typing and try again. Huffman is dead (x.X)\n";

    return callCompWiz;
}

bool ActionsCenter::decompIOPaths(const QString &inPath, const QString &outDirPath)
{
    bool callDecompWiz;
    callDecompWiz = false;
    QFileInfo inputedFile = QFileInfo(inPath);
    callDecompWiz = inputedFile.exists();

    if(callDecompWiz)
    {
        QString outDir = "";

        if(outDirPath.isEmpty())
        {
            qDebug()<< "Huffman says: Empty String to outDir original file\n";
            outDir = inputedFile.canonicalPath();
            qDebug()<< "User file's input path:\n\t\t\t"<< qPrintable(inPath);
            qDebug()<< "Huffman Dir output path:\n\t\t\t" << qPrintable(outDir);

            callDecompWiz = QDir::setCurrent(outDir);
            if(callDecompWiz)
                qDebug() << "Ok! Huffman current directory is... " << qPrintable(outDir) <<"\n\t\t\t ...your dehuffed file will be there!\n";
            else
                qDebug() << "D'Oh! current working directory was not changed.\nCheck your typing and try again, if it's not a typo verify with you admin if you have rights to use it and give it another try!\nHuffman is Dead (x.X)";
        }

        else
        {
            qDebug() << "Huffman says: non-empty outDir";
            QDir huffOutDir = QDir(outDirPath);
            callDecompWiz = huffOutDir.exists();

            if(callDecompWiz)
            {
                callDecompWiz = QDir::setCurrent(outDirPath);
                if(callDecompWiz)
                    qDebug() << "Ok! Huffman current directory is... " << qPrintable(outDirPath) <<"\n\t\t\t ...your dehuffed file will be there!\n";
            }
            else
                qDebug() << "D'Oh! output directory not exists.\nSorry, but Huffman was designed to not create Directories\nTry create a directory using O.S. file's manager and give it another try!";
        }
    }
    else
        qDebug() << "System says: Input file doesn't exist.\nCheck your typing and try again. Huffman is dead (x.X)\n";

    return callDecompWiz;
}
