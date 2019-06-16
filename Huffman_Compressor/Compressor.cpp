//
// Created by dcamachog1501 on 01/06/19.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <RAID/FilePart.h>
#include "Compressor.h"
#include "../Structures/List.h"
#include "Huffman_Tree.h"

Compressor::Compressor()
{

}

Compressor::Codified_File* Compressor::compress(vector<char> digits,string ext,string name,string org)
{
    cout<<"Comprimiendo..."<<endl;
    vector<Huffman_Node::Character> output;
    vector<char> found;
    int ind=digits.size();
    for (int i = 0; i < ind; i++)
    {
        auto it = std::find(found.begin(), found.end(), digits.at(i));
        if (it == found.end()) {
            int cont = 0;
            for (int j = i; j <(ind-i); j++)
            {
                if (digits.at(j) == digits.at(i))
                {
                    cont++;
                }
            }
            auto d = Huffman_Node::Character();
            d.setDigit(digits.at((i)));
            d.setTimes(cont);
            output.push_back(d);
            found.push_back(digits.at(i));
        }
        if(found.size()>=256)
        {
            break;
        }
    }
    List<Huffman_Node*>*HuffList= VecToList(output);

    while(HuffList->getLength()>=2)
    {
        HuffList=sort(HuffList);
        Huffman_Node::Character c= Huffman_Node::Character();
        c.setTimes(HuffList->getHead()->getValue()->getValue()->getTimes()+HuffList->getHead()->getNext()->getValue()->getValue()->getTimes());
        c.setDigit(nullptr);
        Huffman_Node *n= new Huffman_Node(c);
        n->setRight(HuffList->getHead()->getValue());
        n->setleft(HuffList->getHead()->getNext()->getValue());
        HuffList->delFirst();
        HuffList->delFirst();
        HuffList->add(n);
    }
    Huffman_Tree HuffTree= Huffman_Tree();
    HuffTree.setTop(HuffList->getHead()->getValue());
    map<char,string>codified=codifier(HuffTree,output);
    string codigote=encoder(codified,digits);
    Codified_File* out= new Codified_File(codigote,HuffTree,ext,name,codified,org);
    return out;
}

List<Huffman_Node*>* Compressor::VecToList(vector<Huffman_Node::Character> vec)
{
    List<Huffman_Node*>*out= new List<Huffman_Node*>();
    for(int i= 0;i<vec.size();i++)
    {
        Huffman_Node* h= new Huffman_Node(vec.at(i));
        out->add(h);
    }
    return out;
}

List<Huffman_Node*>* Compressor::sort(List<Huffman_Node*>* list)
{
    Node<Huffman_Node*>* temp= list->getHead();
    while((temp->getNext()!= nullptr)&&(temp->getNext()->getNext()!= nullptr))
    {
        bool cond=((temp->getNext()->getValue()->getValue()->getTimes()==temp->getNext()->getNext()->getValue()->getValue()->getTimes())&&(temp->getNext()->getValue()->getValue()->getDigit()!=nullptr)&&(temp->getNext()->getNext()->getValue()->getValue()->getDigit()==nullptr));
        if(temp->getValue()->getValue()->getDigit()==list->getHead()->getValue()->getValue()->getDigit())
        {
            bool subcond=((temp->getValue()->getValue()->getTimes()==temp->getNext()->getValue()->getValue()->getTimes())&&(temp->getNext()->getValue()->getValue()->getDigit()==nullptr)&&(temp->getValue()->getValue()->getDigit()!=nullptr));
            bool altercond=(temp->getValue()->getValue()->getTimes())>(temp->getNext()->getValue()->getValue()->getTimes());
            if(altercond||subcond)
            {
                list->setHead(temp->getNext());
                temp->setNext(temp->getNext()->getNext());
                list->getHead()->setNext(temp);
                temp=list->getHead();
            }
        }
        if(temp->getNext()->getNext()!= nullptr)
        {
            if ((temp->getNext()->getNext()->getValue()->getValue()->getTimes()<temp->getNext()->getValue()->getValue()->getTimes())||cond)
            {
                Node<Huffman_Node *> *temp2 = temp->getNext()->getNext();
                temp->getNext()->setNext(temp2->getNext());
                temp2->setNext(temp->getNext());
                temp->setNext(temp2);
                temp = list->getHead();
            }
            else
            {
                temp=temp->getNext();
            }
        }
    }
    list->arrange();
    return list;
}

