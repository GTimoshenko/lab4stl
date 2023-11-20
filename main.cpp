#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

class Catalogue {
public:
    string fileName; // название файла
    string creationDate; // дата создания файла
    string volume; // объем файла
    string type; // тип файла
    string modificationDate; // дата последнего изменения файла

    Catalogue() {}
};


class Container {
public:
    string containerName = ""; // имя контейнера для factory метода
    virtual void getContainer() {} // ф-я для получения данных из контейнера
    virtual void readContainer() {} // ф-я для внесения данных в контейнер
    virtual vector<Catalogue> getVector() {}
    virtual void setVector(vector<Catalogue> a) {}
    virtual multimap<string, Catalogue*> getMMap() {}
};


class Vector : public Container{
public:
    Vector() {
        containerName = "Vector";
    }
    vector <Catalogue> list;

    void readContainer() override {
        ifstream iFile;

        iFile.open("/Users/glebushnik/Desktop/cpplab/lab4/fileCatalogue.txt");
        if(iFile.is_open()) {
            while(iFile) {
                Catalogue c = *new Catalogue();
                iFile >> c.fileName
                      >> c.creationDate
                      >> c.volume
                      >> c.type
                      >> c.modificationDate;
                if(!(c.fileName == "" ||
                     c.creationDate == "" ||
                     c.modificationDate == "" ||
                     c.volume == "" ||
                     c.type == ""))
                list.push_back(c);
            }
        } else {
            throw 1;
        }
    }

    void getContainer() override {
        auto it = this->list.begin();
        while(it < this->list.end()) {
            cout << "Filename : " << it->fileName << endl;
            cout << "Created at : " << it->creationDate << endl;
            cout << "Volume : " << it->volume << endl;
            cout << "Type : " << it->type << endl;
            cout << "Modified at : " << it->modificationDate << endl;
            cout << endl;
            it++;
        }
    }

    void setVector(vector<Catalogue> a) override {
        this->list = a;
    }
    vector <Catalogue> getVector() override {
        return this->list;
    }
};


class MultiMap : public Container{
public:
    multimap<string, Catalogue*> mmap;
    MultiMap() {
        containerName = "MultiMap";
    }
    void readContainer() override {
        ifstream iFile;
        iFile.open("/Users/glebushnik/Desktop/cpplab/lab4/fileCatalogue.txt");
        if(iFile.is_open()) {
            while(iFile) {
                Catalogue* c = new Catalogue();
                iFile >> c->fileName
                      >> c->creationDate
                      >> c->volume
                      >> c->type
                      >> c->modificationDate;
                if(!(c->fileName == "" ||
                   c->creationDate == "" ||
                   c->modificationDate == "" ||
                   c->volume == "" ||
                   c->type == ""))
                mmap.insert(pair<string, Catalogue*>(c->modificationDate, c));
            }
        } else {
            throw 1;
        }
    }

    void getContainer() override {
        this->getMMap();
        multimap<string, Catalogue*>::reverse_iterator it;
        for(it = this->mmap.rbegin(); it!=this->mmap.rend(); it++){
            cout << "Filename : " << it->second->fileName<< endl;
        }
    }

    multimap <string, Catalogue*> getMMap() {
        return this->mmap;
    }
};


class FactoryContainer {
public:
    static Container* createContainer(string id){
        if(id == "vector" || id == "Vector" || id == "VECTOR") return new Vector();
        else if(id == "multimap" || id == "MultiMap" || id == "MULTIMAP") return new MultiMap();
        else return nullptr;
    }
};

int main() {
    Container* c1 = nullptr;
    string name;
    cin >> name;
    c1 = FactoryContainer::createContainer(name);
    c1->readContainer();
    //vector<Catalogue> a = c1->getVector();
    /*sort(a.begin(), a.end()-1, [](const Catalogue& c1, const Catalogue& c2) {
        string c1d = c1.modificationDate.substr(0, 2);
        string c2d = c2.modificationDate.substr(0, 2);
        //01.01.2001
        string c1m = c1.modificationDate.substr(3, 2);
        string c2m = c2.modificationDate.substr(3, 2);

        string c1y = c1.modificationDate.substr(6, 4);
        string c2y = c2.modificationDate.substr(6, 4);
        if(c1y != c2y) return c1y > c2y;
        if(c1m != c2m) return c1m > c2m;
        if(c1d != c2d) return c1d > c2d;
    });*/

    //c1->setVector(a);
    c1->getContainer();
    return 0;
}