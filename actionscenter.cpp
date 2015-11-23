#include "actionscenter.h"

void ActionsCenter::completePseudoBinNumString(QString &string, int completeToThisSize)
{
    qint64 nmStringSize = string.size(); //Pex tip cascade applied!

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

bool ActionsCenter::makeCount(QFile *userInFile, quint64 *arrayOfOccurrences)
{
    bool status = false;
    Q_ASSERT_X(userInFile->open(QIODevice::ReadOnly), Q_FUNC_INFO, "D'oh! Are you sure that provider a valid filename. Aren't you?\nIf, yes: sorry it can not be opened, Try verify if it is in use for other program and try again later\nIf, no: Verify you type and try again!");

    QByteArray fileData = userInFile->readAll(); //Pex tip!
    qint64 readDataSize = fileData.size(); //Pex tip!
    userInFile->close();

    for(int i=0;  i<readDataSize; ++i)
    {
        ++arrayOfOccurrences[(uchar (fileData[i]))];
        if((i+1) >= readDataSize)
            status = true;
    }

    return status;
}

int ActionsCenter::compCalcBinTrashSize(QVector<QString> &huffCodingPatternsRepository, quint64 *arrayOfOccurrences)
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

QByteArray ActionsCenter::compCalcFilenameSize(QFile *&userInFile)
{
    return QFileInfo(*userInFile).fileName().toUtf8();
}

QFile* ActionsCenter::compGenHeader(QString outCustomName, QString binTrashSize, QString binTreeSize, QString binFilenameSize, const QByteArray &originalName, const QByteArray &treeRep)
{
    QFile *headerTmp = new QFile(outCustomName);
    BitArray *helper = new BitArray(24);

    Q_ASSERT_X(headerTmp->open(QIODevice::WriteOnly), Q_FUNC_INFO, "Temp header can't be create. Not know how to handle it");

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

bool ActionsCenter::compGenHuffFile(QFile *userInFile, QFile* outHuffFile, QVector<QString> &huffCodingPatternsRepository, int trashSize)
{
    Q_ASSERT_X(outHuffFile->open(QIODevice::WriteOnly | QIODevice::Append), Q_FUNC_INFO, "Temp out file can't be created. Not know how to handle it");
    Q_ASSERT_X(userInFile->open(QIODevice::ReadOnly), Q_FUNC_INFO, "I can't open the provided Input File, verify if its a valid filename and try again from the top, sorry it hard to say.\nHuffman is gets fuzzy (o.O)");

    bool status = false;
    bool trashHandle = false;
    bool bitsFit = true;

    if(trashSize)
        trashHandle = true;

    BitArray *helper = new BitArray((1024*30));
    QByteArray availableDataInByte;

    QByteArray data = userInFile->readAll();
    qint64 realDataSize = data.size(); //Pex tip cascade applied
    userInFile->close();

    for(qint64 i = 0; i<realDataSize; ++i)
    {
        QString encodedByte = huffCodingPatternsRepository[(quint8 (data.at((int (i)))))];
        if(encodedByte.size() >= helper->theresNBitsAvailable())
        {
            bitsFit = false;
            availableDataInByte = helper->turnChunkOfBitsToByte();

            if(availableDataInByte.isEmpty())
            {
                qDebug()<< "Error empty byteArray returned";
                status = false;
                return status;
            }
            else
                outHuffFile->write(availableDataInByte);

        }

        helper->addBitArray(encodedByte);

        if((i+1) == realDataSize)
            status = true;
    }

    if(bitsFit)
    {
        availableDataInByte = helper->turnChunkOfBitsToByte();
        outHuffFile->write(availableDataInByte);
    }

    if(trashHandle)
    {
        helper->completeMe(trashSize);
        availableDataInByte = helper->turnChunkOfBitsToByte();

        if(availableDataInByte.isEmpty())
        {
            qDebug()<< "Error empty byteArray returned";
            status = false;
        }

        else
        {
            outHuffFile->write(availableDataInByte);
            status = true;
        }
    }

    helper->~BitArray();
    outHuffFile->close();
    return status;
}

int ActionsCenter::compCompress(const QString &filePath, const QString &customOutname)
{
    bool status = false;
    QFile *inputedFile = new QFile(filePath);
    quint64 countedBinData[256] = {0};

    qDebug("%s\n\t\t%s","Compress (1/3)","--- \t\tCounting Jobs (1)\t\t ---");

    status = this->makeCount(inputedFile,countedBinData);

    if(status)
        qDebug()<<"\t\t----------------------------------------------------\n\nCompress (1/3): Done!\n";
    else
    {
        qDebug()<<"Error in Compress (1/3) - Counting Jobs (1): Error in open file to count occurrences of bytes.\nHuffman is Dead(x_X)\n";
        return 0xC0;
    }

    HuffTree *fileTree = new HuffTree();

    qDebug("%s\n\t\t%s", "Compress (2/3)", "--- \t\tTree Jobs (3)\t\t ---");

    status = fileTree->builder(countedBinData);

    if(status)
    {
        qDebug()<<"\t\t--------------------------------------------\n\nCompress (2/3): Done!\n";
        qDebug("%s\n\t\t%s", "Compress (3/3)", "--- \t\t\tHuff Jobs (5)\t\t\t ---");
    }
    else
    {
        qDebug()<<"Error in Compress (2/3) - Tree Jobs (3): Error in build the Tree.\nHuffman is Dead (x_X)\n";
        return 0xC1;
    }

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

    QFile *huffOutFile = compGenHeader(customOutname, trashBits, treeSize, fileNSize, fileNameString, fileTree->getTrasversedTreeRepresentation());
    qDebug("\n\t\t\t%s\n","-------------------------------------------");

    qDebug("\n\t\t\t%s\n", "--- Task (5/5): Apply Huff on file\t---");

    status = compGenHuffFile(inputedFile, huffOutFile, codedPatternsRepository, bitsTrash);
    if(status)
        qDebug()<<"\n\t\t\t\t\t\tHuffed!\n\t\t\t----------------------------------------------------\n\nCompress (3/3): Done!";
    else
    {
        qDebug() << "Error in Compress (3/3) - HuffJobs (5): Fail in compress the file.\nHuffman is Dead (x_X)\n";
        huffOutFile->deleteLater();
        return 0xC3;
    }
    return 0x00;
}

bool ActionsCenter::decompReadingHeader(QFile *userInFile, QByteArray &preOrderedTrasversedTree, QByteArray &fileNameString, qint8 &trashNumSize, qint64 &treeNumSize, qint64 &fileNumSize)
{
    Q_ASSERT_X(userInFile->open(QIODevice::ReadOnly), Q_FUNC_INFO, "File can't be opened");

    quint64 availableByte = userInFile->bytesAvailable();

    if(availableByte > 3)
    {
        BitArray *helper = new BitArray(24);
        QByteArray headerHolder = userInFile->read(3);
        bool flag = helper->turnByteToChunkOfBits(headerHolder);

        headerHolder.clear();
        headerHolder = helper->turnChunkOfBitsToByte(helper->getBitArray());

        trashNumSize = (((uchar(headerHolder[0])) &0xE0)>>5);
        treeNumSize = (((quint16(headerHolder[0])) &0x1F)<<8) | ((quint16(headerHolder[1])) &0xFF);

        fileNumSize = ((uchar(headerHolder[2])) &0xFF);

        qint64 availableDataSize = userInFile->bytesAvailable();
        qint64 pseudoAvailableDataSizeNeeded = (treeNumSize+fileNumSize);

        if(availableDataSize <= pseudoAvailableDataSizeNeeded)
        {
            qDebug() << "there's no enough data to retrieve from it's header...\n file availavle bytes: "
                     << QString::number(availableDataSize) << "\n pseudo available bytes needed:    "
                     << QString::number(pseudoAvailableDataSizeNeeded);
            userInFile->close();
            return false;
        }

        headerHolder.clear();
        headerHolder = userInFile->read(fileNumSize);
        fileNameString = headerHolder;

        headerHolder.clear();
        headerHolder = userInFile->read(treeNumSize);

        preOrderedTrasversedTree = headerHolder;
        userInFile->close();
        return true;
    }
    else
    {
        qDebug()<< "There's no data enough for retrieve header. Not know how to handle, Nothing to do with it. File is corrupted";
        userInFile->close();
        return false;
    }
}

bool ActionsCenter::decompRetrieveOriginalData(QByteArray customOutName, QFile *userCompFile, HuffTree *&tree, const qint64 treeSize, const qint64 filenameSize, const qint8 trash)
{
    QFile *retrievedFile = new QFile(customOutName);

    Q_ASSERT_X(retrievedFile->open(QIODevice::WriteOnly), Q_FUNC_INFO, "Sorry, I can't open or even create a file to retrive your data. Check with your adm if you had rights for use this directory, and so on try again later. Huffman dead (x_X)");
    Q_ASSERT_X(userCompFile->open(QIODevice::ReadOnly), Q_FUNC_INFO, "Sorry, I can't open your provided huffman file. Check you typing and try again later, Huffman is dead (x_X)");

    BitArray *helper = new BitArray(1024*30);
    bool flag = userCompFile->seek(3+treeSize+filenameSize);

    QByteArray holderData = userCompFile->readAll();
    QByteArray holderOriginalData = QByteArray();

    while(!holderData.isEmpty() & flag)
    {
        flag = helper->turnByteToChunkOfBits(holderData);
        flag = tree->retriveDataByTrasversed(trash,helper,holderOriginalData);
    }

    retrievedFile->write(holderOriginalData);
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
        return 0xD1;
    }

    QString outHandler = QDir::currentPath()+'/'+ stringFile;
    status = QFileInfo::exists(outHandler);

    if(status)
    {
        qDebug()<< "\n\nOut huff file:\n\t\t\t" << qPrintable(outHandler)
                << "\nD'Oh! output File already exist.\nSorry, but Huffman was designed to not allowed overwritten files.\nCheck the output info above and solves it conflicts by your own and give it another try!\nHuffman is dead (x.X)\n";
        return 0xD0;
    }

    else
    {
        qDebug() << "Descompress jobs - (2/3)";
        status = tree->rebuilder(stringTree);

        if(stringTree.isEmpty())
        {
            if(status)
                qDebug() << "Descompress jobs - Done (2/3)";
            else
            {
                qDebug() << "Error in jobs (2/3) - Error in rebuild tree\n"
                         << "remain representation hints (it should be zero): " << QString::number(stringTree.size()) << ". That's means some nodes are not placed into the tree, something fails so bad! Not know how to handle it\n Huffman is Dead(x.X)\n\n";
                return 0xD2;
            }
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
    QPair<bool, QString> callCompWiz ;
    QFileInfo inputedFile = QFileInfo(inPath);

    callCompWiz.first = inputedFile.exists();

    if(callCompWiz.first)
    {
        QString outName = "";

        if(outPath.isEmpty())
        {
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
            {
                callCompWiz.second = outName;
            }

            else
            {
                qDebug() << "D'Oh! output File already exists.\nSorry, but Huffman, It was designed to not allow overwritten files.\nDid you know you can type-in an out name, didn't you?\nNow you can check the in/output information above and give it another try after solving this conflict by your own!\nHuffman is dead (x_X)\n";
            }
        }

        else
        {
            if(outPath.contains(".huff"))
            {
                outName += outPath;
            }
            else
                outName += outPath + ".huff";

            qDebug()<< "Huffman gen output path:\n\t\t\t" << qPrintable(outName) <<"\n\n";
            QFileInfo huffOutFile = QFileInfo(outName);
            callCompWiz.first = !huffOutFile.exists();

            if(callCompWiz.first)
            {
                callCompWiz.second = outName;
            }
            else
            {
                qDebug() << "D'Oh! output File already exists.\nSorry, but Huffman, It was designed to not allow overwritten files.\nCheck your typing and give it another try!\nHuffman is dead (x_X)";
            }
        }
    }
    else
        qDebug() << "The system says: Input file doesn't exist.\nCheck your typing and try again.\nHuffman is dead (x_X)\n";

    return callCompWiz;
}

QPair<bool,QString> ActionsCenter::decompIOPaths(const QString &inPath, const QString &outDirPath)
{
    qDebug() << "inputed file:\t" << inPath << "\n\n" << "out local:\t" << outDirPath << "\n\n";
    QPair<bool, QString> callDecompWiz;
    callDecompWiz.first = false;
    QFileInfo inputedFile = QFileInfo(inPath);
    callDecompWiz.first = inputedFile.exists();

    if(callDecompWiz.first)
    {
        callDecompWiz.second = inputedFile.canonicalFilePath();
        QString outDir = "";

        if(outDirPath.isEmpty())
        {
            outDir = inputedFile.canonicalPath();
            qDebug()<< "User file's input path:\n\t\t\t"<< qPrintable(inPath);
            qDebug()<< "Huffman Dir output path:\n\t\t\t" << qPrintable(outDir);

            callDecompWiz.first = QDir::setCurrent(outDir);

            if(callDecompWiz.first)
                qDebug() << "Ok! Huffman current directory is... " << qPrintable(outDir) <<"\n\t\t\t ...your dehuffed file will be there!\n";
            else
                qDebug() << "D'Oh! current working directory was not changed.\nCheck your typing and try again, if it's not a typo verify with you admin if you have rights to use it and give it another try!\nHuffman is Dead (x_X)\n";
        }

        else
        {
            qDebug() << "Huffman says: non-empty outDir";
            QDir huffOutDir = QDir(outDirPath);
            callDecompWiz.first = huffOutDir.exists();

            if(callDecompWiz.first)
            {
                callDecompWiz.first = QDir::setCurrent(outDirPath);
                if(callDecompWiz.first)
                    qDebug() << "Ok! Huffman current directory is... " << qPrintable(outDirPath) <<"\n\t\t\t ...your dehuffed file will be there!\n";
            }
            else
                qDebug() << "D'Oh! output directory doesn't exist.\nSorry, but Huffman was designed to not create Directories\nTry creating a directory using O.S. file's manager and give it another try!\nHuffman is Dead (x_X)\n";
        }
    }
    else
        qDebug() << "The system says: Input file doesn't exist.\nCheck your typing and try again.\nHuffman is Dead (x_X)\n";

    return callDecompWiz;
}
