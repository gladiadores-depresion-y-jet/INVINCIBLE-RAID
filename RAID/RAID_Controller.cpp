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
        Disk *d = new Disk(i,"RAID/Disk_"+to_string(i));
        map<string,FilePart*>* col=fileFetcher(d->getAdress());
        d->setFileColumn(col);
        this->disks->insert(pair<string,Disk*>("Disk_"+to_string(i),d) );
        cout<<"Disco "<<to_string(i)<<" creado\n"<<endl;
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
    diskVerifier();
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
    this->comp=Compressor();
    diskVerifier();
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
    Compressor::Codified_File* code=comp.compress(data,input.at(1),inp.at(1),dir);

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
    int part=1;
    int parityind=0;
    for(int i=0;i<4;i++)
    {
        if(i==c_disk)
        {
            parityind=i;
            ofstream outT(this->disks->at("Disk_"+to_string(c_disk+1))->getAdress()+"/"+coded->getName()+"_Parity.txt",ios::out|ios::binary);
            outT<<"Size:"+to_string(parity.at(3).getRealSize())<<endl;
            outT<<parity.at(3).getFragment()<<endl;
            FilePart* f= new FilePart(coded->getName()+"_Parity.txt",this->disks->at("Disk_"+to_string(c_disk+1))->getAdress()+"/"+coded->getName()+"_Parity.txt",coded->getName(),4);
            this->disks->at("Disk_"+to_string(c_disk+1))->getfileColumn()->emplace(std::pair<string,FilePart*>(coded->getName()+"_Parity.txt",f));

            ofstream outI(this->disks->at("Disk_"+to_string(c_disk+1))->getAdress()+"/"+coded->getName()+"_Info.txt",ios::out|ios::binary);
            outI<<"TotalSize:"+to_string(coded->getCodigote().size())<<endl;
            FilePart* inf= new FilePart(coded->getName()+"_Info.txt",this->disks->at("Disk_"+to_string(c_disk+1))->getAdress()+"/"+coded->getName()+"_Info.txt",coded->getName(),0);
            this->disks->at("Disk_"+to_string(c_disk+1))->getfileColumn()->emplace(std::pair<string,FilePart*>(coded->getName()+"_Info.txt",inf));


        }
        else
        {
            ofstream outT(this->disks->at("Disk_"+to_string(i+1))->getAdress()+"/"+coded->getName()+"_Part_"+to_string(part)+".txt",ios::out|ios::binary);
            if(parityind!=0)
            {
                outT<<"Size:"+to_string(parity.at(parityind).getRealSize())<<endl;
                outT<<parity.at(parityind).getFragment()<<endl;
                parityind++;
            }
            else
            {
                outT<<"Size:"+to_string(parity.at(i).getRealSize())<<endl;
                outT<<parity.at(i).getFragment()<<endl;
            }
            FilePart* f= new FilePart(coded->getName()+"_Part_"+to_string(part)+".txt",this->disks->at("Disk_"+to_string(i+1))->getAdress()+"/"+coded->getName()+"_Part_"+to_string(part)+".txt",coded->getName(),part);
            std::pair<string,FilePart*> p(coded->getName()+"_Part_"+to_string(part)+".txt",f);
            this->disks->at("Disk_"+to_string(i+1))->getfileColumn()->emplace(p);

            ofstream outI(this->disks->at("Disk_"+to_string(i+1))->getAdress()+"/"+coded->getName()+"_Info.txt",ios::out|ios::binary);
            outI<<"TotalSize:"+to_string(coded->getCodigote().size())<<endl;
            FilePart* inf= new FilePart(coded->getName()+"_Info.txt",this->disks->at("Disk_"+to_string(i+1))->getAdress()+"/"+coded->getName()+"_Info.txt",coded->getName(),0);
            this->disks->at("Disk_"+to_string(i+1))->getfileColumn()->emplace(std::pair<string,FilePart*>(coded->getName()+"_Info.txt",inf));
            if(i>2)
            {
                imageSplitter("Incoming/"+coded->getName()+"."+coded->getExt(),coded->getName(),i+1,2);
            }
            else
            {
                imageSplitter("Incoming/"+coded->getName()+"."+coded->getExt(),coded->getName(),i+1,i);
            }
            part++;

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
    FilePart* f= new FilePart(coded->getName()+"_Tree.txt","../Trees/"+coded->getName()+"_Tree.txt",coded->getName(),0);
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

    strncpy(act1, out.at(0).getFragment().c_str(), max);
    strncpy(act2, out.at(1).getFragment().c_str(), max);
    strncpy(act3, out.at(2).getFragment().c_str(), max);

    for(int k=0;k<max;k++)
    {
        string s="";
        s+=act1[k];
        s+=act2[k];
        s+=act3[k];

        int mycount= count(s.begin(),s.end(),'1');
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
    int cont = 0;
    int disk = 0;
    for (int i = 1; i <= 4; i++)
    {
        DIR *rDir = opendir(("RAID/Disk_" + to_string(i)).c_str());
        if (rDir == nullptr)
        {
            disk=i;
            cont++;
        }
        closedir(rDir);
    }
    if (cont > 1&& cont<4)
    {
        cout << "ERROR: Mas de un disco se ha dañado, no se puede continuar con la ejecucion :(" << endl;
        throw std::exception();
    }
    else if(cont==4)
    {
        for(int i=0;i<this->disks->size();i++)
        {
            dirCreator(("RAID/Disk_"+to_string(i+1)).c_str());
        }
    }
    else if (cont > 0)
    {
        cout << "El disco " << to_string(disk) << " se ha danado, iniciando reconstruccion..." << endl;
        dirCreator(("RAID/Disk_"+to_string(disk)).c_str());
        if (reconstructDisk(disk))
        {
            cout << "Disco " << to_string(disk) << " reestablecido correctamente\n" << endl;
        }
        else
        {
            cout << "Disco " << to_string(disk) << " no se ha podido reestablecer\n" << endl;
            cout << "Finalizando ejecucion..." << endl;
            throw std::exception();
        }
    }
}
bool RAID_Controller::reconstructDisk(int number)
{
    Disk* reference=this->disks->at("Disk_"+to_string(number));
    Disk* antreference;

    if(number==4)
    {
        antreference=this->disks->at("Disk_1");
    }
    else
    {
        antreference=this->disks->at("Disk_"+to_string(number+1));
    }
    if(reference->getfileColumn()->size()==0)
    {
        vector<FilePart*> goodFiles;
        vector<FilePart*>currentParts;
        vector<FilePart*>currentImgs;

        vector<string> found;
        for(int i=1;i<=4;i++)
        {
            for(auto const& imap:*this->disks->at("Disk_"+to_string(i))->getfileColumn())
            {
                goodFiles.push_back(this->disks->at("Disk_"+to_string(i))->getfileColumn()->at(imap.first));
            }
        }
        for(int i=0;i<goodFiles.size();i++)
        {
            string searchingName=goodFiles.at(i)->getPureName();
            if(std::find(found.begin(), found.end(), searchingName) == found.end())
            {
                for(int j=0;j<goodFiles.size();j++)
                {
                    string comparingName=goodFiles.at(j)->getPureName();
                    string fileName=goodFiles.at(j)->getFileName();
                    if(comparingName==searchingName&&(fileName.find("Part_") != std::string::npos||fileName.find("Parity") != std::string::npos))
                    {
                        currentParts.push_back(goodFiles.at(j));
                    }
                    else if(comparingName==searchingName&&fileName.find("ImgPt_") != std::string::npos)
                    {
                        currentImgs.push_back(goodFiles.at(j));
                    }
                }
                vector<int>numbs={currentParts.at(0)->getpartNumb(),currentParts.at(1)->getpartNumb(),currentParts.at(2)->getpartNumb()};
                int missing=0;
                for(int i=1;i<=4;i++)
                {
                    if(std::find(numbs.begin(), numbs.end(),i) == numbs.end())
                    {
                        missing=i;
                        break;
                    }
                }
                if(missing!=0)
                {
                    FilePart* f= new FilePart(currentParts.at(0)->getPureName()+"_Part_"+to_string(missing)+".txt",reference->getAdress()+"/"+currentParts.at(0)->getPureName()+"_Part_"+to_string(missing)+".txt",currentParts.at(0)->getPureName(),missing);
                    reference->getfileColumn()->emplace(pair<string,FilePart*>(f->getFileName(),f));
                }
                missing=0;
                numbs={currentImgs.at(0)->getpartNumb()%10,currentImgs.at(1)->getpartNumb()%10};
                for(int i=1;i<=3;i++)
                {
                    if(std::find(numbs.begin(), numbs.end(),i) == numbs.end())
                    {
                        missing=i;
                        break;
                    }
                }
                if(missing!=0)
                {
                    vector<string> input;
                    string s=currentImgs.at(0)->getFileName();
                    boost::split(input,s, boost::is_any_of("."));
                    FilePart* f= new FilePart(currentImgs.at(0)->getPureName()+"_ImgPt_"+to_string(missing)+"."+input.at(1),reference->getAdress()+"/"+currentImgs.at(0)->getPureName()+"_ImgPt_"+to_string(missing)+"."+input.at(1),currentParts.at(0)->getPureName(),50+missing);
                    reference->getfileColumn()->emplace(pair<string,FilePart*>(f->getFileName(),f));
                }
                currentParts={};
                currentImgs={};
                found.push_back(searchingName);
            }
        }

    }
    repair(reference,antreference);
    return true;
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
                boost::split(input,dp_name, boost::is_any_of("_"));
                size_t n = std::count(dp_name.begin(), dp_name.end(), '_');
                FilePart *f;
                if(n==2)
                {
                    if (input.at(1) == "ImgPt")
                    {
                        f = new FilePart(dp_name, dir + "/" + dp_name, input.at(0), 5*10+stoi(input.at(2)));
                        out->insert(pair<string, FilePart *>(dp_name, f));
                    }
                    else
                    {
                        f = new FilePart(dp_name, dir + "/" + dp_name, input.at(0), stoi(input.at(2)));
                        out->insert(pair<string, FilePart *>(dp_name, f));
                    }
                }
                else
                {
                    if(input.at(1)=="Info.txt")
                    {
                        f = new FilePart(dp_name, dir + "/" + dp_name, input.at(0), 0);
                        out->insert(pair<string, FilePart *>(dp_name, f));
                    }
                    else if(input.at(1)=="Parity.txt")
                    {
                        f = new FilePart(dp_name, dir + "/" + dp_name, input.at(0), 4);
                        out->insert(pair<string, FilePart *>(dp_name, f));
                    }
                }
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
                boost::split(input,dp_name, boost::is_any_of("_"));
                FilePart *f= new FilePart(dp_name,dir+"/"+dp_name,input.at(0),0);
                out->insert(pair<string,FilePart*>(dp_name,f));
            }
        }
    }
    return out;
}
void RAID_Controller::diskVerifierT()
{
        thread diskT= thread(&RAID_Controller::diskVerifier,this);
        diskT.detach();
}

