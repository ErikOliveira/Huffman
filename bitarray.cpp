#include "bitarray.h"

void BitArray::state(const QString method)
{
    qDebug()<<"bitarray state in "<<"BitArray::"+method.toUtf8()+"() method:"
           <<"\n pseudoBegin:   "<<QString::number(m_pseudoBeginIndex)
          <<"\n pseudoEnd:  "<<QString::number(m_pseudoEndIndex)
         <<"\n maxSize: "<<QString::number(m_maximumSize)
        <<"\n pseudoSize:   "<<QString::number(this->pseudoSize())
       <<"\n ---state end---\n";
}

void BitArray::helperConstructor()
{
    m_bitArray = new QBitArray();
    m_pseudoBeginIndex =0;
    m_pseudoEndIndex =0;
    m_maximumSize =-1;
}

void BitArray::helperOperatorAndToSetBitArray(const QBitArray &bitArray)
{
    m_bitArray->fill(true);
    m_bitArray->operator &=(bitArray);
    m_pseudoBeginIndex =0;
    m_pseudoEndIndex = bitArray.size();
}

void BitArray::helperAddBitToEnd(bool value)
{
    if(m_maximumSize > 0)
    {
        if((m_pseudoEndIndex+1) <= m_maximumSize)
        {
            m_bitArray->setBit(m_pseudoEndIndex,value);
            ++m_pseudoEndIndex;
        }
        else
            qDebug()<<"Sorry already reach end. Nothing was added";
    }
    else
    {
        qint64 mBitArraySize = m_bitArray->size();
        if((m_pseudoEndIndex+1) < mBitArraySize)
        {
            m_bitArray->setBit(m_pseudoEndIndex,value);
            ++m_pseudoEndIndex;
        }
        else
        {
            m_bitArray->resize((mBitArraySize+8));
            this->helperAddBitToEnd(value);
        }
    }
}

void BitArray::helperAddBitLoop(const QBitArray &bitArray)
{
    qint64 size = bitArray.size(); //Pex tip cascade applied!
    for(qint64 i = 0; i<size; ++i)
    {
        this->helperAddBitToEnd(bitArray.at(i));
    }
}

void BitArray::helperAddBitStringLoop(const QString &string)
{
    qint64 size = string.size(); //Pex tip cacate applied!
    for(qint64 i = 0; i<size; ++i)
    {
        this->helperAddBitToEnd(((string.at(i) == '1')? true:false));
    }
}

void BitArray::helperAddReversedBitLoop(const QBitArray &bitArray)
{
    qint64 nBytes = bitArray.size()/8;

    for(qint64 i=1; i<=nBytes; ++i)
    {
        qint64 upperBoundary = ((i*8)-1); //Pex tip cascade applied!
        qint64 lowerBoundary = ((i*8)-8);

        for(qint64 j= upperBoundary; j>= lowerBoundary; --j)
        {
            this->helperAddBitToEnd(bitArray.at(j));
        }
    }
}

BitArray::BitArray()
{
    this->helperConstructor();
}

BitArray::~BitArray()
{
    this->clear();
    delete m_bitArray;
}

BitArray::BitArray(qint64 maxSize)
{
    this->helperConstructor();
    m_bitArray->resize(maxSize);
    this->setMaximumSize(maxSize);
}

void BitArray::setMaximumSize(qint64 maxSize)
{
    m_maximumSize = maxSize;
}

void BitArray::setPseudoBeginIndex(qint64 beginIndex)
{
    m_pseudoBeginIndex = beginIndex;
}

void BitArray::setPseudoEndIndex(qint64 endIndex)
{
    m_pseudoEndIndex = endIndex;
}

void BitArray::setBitArray(const QBitArray &setThisToIt)
{
    qint64 bitToSetSize = setThisToIt.size(); //Pex tip cascade applied!

    if(m_maximumSize > 0)
    {
        if(bitToSetSize > m_maximumSize)
        {
            m_maximumSize += ((bitToSetSize - m_maximumSize)+8); //Reserve a secure extra space at end
            m_bitArray->resize(m_maximumSize);
            setBitArray(setThisToIt);
        }
        else
        {
            this->helperOperatorAndToSetBitArray(setThisToIt);
        }
    }
    else
    {
        m_bitArray->resize((bitToSetSize+8));
        this->helperOperatorAndToSetBitArray(setThisToIt);
    }
}

