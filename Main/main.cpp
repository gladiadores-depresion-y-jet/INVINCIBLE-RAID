#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include "../RAID/RAID_Controller.h"
#include "../Huffman_Compressor/Compressor.h"
#include "server/requestHandler.h"
#include <pistache/endpoint.h>

using namespace Pistache;
using namespace std;
using namespace cv;
int main()
{
    auto comp= new Compressor();
    auto *r= new RAID_Controller();
    bool cond=true;
    while(true)
    {
        if(cond)
        {
            /*Compressor::Codified_File* T1=r->imageDecomposer("Incoming/Prueba.jpg");
            Compressor::Codified_File* T2=r->imageDecomposer("Incoming/Prueba2.jpg");
            Compressor::Codified_File* T3=r->imageDecomposer("Incoming/Prueba3.jpg");
            Compressor::Codified_File* T4=r->imageDecomposer("Incoming/Prueba4.jpg");
            Compressor::Codified_File* T5=r->imageDecomposer("Incoming/Prueba5.jpg");
            Compressor::Codified_File* T6=r->imageDecomposer("Incoming/PruebaSing.jpeg");*/
            Compressor::Codified_File* T7=r->imageDecomposer("Incoming/Prueba6.gif");



            /*Compressor::Codified_File *t1 = r->merge("Prueba_");
            Compressor::Codified_File *t2= r->merge("Prueba2_");
            Compressor::Codified_File *t3= r->merge("Prueba3_");
            Compressor::Codified_File *t4= r->merge("Prueba4_");
            Compressor::Codified_File *t5= r->merge("Prueba5_");
            Compressor::Codified_File *t6= r->merge("PruebaSing_");

            comp->writeToDiskDecomp(comp->decompress(t1));
            comp->writeToDiskDecomp(comp->decompress(t2));
            comp->writeToDiskDecomp(comp->decompress(t3));
            comp->writeToDiskDecomp(comp->decompress(t4));
            comp->writeToDiskDecomp(comp->decompress(t5));
            comp->writeToDiskDecomp(comp->decompress(t6));
*/            cond=false;
        }
    }
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

    /*Compressor::Codified_File* c=r->imageDecomposer("Prueba.jpg");
    c=r->imageDecomposer("Prueba2.jpg");*/
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

    /*Address addr(Ipv4::any(), Port(9081));
    auto opts = Http::Endpoint::options()
            .threads(1);

    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(Http::make_handler<requestHandler>());
    server.serve();

    server.shutdown();*/

}