void RAID_Controller::XORrecovery(FilePart *part,FilePart* info)
{
    vector<FilePart*> parts;
    for(int i=0;i<this->disks->size();i++)
    {
        for(auto const& imap:*this->disks->at("Disk_"+to_string(i+1))->getfileColumn())
        {
            if(imap.second->getPureName()==part->getPureName()&&(imap.second->getFileName().find("Part_") != string::npos||imap.second->getFileName().find("Parity") != string::npos)&&imap.second->getpartNumb()!=part->getpartNumb())
            {
                parts.push_back(this->disks->at("Disk_"+to_string(i+1))->getfileColumn()->at(imap.first));
            }
        }

    }
    std::ifstream file(info->getFilePath());
    std::string str;
    getline(file,str);
    vector<string> inp;
    boost::split(inp,str,boost::is_any_of(":"));

    int totalS=stoi(inp.at(1));
    int p_size=totalS/3;
    int extras=totalS%3;

    int size1=p_size;
    int size2=p_size;
    int size3=p_size;

    int arr[3]={size1,size2,size3};
    while(extras>0)
    {
        for(int i=0;i<2;i++)
        {
            if(extras==0)
            {
                break;
            }
            arr[i]++;
            extras--;
        }
    }
    int realS=arr[part->getpartNumb()-1];
    vector<string>codes;
    for(int i=0;i<parts.size();i++)
    {
        std::ifstream file(parts.at(i)->getFilePath());
        std::string str;
        getline(file,str);
        getline(file,str);
        codes.push_back(str);
    }
    vector<RAID_Controller::Frag> frags=parityCalculator(codes);
    ofstream outT(part->getFilePath());
    outT<<"Size:"<<realS<<endl;
    outT<<frags.at(3).getFragment()<<endl;

}

