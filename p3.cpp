#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
using namespace std;
int codeSize = 5;
int nameSize = 11;
int lnameSize = 20;
int carrSize = 15;

struct header{
    int posicion;
    int size;
    void print(){
        cout<<"Pos: "<<posicion<<endl;
        cout<<"Size: "<<size<<endl;
    }
};
struct Alumno
{
    string nombre;
    string apellidos;
    string carrera;
    float mensualidad;
    void print(){
        cout<<"Nombre: "<<this->nombre<<endl; 
        cout << "Apellidos: "<<this->apellidos<<endl;
        cout << "Carrera: "<<this->carrera<<endl;
        cout << "Mensualidad: "<<this->mensualidad<<endl;
        cout<<"------------------------"<<endl;
    }
    Alumno(){}
    
    Alumno(string nombre,string apellidos, string carrera,float mensualidad){
        this->nombre = nombre;
        this->apellidos = apellidos;
        this->carrera = carrera;
        this->mensualidad = mensualidad;   
    }
};

class FixedRecord{
    private:
        string fileName;
        string headerName;

    bool is_empty(std::fstream& pFile){
        return pFile.peek() == std::fstream::traits_type::eof();
    }

    header desempaquetarHead(char buffer[]){
        string size="";
        string posicion="";
        for (int i =0;i<sizeof(int);i++){
            posicion+=buffer[i];
        }
        for (int i =sizeof(int);i<2*sizeof(int);i++){
            size+=buffer[i];
        }
        header head{stoi(posicion),stoi(size)};
        return head;
    }

    void int_to_char(char buffer[],int num){
        for (int i =0;i<sizeof(int);i++){
            if (i<to_string(num).length()){
                buffer[i] = to_string(num)[i];
            }
            else buffer[i] = ' ';
        }
    }

    void encabezado(char buffer[],int size,string fileName){

        string temp = string(buffer);
        fstream file;
        int acumulado = 0;
        file.open(fileName);//

        int end = temp.find('\n');
        acumulado+=end+1;
        temp.erase(temp.begin(),temp.begin()+end+1);
        char buffer2[sizeof(int)] = {' '};
        int_to_char(buffer2,end+1);
        file<<buffer2;
        while(temp.length()>1){
            end = temp.find('\n');
            acumulado+=end+1;
            temp.erase(temp.begin(),temp.begin()+end+1);
            int_to_char(buffer2,end);
            file<<buffer2;
            file<<"\n";
            if(temp.length()>0){
                int_to_char(buffer2,acumulado);
                file<<buffer2;
            }
        }
        file.close();
    }

    public:
    FixedRecord(string fileName,string headerName){
        this->fileName = fileName;
        this->headerName = headerName;
        fstream mainFile;
        fstream headerFile;
        mainFile.open(fileName);
        vector<int> vec;
        if (mainFile.is_open()){
            mainFile.seekg(0,ios::end);
            int size = mainFile.tellg();
            mainFile.seekg(0,ios::beg);
            char buffer[size];
            mainFile.read((char*) &buffer,size);
            mainFile.close();
            encabezado(buffer,size,headerName);
        }
    }
    ~FixedRecord(){}

    Alumno desempaquetar(char buffer[]){
        string array[4];
        string temp = "";
        int k =0;
        int i = 0;
        for (int i =0;i<string(buffer).length();i++){
            if (buffer[i]!='|' && i!=string(buffer).length()-1){
                temp+=buffer[i];
            }
            else{
                array[k++] = temp;
                temp.clear();
            }
        }
        Alumno record{array[0],array[1],array[2],stof(array[3])};
        return record;
    }

    void completeChar(char cinit[],char csize[],int init,int end){
        string sinit = to_string(init);
        string size = to_string(end-init);
        for (int i = 0;i<sizeof(int);i++){
                    if (i<sinit.length()) cinit[i] = sinit[i];
                    else cinit[i] = ' ';
        }
        for (int i = 0;i<sizeof(int);i++){
            if (i<size.length()) csize[i] = size[i];
            else csize[i] = ' ';
        }
    }

    void format(Alumno &record){
        record.nombre+="|";
        record.apellidos+="|";
        record.carrera+="|";
    }

