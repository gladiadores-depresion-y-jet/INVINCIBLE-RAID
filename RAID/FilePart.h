//
// Created by dcamachog1501 on 01/06/19.
//

#ifndef RAID_5_FILEPART_H
#define RAID_5_FILEPART_H

#include <string>

using namespace std;
class FilePart
{
    private:
        string purename;
        int partNumb;
        string filename;
        string path;
    public:
        /**
         * Constructor de la clase FilePart
         * @param fNam Nombre del archivo.
         * @param path Path a la imagen en el disco.
         * @param pn Nombre del archivo sin extension.
         * @param pr Numero de parte.
         */
        FilePart(string fNam,string path,string pn,int pr);
        /**
         *
         * @return
         */
        string getFileName();
        string getFilePath();
        string getPureName();
        int getpartNumb();

};


#endif //RAID_5_FILEPART_H
