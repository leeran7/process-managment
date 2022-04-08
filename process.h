//
//  process.h
//  lab4
//
//  Created by Leeran Farin on 3/23/22.
//

#ifndef process_h
#define process_h
#include <string>
#include <iomanip>
#include <iostream>
using namespace std;


class Process {
public:
    int priority;
    int start;
    int pid;
    int tat;
    int completion;
    int completedIndex;
    int arrivalTime;
    int executionTime;
    int initialExecutionTime;
    void init(int id, int arrival, int pr, int exec){
        this->priority = pr;
        this->pid = id;
        this->arrivalTime = arrival;
        this->executionTime = exec;
        this->completion = 0;
        this->tat = 0;
        this->completedIndex = 0;
        this->start = -1;
        this->initialExecutionTime = exec;
    }
    void reset(){
        this->tat = 0;
        this->completion = 0;
        this->completedIndex = 0;
        this->start = -1;
        this->executionTime = this->initialExecutionTime;
    }
    Process operator=(const Process& p){
        this->priority = p.priority;
        this->start = p.start;
        this->pid = p.pid;
        this->tat = p.tat;
        this->completion = p.completion;
        this->completedIndex = p.completedIndex;
        this->arrivalTime = p.arrivalTime;
        this->executionTime = p.executionTime;
        return *this;
    }
    Process operator<(const Process& one){
        return this->completedIndex < one.completedIndex ? *this : one;
    }
};

class Manager {
    Process* processes;
    int processCount;
public:
    Manager(int count){
        this->processCount = count;
        this->processes = new Process[count];
        initProcesses();
    }
    void printError(string message){
        if(message.length() > 0){
            cout << message << endl;
        }
    }
    void initProcesses(){
        for(int i = 0; i < processCount; i++){
            int arrivalTime = -1;
            int priority = -1;
            int executionTime = -1;
            string errorMsg = "";
            while(arrivalTime < 0){
                printError(errorMsg);
                printf("What is the arrivalTime for pid[ %d ]: ", i + 1);
                cin >> arrivalTime;
                errorMsg = "arrivalTime must be greater than 0...";
            }
            errorMsg = "";
            while(priority < 0){
                printError(errorMsg);
                printf("What is the priority for pid[ %d ]: ", i + 1);
                cin >> priority;
                errorMsg = "priority must be greater or equal to 0...";
            }
            errorMsg = "";
            while(executionTime < 0){
                printError(errorMsg);
                printf("What is the executionTime for pid[ %d ]: ", i + 1);
                cin >> executionTime;
                errorMsg = "executionTime must be greater than 0...";
            }
            this->processes[i].init(i + 1, arrivalTime, priority, executionTime);
        }
    }
    Manager operator=(const Manager& man){
        Manager* value = new Manager(man.processCount);
        value->processCount = this->processCount;
        for(int i = 0; i < processCount; i++){
            value->processes[i].init(processes[i].pid, processes[i].arrivalTime, processes[i].priority, processes[i].executionTime);
        }
        return *value;
    }
    friend ostream& operator<<(ostream& cout, const Manager& man){
        cout << setw(43) << "Process Data" << endl << endl;
        cout << setw(15) << "PID" << setw(15) << "START" << setw(15) << "COMPLETION" << setw(15) << "TAT" << endl;
        for(int i = 0; i < man.processCount; i++){
            cout << setw(15) << man.processes[i].pid;
            cout << setw(15) << man.processes[i].start;
            cout << setw(15) << man.processes[i].completion;
            cout << setw(15) << man.processes[i].tat;
            cout << endl;
        }
        return cout;
    }
    void resetMemory(){
        for(int i = 0; i < this->processCount; i++){
            this->processes[i].reset();
        }
    }

    void getAverageTAT(){
        double totalTAT = 0;
        for(int i = 0; i < this->processCount; i++){
            totalTAT += this->processes[i].tat;
        }
        cout << "Average TAT: " << totalTAT / (double)this->processCount << endl;
    }

