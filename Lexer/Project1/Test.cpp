#include <iostream>
#include <fstream>
using namespace std;
// 42949 - 4294967297
struct File
{
    File(const char* fileName)
    {
        this->fileName = fileName;
    }

    const char* fileName;
    ofstream inputFile;

    template<typename T>
    void _(T item)
    {
        inputFile.open(fileName, ios_base::app);

        inputFile << item;

        inputFile.close();
    }
};



int main() 
{   
    char symbol = '_';
}