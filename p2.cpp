#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
using namespace std;
int codeSize = 5;
int nameSize = 11;
int lnameSize = 20;
int carrSize = 15;
struct Alumno{
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];
    int ciclo;
    float mensualidad;
    int next = 0;

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
        cout<<"Ciclo: "<<ciclo<<endl;
        cout<<"Mensualidad: "<<mensualidad<<endl;
        cout<<"------------------------"<<endl;
    }
    Alumno(){}
    
    Alumno(string codigo,string nombre,string apellidos, string carrera,int ciclo,float mensualidad){
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
        this->ciclo = ciclo;
        this->mensualidad = mensualidad;
    }

    void formatBIN(char buffer[], int size){
        int n = string(buffer).length();
        for (int i =0;i<size;i++){
            if (i<n){ }
            else{
                buffer[i] = ' ';
            }
        }
    }


    void format(){
        formatBIN(this->codigo,5);
        formatBIN(this->nombre,11);
        formatBIN(this->apellidos,20);
        formatBIN(this->carrera,15);
    }
};

class FixedRecord{
    private:
        string fileName;
    public:
    FixedRecord(string fileName){
        this->fileName = fileName;
        fstream mainFile;
        mainFile.open(fileName);
        if (mainFile.is_open()){
            char buffer[sizeof(int)];
            mainFile.read((char*) &buffer,sizeof(int));
            if (string(buffer)!= "-1  "){
                char code[5];
                mainFile.seekg(0, std::ios::beg);
                mainFile<<"-1  \n";
            }
            mainFile.close();
        }
        else{
            cout<<"No se pudo abrir el archivo";
        }
    }   
    ~FixedRecord(){

    }

    bool deleteRecord(int pos){
        fstream file;
        file.open(this->fileName);
        if (file.is_open()){
            char last[sizeof(int)];
            char cpos[sizeof(int)] = {' '};
            file.seekg(0,ios::beg);
            file.read((char*) &last,sizeof(int));
            file.seekg(pos*sizeof(Alumno)+5);
            file.write(last,4);
            file<<"\n";
            //
            file.seekg(0,ios::beg);
            for (int i =0;i<sizeof(int);i++){
                if (i<to_string(pos).length()){
                    cpos[i]=to_string(pos)[i];
                }
                else{
                    cpos[i] = ' ';
                }
            }
            file<<cpos;
        }
        else{
            cout<<"No se pudo abrir el archivo";
        }
    }
    
    // vector<Alumno> load(){
    //     //para leer todos los registros del archivo
    //     ifstream inFile;
    //     vector<Alumno> aula;
    //     Alumno record;
    //     char buffer[sizeof(record)];
    //     inFile.open(this->fileName,ios::binary);
    //     if (inFile.is_open()){
    //         while(!inFile.eof()){
    //             inFile.read((char*) &buffer,sizeof(record));
    //             inFile.get();
    //             inFile.get();
    //             record = desempaquetar(buffer);
    //             aula.push_back(record);
    //         }
    //         inFile.close();
    //     }
    //     return aula;
    // }




    void add(Alumno record){
       //para agregar un nuevo registro al archivo
        fstream outFile;
        char buffer[sizeof(int)];
        outFile.open(this->fileName);
        if (outFile.is_open()){
            record.format();
            outFile.seekg(0, ios::beg);
            char buffer[sizeof(int)];
            outFile.read((char*) &buffer,sizeof(int));
            if (stoi(string(buffer))== -1){
                outFile.seekg(0,ios::end);
                outFile.write((char*)&record,(sizeof(record)));
                outFile<<"0   \n";
                outFile<< "\n";
                outFile<< flush;
            }
            else{
                outFile.seekg(5+stoi(string(buffer))*sizeof(Alumno), ios::beg);
                char last[sizeof(int)];
                outFile.read((char*) &last,4);
                outFile.seekg(0,ios::beg);
                outFile<<last;
                outFile.seekg(5+(stoi(string(buffer))-1)*sizeof(Alumno), ios::beg);
                outFile.write((char*)&record,(sizeof(record)));
            }
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
        string ciclo = "";
        string mensualidad= "";
        
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
        for (auto i=codeSize+nameSize+lnameSize+carrSize;i<codeSize+nameSize+lnameSize+carrSize+sizeof(int);i++){
            ciclo+= buffer[i];
        } 
        for (auto i=codeSize+nameSize+lnameSize+carrSize+sizeof(int);i<codeSize+nameSize+lnameSize+carrSize+sizeof(int)+sizeof(float);i++){
            ciclo+= buffer[i];
        }
        Alumno record{codigo,nombre,apellidos,carrera,stoi(ciclo),stof(mensualidad)};
        return record;
    }
    Alumno readRecord(int pos){
        //para obtener el registro de la posición “pos”.
        ifstream inFile;
        Alumno record;
        inFile.open(this->fileName);
        if (inFile.is_open()){
            char buffer[sizeof(Alumno)];
            buffer[sizeof(Alumno)-sizeof(int)] = '0';
            while(1){
                inFile.seekg(pos*sizeof(Alumno)+5,ios::beg);
                inFile.read((char*) &buffer,sizeof(Alumno));
                cout<<buffer;
                break;
                if(buffer[sizeof(Alumno)-sizeof(int)] == '0'){
                    record = desempaquetar(buffer);
                    break;
                } 
                else pos++;
            }
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
    FixedRecord manager("datosp2.txt");

    //testing deletion
    manager.deleteRecord(1);

    //testing readRecord
    // auto alumno = manager.readRecord(0);
    // //alumno.print();

    //testing add()
    Alumno nuevoAlumno{"0008","Mauricio","Rodriguez","Computacion",9,1589.90};
    manager.add(nuevoAlumno);

    //testing load
    // vector<Alumno> vec = manager.load();
    // for (auto i = 0;i<vec.size();i++){
    //   vec[i].print();
    // }

    return 0;
}