    void getReadyProcesses(int currentTime, int& count, int& highest){
        count = 0;
        highest = 999;
        for(int i = 0; i < this->processCount; i++){
            if(this->processes[i].executionTime > 0 && this->processes[i].arrivalTime <= currentTime){
                count++;
                if(this->processes[i].priority < highest){
                    highest = this->processes[i].priority;
                }
            }
        }
    }

    bool isCompleted(){
        int completed = true;
        for(int i = 0; i < this->processCount; i++){
            if(this->processes[i].executionTime > 0){
                completed = false;
                break;
            }
        }
        return completed;
    }

    void calculateTAT(){
        for(int i = 0; i < this->processCount; i++){
            this->processes[i].tat = this->processes[i].completion - this->processes[i].arrivalTime;
        }
    }

    void getShortestReadyProcess(int time, int& idx, int& shortest){
        for(int i = 0; i < this->processCount; i++){
            if(this->processes[i].executionTime > 0 && this->processes[i].arrivalTime <= time && this->processes[i].completedIndex == 0){
                if(this->processes[i].executionTime < shortest){
                    shortest = this->processes[i].executionTime;
                    idx = i;
                }
            }
        }
    }

    void Priority(){
        cout << endl;
        cout << setw(20) << "Priority Scheduling" << endl;
        int time = 0;
        int highest = 0;
        int idx = 1;
        string order = "";
        bool running = !isCompleted();
        int currentProcess = 0;
        while(running){
            int count = 0;
            getReadyProcesses(time, count, highest);
            if(count > 0){
                bool ran = false;
                for(int i = 0; i < this->processCount; i++){
                    if(this->processes[i].priority == highest && !ran && this->processes[i].arrivalTime <= time && this->processes[i].completedIndex == 0){
                        if(this->processes[i].executionTime > 0){
                            if(i != currentProcess){
                                order = order + "P" + to_string(this->processes[i].pid) + "->";
                            }
                            if(this->processes[i].start == -1){
                                this->processes[i].start = time;
                            }
                            this->processes[i].executionTime--;
                            currentProcess = i;
                            if(this->processes[i].executionTime == 0){
                                this->processes[i].completion = time + 1;
                                this->processes[i].completedIndex = idx;
                                idx++;
                            }
                            ran = true;
                        }
                    }   
                }
            }
            time++;
            if(isCompleted()){
                break;
            }
        }
        calculateTAT();
        cout << *this;
        getAverageTAT();
        cout << "Order: " << order << endl;
        resetMemory();
    }

    void SJN(){
        cout << endl;
        cout << setw(20) << "Shortest Job Next" << endl;
        int time = 0;
        int idx = 0;
        bool completed = !isCompleted();
        int initialShortest = 999;
        string order = "";
        while(completed){
            int shortest = initialShortest;
            getShortestReadyProcess(time, idx, shortest);   
            if(shortest != initialShortest){
                if(this->processes[idx].executionTime > 0){
                    this->processes[idx].start = time;
                    time += this->processes[idx].executionTime;
                    this->processes[idx].completion = time;
                    this->processes[idx].completedIndex = idx + 1;
                    this->processes[idx].executionTime = 0;
                    order = order + "P" + to_string(this->processes[idx].completedIndex) + "->";
                }
            } else {
                time++;
            }
            if(isCompleted()){
                break;
            } 
        }
        
        calculateTAT();
        cout << *this;
        getAverageTAT();
        cout << "Order: " << order << endl;
        resetMemory();
    }

    void FCFS(){
        cout << endl;
        cout << setw(20) << "First Come First Serve" << endl;
        int time = 0;
        int i = 0;
        string order = "";
        while(i < this->processCount){
            if(this->processes[i].arrivalTime <= time){
                this->processes[i].start = time;
                time += this->processes[i].executionTime;
                this->processes[i].completion = time;
                this->processes[i].tat = this->processes[i].completion - this->processes[i].arrivalTime;
                this->processes[i].completedIndex = i + 1;
                this->processes[i].executionTime = 0;
                order = order + "P" + to_string(this->processes[i].completedIndex) + "->";
                i++;
            }
            else{
                time++;
            }
        }
        cout << *this;
        getAverageTAT();
        cout << "Order: " << order << endl;
        resetMemory();
    }
};

#endif /* process_h */
