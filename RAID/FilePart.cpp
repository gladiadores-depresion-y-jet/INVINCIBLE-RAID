//
// Created by dcamachog1501 on 01/06/19.
//

#include "FilePart.h"

FilePart::FilePart(string fNam,string p,string pname,int pr)
{
    this->filename=fNam;
    this->path=p;
    this->purename=pname;
    this->partNumb=pr;
}
string FilePart::getFileName()
{
    return this->filename;
}

string FilePart::getFilePath()
{
    return this->path;
}
string FilePart::getPureName()
{
    return this->purename;
}
int FilePart::getpartNumb()
{
    return this->partNumb;
}

void FilePart::setPath(string dir)
{
    this->path=dir;
}
