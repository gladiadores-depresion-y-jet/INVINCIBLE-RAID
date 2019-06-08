//
// Created by dcamachog1501 on 01/06/19.
//

#ifndef RAID_5_COMPRESSOR_H
#define RAID_5_COMPRESSOR_H


#include <vector>
#include "Huffman_Node.h"
#include "../Structures/List.h"
#include "Huffman_Tree.h"
#include <map>

using namespace std;
class Compressor
{
    public:

        struct Code
        {
            private:
                char character;
                string coded;
            public:
                Code(Huffman_Node::Character ch, string cd)
                {
                    this->character=*ch.getDigit();
                    this->coded=cd;
                }
                string getCoded()
                {
                    return this->coded;
                }
                char getCharacter()
                {
                    return this->character;
                }
        };
        struct Codified_File
        {
            private:
                int min;
                string codigote;
                string ext;
                string name;
                map<char,string> codes;
                Huffman_Tree tree;
        public:
            Codified_File(string c, Huffman_Tree ht,string ext,string nam,map<char,string> cod )
            {
                this->codes=cod;
                this->codigote=c;
                this->tree=ht;
                this->ext=ext;
                this->name=nam;
            }
            string getCodigote()
            {
                return this->codigote;
            }
            Huffman_Tree getTree()
            {
                return this->tree;
            }
            string getExt()
            {
                return this->ext;
            }
            string getName()
            {
                return this->name;
            }
            map<char,string> getCodes()
            {
                return this->codes;
            }
        };
        struct Decodified_File
        {
            private:
                vector<char> digits;
                string ext;
                string name;
            public:
                Decodified_File(vector<char> v,string ex,string n)
                {
                    this->digits=v;
                    this->ext=ex;
                    this->name=n;
                }
                vector<char> getDigits()
                {
                    return this->digits;
                }
                string getExt()
                {
                    return this->ext;
                }
                string getName()
                {
                    return this->name;
                }

        };
        struct Codifier_Node
        {
            private:
                int p_size;
                string dir;
                map<char,string> codes;
                bool done;
            public:
                Codifier_Node(int ps,string dir)
                {
                    this->p_size=ps;
                    this->dir=dir;
                    this->done=false;
                }
                bool isDone()
                {
                    return this->done;
                }
                void add(string code,char ch)
                {
                    codes[ch]=code;
                    if(codes.size()==p_size)
                    {
                        this->done=true;
                    }
                }
                map<char,string> getCodes()
                {
                    return this->codes;
                }
                void setDir(string s)
                {
                    this->dir=s;
                }
                string getDir()
                {
                    return this->dir;
                }
        };
        Compressor();
        Codified_File* compress(vector<char> digits,string ext,string nam);
        Decodified_File* decompress(Codified_File* code);
        Decodified_File* alterdecode(Codified_File* code);
        List<Huffman_Node *> * VecToList(vector<Huffman_Node::Character> vec);
        List<Huffman_Node*>* sort(List<Huffman_Node*>* list);
        map<char,string> codifier(Huffman_Tree tree,vector<Huffman_Node::Character> keys);
        void print(List<Huffman_Node*> list);
        Codifier_Node* backTrackCoder(Codifier_Node* cod, Huffman_Node* temp,Huffman_Node* alttemp ,string code);
        void print(vector<Code>,vector<char> word);
        string encoder(map<char,string> codes, vector<char> keys);
        Codified_File* treeReconstructor(string dirTree, string dirCodigo);
};


#endif //RAID_5_COMPRESSOR_H