void Compressor::print(List<Huffman_Node *> list)
{
    Node<Huffman_Node*>* temp=list.getHead();
    while(temp!= nullptr)
    {
        cout<<" = "<<to_string(temp->getValue()->getValue()->getTimes())<<endl;
        temp=temp->getNext();
    }
}

map<char,string> Compressor::codifier(Huffman_Tree tree,vector<Huffman_Node::Character> keys)
{
    Codifier_Node *codifier= new Codifier_Node(keys.size(),"1");
    Codifier_Node* cd=backTrackCoder(codifier,tree.getTop(), nullptr,"");

    return cd->getCodes();

}

Compressor::Codifier_Node* Compressor::backTrackCoder(Codifier_Node* cod, Huffman_Node* temp,Huffman_Node* alttemp,string code)
{
    if(cod->isDone())
    {
        return cod;
    }
    else if(temp->getValue()->getDigit()!= nullptr)
    {
            cod->add(code,*temp->getValue()->getDigit());
            cod->setDir("-1");
            return cod;
    }
    else
    {
        Codifier_Node* ret=backTrackCoder(cod,temp->getRight(),temp,code+"1");
        if(ret->getDir()=="-1")
        {
            cod->setDir("0");
            Codifier_Node* retleft=backTrackCoder(cod,temp->getLeft(),temp,code+"0");
            if(retleft->getDir()=="-1")
            {
                retleft->setDir("-1");
                return retleft;
            }
            else
            {
                return retleft;
            }
        }
        else
        {
            return ret;
        }
    }
}
void Compressor::print(vector<Compressor::Code> codes, vector<char> digits)
{
    /*for(int j=0;j<codes.size();j++)
    {
        cout<<codes.at(j).getCharacter()<<" : "<<codes.at(j).getCoded()<<"\n"<<endl;
    }
*/
}

string Compressor::encoder(map<char,string> codes, vector<char>keys)
{
    string out="";
    int size=keys.size();
    int csize=codes.size();
    int cont=0;
    int its=0;
    vector<Code>found;
    for(int i=0;i<size;i++)
    {
        out+=codes[keys.at(i)];
    }
    cout<<"Real bit size: "<<to_string(size*8)<<"\n"<<endl;
    cout<<"Compressed bit size:"<<out.size()<<endl;
    return out;
}

Compressor::Decodified_File* Compressor::decompress(Compressor::Codified_File* code)
{
    cout<<"Descomprimiendo..."<<endl;
    vector<char> out;
    string var= code->getCodigote();
    int ind= code->getCodigote().size();
    int i=0;
    while(i<ind)
    {
        char act=var.at(i);
        Huffman_Node* temp=code->getTree().getTop();
        while(temp->getValue()->getDigit()==nullptr)
        {
            if(act=='1')
            {
                temp=temp->getRight();
            }
            else if(act=='0')
            {
                temp=temp->getLeft();
            }
            if((i+1)==ind)
            {
                i++;
                break;
            }
            act=var.at(++i);
            /*cout<<i<<endl;*/
        }
        out.push_back(*temp->getValue()->getDigit());
    }

    cout<<"Tamano del archivo descomprimido: "<<out.size()<<" Bytes"<<endl;
    Decodified_File* dec= new Decodified_File(out,code->getExt(),code->getName());
    return dec;
}