    vector<Alumno> load(){
        //para leer todos los registros del archivo
        ifstream mainFile;
        vector<Alumno> aula;
        mainFile.open(this->fileName);
        mainFile.seekg(0,ios::end);
        int end = mainFile.tellg();
        char buffer[end];
        string helper = "";
        int k;
        string array[4];

        if (mainFile.is_open()){
            mainFile.seekg(0,ios::beg);
            mainFile.read((char*) &buffer,end);
            mainFile.close();
            string newBuffer = string(buffer);
            newBuffer.erase(newBuffer.begin(),newBuffer.begin()+newBuffer.find("\n")+1);
            for (int i =0;i<newBuffer.length();i++){
                if (newBuffer[i] != '|' &&  newBuffer[i] != '\n'){
                    helper+= newBuffer[i];
                }
                else if (newBuffer[i]=='|'){
                    array[k++] = helper;
                    helper.clear();
                }
                else if (newBuffer[i] == '\n'|| i == newBuffer.length()-1){
                    array[3] = helper;
                    helper.clear();
                    Alumno record{array[0],array[1],array[2],stof(array[3])};
                    aula.push_back(record);
                    k = 0;
                }
            }

        }
        return aula;
    }

    void add(Alumno record){
       //para agregar un nuevo registro al archivo
        fstream outFile;
        fstream headerFile;
        header head;
        int init;
        int end;
        outFile.open(this->fileName);
        if (outFile.is_open()){
            format(record);
            outFile.seekg(0, ios::end);
            init = outFile.tellg();
            outFile << record.nombre;
            outFile << record.apellidos;
            outFile << record.carrera;
            outFile<< record.mensualidad;
            outFile<< "\n";
            end = outFile.tellg();
            outFile<< flush;
            outFile.close();
            
            headerFile.open(this->headerName);
            if (headerFile.is_open()){
                headerFile.seekg(0, ios::end);
                char cinit[4];
                char csize[4];
                completeChar(cinit,csize,init,end);
                headerFile.write(cinit,sizeof(int));
                headerFile.write(csize,sizeof(int));
                headerFile<< "\n";
                headerFile.flush();
                headerFile.close();
            }
            else{
                cout<<"No se pudo abrir el archivo header";
            }
        }
        else{
            cout<<"No se pudo abrir el archivo principal";
        }
    }

    Alumno readRecord(int pos){
        //para obtener el registro de la posición “pos”.
        ifstream inFile;
        ifstream headerFile;
        header head;
        Alumno record;
        headerFile.open(headerName);
        if (headerFile.is_open()){
            headerFile.seekg(pos* sizeof(header)+pos);
            char buffer[sizeof(header)];
            headerFile.read((char*) &buffer,sizeof(header));
            //cout<<buffer;
            head = desempaquetarHead(buffer);
            headerFile.close(); 
            
            inFile.open(fileName);
            if (inFile.is_open()){
                char buffer2[head.size];
                inFile.seekg(head.posicion);
                inFile.read((char*) &buffer2,head.size);
                //cout<<buffer2;
                record = desempaquetar(buffer2);
                inFile.close();
            }
            else{
                cout<<"No se pudo abrir el archivo de datos";
            }
        }
        else{
            cout<<"No se pudo abrir el archivo header";
        }
        return record;
    }


};


int main(int argc, char const *argv[])
{
    FixedRecord manager("datos2.txt","cabecera.txt");
    //testing add()
    Alumno test1{"Mauricio","Rodriguez","Computacion",1516.89};
    Alumno test2{"Penny","Vargas Cordero","Industrial",1760.00};
    Alumno test3{"Sheldon","Cooper Quizpe","Mecatronica",1460.00};
    Alumno test4{"Penny","Vargas Cordero","Industrial",1560.00};
    Alumno test5{"Howard","Paredes Zegarra","Computacion",1560.10};
    manager.add(test1);
    manager.add(test2);
    manager.add(test3);
    manager.add(test4);
    manager.add(test5);


    // // testing readRecord
    auto alumno = manager.readRecord(5);
    alumno.print();


    // //testing load
    vector<Alumno> vec = manager.load();
    for (auto i = 0;i<vec.size();i++){
      vec[i].print();
    }
    // return 0;
}
