//
// Created by dcamachog1501 on 01/06/19.
//

#ifndef RAID_5_RAID_CONTROLLER_H
#define RAID_5_RAID_CONTROLLER_H


#include "../Structures/List.h"
#include "../Huffman_Compressor/Compressor.h"
#include <dirent.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <boost/algorithm/string.hpp>
#include <experimental/filesystem>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../Structures/Node.h"
#include "Disk.h"
#include "boost/algorithm/string.hpp"
#include <thread>
#include <cstdio>

namespace fs = std::experimental::filesystem;
using namespace cv;

class RAID_Controller
{
    private:

        int c_disk;
        map<string,Disk*>* disks;
        map<string,FilePart*>* trees;
        Compressor* comp;
    public:
    /**
     * Estructura que almacena un fragmento del codigo largo del Huffman y su tama?o original.
     */
    struct Frag
    {
        private:
            string fragment;
            int real_s;
        public:

            Frag(int s,string f)
            {
                this->fragment=f;
                this->real_s=s;
            }
            int getRealSize()
            {
                return this->real_s;
            }
            string getFragment()
            {
                return this->fragment;
            }
            void setFragment(string f)
            {
                this->fragment=f;
            }
    };
        /**
         * Constructor de la clase RAID_Controller.
         */
        RAID_Controller();
        /**
         * Metodo que inicaliza los discos del RAID asi como las carpetas necesarias para su ejecucion.
         */
        void Initializer();
        /**
         * Metodo para crear directorios
         * @param dir Direccion donde se quiere crear un directorio.
         * @return Condicion de si se pudo crear el directorio.
         */
        bool dirCreator(const char* dir);
        /**
         * Metodo para la inicializaci?n de los discos del RAID.
         */
        void diskInitializer();
        /**
         * Metodo para descomponer imagenes y guardarlas en los discos.
         * @param dir Direccion de la imagen en el disco.
         * @return Archivo comprimido.
         */
        Compressor::Codified_File* imageDecomposer(string dir);
        /**
         * Metodo para dividir las imagenes en 3
         * @param dir Directorio de la imagen.
         * @param outDir Directorio de salida de la imagen fraccionada.
         * @param disk_n Numero de disco donde se quiere guardar el fragmento de imagen
         * @param i Fragmento de imagen que se quiere obtener.
         */
        void imageSplitter(string dir,string outDir,int disk_n,int i);
        /**
         * Metodo para guardar los fragmentos de imagen en los discos.
         * @param coded Archivo codificado.
         * @param parity Fragmentos a guardar.
         */
        void diskWriter(Compressor::Codified_File* coded,vector<Frag> parity);
        /**
         * Metodo para dividir el codigo largo generado por el algoritmo de comoresion Huffman.
         * @param codigote Codgigo a fraccionar.
         * @return Vector de fragmentos.
         */
        vector<string> codigoteSplitter(string codigote);
        /**
         * Metodo para calcular la paridad de un archivo.
         * @return Vector de fragmentos, incluyendo el fragento de paridad.
         */
        vector<Frag> parityCalculator(vector<string>);
        /**
         * Metodo para reconstruir el disco en caso de un fallo.
         * @param number Numero de disco.
         * @return Resultado de la operacion.
         */
        bool reconstructDisk(int number);
        /**
         * Metodo que veirfica el estado de todos los discos.
         */
        void diskVerifier();
        /**
         * Metodo para obtener todas los archivos guardados fuera de ejecucion a los discos.
         * @param dir Directorio en el que se quiere buscar.
         * @return Columna de archivos.
         */
        map<string,FilePart*>* fileFetcher(string dir);
        /**
         * Metodo para obtener los arboles almacenados fuera de ejecucion.
         * @return Columna de arboles.
         */
        map<string,FilePart*>* treesFetcher();
        /**
         * Metodo para recalcular el fragmento perdido de codigo de una imagen.
         * @param part Informacion basica de la parte perdida.
         * @param info Informacion para la reconstruccion de la parte faltante.
         */
        void XORrecovery(FilePart *part,FilePart* info);
        /**
         * Metodo para reparar el disco perdido.
         * @param reference Disco a reconstruir.
         * @param antreference Disco siguiente a la referencia.
         */
        void repair(Disk* reference,Disk* antreference);
        /**
         * Metodo para unir todas las piezas de un codigo largo.
         * @param name Nombre del archivo a unir.
         * @return Archivo codificado.
         */
        Compressor::Codified_File* merge(string name);
        /**
         * Metodo para restaurar una imagen que se haya perdido por un fallo.
         * @param img Parte perdida de la imagen.
         * @param diskN Numero de disco donde estaba la parte.
         */
        void imageRestore(FilePart* img,int diskN);
        /**
         * Metodo queutiliza el REST server para solicitar una imagen almacenada en el disco.
         * @param id Nombre de la imagen.
         * @return String de caracteres.
         */
        string getImage(string id);
};


#endif //RAID_5_RAID_CONTROLLER_H
