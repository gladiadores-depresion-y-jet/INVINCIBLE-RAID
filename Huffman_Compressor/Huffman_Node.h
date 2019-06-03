//
// Created by dcamachog1501 on 02/06/19.
//

#ifndef RAID_5_HUFFMAN_NODE_H
#define RAID_5_HUFFMAN_NODE_H


class Huffman_Node
{
    public:
        struct Character
        {
        private:
            int times;
            char* digit;
        public:
            Character()
            {
                this->times=0;
                this->digit= new char();
            }
            int getTimes()
            {
                return this->times;
            }
            char* getDigit()
            {
                return this->digit;
            }
            void setTimes(int t)
            {
                this->times=t;
            }
            void setDigit(char d)
            {
                *this->digit=d;
            }
            void setDigit(char* d)
            {
                this->digit=d;
            }
        };
        Huffman_Node(Character c);
        Huffman_Node* getRight();
        Huffman_Node*getLeft();
        Character* getValue();
        void setRight(Huffman_Node* hn);
        void setleft(Huffman_Node* hn);

    private:
        Character* value;
        Huffman_Node* right;
        Huffman_Node* left;

    };


#endif //RAID_5_HUFFMAN_NODE_H
