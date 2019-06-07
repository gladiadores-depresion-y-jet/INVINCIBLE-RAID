#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include "../RAID/RAID_Controller.h"
#include "../Huffman_Compressor/Compressor.h"

using namespace std;
int main()
{
    auto comp= new Compressor();

    auto *r= new RAID_Controller();
    Compressor::Codified_File* c=r->imageDecomposer("Prueba.jpg");
    Compressor::Decodified_File* dec=comp->decompress(c);
    r->compose(dec);
    /*vector<char> data={'c','a','r','d','a','m','o','m','o'};
    comp->compress(data,".GIF","temp");*/
}