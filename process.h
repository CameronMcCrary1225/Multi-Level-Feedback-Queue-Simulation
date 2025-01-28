#ifndef PROCESS_H
#define PROCESS_H
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
enum class Priority {
    RealTime,
    High,
    AboveNormal,
    Normal,
    BelowNormal,
    Idle
};

struct Process {
    std::string id;
    int startTime;
    int burstTime;
    int timeRemaining; // Time left for the process to run
    Priority priority;
    bool isDone;  // Indicates if the process is done
    int timeScheduled;
    int timeFinished;
    int serviceTime; // Time the process has actually been executed
    int waitingTime; // Time the process has spent waiting
    int turnaroundTime;


    Process(const std::string& _id, int _startTime, int _burstTime, Priority _priority)
        : id(_id), startTime(_startTime), burstTime(_burstTime), timeRemaining(_burstTime), 
          priority(_priority), isDone(false), timeScheduled(-1), timeFinished(-1), serviceTime(0), waitingTime(0) {}

};

std::vector<Process> parseProcessFile(const std::string& filename);
void printProcess(const Process& p);
void calculateAndPrintMetrics(std::vector<Process>& processes);
#endif
