Multi-Level Feedback Queue Simulation

This project simulates a Multi-Level Feedback Queue (MLFQ) scheduler, inspired by the behavior of Windows scheduling. It was created as a final project for an Operating Systems course. While the project is still a work-in-progress, it successfully implements a priority-based queue system along with several well-known scheduling algorithms.

Features

- Implemented Scheduling Algorithms:
  - Round Robin
  - Shortest Process Next
  - Shortest Remaining Time
  - Highest Response Ratio Next
  - First In First Out
  
- Priority Queue Simulation: Implements a scheduler based on priority levels similar to Windows' scheduling system.
- Partial MLFQ Implementation: The project includes the basic structure of multi-level queues but is currently limited in functionality (e.g., queue ordering, demotion/promotion of tasks).

Installation

Prerequisites

Ensure you have WSL (Windows Subsystem for Linux) or a Linux environment, along with make installed.

Steps

1. Clone the repository to your local machine:
   git clone https://github.com/username/Multi-Level-Feedback-Queue-Simulation.git

2. Navigate to the project directory:
   cd Multi-Level-Feedback-Queue-Simulation

3. Run make to compile the project:
   make

4. Once the compilation is complete, run the scheduler:
   ./scheduler

5. The program will display a list of schedulers and the overall Multi-Level Feedback Queue (MLFQ).

Usage

The program simulates the functioning of various scheduling algorithms, though the MLFQ is still a work-in-progress. The following algorithms are fully implemented:

- Round Robin
- Shortest Process Next
- Shortest Remaining Time
- Highest Response Ratio Next
- First In First Out

Known Limitations:
- The MLFQ structure is incomplete. At present, only the scheduler inside the queues works, with processes being scheduled based on their priority levels.
- Demotion and Promotion mechanisms for processes are not yet functioning.
- Future improvements are planned to fix the ordering of queues and implement a full working MLFQ scheduler.

Contributing

I am the sole contributor to this project. However, if you would like to contribute or provide suggestions, feel free to fork the repository and open issues.

License

This project does not have a specific license. You are welcome to use or modify it, but please respect the original work.

Technologies Used

- C++: The core language used for the project.
- Makefile: To manage the build process.
- Standard C++ Libraries: Used for managing scheduling and queues.

Future Plans

- Queue Ordering: Finalize the multi-level queue structure to properly implement the ordering of queues.
- Demotion/Promotion: Implement functionality to handle the demotion and promotion of processes between levels based on their behavior.
- Full MLFQ Simulation: Complete the MLFQ scheduler to match the behavior of an actual operating system scheduler.

Acknowledgements

This project was developed as part of my Operating Systems final project, and it greatly expanded my knowledge of scheduling algorithms and system programming.
