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

    Catalogue getFile() {
        cout << "Filename : " << this->fileName << endl;
        cout << "Created at : " << this->creationDate << endl;
        cout << "Volume : " << this->volume << endl;
        cout << "Type : " << this->type << endl;
        cout << "Modified at : " << this->modificationDate << endl;
        cout << endl;
        return *this;
    }
};


class Container {
public:
    string containerName = ""; // имя контейнера для factory метода
    virtual vector<Catalogue> getContainer() {} // ф-я для получения данных из контейнера
    virtual void setContainer() {} // ф-я для внесения данных в контейнере
    virtual void sortContainer() {} // ф-я для соритровки данных в контейнере
    virtual void findByType(string type, string number) {} // ф-я для поиска 'n' фаайлов по заданному типу
};


class Vector : public Container{
public:
    Vector() {
        containerName = "Vector";
    }
    vector <Catalogue> list;

    void setContainer() override {
        ifstream iFile;

        iFile.open("/Users/glebushnik/Desktop/labs/cpplab/stllab/fileCatalogue.txt");
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

    vector<Catalogue> getContainer() override {
        auto it = this->list.begin();
        while(it < this->list.end()) {
            it->getFile();
            it++;
        }

        return this->list;
    }

    void sortContainer() override {
        sort(this->list.begin(), this->list.end(), [](const Catalogue& c1, const Catalogue& c2) {
            string c1d = c1.modificationDate.substr(0, 2);
            string c2d = c2.modificationDate.substr(0, 2);

            string c1m = c1.modificationDate.substr(3, 2);
            string c2m = c2.modificationDate.substr(3, 2);

            string c1y = c1.modificationDate.substr(6, 4);
            string c2y = c2.modificationDate.substr(6, 4);

            if (c1y != c2y) return c1y > c2y;
            if (c1m != c2m) return c1m > c2m;
            if (c1d != c2d) return c1d > c2d;
        });
    }

    void findByType(string type, string number) override{
        int i = 0;
        bool exists = 0;

        if(!isdigit(number[0])) throw 4;

        int num = stoi(number);
        for(auto it: this->list) {
            if(i < num) {
                if (it.type == type) {
                    exists = 1;
                    cout << i+1 << ")" << endl;
                    it.getFile();
                    i++;
                }
            }
        }

        if(exists == 0) {
            throw 3;
        }

        cout << "Files found by parameter: " << type << endl;
    }
};


class MultiMap : public Container{
public:
    multimap<string, Catalogue*> mmap;
    MultiMap() {
        containerName = "MultiMap";
    }
    void setContainer() override {
        ifstream iFile;
        iFile.open("/Users/glebushnik/Desktop/labs/cpplab/stllab/fileCatalogue.txt");
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

    vector<Catalogue> getContainer() override {
        vector<Catalogue> list;
        multimap<string, Catalogue*>::reverse_iterator it;
        for(it = this->mmap.rbegin(); it!=this->mmap.rend(); it++){
            it->second->getFile();
        }
        return list;
    }

    vector <Catalogue> mapMultiMap() {
        vector<Catalogue> list;
        multimap<string, Catalogue*>::reverse_iterator it;
        for(it = this->mmap.rbegin(); it!=this->mmap.rend(); it++){
            list.push_back(*it->second);
        }
        return list;
    }

    void findByType(string type, string number) override {
        vector<Catalogue> a = this->mapMultiMap();

        int i = 0;
        bool exists = 0;

        if(!isdigit(number[0])) throw 4;

        int num = stoi(number);
        for(auto it: a) {
            if(i < num) {
                if (it.type == type) {
                    exists = 1;
                    cout << i+1 << ")" << endl;
                    it.getFile();
                    i++;
                }
            }
        }

        if(exists == 0) {
            throw 3;
        }

        cout << "Files found by parameter: " << type << endl;
    }
};


class FactoryContainer {
public:
    static Container* createContainer(string id){
        if(id == "vector" || id == "Vector" || id == "VECTOR") return new Vector();
        else if(id == "multimap" || id == "MultiMap" || id == "MULTIMAP") return new MultiMap();
        else {
            throw 2;
        }
    }
};

int main() {
    try {
        Container *c1 = nullptr;
        string name, fileType, num;
        cin >> name;
        c1 = FactoryContainer::createContainer(name);
        c1->setContainer();
        if(c1->containerName == "Vector") c1->sortContainer();
        cout << "Введите тип файла, по которому необохдимо произвести поиск: ";
        cin >> fileType;
        cout << "Введите максимальное количество файлов на вывод: ";
        cin >> num;
        c1->findByType(fileType, num);
        //c1->getContainer();
        return 0;
    } catch (int e) {
        if (e == 1) {
            cout << "Error occured while opening file";
            return 1;
        }

        if(e == 2) {
            cout << "Incorrect name of container";
            return 2;
        }

        if(e == 3) {
            cout << "No files found with this parameter.";
            return 3;
        }

        if(e == 4) {
            cout << "Field 'number' should contain number.";
            return 4;
        }
    }
}