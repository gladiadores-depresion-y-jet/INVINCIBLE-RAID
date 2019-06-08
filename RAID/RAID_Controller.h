//
// Created by dcamachog1501 on 01/06/19.
//

#ifndef RAID_5_RAID_CONTROLLER_H
#define RAID_5_RAID_CONTROLLER_H


#include "../Structures/List.h"
#include "Disk.h"
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

namespace fs = std::experimental::filesystem;
using namespace cv;

class RAID_Controller
{
    private:
        int disks;
        List<Disk*>* diskList;
        Compressor *comp;
    public:
        RAID_Controller();
        void Initializer();
        bool dirCreator(const char* dir);
        void diskInitializer(bool exists);
        Disk* diskGetter(int diskN);
        Compressor::Codified_File* imageDecomposer(string dir);
        void imageSplitter(string dir,string outDir);
        void diskWriter(Compressor::Codified_File* coded);


    void compose(Compressor::Decodified_File *DecFile);
};


#endif //RAID_5_RAID_CONTROLLER_H
