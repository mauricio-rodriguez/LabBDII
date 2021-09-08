#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Alumno
{
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];
};

class FixedRecord{
    private:
        string fileName;
    public:
    FixedRecord(string fileName){
        this->fileName = fileName;
    }
    ~FixedRecord(){

    }

    vector<Alumno> load(){
        //para leer todos los registros del archivo
        ifstream inFile;
        vector<Alumno> aula;
        Alumno record;
        inFile.open(this->fileName);
        if (inFile.is_open()){
            while(inFile){
            inFile.read(record.codigo,5);
            inFile.read(record.nombre,11);
            inFile.read(record.apellidos,20);
            inFile.read(record.carrera,15);
            aula.push_back(record);
            inFile.get();
        }
            inFile.close();
        }
        return aula;
    }

    void add(const Alumno &record){
       //para agregar un nuevo registro al archivo
        ofstream outFile;
        outFile.open(this->fileName);
        if (outFile.is_open()){
            outFile.write(record.codigo,5);
            outFile.write(record.nombre,11);
            outFile.write(record.apellidos,20);
            outFile.write(record.carrera,15);
            outFile<< "\n";
            outFile<< flush;
            outFile.close();
        }
        else{
            cout<<"No se pudo abrir el archivo";
        }
    }
    Alumno readRecord(int pos){
        //para obtener el registro de la posición “pos”.
        ifstream inFile;
        Alumno record;
        inFile.open(this->fileName);
        if (inFile.is_open()){
            inFile.seekg(pos*sizeof(record));
            // inFile.read(record.codigo,5);
            // inFile.read(record.nombre,11);
            // inFile.read(record.apellidos,20);
            // inFile.read(record.carrera,15);
            // inFile.get();
            inFile.read((char *) &record,sizeof(record));
            inFile.close();
        }
        else{
            cout<<"No se pudo abrir el archivo";
        }
        return record;
    }

};


int main(int argc, char const *argv[])
{
    FixedRecord manager("datos1.txt");
    Alumno nuevoAlumno{};
    auto alumno = manager.readRecord(1);
    cout<<alumno.nombre;

    // auto vec = manager.load();
    // for (auto i = 0;i<vec.size();i++){
    //     cout<<vec[i].nombre;
    // }
    // return 0;
}
