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
        string filename;
        string path;
    public:
        FilePart(string fNam,string path);
        string getFileName();
        string getFilePath();

};


#endif //RAID_5_FILEPART_H
