#include "scheduler.h"
#include "process.h"
#include <deque>
#include <vector>
#include <iostream>
#include "MLFQ.h"
#include <deque>

int chooseScheduler() {
    int choice;
    std::cout << "Choose a scheduler:" << std::endl;
    std::cout << "1. Round Robin" << std::endl;
    std::cout << "2. Shortest Process Next" << std::endl;
    std::cout << "3. Shortest Remaining Time" << std::endl;
    std::cout << "4. Highest Response Ratio Next" << std::endl;
    std::cout << "5. First In First Out" << std::endl;
    std::cout << "6. Multi-Level Feedback Queue" << std::endl;
    std::cout << "Enter your choice (1-6): ";
    std::cin >> choice;

    while (choice < 1 || choice > 6) {
        std::cout << "Invalid choice. Please enter 1 for Round Robin, 2 for Shortest Process Next, 3 for Shortest Remaining Time, or 4 for Highest Response Ratio Next: ";
        std::cin >> choice;
    }

    return choice;
}

int roundrobin(int& curTime, std::vector<Process>& procList, const int& timeQuantum) {
    static int timeToNextSched = timeQuantum;
    static std::deque<int> ready;

    int idx = -1;

    for(int i = 0, i_end = procList.size(); i < i_end; ++i) {
        if(procList[i].startTime == curTime) {
            ready.push_back(i);
        }
    }

    if(timeToNextSched == 0 || procList[ready[0]].isDone) {
        if(!procList[ready[0]].isDone) {
            ready.push_back(ready[0]);
        }
        ready.pop_front();
        timeToNextSched = timeQuantum;
    }

    if(ready.size() > 0) {
        idx = ready[0];
        --timeToNextSched;
    } else {
        idx = -1;
        timeToNextSched = 0;
    }

    return idx;
}

int ShortestProcessNext(std::vector<Process>& procList, int& curTime) {
    if (procList.empty()) {
        return -1;
    }

    int idx = -1;

    for (int i = 0, i_end = procList.size(); i < i_end; i++) {
        if(procList[i].timeRemaining - procList[i].burstTime < 0 && procList[i].timeRemaining != 0) {
            return i;
        }
        if (!procList[i].isDone && procList[i].startTime <= curTime) {
            if (idx == -1 || procList[i].timeRemaining < procList[idx].timeRemaining) {
                idx = i;
            }
        }
    }

    return idx;
}

int ShortestRemainingTime(std::vector<Process>& procList, int& curTime) {
    if (procList.empty()) {
        return -1;
    }

    int idx = -1;

    for (int i = 0, i_end = procList.size(); i < i_end; i++) {
        if (!procList[i].isDone && procList[i].startTime <= curTime) {
            int timeLeft = procList[i].timeRemaining;
            if (idx == -1 || timeLeft < procList[idx].timeRemaining) {
                idx = i;
            }
        }
    }

    return idx;
}

int HighestResponseRatioNext(std::vector<Process>& procList, int curTime) {
    int idx = -1;

    for (int i = 0, i_end = procList.size(); i < i_end; i++) {
        if (!procList[i].isDone && procList[i].startTime <= curTime) {
            int waitingTime = curTime - procList[i].startTime;
            if (waitingTime < 0) waitingTime = 0;

            int expectedServiceTime = procList[i].burstTime;
            float ratio = (waitingTime + expectedServiceTime) / (float)expectedServiceTime;

            if (idx == -1) {
                idx = i;
            } else {
                int prevWaitingTime = curTime - procList[idx].startTime;
                if (prevWaitingTime < 0) prevWaitingTime = 0;

                int prevExpectedServiceTime = procList[idx].burstTime;
                float prevRatio = (prevWaitingTime + prevExpectedServiceTime) / (float)prevExpectedServiceTime;

                if (ratio > prevRatio) {
                    idx = i;
                }
            }
        }
    }

    return idx;
}

int FIFO(std::vector<Process>& procList, int& curTime) {
    for (int i = 0, i_end = procList.size(); i < i_end; ++i) {
        if (!procList[i].isDone && procList[i].startTime <= curTime) {
            return i;
        }
    }
    return -1;
}

