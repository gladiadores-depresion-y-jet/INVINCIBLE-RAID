//
// Created by dcamachog1501 on 01/06/19.
//

#include "Huffman_Node.h"


Huffman_Node::Huffman_Node(Character c)
{
    this->left= nullptr;
    this->right= nullptr;
    this->value= new Character();
    *this->value= c;
    this->codified=false;
}

Huffman_Node *Huffman_Node::getRight()
{
    return this->right;
}

Huffman_Node *Huffman_Node::getLeft()
{
    return this->left;
}

Huffman_Node::Character *Huffman_Node::getValue()
{
    return this->value;
}

void Huffman_Node::setRight(Huffman_Node *hn)
{
    this->right=hn;
}

void Huffman_Node::setleft(Huffman_Node *hn)
{
    this->left=hn;
}

bool Huffman_Node::isCodified()
{
    return codified;
}

void Huffman_Node::codify()
{
this->codified=true;
}