void RAID_Controller::repair(Disk* reference,Disk* antreference)
{
    vector<FilePart*> parts;
    vector<FilePart*> parity;
    vector<FilePart*> imgs;
    FilePart* info;
    for(auto const& imap:*reference->getfileColumn())
    {
        string name=imap.first;
        if (name.find("Part_") != string::npos)
        {
            parts.push_back(reference->getfileColumn()->at(imap.first));
        }
        else if(name.find("Parity") != string::npos)
        {
            parity.push_back(reference->getfileColumn()->at(imap.first));
        }
        else if(name.find("ImgPt_") != string::npos)
        {
            imgs.push_back(reference->getfileColumn()->at(imap.first));
        }

    }
    map<string,FilePart*> infoFile;
    for(auto const& imap:*antreference->getfileColumn())
    {
        string name=imap.first;
        info=antreference->getfileColumn()->at(name);
        if(name.find("Info") != string::npos)
        {
            FilePart* inf= new FilePart(info->getFileName(),reference->getAdress()+"/"+info->getFileName(),info->getPureName(),info->getpartNumb());
            string n=antreference->getfileColumn()->at(name)->getPureName();
            infoFile.emplace(pair<string,FilePart*>(n,info));
        }
    }
    for(auto const& imap:infoFile)
    {
        string name=imap.first;
        for(int i=0;i<parts.size();i++)
        {
            if(parts.at(i)->getPureName()==name)
            {
                XORrecovery(parts.at(i),infoFile.at(name));
            }
        }
    }
}

Compressor::Codified_File* RAID_Controller::merge(string name)
{
    diskVerifier();
    this->comp=Compressor();
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
    vector<string*>str;
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

            string* s= new string(intr.substr(0,stoi(input.at(1))));
            str.push_back(s);
        }
    }
    codigote=*str.at(0)+*str.at(1)+*str.at(2);
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
    Compressor::Codified_File* c= this->comp.treeReconstructor(dirTree,codigote);
    return c;
}