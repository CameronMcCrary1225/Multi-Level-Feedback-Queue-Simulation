#ifndef MLFQ_H
#define MLFQ_H
#include <algorithm>
#include "process.h"
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "scheduler.h"
#include <unordered_map>
using namespace std;

extern int CURRENTTIME;
extern int SERVICETIME;
extern int WAITINGTIME;
extern int TURNAROUNDTIME;

class Scheduler {
protected:
    std::vector<Process> queue;

public:
    virtual void schedule() = 0;

    void addProcess(const Process& proc) {
        queue.push_back(proc);
    }

    void removeProcess(Process& procToRemove) {
        for (auto it = queue.begin(); it != queue.end(); ++it) {
            if (it->id == procToRemove.id) {
                queue.erase(it);
                std::cout << "Process " << procToRemove.id << " removed from the queue." << std::endl;
                return;
            }
        }
        std::cout << "Error: Process " << procToRemove.id << " not found in the queue." << std::endl;
    }

    std::vector<Process>& getQueue() {
        return queue;
    }

    bool isEmpty() const {
        return queue.empty();
    }

    virtual void reorderQueue() {};

    void removeCompletedProcesses() {
        for (auto it = queue.begin(); it != queue.end();) {
            Process& proc = *it;
            if (proc.timeScheduled >= proc.burstTime) {
                proc.isDone = true;
                proc.timeFinished = CURRENTTIME;
                proc.serviceTime = proc.timeScheduled;
                proc.waitingTime = proc.timeFinished - proc.startTime - proc.serviceTime;
                std::cout << "Process " << proc.id << " completed at time " << CURRENTTIME << std::endl;
                std::cout << "Service Time: " << proc.serviceTime << std::endl;
                std::cout << "Waiting Time: " << proc.waitingTime << std::endl;
                std::cout << "Turnaround Time: " << (proc.timeFinished - proc.startTime) << std::endl;
                SERVICETIME += proc.serviceTime;
                WAITINGTIME += proc.waitingTime;
                TURNAROUNDTIME += (proc.timeFinished - proc.startTime);
                it = queue.erase(it);
            } else {
                ++it;
            }
        }
    }
};

class REALTIME : public Scheduler {
public:
    void schedule() override {
        int idx = roundrobin(CURRENTTIME, queue, 1);
        if (idx != -1) {
            Process& proc = queue[idx];
            proc.timeScheduled += 1;
            proc.serviceTime +=1;
            proc.timeFinished +=1;
            std::cout << "Scheduled REALTIME process: " << proc.id << " at time " << CURRENTTIME 
                      << " (Time allocated: " << proc.timeScheduled << ")" << std::endl;
        }
    }
};

class HIGH : public Scheduler {
public:
    void schedule() override {
        int idx = HighestResponseRatioNext(queue, CURRENTTIME);
        if (idx != -1) {
            Process& proc = queue[idx];
            proc.timeScheduled += 1;
            proc.serviceTime +=1;
            proc.timeFinished +=1;
            std::cout << "Scheduled HIGH priority process: " << proc.id << " at time " << CURRENTTIME 
                      << " (Time allocated: " << proc.timeScheduled << ")" << std::endl;
        }
    }
};

class ABOVENORMAL : public Scheduler {
public:
    void schedule() override {
        int idx = HighestResponseRatioNext(queue, CURRENTTIME);
        if (idx != -1) {
            Process& proc = queue[idx];
            proc.timeScheduled += 1;
            proc.serviceTime +=1;
            proc.timeFinished +=1;
            std::cout << "Scheduled ABOVENORMAL priority process: " << proc.id << " at time " << CURRENTTIME 
                      << " (Time allocated: " << proc.timeScheduled << ")" << std::endl;
        }
    }
};

class NORMAL : public Scheduler {
public:
    void schedule() override {
        int idx = HighestResponseRatioNext(queue, CURRENTTIME);
        if (idx != -1) {
            Process& proc = queue[idx];
            proc.timeScheduled += 1;
            proc.serviceTime +=1;
            proc.timeFinished +=1;
            std::cout << "Scheduled NORMAL priority process: " << proc.id << " at time " << CURRENTTIME 
                      << " (Time allocated: " << proc.timeScheduled << ")" << std::endl;
        }
    }
};

class BELOWNORMAL : public Scheduler {
public:
    void schedule() override {
        int idx = FIFO(queue, CURRENTTIME);
        if (idx != -1) {
            Process& proc = queue[idx];
            proc.timeScheduled += 1;
            proc.serviceTime +=1;
            proc.timeFinished +=1;
            std::cout << "Scheduled BELOWNORMAL process: " << proc.id << " at time " << CURRENTTIME 
                      << " (Time allocated: " << proc.timeScheduled << ")" << std::endl;
        }
    }
};

class IDLE : public Scheduler {
public:
    void schedule() override {
        int idx = FIFO(queue, CURRENTTIME);
        if (idx != -1) {
            Process& proc = queue[idx];
            proc.timeScheduled += 1;
            proc.serviceTime +=1;
            proc.timeFinished +=1;
            std::cout << "Scheduled IDLE process: " << proc.id << " at time " << CURRENTTIME 
                      << " (Time allocated: " << proc.timeScheduled << ")" << std::endl;
        } else {
            std::cout << "IDLE: No process to schedule at time " << CURRENTTIME << std::endl;
        }
    }
};

class MLFQScheduler {
public:
    REALTIME rt1;
    REALTIME rt2;
    HIGH h1;
    HIGH h2;
    ABOVENORMAL an1;
    ABOVENORMAL an2;
    NORMAL n1;
    NORMAL n2;
    BELOWNORMAL bn1;
    BELOWNORMAL bn2;
    IDLE i1;
    IDLE i2;

    std::vector<Scheduler*> queues = {
        &rt1, &rt2,
        &h1, &h2,
        &an1, &an2,
        &n1, &n2,
        &bn1, &bn2,
        &i1, &i2
    };
    std::unordered_map<std::string, Scheduler*> processQueueMap;

    void admitnewprocess(int& curTime, std::vector<Process>& procList);

    void runqueues(int& curTime);
    void updatestats(std::vector<Process>& procList);

    void schedule(int& curTime, std::vector<Process>& procList);
    void moveProcess(Process& proc, bool forward = true);
    int calculateQueueScore(Scheduler *queue);
    std::string getQueueName(Scheduler *queue);
    void orderqueues();
};

#endif
