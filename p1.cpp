#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
using namespace std;
int codeSize = 5;
int nameSize = 11;
int lnameSize = 20;
int carrSize = 15;
struct Alumno
{
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];

    void print(){
        cout<<"Codigo: "; 
        for(int i = 0; i < 5; ++i){
            cout << codigo[i];
        }
        cout << endl;
        cout << "Nombre: ";
        for(int i = 0; i < 11; ++i){
            cout << nombre[i];
        }
        cout << endl;
        cout << "Apellido: ";
        for(int i = 0; i < 20; ++i){
            cout << apellidos[i];
        }     
        cout << endl;
        cout << "Carrera: ";
        for(int i = 0; i < 15; ++i){
            cout << carrera[i];
        }
        cout << endl;
        cout<<"------------------------"<<endl;
    }
    Alumno(){}
    
    Alumno(string codigo,string nombre,string apellidos, string carrera){
        for (int i = 0; i < 5; ++i){
            if(i >= codigo.size()) {
                this->codigo[i] = ' ';
            } else {
                this->codigo[i] = codigo[i];
            }
        }

        for(int i = 0; i < 11; ++i){
            if(i >= nombre.size()) {
                this->nombre[i] = ' ';
            } else {
                this->nombre[i] = nombre[i];
            }
        }

        for(int i = 0; i < 20; ++i) {
            if(i >= apellidos.size()) {
                this->apellidos[i] = ' ';
            } else {
                this->apellidos[i] = apellidos[i];
            }
        }

        for(int i = 0; i < 15; ++i) {
            if(i >= carrera.size()) {
                this->carrera[i] = ' ';
            } else {
                this->carrera[i] = carrera[i];
            }
        }
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
        char buffer[sizeof(record)];
        inFile.open(this->fileName);
        if (inFile.is_open()){
            while(!inFile.eof()){
                inFile.read((char*) &buffer,sizeof(record));
                inFile.get();
                inFile.get();
                record = desempaquetar(buffer);
                aula.push_back(record);
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

    Alumno desempaquetar(char * buffer){
        string codigo="";
        string nombre = "";
        string apellidos = "";
        string carrera = "";
        
        for (auto i=0;i<codeSize;i++){
            codigo+= buffer[i];
        }

        for (auto i=codeSize;i<codeSize+nameSize;i++){
            nombre+= buffer[i];
        }
        for (auto i=codeSize+nameSize;i<codeSize+nameSize+lnameSize;i++){
            apellidos+= buffer[i];
        }
        for (auto i=codeSize+nameSize+lnameSize;i<codeSize+nameSize+lnameSize+carrSize;i++){
            carrera+= buffer[i];
        } 
        Alumno record{codigo,nombre,apellidos,carrera};
        return record;
    }
    Alumno readRecord(int pos){
        //para obtener el registro de la posici??n ???pos???.
        ifstream inFile;
        Alumno record;
        inFile.open(this->fileName);
        if (inFile.is_open()){
            inFile.clear();
            inFile.seekg(pos* sizeof(record)+4);
            char buffer[sizeof(record)];
            inFile.read((char*) &buffer,sizeof(record));
            record = desempaquetar(buffer);
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
    //testing readRecord
    auto alumno = manager.readRecord(2);
    alumno.print();

    //testing add()
    Alumno nuevoAlumno{"0008","Mauricio","Rodriguez","Computacion"};
    manager.add(nuevoAlumno);

    //testing load
    vector<Alumno> vec = manager.load();
    for (auto i = 0;i<vec.size();i++){
      vec[i].print();
    }
    return 0;
}
