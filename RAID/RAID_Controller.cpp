//
// Created by dcamachog1501 on 01/06/19.
//

#include <fstream>
#include <iostream>
#include "RAID_Controller.h"

namespace fs = std::experimental::filesystem;

RAID_Controller::RAID_Controller()
{
    Initializer();
}

void RAID_Controller::Initializer()
{
    this->c_disk=3;
    this->comp= new Compressor();
    this->disks= new map<string,Disk*>();
    DIR *rDir=opendir("RAID");
    if(rDir== nullptr)
    {
        if (dirCreator("RAID"))
        {
            cout << "RAID creado\n";
            diskInitializer();
        }
        else
        {
            cout<<"No se puede crear el RAID"<<endl;
        }

    }
    else
    {
        cout<<"El RAID ya existe\n"<<endl;
        diskInitializer();
    }
    DIR *inDir=opendir("Incoming");
    if(inDir== nullptr)
    {
        dirCreator("Incoming");
    }

    inDir=opendir("Outport");
    if(inDir== nullptr)
    {
        dirCreator("Outport");
    }

}

void RAID_Controller::diskInitializer()
{

    for(int i=1;i<=4;i++)
    {
        DIR *rDir=opendir(("RAID/Disk_"+to_string(i)).c_str());
        if(rDir!= nullptr)
        {
            Disk *d = new Disk(i,"RAID/Disk_"+to_string(i));
            map<string,FilePart*>* col= new map<string,FilePart*>();
            d->setFileColumn(col);
            this->disks->insert(pair<string,Disk*>("Disk_"+to_string(i),d) );
            d->setFileColumn(fileFetcher("RAID/Disk_"+to_string(i)));
        }
        else
        {
            cout<<"El disco "<<to_string(i)<<" no existe"<<endl;
            dirCreator(("RAID/Disk_"+to_string(i)).c_str());
            Disk *d = new Disk(i,"RAID/Disk_"+to_string(i));
            map<string,FilePart*>* col= new map<string,FilePart*>();
            d->setFileColumn(col);
            this->disks->insert(pair<string,Disk*>("Disk_"+to_string(i),d) );
            cout<<"Disco "<<to_string(i)<<" reestablecido, iniciando reconstruccion\n"<<endl;
        }
    }
    string dir="../Trees";
    DIR *rDir=opendir(dir.c_str());
    if(rDir== nullptr)
    {
        dirCreator(dir.c_str());
        this->trees=new map<string,FilePart*>();
    }
    else
    {
        this->trees=treesFetcher();
    }
    cout<<"Discos inicializados correctamente"<<endl;
}

bool RAID_Controller::dirCreator(const char* dir)
{
    if (mkdir(dir, 0777) == -1)
    {
        cerr << "Error :  " << strerror(errno) << endl;
        return false;
    }
    return true;
}

