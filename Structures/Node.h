//
// Created by dcamachog1501 on 01/06/19.
//

#ifndef RAID_5_NODE_H
#define RAID_5_NODE_H

template <class T>
class Node
{
private:
    T value;
    int *order;
    Node* next;
public:
    /**
     * Constructor de la clase Node.
     */
    Node()
    {
        this->next= nullptr;
    }
    /**
     * Constructor alterno de la clase Node.
     * @param v Valor a almacenar.
     * @param o Identificador del nodo.
     */
    Node(T v,int o)
    {
        this->value=v;
        this->order=new int(o);
        this->next= nullptr;
    }
    /**
     * Metodo para obtener el valor que alamacena el nodo.
     * @return Valor almacenado.
     */
    T getValue()
    {
        return value;
    }
    /**
     * Metodo para obetener el nodo siguiente al nodo actual.
     * @return Nodo siguiente.
     */
    Node* getNext()
    {
        return next;
    }
    /**
     * Metodo para establecer el nodo siguiente al nodo actual.
     * @param node Nodo siguiente.
     */
    void setNext(Node<T>* node)
    {
        this->next=node;
    }
    /**
     * Metodo para borrar todos los valores almacenados en el nodo.
     */
    void purge()
    {
        delete(this->order);
        this->value= nullptr;
        this->next= nullptr;
    }
    /**
     * Metodo para establecer el identificador del nodo.
     * @param o Identificador del nodo.
     */
    void setOrder(int o)
    {
        *this->order=o;
    }
};

#endif //RAID_5_NODE_H
