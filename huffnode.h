#ifndef HUFFNODE_H
#define HUFFNODE_H
#include <cstddef>
#include <QDebug>

class Huffnode
{
   unsigned int m_freq;
   unsigned char m_char;
   Huffnode* m_left;
   Huffnode* m_right;

   void makeLeafHelper(Huffnode *leafIt);
public:

   //Constructors & Desconstructors
        //Default ones...
        Huffnode();
        ~Huffnode();
       //Custom...
       Huffnode(unsigned int freq, unsigned char character, Huffnode *left, Huffnode *right);

    //Settlers & Getters
        //Settlers...
        void setFrequency(unsigned int freq);
        void setCharacter(unsigned char character);
        void setLeft(Huffnode* left);
        void setRight(Huffnode* right);

        //Getters...
        unsigned int getFrequency();
        unsigned char getCharacter();
        Huffnode* getLeft();
        Huffnode* getRight();

    //Miscellaneous
    bool isLeaf(); //tests if the node is leaf;
    void makeItLeaf(); //properly make a node tree of itselfs
};

#endif // HUFFNODE_H