Compressor::Codified_File* RAID_Controller::imageDecomposer(string dir)
{
    vector<string> input;
    boost::split(input,dir, boost::is_any_of("."));
    ifstream ifs(dir, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();
    char* buffer = new char[pos];
    ifs.seekg(0, ios::beg);
    ifs.read(buffer, pos);
    std::vector<char> data(buffer, buffer+int(pos));
    vector<string> inp;
    boost::split(inp,input[0], boost::is_any_of("/"));
    Compressor::Codified_File* code=comp->compress(data,input.at(1),inp.at(1),dir);
    vector<Frag> f= parityCalculator(codigoteSplitter(code->getCodigote()));
    diskWriter(code,f);
    return code;
}

void RAID_Controller::compose(Compressor::Decodified_File *DecFile)
{
    ofstream outfile(DecFile->getName()+"_New."+DecFile->getExt(), ios::out | ios::binary);
    outfile.write(&DecFile->getDigits()[0], DecFile->getDigits().size());
}

void RAID_Controller::imageSplitter(string dir, string name,int disk_n,int i)
{
    Mat image =imread(dir);
    Rect crop;
    if(i==0)
    {
        crop= Rect(0,0,image.cols/3,image.rows);
    }
    else if (i==1)
    {
        crop= Rect(image.cols/3,0,image.cols/3,image.rows);
    }
    else if(i==2)
    {
        crop= Rect(2*image.cols/3,0,image.cols/3,image.rows);
    }
    Mat cropped=image(crop);
    imwrite(this->disks->at("Disk_"+to_string(disk_n))->getAdress()+"/"+name+"_"+"ImgPt_"+to_string(i+1)+".jpg",cropped);
}

void RAID_Controller::diskWriter(Compressor::Codified_File* coded,vector<Frag> parity)
{
    for(int i=0;i<4;i++)
    {
        if(i==c_disk)
        {
            ofstream outT(this->disks->at("Disk_"+to_string(c_disk+1))->getAdress()+"/"+coded->getName()+"_Parity.txt",ios::out|ios::binary);
            outT<<"Size:"+to_string(parity.at(i).getRealSize())<<endl;
            outT<<parity.at(i).getFragment()<<endl;
            FilePart* f= new FilePart(coded->getName()+"_Parity.txt",this->disks->at("Disk_"+to_string(c_disk+1))->getAdress()+"/"+coded->getName()+"_Parity.txt");
            this->disks->at("Disk_"+to_string(c_disk+1))->getfileColumn()->emplace(std::pair<string,FilePart*>(coded->getName()+"_Parity.txt",f));

            ofstream outI(this->disks->at("Disk_"+to_string(c_disk+1))->getAdress()+"/"+coded->getName()+"_Info.txt",ios::out|ios::binary);
            outI<<"TotalSize:"+to_string(coded->getCodigote().size())<<endl;
            FilePart* inf= new FilePart(coded->getName()+"_Info.txt",this->disks->at("Disk_"+to_string(c_disk+1))->getAdress()+"/"+coded->getName()+"_Info.txt");
            this->disks->at("Disk_"+to_string(c_disk+1))->getfileColumn()->emplace(std::pair<string,FilePart*>(coded->getName()+"_Info.txt",inf));


        }
        else
        {
            ofstream outT(this->disks->at("Disk_"+to_string(i+1))->getAdress()+"/"+coded->getName()+"_Part_"+to_string(i+1)+".txt",ios::out|ios::binary);
            outT<<"Size:"+to_string(parity.at(i).getRealSize())<<endl;
            outT<<parity.at(i).getFragment()<<endl;
            FilePart* f= new FilePart(coded->getName()+"_Part_"+to_string(i+1)+".txt",this->disks->at("Disk_"+to_string(i+1))->getAdress()+"/"+coded->getName()+"_Part_"+to_string(i+1)+".txt");
            std::pair<string,FilePart*> p(coded->getName()+"_Part_"+to_string(i+1)+".txt",f);
            this->disks->at("Disk_"+to_string(i+1))->getfileColumn()->emplace(p);

            ofstream outI(this->disks->at("Disk_"+to_string(i+1))->getAdress()+"/"+coded->getName()+"_Info.txt",ios::out|ios::binary);
            outI<<"TotalSize:"+to_string(coded->getCodigote().size())<<endl;
            FilePart* inf= new FilePart(coded->getName()+"_Info.txt",this->disks->at("Disk_"+to_string(i+1))->getAdress()+"/"+coded->getName()+"_Info.txt");
            this->disks->at("Disk_"+to_string(i+1))->getfileColumn()->emplace(std::pair<string,FilePart*>(coded->getName()+"_Info.txt",inf));
            if(i>2)
            {
                imageSplitter("Incoming/"+coded->getName()+"."+coded->getExt(),coded->getName(),i+1,2);
            }
            else
            {
                imageSplitter("Incoming/"+coded->getName()+"."+coded->getExt(),coded->getName(),i+1,i);
            }

        }
    }
    c_disk--;
    if(c_disk<0)
    {
        c_disk=4;
    }
    ofstream outT("../Trees/"+coded->getName()+"_Tree.txt",ios::out|ios::binary);
    int ind=coded->getCodes().size();
    outT<<coded->getName()<<endl;
    outT<<coded->getExt()<<endl;
    int size=coded->getCodes().size();
    std::map<char, string>::const_iterator it = coded->getCodes().begin();
    for(int i=0;i<size; i++)
    {
        char key = it->first;
        string s;
        s=key;
        string value = it->second;
        outT<<s+value<<endl;
        it++;
    }
    outT.close();
    FilePart* f= new FilePart(coded->getName()+"_Tree.txt","../Trees/"+coded->getName()+"_Tree.txt");
    this->trees->insert(pair<string,FilePart*>(coded->getName()+"_Tree.txt",f));
}

vector<string> RAID_Controller::codigoteSplitter(string codigote)
{
    int size= codigote.size();
    int p_size=size/3;
    int extras=size%3;
    int current=0;
    vector<string> codes;
    for(int i=0;i<3;i++)
    {
        string out="";
        if(extras>0)
        {
            int cplus=current;
            for(int j=current;j<p_size+1+cplus;j++)
            {
                out+=codigote.at(j);
                current++;
            }
            extras--;
        }
        else
        {
            int cplus=current;
            for(int j=current;j<p_size+cplus;j++)
            {
                out+=codigote.at(j);
                current++;
            }
        }
        codes.push_back(out);
    }

    return codes;
}

vector<RAID_Controller::Frag> RAID_Controller::parityCalculator(vector<string> codes) {
    vector<Frag> out;
    int max = codes.at(0).size();
    for (int i = 1; i < 3; i++) {
        if (codes.at(i).size() > max) {
            max = codes.at(i).size();
        }
    }
    for (int j = 0; j < 3; j++) {
        if (codes.at(j).size() < max) {
            Frag f = Frag(codes.at(j).size(), codes.at(j) + "0");
            out.push_back(f);
        } else {
            Frag f = Frag(codes.at(j).size(), codes.at(j));
            out.push_back(f);
        }
    }
    string parity;
    char *act1=new char[max];
    char *act2=new char[max];
    char *act3=new char[max];

    strncpy(act1, out.at(0).getFragment().c_str(), sizeof(act1));
    strncpy(act2, out.at(1).getFragment().c_str(), sizeof(act2));
    strncpy(act3, out.at(2).getFragment().c_str(), sizeof(act3));

    for(int k=0;k<max;k++)
    {
        char act[3]={act1[k],act2[k],act3[k]};
        int mycount= count(act,act+2,'1');
        if(mycount==0||mycount==2)
        {
            parity+="0";
        }
        else if(mycount==1||mycount==3)
        {
            parity+="1";
        }
    }
    Frag f= Frag(max,parity);
    out.push_back(f);

    return out;

}

void RAID_Controller::diskVerifier()
{
    for (int i = 1; i <= 4; i++)
    {
        DIR *rDir = opendir(("RAID/Disk_" + to_string(i)).c_str());
        if (rDir == nullptr)
        {
            cout << "El disco " << to_string(i) << " no existe" << endl;
            dirCreator(("RAID/Disk_" + to_string(i)).c_str());
            reconstructDisk(i);
            cout << "Disco " << to_string(i) << " reestablecido, iniciando reconstrucción\n" << endl;
        }
    }

}
void RAID_Controller::reconstructDisk(int number)
{

}

map<string, Disk *> *RAID_Controller::getDisks()
{
    return this->disks;
}

map<string, FilePart *> *RAID_Controller::fileFetcher(string dir)
{
    map<string,FilePart*>* out= new map<string,FilePart*>();
    DIR *rDir=opendir(dir.c_str());
    if(rDir!= nullptr)
    {
        struct dirent *dp;
        while ((dp = readdir(rDir)) != NULL)
        {
            if (string(dp->d_name) != "." && string(dp->d_name) != "..") {
                string dp_name = string(dp->d_name);
                vector<string> input;
                FilePart *f= new FilePart(dp_name,dir+"/"+dp_name);
                out->insert(pair<string,FilePart*>(dp_name,f));
            }
        }
    }
    return out;

}

map<string, FilePart *> *RAID_Controller::treesFetcher()
{
    map<string,FilePart*>*out= new map<string,FilePart*>;
    string dir="../Trees";
    DIR *rDir=opendir(dir.c_str());
    if(rDir!= nullptr)
    {
        struct dirent *dp;
        while ((dp = readdir(rDir)) != NULL)
        {
            if (string(dp->d_name) != "." && string(dp->d_name) != "..") {
                string dp_name = string(dp->d_name);
                vector<string> input;
                FilePart *f= new FilePart(dp_name,dir+"/"+dp_name);
                out->insert(pair<string,FilePart*>(dp_name,f));
            }
        }
    }
    return out;
}

Compressor::Codified_File* RAID_Controller::merge(string name)
{
    vector<FilePart*> parts;
    for(int i=0;i<this->disks->size();i++)
    {
        for(auto const& imap:*this->disks->at("Disk_"+to_string(i+1))->getfileColumn())
        {
            if(boost::contains(imap.first,name))
            {
                parts.push_back(imap.second);
            }
        }
    }
    string codigote;
    int real_P=0;
    for(int j=0;j<parts.size();j++)
    {
        if(boost::contains(parts.at(j)->getFileName(),"Part_"))
        {
            vector<string> input;
            FilePart* act=parts.at(j);
            string intr;
            std::ifstream file(act->getFilePath());
            getline(file,intr);
            boost::split(input,intr, boost::is_any_of(":"));
            getline(file,intr);
            codigote+=intr.substr(0,stoi(input.at(1)));
            real_P++;
        }
    }
    int p=codigote.size();
    string dirTree;
    for(int k=0;k<this->trees->size();k++)
    {
        for(auto const& imap:*this->trees)
        {
            if(boost::contains(imap.first,name))
            {
               dirTree=imap.second->getFilePath();
               break;
            }
        }
    }
    Compressor::Codified_File* c= this->comp->treeReconstructor(dirTree,codigote);
    return c;
}
