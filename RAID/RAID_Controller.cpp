//
// Created by dcamachog1501 on 01/06/19.
//

#include <fstream>
#include <iostream>
#include "RAID_Controller.h"

RAID_Controller::RAID_Controller()
{
    Initializer();
}

void RAID_Controller::Initializer()
{
    this->diskList=new List<Disk*>();
    this->comp= new Compressor();
    DIR *rDir=opendir("RAID");
    if(rDir== nullptr)
    {
        if (dirCreator("RAID"))
        {
            cout << "RAID creado\n";
            diskInitializer(false);
        }
        else
        {
            cout<<"No se puede crear el RAID"<<endl;
        }

    }
    else
    {
        cout<<"El RAID ya existe\n"<<endl;
        diskInitializer(true);
    }


}

void RAID_Controller::diskInitializer(bool exists)
{
    if(exists)
    {
        for(int i=1;i<=4;i++)
        {
            DIR *rDir=opendir(("RAID/Disk_"+to_string(i)).c_str());
            if(rDir!= nullptr)
            {

                auto d = new Disk(i);
                struct dirent *dp;
                while ((dp = readdir(rDir)) != NULL)
                {
                    if(string(dp->d_name)!="."&&string(dp->d_name)!="..")
                    {
                        string dp_name=string(dp->d_name);
                        vector<string> input;
                        FilePart* f;
                        if(input.size()>1)
                        {
                            boost::split(input,dp_name, boost::is_any_of("$"));
                            f= new FilePart(stoi(input.at(1)),input.at(0));
                        }
                        else
                        {
                            f= new FilePart(dp_name);
                        }
                        d->getfileColumn()->add(f);
                    }
                }
                d->parityCounter();
                this->diskList->add(d);
            }
            else
            {
                cout<<"El disco "<<to_string(i)<<" no existe"<<endl;
                dirCreator(("RAID/Disk_"+to_string(i)).c_str());
                cout<<"Disco "<<to_string(i)<<" reestablecido, iniciando reconstrucción\n"<<endl;
            }
        }
        cout<<"Discos inicializados correctamente"<<endl;
    }
    else
    {
        for(int i=1; i<=4;i++)
        {
            auto* d = new Disk(i);
            this->diskList->add(d);
            dirCreator(("RAID/Disk_"+to_string(i)).c_str());
        }
        this->disks=this->diskList->getLength();
        cout<<"Discos inicializados correctamente"<<endl;
    }

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

Disk *RAID_Controller::diskGetter(int diskN)
{
    auto temp=this->diskList->getHead();

    while(temp!= nullptr)
    {
        if(temp->getValue()->getDiskN()==diskN)
        {
            return temp->getValue();
        }
    }
    return nullptr;
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

    Compressor::Codified_File* code=comp->compress(data,input.at(1),input.at(0));
    diskWriter(code);
    return code;
}

void RAID_Controller::compose(Compressor::Decodified_File *DecFile)
{
    ofstream outfile(DecFile->getName()+"_New."+DecFile->getExt(), ios::out | ios::binary);
    outfile.write(&DecFile->getDigits()[0], DecFile->getDigits().size());
}

void RAID_Controller::imageSplitter(string dir, string name)
{
    Mat image =imread(dir);
    for(int i=0;i<3;i++)
    {
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
        imwrite("Crap_"+to_string(i)+".jpg",cropped);
    }

}

void RAID_Controller::diskWriter(Compressor::Codified_File* coded)
{
    ofstream outfile(coded->getName()+"_Codigote.txt", ios::out | ios::binary);
    ofstream out;
    out.open(coded->getName()+"_Codigote.txt");

    out<<coded->getCodigote()<<endl;

    out.close();
    imageSplitter(coded->getName()+"."+coded->getExt(),"");

    ofstream outtree(coded->getName()+"_Tree.txt",ios::out|ios::binary);
    ofstream outT;
    outT.open(coded->getName()+"_Tree.txt");

    int ind=coded->getCodes().size();
    outT<<coded->getName()<<endl;
    outT<<coded->getExt()<<endl;
    for(int i=0;i<ind;i++)
    {
        string s;
        s=coded->getCodes().at(i).getCharacter();
        outT<<s+coded->getCodes().at(i).getCoded()<<endl;
    }
    outT.close();
}
