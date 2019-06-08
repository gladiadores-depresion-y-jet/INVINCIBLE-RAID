#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include "../RAID/RAID_Controller.h"
#include "../Huffman_Compressor/Compressor.h"
using namespace std;
using namespace cv;
int main()
{
    auto comp= new Compressor();

    auto *r= new RAID_Controller();
    Compressor::Codified_File* c=r->imageDecomposer("Prueba.jpg");
    //Compressor::Decodified_File* dec=comp->decompress(c);
    r->diskWriter(c);
    Compressor::Codified_File* cod =comp->treeReconstructor("Prueba_Tree.txt","Prueba_Codigote.txt");

    Compressor::Decodified_File* dec=comp->decompress(cod);
    r->compose(dec);

    /*vector<char> data={'c','a','r','d','a','m','o','m','o'};
    comp->compress(data,".GIF","temp");*/

}