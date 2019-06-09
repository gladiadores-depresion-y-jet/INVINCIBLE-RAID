//
// Created by dcamachog1501 on 01/06/19.
//

#include "FilePart.h"

FilePart::FilePart(string fNam,string p)
{
    this->filename=fNam;
    this->path=p;
}
string FilePart::getFileName()
{
    return this->filename;
}

string FilePart::getFilePath()
{
    return this->path;
}


