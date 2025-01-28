#include "process.h"
#include <iomanip>
#include "MLFQ.h"
Priority stringToPriority(const std::string& priorityStr) {
    if (priorityStr == "RealTime") return Priority::RealTime;
    if (priorityStr == "High") return Priority::High;
    if (priorityStr == "AboveNormal") return Priority::AboveNormal;
    if (priorityStr == "Normal") return Priority::Normal;
    if (priorityStr == "BelowNormal") return Priority::BelowNormal;

    if (priorityStr == "Idle") return Priority::Idle;
    throw std::invalid_argument("Invalid priority string");
}

std::vector<Process> parseProcessFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Process> processes;

    if (file.is_open()) {
        std::string line;

        while (std::getline(file, line)) {
            if (line.empty()) continue; 

            std::istringstream ss(line);
            std::string id, priorityStr;
            int startTime, burstTime;

            ss >> id >> startTime >> burstTime >> priorityStr;

            try {
                Priority priority = stringToPriority(priorityStr);

                Process p(id, startTime, burstTime, priority);
                processes.push_back(p);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error parsing process. Invalid priority string: " << priorityStr << std::endl;
                continue; 
            }
        }
        file.close();
    } else {
        std::cerr << "Error opening file!" << std::endl;
    }

    return processes;
}
void printProcess(const Process& p) {
    std::cout << "Process ID: " << p.id
              << ", Start Time: " << p.startTime
              << ", Burst Time: " << p.burstTime
              << ", Priority: ";
    
    switch (p.priority) {
        case Priority::RealTime: std::cout << "RealTime"; break;
        case Priority::High: std::cout << "High"; break;
        case Priority::AboveNormal: std::cout << "AboveNormal"; break;
        case Priority::Normal: std::cout << "Normal"; break;
        case Priority::BelowNormal: std::cout << "BelowNormal"; break;
        case Priority::Idle: std::cout << "Idle"; break;
    }
    std::cout << std::endl;
}
void calculateAndPrintMetrics(std::vector<Process>& processes) {
    double totalTurnaroundTime = 0.0;
    double totalServiceTime = 0.0;
    double totalWaitingTime = 0.0;
    double totalTurnaroundRatio = 0.0;

    std::cout << std::setw(10) << "Process ID"
              << std::setw(15) << "Service Time"
              << std::setw(15) << "Turnaround Time"
              << std::setw(15) << "Waiting Time"
              << std::setw(25) << "Turnaround/Service Time Ratio"
              << std::endl;

    for (const auto& p : processes) {
        // Calculate Turnaround Time: The time from arrival to completion
        int turnaroundTime = p.timeFinished - p.startTime;

        // Service Time: This is tracked in the Process struct
        int serviceTime = p.serviceTime;

        // Calculate Waiting Time: The difference between turnaround time and service time
        int waitingTime = turnaroundTime - serviceTime;

        // Calculate Turnaround/Service Time ratio
        double turnaroundServiceRatio = (serviceTime == 0) ? 0 : static_cast<double>(turnaroundTime) / serviceTime;

        // Accumulate totals for averages later
        totalTurnaroundTime += turnaroundTime;
        totalServiceTime += serviceTime;
        totalWaitingTime += waitingTime;
        totalTurnaroundRatio += turnaroundServiceRatio;

        std::cout << std::setw(10) << p.id
                  << std::setw(15) << serviceTime
                  << std::setw(15) << turnaroundTime
                  << std::setw(15) << waitingTime
                  << std::setw(25) << turnaroundServiceRatio
                  << std::endl;
    }


    double averageTurnaroundTime = totalTurnaroundTime / processes.size();
    double averageServiceTime = totalServiceTime / processes.size();
    double averageWaitingTime = totalWaitingTime / processes.size();
    double averageTurnaroundRatio = totalTurnaroundRatio / processes.size();


    std::cout << "\nAverage Turnaround Time: " << averageTurnaroundTime << std::endl;
    std::cout << "Average Service Time: " << averageServiceTime << std::endl;
    std::cout << "Average Waiting Time: " << averageWaitingTime << std::endl;
    std::cout << "Average Turnaround/Service Time Ratio: " << averageTurnaroundRatio << std::endl;
}
