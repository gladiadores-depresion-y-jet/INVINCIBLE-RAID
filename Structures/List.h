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
    int length;
    List<T>* next;
public:
    /**
     * Constructor de la clase List.
     */
    List()
    {
        this->head= nullptr;
        this->length=0;
        this->next= nullptr;
    }
    /**
     * Metodo para añadir un valor a la lista.
     * @param v Valor a añadir.
     */
    void add(T v)
    {
        Node<T>* n= new Node<T>(v,++length);
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
     * Metodo para eliminar un valr de la lista.
     * @param o Identificador del nodo donde esta almacenado el valor.
     */
    void del(int o)
    {
        Node<T>* temp=this->head;
        if(temp->getOrder()==o)
        {
            this->head=temp->getNext();
            temp->purge();
            delete(temp);
            Node<T>* temp2= this->head;
            while(temp2!= nullptr)
            {
                temp2->setOrder(temp2->getOrder()-1);
                temp2=temp2->getNext();
            }
            temp2= nullptr;
            delete(temp2);
        }
        else
        {
            while (temp->getNext()->getOrder() != o) {
                temp = temp->getNext();
            }
            Node<T> *temp2 = temp->getNext();
            temp->setNext(temp2->getNext());
            temp2->purge();
            temp = temp->getNext();
            while (temp != nullptr) {
                temp->setOrder(temp->getOrder() - 1);
                temp = temp->getNext();
            }
            delete (temp);
            delete (temp2);
        }
        length--;
    }
    /**
     * Metodo para obtener la lista qu ele sigue a la lista actual.
     * @return Lista siguiente.
     */
    List<T>* getNext()
    {
        return this->next;
    }
    /**
     * Metodo para obtener el tamaño de la lista.
     * @return Tamaño de la lista.
     */
    int getLength()
    {
        return this->length;
    }
    /**
     * Metodo para establecer la lista que le sigue a la lista actual.
     * @param l Lista siguiente.
     */
    void setNext(List<T>* l)
    {
        this->next=l;
    }
    /**
     * Metodo para obtener la cabeza de la lista.
     * @return Nodo cabeza de la lista.
     */
    Node<T>* getHead()
    {
        return this->head;
    }
    /**
     * Metodo para eliminar todos los elementos de la lista.
     */
    void purge()
    {
        int l=this->length;
        for(int i=1;i<=l;i++)
        {
            del(1);
        }
    }
    void setHead(Node<T>* h)
    {
        this->head=h;
    }
    void release(int o)
    {
        Node<T>* temp=this->head;
        if(temp->getOrder()==o)
        {
            this->head=temp->getNext();
            Node<T>* temp2= this->head;
            while(temp2!= nullptr)
            {
                temp2->setOrder(temp2->getOrder()-1);
                temp2=temp2->getNext();
            }
            temp2= nullptr;
            delete(temp2);
        }
        else
        {
            while (temp->getNext()->getOrder() != o)
            {
                temp = temp->getNext();
            }
            Node<T> *temp2 = temp->getNext();
            temp->setNext(temp2->getNext());
            temp = temp->getNext();
            while (temp != nullptr) {
                temp->setOrder(temp->getOrder() - 1);
                temp = temp->getNext();
            }
            delete (temp);
        }
        this->length--;
    }
    void adjustSize()
    {
        Node<T>* temp= this->head;
        int cont=0;
        while(temp!= nullptr)
        {
            cont++;
            temp=temp->getNext();
        }
        cout<<"Real Size: "<<to_string(cont)<<endl;
    }
};

#endif //RAID_5_LIST_H