QBitArray BitArray::getBitArray() const
{
    return *m_bitArray;
}

qint64 BitArray::getPseudoBeginIndex() const
{
    return m_pseudoBeginIndex;
}

qint64 BitArray::getPseudoEndIndex() const
{
    return m_pseudoEndIndex;
}

qint64 BitArray::getMaximumSize() const
{
    return m_maximumSize;
}

QString BitArray::getReadableBitArray()
{
    QString readable = "";
    for(qint64 i=m_pseudoBeginIndex; i<m_pseudoEndIndex; ++i)
        readable.append(((m_bitArray->at(i))?'1':'0'));
    return readable;
}

void BitArray::clear()
{
    m_bitArray->clear();
    m_pseudoBeginIndex =0;
    m_pseudoEndIndex =0;
    m_maximumSize =-1;
}

void BitArray::completeMe(int nBitsToAdd)
{
    if(m_maximumSize > 0)
    {
        if(nBitsToAdd > 0)
        {
            if(m_pseudoEndIndex+nBitsToAdd <= m_maximumSize)
            {
                m_bitArray->fill(false,m_pseudoEndIndex,((m_pseudoEndIndex+nBitsToAdd)-1));
                m_pseudoEndIndex += nBitsToAdd;
            }
            else
            {
                m_maximumSize +=nBitsToAdd;
                m_bitArray->resize(m_maximumSize);
                m_pseudoEndIndex = m_maximumSize;
            }
        }
    }
    else
    {
        m_bitArray->resize((m_bitArray->size())+nBitsToAdd);
        m_pseudoEndIndex = m_bitArray->size();
    }
}

void BitArray::addBitArray(const QBitArray &addThisIntoIt)
{
    if(m_maximumSize > 0)
    {
        qint64 bitArrayToAddSize = addThisIntoIt.size(); //Pex tip cascade applied!
        if(m_pseudoEndIndex+bitArrayToAddSize < m_maximumSize)
        {
            this->helperAddBitLoop(addThisIntoIt);
        }
        else
        {
            qint64 holderEnd = m_pseudoEndIndex;
            this->completeMe((m_maximumSize - (m_pseudoEndIndex+bitArrayToAddSize)));
            m_pseudoEndIndex = holderEnd;
            this->helperAddBitLoop(addThisIntoIt);
        }
    }
    else
    {
        this->helperAddBitLoop(addThisIntoIt);
    }
}

void BitArray::addBitArray(const QString &addThisLikeBitArray)
{
    if(m_maximumSize > 0)
    {
        qint64 stringToAddSize = addThisLikeBitArray.size(); //Pex tip cascade applied!
        if(m_pseudoEndIndex+stringToAddSize < m_maximumSize)
        {
            this->helperAddBitStringLoop(addThisLikeBitArray);
        }
        else
        {
            qint64 holderEnd = m_pseudoEndIndex;
            this->completeMe(((m_pseudoEndIndex+stringToAddSize) - m_maximumSize));
            m_pseudoEndIndex = holderEnd;
            this->helperAddBitStringLoop(addThisLikeBitArray);
        }
    }
    else
    {
        this->helperAddBitStringLoop(addThisLikeBitArray);
    }
}

void BitArray::addReversedBitArray(const QBitArray &addThisFromBackToFrontBytePerByteLike)
{
    if(m_maximumSize > 0)
    {
        qint64 bitArrayToAddSize = addThisFromBackToFrontBytePerByteLike.size(); //Pex tip cascade applied!
        if((m_pseudoEndIndex-1)+bitArrayToAddSize < m_maximumSize)
        {
            this->helperAddReversedBitLoop(addThisFromBackToFrontBytePerByteLike);
        }
        else
        {
            qint64 holderEndIndex = m_pseudoEndIndex;
            this->completeMe((m_pseudoEndIndex+bitArrayToAddSize)- m_maximumSize);
            m_pseudoEndIndex = holderEndIndex;
            this->addReversedBitArray(addThisFromBackToFrontBytePerByteLike);
        }
    }
    else
    {
        this->helperAddReversedBitLoop(addThisFromBackToFrontBytePerByteLike);
    }
}

