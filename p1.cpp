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

    void print(){
        cout<<"Codigo: "<<this->codigo<<" "; 
        cout<<"nombre: "<<this->nombre<<" ";
        cout<<"apellidos: "<<this->apellidos<<" ";
        cout<<"carrera: "<<this->carrera<<" ";
        cout<<endl;
    }

    void formatTXT(char buffer[], int size){
        int n = string(buffer).length();
        for (int i =0;i<size;i++){
            if (i<n){ }
            else{
                buffer[i] = ' ';
            }
        }
    }

    void format(){
        formatTXT(this->codigo,5);
        formatTXT(this->nombre,11);
        formatTXT(this->apellidos,20);
        formatTXT(this->carrera,15);
    }
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
                inFile.get(record.codigo,5);
                inFile.get(record.nombre,11);
                inFile.get(record.apellidos,20);
                inFile.get(record.carrera,15);
                aula.push_back(record);
                inFile.get();
            }
            inFile.close();
        }
        return aula;
    }

    void add(Alumno record){
       //para agregar un nuevo registro al archivo
        fstream outFile;
        outFile.open(this->fileName);
        if (outFile.is_open()){
            record.format();
            outFile.seekg(0, ios::end);
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
            inFile.clear();
            inFile.seekg(pos* sizeof(record)+1);
            inFile.get(record.codigo,5);
            inFile.get(record.nombre,11);
            inFile.get(record.apellidos,20);
            inFile.get(record.carrera,15);
            inFile.get();
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
    // vector<Alumno> vec = manager.load();
    // Alumno nuevoAlumno{"0002","Mauricio","Rodriguez","Computacion"};
    // manager.add(nuevoAlumno);

    auto alumno = manager.readRecord(2);
    alumno.print();

    // for (auto i = 0;i<vec.size();i++){
    //   vec[i].print();
    // }
    return 0;
}