void runScheduler(int schedulerChoice, std::vector<Process>& processes, int& CURRENTTIME, const int timeQuantum) {
    bool allProcessesComplete = false;

    while (!allProcessesComplete) {
        switch (schedulerChoice) {
            case 1: {
                int idx = roundrobin(CURRENTTIME, processes, timeQuantum);
                if (idx != -1) {
                    Process& p = processes[idx];
                    p.serviceTime++;
                    --p.timeRemaining;
                    CURRENTTIME++;

                    if (p.timeRemaining == 0) {
                        p.isDone = true;
                        p.timeFinished = CURRENTTIME;
                        std::cout << "Process " << p.id << " finished at time " << CURRENTTIME << std::endl;
                    }
                } else {
                    for (auto& p : processes) {
                        if (!p.isDone && p.timeScheduled <= CURRENTTIME) {
                            p.waitingTime++;
                        }
                    }
                    CURRENTTIME++;
                }
                break;
            }
            case 2: {
                int idx = ShortestProcessNext(processes, CURRENTTIME);
                if (idx != -1) {
                    Process& p = processes[idx];
                    p.serviceTime++;
                    p.timeRemaining -= 1;
                    CURRENTTIME++;

                    if (p.timeRemaining == 0) {
                        p.isDone = true;
                        p.timeFinished = CURRENTTIME;
                        std::cout << "Process " << p.id << " finished at time " << CURRENTTIME << std::endl;
                    }
                }
                break;
            }
            case 3: {
                int idx = ShortestRemainingTime(processes, CURRENTTIME);
                if (idx != -1) {
                    Process& p = processes[idx];
                    p.serviceTime++;
                    p.timeRemaining -= 1;
                    CURRENTTIME++;

                    if (p.timeRemaining == 0) {
                        p.isDone = true;
                        p.timeFinished = CURRENTTIME;
                        std::cout << "Process " << p.id << " finished at time " << CURRENTTIME << std::endl;
                    }
                }
                break;
            }
            case 4: {
                int idx = HighestResponseRatioNext(processes, CURRENTTIME);
                if (idx != -1) {
                    Process& p = processes[idx];
                    p.serviceTime++;
                    p.timeRemaining -= 1;
                    CURRENTTIME++;

                    if (p.timeRemaining == 0) {
                        p.isDone = true;
                        p.timeFinished = CURRENTTIME;
                        std::cout << "Process " << p.id << " finished at time " << CURRENTTIME << std::endl;
                    }
                }
                break;
            }
            case 5: {
                int idx = FIFO(processes, CURRENTTIME);
                if (idx != -1) {
                    Process& p = processes[idx];
                    p.serviceTime++;
                    p.timeRemaining -= 1;
                    CURRENTTIME++;

                    if (p.timeRemaining == 0) {
                        p.isDone = true;
                        p.timeFinished = CURRENTTIME;
                        std::cout << "Process " << p.id << " finished at time " << CURRENTTIME << std::endl;
                    }
                }
                break;
            }
            case 6: {
                MLFQScheduler mlfqScheduler;
                
                while (true) {
                    mlfqScheduler.schedule(CURRENTTIME, processes);

                    bool allCompleted = true;
                    for (const auto& p : processes) {
                        if (!p.isDone) {
                            allCompleted = false;
                            break;
                        }
                    }

                    if (allCompleted) {
                        break;
                    }

                    CURRENTTIME++;
                }

                float length = processes.size();
                std::cout << "\nAll processes are completed using MLFQ!" << std::endl;
                std::cout << "Service Time: " << SERVICETIME / length << std::endl;
                std::cout << "Waiting Time: " << WAITINGTIME / length << std::endl;
                std::cout << "Turnaround Time: " << TURNAROUNDTIME / length << std::endl;

                break;
            }
            default:
                std::cerr << "Invalid scheduler choice!" << std::endl;
                return;
        }

        if (schedulerChoice != 6) {
            allProcessesComplete = true;
            for (const auto& process : processes) {
                if (!process.isDone) {
                    allProcessesComplete = false;
                    break;
                }
            }
        }
    }
    std::cout << "All processes are completed!" << std::endl;

    if (schedulerChoice != 6) {
        calculateAndPrintMetrics(processes);
    }
}
