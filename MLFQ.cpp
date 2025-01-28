#include "MLFQ.h"

void MLFQScheduler::schedule(int& curTime, std::vector<Process>& procList)
{
    admitnewprocess(curTime, procList); 
    runqueues(curTime);  
    updatestats(procList);
    //orderqueues();
}

void MLFQScheduler::moveProcess(Process& proc, bool forward) {
    if (proc.isDone) {
        std::cout << "Process " << proc.id << " is already completed. No movement." << std::endl;
        return;
    }

    if (processQueueMap.find(proc.id) == processQueueMap.end()) {
        std::cout << "Error: Process " << proc.id << " not found in processQueueMap." << std::endl;
        return;
    }

    Scheduler* currentQueue = processQueueMap[proc.id];
    auto it = std::find(queues.begin(), queues.end(), currentQueue);
    if (it == queues.end()) {
        std::cout << "Error: Process queue not found in the vector." << std::endl;
        return;
    }

    size_t currentIndex = std::distance(queues.begin(), it);

    int nextIndex = forward ? currentIndex + 1 : currentIndex - 1;
    if (nextIndex >= queues.size() || nextIndex < 0) {
        std::cout << "Cannot move process beyond the bounds of the queue list." << std::endl;
        return;
    }

    
    currentQueue->removeProcess(proc); 
    std::cout << "Process " << proc.id << " removed from queue " << currentIndex << std::endl;
    queues[nextIndex]->addProcess(proc);


    processQueueMap[proc.id] = queues[nextIndex];

    std::cout << "Process " << proc.id << " moved from queue " << currentIndex << " to queue " << nextIndex << std::endl;

    for (size_t i = 0; i < queues.size(); ++i) {
        std::cout << "Queue " << i << ": ";
        for (const auto& proc : queues[i]->getQueue()) {
            std::cout << proc.id << " ";
        }
        std::cout << std::endl;
    }
}

#include <typeinfo>  

void MLFQScheduler::orderqueues() {
    std::vector<Scheduler*> queuesToReorder = {
        queues[4], queues[5], queues[6], queues[7], queues[8], queues[9], queues[10], queues[11]
    };

    std::vector<std::pair<int, Scheduler*>> queueScores;

    for (auto* queue : queuesToReorder) {
        int score = calculateQueueScore(queue);  
        queueScores.push_back({score, queue});
    }

    std::sort(queueScores.begin(), queueScores.end(), 
        [](const std::pair<int, Scheduler*>& a, const std::pair<int, Scheduler*>& b) {
            return a.first > b.first;
        });

    std::cout << "Reordering queues based on scores: ";
    for (auto& pair : queueScores) {
        std::cout << getQueueName(pair.second) << " ";  
    }
    std::cout << std::endl;

    for (size_t i = 0; i < queuesToReorder.size(); ++i) {
        queues[4 + i] = queueScores[i].second; 
    }
}


void MLFQScheduler::admitnewprocess(int& curTime, std::vector<Process>& procList) {
    for (int i = 0, i_end = procList.size(); i < i_end; i++) {
        if (procList[i].startTime == curTime) {
            std::cout << "Admitting process ID: " << procList[i].id
                      << " at time " << curTime << " with priority "
                      << static_cast<int>(procList[i].priority) << std::endl;
            Scheduler* targetQueue = nullptr;

            switch (procList[i].priority) {
                case Priority::RealTime:
                    rt1.addProcess(procList[i]);
                    targetQueue = &rt1;
                    break;
                case Priority::High:
                    h1.addProcess(procList[i]);
                    targetQueue = &h1;
                    break;
                case Priority::AboveNormal:
                    an1.addProcess(procList[i]);
                    targetQueue = &an1;
                    break;
                case Priority::Normal:
                    n1.addProcess(procList[i]);
                    targetQueue = &n1;
                    break;
                case Priority::BelowNormal:
                    bn1.addProcess(procList[i]);
                    targetQueue = &bn1;
                    break;
                case Priority::Idle:
                    i1.addProcess(procList[i]);
                    targetQueue = &i1;
                    break;
                default:
                    std::cerr << "Unknown priority for process " << procList[i].id << std::endl;
                    break;
            }

            if (targetQueue != nullptr) {
                processQueueMap[procList[i].id] = targetQueue; 
            }
        }
    }
}

void MLFQScheduler::runqueues(int& curTime) {
    for (auto& queue : queues) {
        if (!queue->isEmpty()) {
            Process& proc = queue->getQueue().front();

            if (proc.isDone) {
                continue;
            }

            queue->schedule(); 

            queue->removeCompletedProcesses();
            break;  
        }
    }
}

void MLFQScheduler::updatestats(std::vector<Process>& procList) {
    for (auto& proc : procList) {
        if (!proc.isDone && proc.timeScheduled < proc.burstTime) {
            proc.waitingTime += 1;
        }
    }
}
int MLFQScheduler::calculateQueueScore(Scheduler* queue) {
 
    double weight = 0.0;

    if (dynamic_cast<ABOVENORMAL*>(queue)) {
        weight = 2.0; 
    } else if (dynamic_cast<NORMAL*>(queue)) {
        weight = 1.0;
    } else if (dynamic_cast<BELOWNORMAL*>(queue)) {
        weight = 0.5;
    } else if (dynamic_cast<IDLE*>(queue)) {
        weight = 0.1;
    }

    // Multiply weight by number of processes in the queue
    return static_cast<int>(weight * queue->getQueue().size());
}
std::string MLFQScheduler::getQueueName(Scheduler* queue) {
    if (typeid(*queue) == typeid(ABOVENORMAL)) {
        return "ABOVENORMAL";
    } else if (typeid(*queue) == typeid(NORMAL)) {
        return "NORMAL";
    } else if (typeid(*queue) == typeid(BELOWNORMAL)) {
        return "BELOWNORMAL";
    } else if (typeid(*queue) == typeid(IDLE)) {
        return "IDLE";
    } else {
        return "UNKNOWN"; 
    }
}