void BitArray::erase()
{
    m_bitArray->fill(false);
    m_pseudoBeginIndex =0;
    m_pseudoEndIndex =0;
}

void BitArray::pseudoErase()
{
    qint64 nmPseudoEnd = 0;
    while(!this->pseudoEmpty())
    {
        m_bitArray->setBit(nmPseudoEnd,this->pseudoBeginValue());
        ++m_pseudoBeginIndex;
        ++nmPseudoEnd;
    }

    if(m_maximumSize > 0)
        m_bitArray->fill(false,nmPseudoEnd,(m_maximumSize-1));
    else
        m_bitArray->fill(false,nmPseudoEnd,(m_bitArray->size()-1));

    m_pseudoBeginIndex =0;
    m_pseudoEndIndex = nmPseudoEnd;
}

void BitArray::pseudoFF()
{
    if(!this->pseudoEmpty())
        ++m_pseudoBeginIndex;
    else
        qDebug()<< "Sorry but can't go forward. The end, it's already reached!";
}

void BitArray::pseudoRW()
{
    if((m_pseudoBeginIndex-1) > (-1))
        --m_pseudoBeginIndex;
}

void BitArray::pseudoRemoveFromBegin()
{
    if(!m_bitArray->isEmpty())
    {
        if(m_maximumSize > 0)
        {
            if(((m_pseudoBeginIndex+1) < m_pseudoEndIndex) & ((m_pseudoBeginIndex+1) < m_maximumSize))
            {
                if(m_pseudoBeginIndex != m_pseudoEndIndex)
                {
                    ++m_pseudoBeginIndex;
                }
                else
                {
                    this->erase();
                }
            }
        }
    }
    else
        qDebug()<<"Sorry can't do it. It is a empty bitarray";
}

void BitArray::pseudoRemoveFromEnd(int nBitsToRemove)
{
    if((m_pseudoEndIndex-nBitsToRemove) > m_pseudoBeginIndex)
        m_pseudoEndIndex -=nBitsToRemove;
    else
        this->erase();
}

bool BitArray::theresOneChunkAvaliable()
{
    return ((m_pseudoEndIndex - m_pseudoBeginIndex) > 7);
}

bool BitArray::pseudoBeginValue()
{
    return (m_pseudoEndIndex > m_pseudoBeginIndex)? m_bitArray->at(m_pseudoBeginIndex):false;
}

bool BitArray::pseudoEmpty()
{
    return ((m_pseudoEndIndex-1) < m_pseudoBeginIndex);
}

qint64 BitArray::pseudoSize()
{
    return (m_pseudoEndIndex - m_pseudoBeginIndex);
}

qint64 BitArray::theresNBitsAvailable()
{
    if(m_maximumSize > 0)
        return m_maximumSize - m_pseudoEndIndex;
    else
        return m_bitArray->size() - m_pseudoEndIndex;
}

void BitArray::makeItListOfChunks(QList<QBitArray> &list)
{
    if(list.empty())
    {
        int pseudoTrash = (8-(this->pseudoSize()%8));
        if(pseudoTrash == 8) pseudoTrash=0;
        if(pseudoTrash) this->completeMe(pseudoTrash);

        while(!this->pseudoEmpty()&this->theresOneChunkAvaliable())
            list.append(this->breakOneChunk());
    }
    else
        qDebug() << "That list, it isn't empty. Nothing was do with it";

}

QBitArray BitArray::breakOneChunk()
{
    if(this->theresOneChunkAvaliable())
    {
        QBitArray chunk(8,false);
        qint64 upperBoundary = m_pseudoBeginIndex+8; //Pex tip cascade applied!
        for(qint64 i =m_pseudoBeginIndex; i<upperBoundary; ++i)
        {
            chunk.setBit((i-m_pseudoBeginIndex),m_bitArray->at(i));
        }

        m_pseudoBeginIndex+=8;
        return chunk;
    }
    else
    {
        qDebug()<<"Sorry! It isn't chunkable right now. an Empty bitarray was returned by default";
        return (QBitArray());
    }
}

