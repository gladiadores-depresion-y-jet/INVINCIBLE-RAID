//
// Created by dcamachog1501 on 08/06/19.
//

#ifndef RAID_5_DISK_H
#define RAID_5_DISK_H

#include "FilePart.h"
#include <map>
#include <iostream>
using namespace std;
class Disk
{
private:
    int diskN;
    int regfileN;
    int parityN;
    string adress;
    map <string,FilePart*>* fileColumn;

public:
    Disk(int dNumb,string adr);
    Disk();
    int getDiskN();
    int getFileN();
    map<string,FilePart*>* getfileColumn();
    void setFileColumn(map<string,FilePart*>*);
    string getAdress();


};
#endif //RAID_5_DISK_H
