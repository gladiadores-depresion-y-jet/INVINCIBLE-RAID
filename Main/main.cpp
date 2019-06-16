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
/*    RAID_Controller *r = new RAID_Controller();
    Compressor* comp = new Compressor();*/
    /*Compressor::Codified_File* T1=r->imageDecomposer("Incoming/Prueba.jpeg");
    Compressor::Codified_File* T2=r->imageDecomposer("Incoming/Prueba2.jpg");
    Compressor::Codified_File* T3=r->imageDecomposer("Incoming/Prueba3.jpg");
    Compressor::Codified_File* T4=r->imageDecomposer("Incoming/Prueba4.jpg");
    Compressor::Codified_File* T5=r->imageDecomposer("Incoming/Prueba5.jpg");
    Compressor::Codified_File* T6=r->imageDecomposer("Incoming/PruebaSing.jpeg");*/



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
    comp->writeToDiskDecomp(comp->decompress(t6));*/
    Address addr(Ipv4::any(), Port(9083));
    auto opts = Http::Endpoint::options()
            .threads(1);

    Http::Endpoint server(addr);
    server.init(opts);
    server.setHandler(Http::make_handler<requestHandler>());
    server.serve();

    server.shutdown();

}