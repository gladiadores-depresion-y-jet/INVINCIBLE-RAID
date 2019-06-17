//
// Created by dcamachog1501 on 01/06/19.
//

#ifndef RAID_5_HUFFMAN_TREE_H
#define RAID_5_HUFFMAN_TREE_H


#include "Huffman_Node.h"
class Huffman_Tree
{
    private:
        Huffman_Node* Top;
    public:
        /**
         * Constructor de la clase Huffman_Tree
         */
        Huffman_Tree();
        Huffman_Node* getTop();
        void setTop(Huffman_Node* h);

};


#endif //RAID_5_HUFFMAN_TREE_H
