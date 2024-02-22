#include <iostream>
#include "CycleBreaking.h"
using namespace std;

int main(int argc, char* argv[]){
    if(argc != 3){
        cout << "Usage: ./cb <input_filename> <output_filename>\n";
        return 1;
    }
    CycleBreaking cb;
    cb.readFile(argv[1]);
    cb.working();
    cb.writeFile(argv[2]);
    return 0;
}