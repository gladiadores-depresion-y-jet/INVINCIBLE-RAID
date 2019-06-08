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
    /*auto comp= new Compressor();*/

    auto *r= new RAID_Controller();
    /*string s="{\n"
             "    \"Image0\": \"{\\n    \\\"name\\\": \\\"danielImage\\\",\\n    \\\"author\\\": \\\"SOFIA\\\",\\n    \\\"description\\\": \\\"this shit\\\",\\n    \\\"year\\\": \\\"2018\\\",\\n    \\\"size\\\": \\\"2000\\\"\\n}\\n\",\n"
             "    \"Image1\": \"{\\n    \\\"name\\\": \\\"gabriel\\\",\\n    \\\"author\\\": \\\"GARZA\\\",\\n    \\\"description\\\": \\\"this is a description\\\",\\n    \\\"year\\\": \\\"2019\\\",\\n    \\\"size\\\": \\\"2000\\\"\\n}\\n\"\n"
             "}";
    vector<char> ch;
    for(int i=0;i<s.size();i++)
    {
        ch.push_back(s.at(i));
    }
    Compressor::Codified_File* file=comp->compress(ch,"txt","JSON");*/

    Compressor::Codified_File* c=r->imageDecomposer("Prueba2.jpg");
    //Compressor::Decodified_File* dec=comp->decompress(c);
    //r->diskWriter(c);
    //Compressor::Codified_File* cod =comp->treeReconstructor("Prueba_4_Tree.txt","Prueba_4_Codigote.txt");

    /*Compressor::Decodified_File* dec=comp->decompress(file);
    comp->writeToDiskComp(file);
    comp->writeToDiskDecomp(dec);
    r->compose(dec);
*/
    /*vector<char> data={'c','a','r','d','a','m','o','m','o'};
    comp->compress(data,".GIF","temp");*/

    /*string prueba= "1001010110100101101010";
    vector<string> s= r->codigoteSplitter(prueba);
    r->parityCalculator(s);*/



}