//
//  main.cpp
//  lab4
//
//  Created by Leeran Farin on 3/23/22.
//
#include "process.h"

int main(int argc, const char * argv[]) {
    int processCount = -1;
    string errorMsg = "";
    while(processCount <= 0){
        cout << errorMsg << endl;
        cout << "How many processes would you like to create? ";
        cin >> processCount;
        errorMsg = "Process count must be greater than 0...";
    }
    Manager* manager = new Manager(processCount);
    manager->FCFS();
    manager->Priority();
    manager->SJN();
    return 0;
}
