//
// Created by dcamachog1501 on 08/06/19.
//

#include "Disk.h"

Disk::Disk(int dNumb,string adr)
{
    this->diskN=dNumb;
    this->adress=adr;
    this->regfileN=0;
    this->parityN=0;
}
Disk::Disk()
{
    this->diskN=0;
    this->adress="";
    this->regfileN=0;
    this->parityN=0;
}
int Disk::getDiskN()
{
    return this->diskN;
}

int Disk::getFileN()
{
    return (this->regfileN+this->parityN);
}

map<string,FilePart *> *Disk::getfileColumn()
{
    return this->fileColumn;
}

string Disk::getAdress()
{
    return this->adress;
}

void Disk::setFileColumn(map<string,FilePart*>* f)
{
    this->fileColumn=f;
}
