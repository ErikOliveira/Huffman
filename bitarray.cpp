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
    m_pseudoEndIndex =bitArray.size();
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
        if((m_pseudoEndIndex+1) < m_bitArray->size())
        {
            //qDebug()<<"Add in process";
            m_bitArray->setBit(m_pseudoEndIndex,value);
            ++m_pseudoEndIndex;
        }
        else
        {
            //qDebug()<<"Resize was required. --Smart resize--";
            m_bitArray->resize((m_bitArray->size()+8));
            this->helperAddBitToEnd(value);
        }
    }
}

void BitArray::helperAddBitLoop(const QBitArray &bitArray)
{
    for(qint64 i = 0; i<bitArray.size(); ++i)
    {
        //if(!i) qDebug()<<"adding loop...";
        this->helperAddBitToEnd(bitArray.at(i));
    }
}

void BitArray::helperAddReversedBitLoop(const QBitArray &bitArray)
{
    qint64 nBytes = bitArray.size()/8;

    for(qint64 i=1; i<=nBytes; ++i)
    {
        for(qint64 j=((i*8)-1); j>=((i*8)-8); --j)
        {
            //if(!(i-1)) qDebug()<<"reversed adding loop...";
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
    //qDebug()<<"custom constructor...";
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
    if(m_maximumSize > 0)
    {
        if(setThisToIt.size() > m_maximumSize)
        {
            m_maximumSize += ((setThisToIt.size() - m_maximumSize)+8);
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
        m_bitArray->resize((setThisToIt.size()+8));
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
    //this->state("getReadableBitArray");
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
        m_maximumSize +=nBitsToAdd;
        m_bitArray->resize(m_maximumSize);
        m_pseudoEndIndex = m_maximumSize;
    }
    else
    {
        m_bitArray->resize((m_bitArray->size())+nBitsToAdd);
        m_pseudoEndIndex = m_bitArray->size();
    }
}

void BitArray::addBitArray(const QBitArray &addThisIntoIt)
{
    //qDebug()<<"AddBit method...";
    if(m_maximumSize > 0)
    {
        //qDebug()<<"add-mode 1";
        if(m_pseudoEndIndex+addThisIntoIt.size() < m_maximumSize)
        {
            //qDebug()<<"add-mode 1.1 - the bitarray to add is less than maximum size";
            this->helperAddBitLoop(addThisIntoIt);
        }
        else
        {
            //qDebug()<<"add-mode 1.2 - the bitarray to add is great or equal maximum size";
            qint64 holderEnd = m_pseudoEndIndex;
            this->completeMe((m_maximumSize - (m_pseudoEndIndex+addThisIntoIt.size())));
            m_pseudoEndIndex = holderEnd;
            this->helperAddBitLoop(addThisIntoIt);
        }
    }
    else
    {
        //qDebug()<<"add-mode 2 - unlimited binArray";
        this->helperAddBitLoop(addThisIntoIt);
    }
}

void BitArray::addReversedBitArray(const QBitArray &addThisFromBackToFrontBytePerByteLike)
{
    if(m_maximumSize > 0)
    {
        if((m_pseudoEndIndex-1)+addThisFromBackToFrontBytePerByteLike.size() < m_maximumSize)
        {
            this->helperAddReversedBitLoop(addThisFromBackToFrontBytePerByteLike);
        }
        else
        {
            qint64 holderEndIndex = m_pseudoEndIndex;
            this->completeMe((m_pseudoEndIndex+addThisFromBackToFrontBytePerByteLike.size())- m_maximumSize);
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
    qint64 i=0;
    while(!this->pseudoEmpty())
    {
        m_bitArray->setBit(i,this->pseudoBeginValue());
        ++m_pseudoBeginIndex;
        ++i;
    }

    if(m_maximumSize > 0)
        m_bitArray->fill(false,i,(m_maximumSize-1));
    else
        m_bitArray->fill(false,i,(m_bitArray->size()-1));

    m_pseudoBeginIndex =0;
    m_pseudoEndIndex =(i-1);
}

void BitArray::pseudoFF()
{
    if(m_maximumSize > 0)
    {
        if(!this->pseudoEmpty()) ++m_pseudoBeginIndex;
        else
            qDebug()<< "Sorry but can't go forward. The end, it's already reached!";
    }
    else
    {
        if(!this->pseudoEmpty()) ++m_pseudoBeginIndex;
        else
            qDebug()<< "Sorry but can't go forward. The end, it's already reached!";
    }
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
            //qDebug() << "rmv-mode 1";
            if(((m_pseudoBeginIndex+1) < m_pseudoEndIndex) & ((m_pseudoBeginIndex+1) < m_maximumSize))
            {
                //qDebug() << "pseudo Remotion!";
                if(m_pseudoBeginIndex != m_pseudoEndIndex)
                {
                    ++m_pseudoBeginIndex;
                    //qDebug() << "pseudo Remotion mode 1.1";
                }
                else
                {
                    this->erase();
                    //qDebug() << "pseudo Remotion mode 1.2";
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

void BitArray::makeItListOfChunks(QList<QBitArray> &list)
{
    if(list.empty())
    {
        //this->state("1.1 - makeItListOfChunks");
        int pseudoTrash = (8-(this->pseudoSize()%8));
        if(pseudoTrash == 8) pseudoTrash=0;
        if(pseudoTrash) this->completeMe(pseudoTrash);
        //this->state("1.2 - makeItListOfChunks");

        while(!this->pseudoEmpty()&this->theresOneChunkAvaliable())
            list.append(this->breakOneChunk());
    }
    else
        qDebug() << "That list, it isn't empty. Nothing was do with it";

}

QBitArray BitArray::breakOneChunk()
{
    //this->state("breakOneChunk");
    if(this->theresOneChunkAvaliable())
    {
        //qDebug()<< "Chunk breaking...";
        QBitArray chunk(8,false);

        for(qint64 i =m_pseudoBeginIndex; i<(m_pseudoBeginIndex+8); ++i)
        {
            chunk.setBit((i-m_pseudoBeginIndex),m_bitArray->at(i));
        }

        m_pseudoBeginIndex+=8;
        //qDebug()<<"breaked!";
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
    //qDebug()<<"If stops here problem in turnByteToChunk...";
    QBitArray chunkOfBitsFromByte(chunkOfData.size()*8,false);
    //qDebug()<<"Creation done...";
    for(qint64 i=0; i<chunkOfData.size();++i)
    {
        //if(!i) qDebug()<<"external loop";
        for(quint8 j=0; j<8; ++j)
        {
            //if(!j) qDebug()<<"inner loop";
            chunkOfBitsFromByte.setBit(((i*8)+j),(chunkOfData.at(i)&(1<<(7-j))));
        }
    }

    return chunkOfBitsFromByte;
}

QByteArray BitArray::turnChunkOfBitsToByte(const QBitArray &chunkOfBits)
{
    if(chunkOfBits.size() > 7)
    {
        qDebug()<<"Turning chunk in byte...\nChunk shared 8 size: "<<QString::number(chunkOfBits.size()/8);

        QByteArray byteFromChunkOfBits = QByteArray(chunkOfBits.size()/8,'\0');
        qDebug()<<"ByteArray size:  "<<QString::number(byteFromChunkOfBits.size());

//        byteFromChunkOfBits->fill(0);

        for(qint64 i=0; i<chunkOfBits.size(); ++i)
        {
            //byteFromChunkOfBits.insert((i/8),((byteFromChunkOfBits.at(i/8)) | ((chunkOfBits.at(i)?1:0)<<(i%8))));
            byteFromChunkOfBits[(int (i/8))] = ((byteFromChunkOfBits.at((i/8)) | ((chunkOfBits.testBit(i)?1:0)<<(i%8))));
        }

        //byteFromChunkOfBits.resize(chunkOfBits.size()/8);

        return byteFromChunkOfBits;
    }
    else
    {
        qDebug()<<"Sorry, it isn't turnable. Provided BitArray size, it isn't divisible by eight. A Null ByteArray is returned by default";
        return (QByteArray());
    }
}
