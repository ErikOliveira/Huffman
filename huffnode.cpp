#include "huffnode.h"

//Implementation of default constructor
Huffnode::Huffnode()
{
    this->setFrequency(0);
    this->setCharacter('\0');
    this->setLeft(NULL);
    this->setRight(NULL);
}

//Implementation of default desconstructor
Huffnode::~Huffnode()
{
    if(m_left) delete m_left;
    if(m_right) delete m_right;
}

//Implementation of custom constructor
Huffnode::Huffnode(unsigned int freq, unsigned char character, Huffnode *left, Huffnode *right)
{
    this->setFrequency(freq);
    this->setCharacter(character);
    this->setLeft(left);
    this->setRight(right);
}

//Implementation of setFrequency method
void Huffnode::setFrequency(unsigned int freq)
{
    m_freq = freq;
}

//Implementation of setCharacter method
void Huffnode::setCharacter(unsigned char character)
{
    m_char = character;
}

//Implementation of setLeft method
void Huffnode::setLeft(Huffnode *left)
{
    m_left = left;
}

//Implementation of setRight method
void Huffnode::setRight(Huffnode *right)
{
    m_right = right;
}

//Implementation of getFrequency method
unsigned int Huffnode::getFrequency()
{
    return m_freq;
}

//Implementation of getCharacter method
unsigned char Huffnode::getCharacter()
{
    return m_char;
}

//Implementation of getLeft method
Huffnode *Huffnode::getLeft()
{
    return m_left;
}

//Implementation of getRight method
Huffnode *Huffnode::getRight()
{
    return m_right;
}

//Implementation of isLeaf method
bool Huffnode::isLeaf()
{
    return ((m_left == NULL) & (m_right == NULL));
}

//Implementation of makeItLeaf method
void Huffnode::makeItLeaf()
{
    //qDebug()<< "It's is leaf?" << this->isLeaf();
    if(!this->isLeaf())
    {
        try
        {
            //qDebug() << "Lft delete act";
            delete m_left;

            //qDebug() << "try to pointer lft to null";
            m_left = NULL;

            //qDebug() << "All done with left";
        }
        catch(...)
        {
            /*qDebug() << "delete left fails..."
                     << "\ntry to pointer lft to null";/*a*/
            m_left = NULL;
        }

        try
        {
            //qDebug() << "Rgt delete act";
            delete m_right;

            //qDebug()<< "try to pointer rgt to null";
            m_right = NULL;

            //qDebug() << "all done";
        }
        catch(...)
        {
            /*qDebug() << "delete right fails..."
                        <<"\ntry to pointer rgt to null";/*a*/
            m_right = NULL;
        }
    }
    else
        qDebug() << "it's already leaf nothing to do!";
}
