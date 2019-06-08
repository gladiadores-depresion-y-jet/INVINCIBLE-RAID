//
// Created by dcamachog1501 on 01/06/19.
//

#ifndef RAID_5_LIST_H
#define RAID_5_LIST_H
#include "Node.h"
#include <iostream>
using namespace std;
template <class T>
class List
{
private:
    Node<T>* head;
    int* length;
    List<T>* next;
public:
    /**
     * Constructor de la clase List.
     */
    List()
    {
        this->head= nullptr;
        this->length=new int(0);
        this->next= nullptr;
    }
    /**
     * Metodo para añadir un valor a la lista.
     * @param v Valor a añadir.
     */
    void add(T v)
    {
        Node<T>* n= new Node<T>(v,++*this->length);
        if(this->head== nullptr)
        {
            this->head=n;
        }
        else
        {
            Node<T>* temp= this->head;
            while(temp->getNext()!= nullptr)
            {
                temp=temp->getNext();
            }
            temp->setNext(n);
        }
    }
    /**
     * Metodo para obtener el tamaño de la lista.
     * @return Tamaño de la lista.
     */
    int getLength()
    {
        return *this->length;
    }
    /**
     * Metodo para obtener la cabeza de la lista.
     * @return Nodo cabeza de la lista.
     */
    Node<T>* getHead()
    {
        return this->head;
    }
    void setHead(Node<T>* h)
    {
        this->head=h;
    }
    void arrange()
    {
        Node<T>* temp2= this->head;
        int newOrd=1;
        while(temp2!= nullptr)
        {
            temp2->setOrder(newOrd);
            temp2=temp2->getNext();
            newOrd++;
        }
        temp2= nullptr;
        delete(temp2);
    }
    void delFirst()
    {
        Node<T>* temp= this->head;
        this->head=temp->getNext();
        temp->purge();
        delete(temp);
        *this->length=*this->length-1;
        arrange();
    }
};

#endif //RAID_5_LIST_H