Compressor::Codified_File* Compressor:: treeReconstructor(string dirTree,string codigote)
{
    std::ifstream file(dirTree);
    std::string str;
    Huffman_Node::Character c= Huffman_Node::Character();
    c.setDigit(nullptr);
    Huffman_Node* head= new Huffman_Node(c);
    Huffman_Node* temp=head;

    getline(file,str);
    string name=str;
    getline(file,str);
    string ext=str;
    map<char,string> codes;
    std::getline(file, str);
    while (true)
    {
        if(str=="")
        {
            string t=str;
            getline(file,str);
            if(str!="")
            {
                Huffman_Node::Character cha= Huffman_Node::Character();
                cha.setDigit('\n');
                Huffman_Node* n= new Huffman_Node(cha);
                string code= str;
                codes['\n']=code;

                vector<char> digits;
                for(int i=0;i<code.size();i++)
                {
                    digits.push_back(code.at(i));
                }
                for(int j=0;j<digits.size();j++)
                {
                    char t=digits.at(j);
                    if(j+1==digits.size())
                    {
                        if(t=='1')
                        {
                            temp->setRight(n);
                        }
                        else if(t=='0')
                        {
                            temp->setleft(n);
                        }
                        break;
                    }
                    else if(t=='1')
                    {
                        if(temp->getRight()== nullptr)
                        {
                            Huffman_Node::Character c= Huffman_Node::Character();
                            c.setDigit(nullptr);
                            Huffman_Node* hn= new Huffman_Node(c);
                            temp->setRight(hn);
                        }
                        temp=temp->getRight();
                    }
                    else if(t=='0')
                    {
                        if(temp->getLeft()== nullptr)
                        {
                            Huffman_Node::Character c= Huffman_Node::Character();
                            c.setDigit(nullptr);
                            Huffman_Node* hn= new Huffman_Node(c);
                            temp->setleft(hn);
                        }
                        temp=temp->getLeft();
                    }
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            Huffman_Node::Character cha= Huffman_Node::Character();
            cha.setDigit(str.at(0));
            Huffman_Node* n= new Huffman_Node(cha);
            string code= str.substr(1,str.size());

            codes[str.at(0)]=code;

            vector<char> digits;
            for(int i=0;i<code.size();i++)
            {
                digits.push_back(code.at(i));
            }
            for(int j=0;j<digits.size();j++)
            {
                char t=digits.at(j);
                if(j+1==digits.size())
                {
                    if(t=='1')
                    {
                        temp->setRight(n);
                    }
                    else if(t=='0')
                    {
                        temp->setleft(n);
                    }
                    break;
                }
                else if(t=='1')
                {
                    if(temp->getRight()== nullptr)
                    {
                        Huffman_Node::Character c= Huffman_Node::Character();
                        c.setDigit(nullptr);
                        Huffman_Node* hn= new Huffman_Node(c);
                        temp->setRight(hn);
                    }
                    temp=temp->getRight();
                }
                else if(t=='0')
                {
                    if(temp->getLeft()== nullptr)
                    {
                        Huffman_Node::Character c= Huffman_Node::Character();
                        c.setDigit(nullptr);
                        Huffman_Node* hn= new Huffman_Node(c);
                        temp->setleft(hn);
                    }
                    temp=temp->getLeft();
                }
            }
        }
        getline(file,str);
        temp=head;
    }
    Huffman_Tree* tree= new Huffman_Tree();
    tree->setTop(head);

    auto codifiedFile= new Compressor::Codified_File(codigote,*tree,ext,name,codes,"Incoming/"+name+"."+ext);
    return codifiedFile;
}
void Compressor::writeToDiskDecomp(Compressor::Decodified_File *DecFile)
{
    ofstream outfile("Outport/"+DecFile->getName()+"."+DecFile->getExt(), ios::out | ios::binary);
    outfile.write(&DecFile->getDigits()[0], DecFile->getDigits().size());
    delete(DecFile);
}

string Compressor::getImage(Compressor::Decodified_File *pFile) {

}





