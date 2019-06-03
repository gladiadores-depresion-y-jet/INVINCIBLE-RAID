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
    Compressor::Codified_File* c=r->imageDecomposer("Prueba.jpeg");
    Compressor::Decodified_File* dec=comp->decompress(c);
    r->compose(dec);
    /*vector<char> data={'l','a',' ','m','e','t','a','d','a','t','a',' ','e','s',' ','l','a',' ','d','a','t','a',' ','d','e',' ','l','a',' ','d','a','t','a'};
    comp->compress(data,".GIF","temp");*/
}