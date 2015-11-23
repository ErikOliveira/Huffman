#ifndef BITARRAY_H
#define BITARRAY_H
#include <QDebug>
#include <QBitArray>

class BitArray
{
    QBitArray *m_bitArray;
    qint64 m_maximumSize;
    qint64 m_pseudoBeginIndex;
    qint64 m_pseudoEndIndex;

    void state(const QString method);
    void helperConstructor();
    void helperOperatorAndToSetBitArray(const QBitArray &bitArray);
    void helperAddBitToEnd(bool value);
    void helperAddBitLoop(const QBitArray &bitArray);
    void helperAddBitStringLoop(const QString &string);
    void helperAddReversedBitLoop(const QBitArray &bitArray);
public:

    //Constructors & Desconstructors
        //Default ones...
        BitArray();
        ~BitArray();

        //Custom
        BitArray(qint64 maxSize);

    //Settlers & Getters...
        //Settlers...
        void setMaximumSize(qint64 maxSize);
        void setPseudoBeginIndex(qint64 beginIndex);
        void setPseudoEndIndex(qint64 endIndex);
        void setBitArray(const QBitArray &setThisToIt);

        //Getters...
        QBitArray getBitArray() const;
        qint64 getPseudoBeginIndex() const;
        qint64 getPseudoEndIndex() const;
        qint64 getMaximumSize() const;
        QString getReadableBitArray();

    //Miscellaneous
        //Physical (Hard) actions - may cause mem (re/des)allocation
        void clear();
        void completeMe(int nBitsToAdd);
        void addBitArray(const QBitArray &addThisIntoIt);
        void addBitArray(const QString &addThisLikeBitArray);
        void addReversedBitArray(const QBitArray &addThisFromBackToFrontBytePerByteLike);

        //Logical (Soft) actions - It's abstract something that should be physical
        void erase(); //Fill all bits to zero
        void pseudoErase(); //Save pseudo remain data
        void pseudoFF();
        void pseudoRW();
        void pseudoRemoveFromBegin();
        void pseudoRemoveFromEnd(int nBitsToRemove);

        //Special info - Not modified any data into class
        bool theresOneChunkAvaliable();
        bool pseudoBeginValue();
        bool pseudoEmpty();
        qint64 pseudoSize();
        qint64 theresNBitsAvailable();

    //Tools - Uses external types, perform data turnment or do other tasks
    void makeItListOfChunks(QList<QBitArray> &list);
    QBitArray breakOneChunk();
    QBitArray turnByteToChunkOfBits(const QByteArray &chunkOfData);
    bool turnByteToChunkOfBits(QByteArray &chunkOfData);
    QByteArray turnChunkOfBitsToByte(const QBitArray &chunkOfBits);
    QByteArray turnChunkOfBitsToByte();
};

#endif // BITARRAY_H
