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

namespace fs = std::experimental::filesystem;
using namespace cv;

class RAID_Controller
{
    private:

        int c_disk;
        map<string,Disk*>* disks;
        map<string,FilePart*>* trees;
        Compressor *comp;
    public:
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
        RAID_Controller();
        void Initializer();
        map<string,Disk*>* getDisks();
        bool dirCreator(const char* dir);
        void diskInitializer();
        Compressor::Codified_File* imageDecomposer(string dir);
        void imageSplitter(string dir,string outDir,int disk_n,int i);
        void diskWriter(Compressor::Codified_File* coded,vector<Frag> parity);
        void compose(Compressor::Decodified_File *DecFile);
        vector<string> codigoteSplitter(string codigote);
        vector<Frag> parityCalculator(vector<string>);
        void reconstructDisk(int number);
        void diskVerifier();
        map<string,FilePart*>* fileFetcher(string dir);
        map<string,FilePart*>* treesFetcher();
        Compressor::Codified_File* merge(string name);


};


#endif //RAID_5_RAID_CONTROLLER_H
