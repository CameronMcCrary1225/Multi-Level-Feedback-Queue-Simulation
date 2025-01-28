#include "process.h"
#include "MLFQ.h"
#include "scheduler.h"
#include <iostream>
#include <vector>

int CURRENTTIME = 0;
int SERVICETIME = 0;
int WAITINGTIME = 0;
int TURNAROUNDTIME = 0;

int main() {
    // Parse the process file to get the list of processes
    std::vector<Process> processes = parseProcessFile("proclist.txt");

    // Output the processes to verify (uncomment if you want to print processes)
    /*for (const auto& p : processes) {
        printProcess(p);  
    }*/

    int schedulerChoice = chooseScheduler();  

    std::cout << "\nScheduling processes using selected scheduler...\n";
    

    //This is the most efficent scheduling I got to for the project
    //A larger data set is included in test.txt which i also used to test u will have to change some variables here
    if(schedulerChoice != 6) {
        runScheduler(schedulerChoice, processes, CURRENTTIME, 1);  // Run the scheduler with the selected choice
    } else {
        MLFQScheduler mlfqScheduler;

        for (CURRENTTIME = 0; CURRENTTIME <= 25; CURRENTTIME++) {
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
        }


        float length = processes.size();
        std::cout << "\nAll processes are completed using MLFQ!" << std::endl;
        std::cout << "Service Time: " << SERVICETIME / length << std::endl;
        std::cout << "Waiting Time: " << WAITINGTIME / length << std::endl;
        std::cout << "Turnaround Time: " << TURNAROUNDTIME / length << std::endl;
    }

    return 0;
}
