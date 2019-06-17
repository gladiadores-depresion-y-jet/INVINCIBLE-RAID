//
// Created by dcamachog1501 on 01/06/19.
//

#ifndef RAID_5_COMPRESSOR_H
#define RAID_5_COMPRESSOR_H


#include <vector>
#include "Huffman_Node.h"
#include "../Structures/List.h"
#include "../RAID/Disk.h"
#include "Huffman_Tree.h"
#include <map>

using namespace std;
class Compressor
{
    public:
        /**
         * Estructura el caracter y su codigo asignado por el compresor.
         */
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
        /**
         * Estructura que almacena los datos fundamentales de un archivo pasado por el compresor.
         */
        struct Codified_File
        {
            private:
                int min;
                string codigote;
                string ext;
                string name;
                map<char,string> codes;
                Huffman_Tree tree;
                string origin;
            public:
                Codified_File(string c, Huffman_Tree ht,string ext,string nam,map<char,string> cod,string org)
                {
                    this->codes=cod;
                    this->codigote=c;
                    this->tree=ht;
                    this->ext=ext;
                    this->name=nam;
                    this->origin=org;
                }
                Codified_File()
                {
                    this->codigote="-1";
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
        /**
         * Estructura que almacena los datos fundamentales de una imagen decodificada.
         */
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
        /**
         * Estructura utilizada en la obtencion de codigos durante el backtracking.
         */
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
        /**
         * Constructor de la clase Compressor.
         */
        Compressor();
        /**
         * Metodo para comprimir archivos utilizando Huffman_Coding.
         * @param digits Vector de chars que conforman el archivo.
         * @param ext Extencion del archivo a comprimir.
         * @param nam Nombre del archivo a comprimir.
         * @param org Origen del archivo
         * @return Archivo codificado.
         */
        Codified_File* compress(vector<char> digits,string ext,string nam,string org);
        /**
         * Metodo para descomprimir un archivo codificado.
         * @param code Archivo codificado.
         * @return Archivo decodificado.
         */
        Decodified_File* decompress(Codified_File* code);
        /**
         * Metodo para convertir un vector en una lista enlazada de nodos Huffman.
         * @param vec Vector a convertir.
         * @return Lista obtenida.
         */
        List<Huffman_Node *> * VecToList(vector<Huffman_Node::Character> vec);
        /**
         * Metodo para ordenar una lista de nodos Huffman utilizando Bubblesort.
         * @param list Lista a ordenar.
         * @return Lista ordenada.
         */
        List<Huffman_Node*>* sort(List<Huffman_Node*>* list);
        /**
         * Metodo para obtener la tabla de codigos genrada por el algoritmo de Huffman.
         * @param tree Arbol Huffman del archivo.
         * @param keys Caracteres que conforman el arbol.
         * @return Tabla de caracteres.
         */
        map<char,string> codifier(Huffman_Tree tree,vector<Huffman_Node::Character> keys);
        /**
         * Algoritmo de backtracking para la obtencion de los codigos de la tabla de Huffman.
         * @param cod Nodo donde se guarda el codigo encontrado para determinado caracter.
         * @param temp Nodo que se esta visitando en el momento.
         * @param code Codigo actual encontrado para un caracter.
         * @return Tabla de codigos.
         */
        Codifier_Node* backTrackCoder(Codifier_Node* cod, Huffman_Node* temp,string code);
        /**
         * Metodo para obtener el codigo largo de 1's y 0's que genera el algoritmo de Huffman.
         * @param codes Tabla de codigos.
         * @param keys Caracteres a convertir.
         * @return Codigo largo .
         */
        string encoder(map<char,string> codes, vector<char> keys);
        /**
         * Metodo para recontruir el arbol del archivo a partir de un txt.
         * @param dirTree Direccion del arbol en el diso.
         * @param codigote Codigo largo del archivo.
         * @return Archivo codificado que contiene el arbol y el codigo largo.
         */
        Codified_File* treeReconstructor(string dirTree, string codigote);
        /**
         * Metodo para recontruir imagenes y guardarlas en el disco.
         * @param dec Archivo decodificado.
         */
        void writeToDiskDecomp(Decodified_File* dec);
};


#endif //RAID_5_COMPRESSOR_H