QBitArray BitArray::turnByteToChunkOfBits(const QByteArray &chunkOfData)
{
    qint64 byteChunkSize = chunkOfData.size(); //Pex tip cascade applied!
    QBitArray chunkOfBitsFromByte(byteChunkSize*8,false);

    for(qint64 i=0; i<byteChunkSize; ++i)
    {
        for(quint8 j=0; j<8; ++j)
        {
            chunkOfBitsFromByte.setBit(((i*8)+j),(chunkOfData.at(i)&(1<<(7-j))));
        }
    }

    return chunkOfBitsFromByte;
}

bool BitArray::turnByteToChunkOfBits(QByteArray &chunkOfData)
{
    qint64 numBitstoWrite = this->theresNBitsAvailable();

    if(numBitstoWrite > 7)
    {
        qint64 byteArraySize = chunkOfData.size();
        if(numBitstoWrite >= (byteArraySize*8))
        {
            for(qint64 i=0; i<byteArraySize; ++i)
            {
                quint8 byteNum = chunkOfData[(int (i))];

                this->helperAddBitToEnd((byteNum & (0x80)));
                this->helperAddBitToEnd((byteNum & (0x40)));
                this->helperAddBitToEnd((byteNum & (0x20)));
                this->helperAddBitToEnd((byteNum & (0x10)));
                this->helperAddBitToEnd((byteNum & (0x08)));
                this->helperAddBitToEnd((byteNum & (0x04)));
                this->helperAddBitToEnd((byteNum & (0x02)));
                this->helperAddBitToEnd((byteNum & (0x01)));

            }

            chunkOfData.remove(0,byteArraySize);
        }
        else
        {
            qint64 availableBitsLikeByte = numBitstoWrite/8;
            for(qint64 i=0; i<availableBitsLikeByte; ++i)
            {
                quint8 byteNum = chunkOfData[(int (i))];
                this->helperAddBitToEnd((byteNum & (0x80)));
                this->helperAddBitToEnd((byteNum & (0x40)));
                this->helperAddBitToEnd((byteNum & (0x20)));
                this->helperAddBitToEnd((byteNum & (0x10)));
                this->helperAddBitToEnd((byteNum & (0x08)));
                this->helperAddBitToEnd((byteNum & (0x04)));
                this->helperAddBitToEnd((byteNum & (0x02)));
                this->helperAddBitToEnd((byteNum & (0x01)));
            }
            chunkOfData.remove(0,availableBitsLikeByte);
        }
        return true;
    }
    else
        return false;

}

QByteArray BitArray::turnChunkOfBitsToByte(const QBitArray &chunkOfBits)
{
    if(chunkOfBits.size() > 7)
    {   
        qint64 bitChunkSize = chunkOfBits.size(); //Pex tip cascade applied!
        QByteArray byteFromChunkOfBits = QByteArray(bitChunkSize/8,'\0');

        for(qint64 i=0; i<bitChunkSize; ++i)
        {
            byteFromChunkOfBits[(int (i/8))] = ((byteFromChunkOfBits.at((i/8)) | ((chunkOfBits.testBit(i)?1:0)<<(7-(i%8)))));
        }

        return byteFromChunkOfBits;
    }
    else
    {
        qDebug()<<"Sorry, it isn't turnable. Provided BitArray size, it isn't divisible by eight. A Empty ByteArray is returned by default";
        return (QByteArray());
    }
}

QByteArray BitArray::turnChunkOfBitsToByte()
{
    if(this->theresOneChunkAvaliable())
    {
        QByteArray bytes = QByteArray(this->pseudoSize()/8,'\0');
        qint64 numByteSize = bytes.size(); //Pex tip cascade applied!

        qint64 loopControl = 0;
        for(; ((loopControl/8)< numByteSize); ++loopControl)
        {
            bytes[(int (loopControl/8))] = (bytes.at((loopControl/8)) | ((m_bitArray->testBit(m_pseudoBeginIndex+loopControl)?1:0)<<(7-(loopControl%8))));
        }

        m_pseudoBeginIndex+= loopControl;

        if(this->pseudoEmpty())
            this->erase();
        else
            this->pseudoErase();

        return bytes;
    }
    else
    {
        qDebug()<<"Sorry, it isn't turnable. Provided BitArray size, it isn't divisible by eight. A Empty ByteArray is returned by default";
        return (QByteArray());
    }
}
