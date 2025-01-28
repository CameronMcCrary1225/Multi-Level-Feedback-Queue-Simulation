#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "process.h"

int chooseScheduler();
int roundrobin(int& curTime, std::vector<Process>& procList, const int& timeQuantum);
int ShortestProcessNext(const std::vector<Process>& procList, int curTime);
int ShortestRemainingTime(std::vector<Process>& procList, int& curTime);
int HighestResponseRatioNext(std::vector<Process>& procList, int curTime);
int FIFO(std::vector<Process>& procList, int& curTime);
void runScheduler(int schedulerChoice, std::vector<Process>& processes, int& CURRENTTIME, const int timeQuantum);

#endif
