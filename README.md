# Process-Scheduling-Algorithm

This document covers instructions on how to use three files that execute various types of CPU scheduling algorithms.

Author: Daiki Kubo

# Files 
For FCFS scheduling algorithm:
task1.c

For SRTN scheduling algorithm:
task2.c

For deadline-based scheduling algorithm:
task3.c

Scheduling function header:
scheduler_function.h

PCB data structure header:
scheduler_pcb.h

# Starter
How to compile the files:
gcc [file_name from the above] -o process

*Make sure to have .h header files and processes.txt in the same folder level as the scheduling algorithm files to run properly. 

Running the compiled program:
./process

It prints out all the necessary information required in the terminal and output to results-tasknum.txt

# Important
*If you like to run a compiled program without any user input then make sure to include a text file named "processes.txt" that contains some process information. 

*the reading file must be in .txt format.

*You cannot provide more than one argument following after the ./process as it would return 
invalid arguments.

*You cannot provide a file that does not exist. Otherwise, it would return the "Unable to open" message.


# Input file requirement

There is a specific format that you have to follow in order for the program to correctly scan the content of the file. 

Requirement:

- Each line of the input file has to contain values for one process, a format of which is shown below;

[Process Name] [Arrival Time] [Service Time] [Deadline Value]

The example of the file format is as follows;

=== example.txt ===
A 0 3 2
B 1 2 3 
C 2 2 2
===================

For the first line of the input file represents process A with an arrival time of 0, service time of 3, and deadline value of 2.

# Instruction

BASICS:
1. Reading processes.txt file and conduct a scheduling algorithm.
	Syntax:	Since processes.txt file is a default file, it reads processes.txt file
		./process 
2. Reading a specific text file and conduct a scheduling algorithm.
	Syntax: 
		./process [absolute_file_name]

(Task 1) Running FCFS Scheduling algorithm:
FCFS scheduling algorithm is contained in the program file named task1.c.

Functionality:
FCFS is a non-preemptive scheduling algorithm that schedules an order of execution for a process based on which process comes into a ready queue first. 
To simulate this behavior, there is a function named fcfsDecisionMaking() function in the program that determines which process in the READY state has the earliest entry time. 
In the fcfsScheduling() functionality, it loads all the current processes where the entry time of the process is less than equal to the time that a program keeps counting for. 
From the processes loaded, it determines which process has to be executed based on the decision-making algorithm. 


Assumption:
- An entry time of a process should be less than 10000
- There are at most 100 processes
- The processes listed in order of arrival time in the given text file
- Since we use strong comparison, if there are two processes, holding the earliest entry times then the last one will be chosen. 
- There is no process with a service time that is less than equal to 0.
- There is no process with an entry time that is less than 0. 
- There is no process with a deadline time that is less than equal to 0.  
 
There are some steps required to run the program, which is shown below:

1. Compile the task1 file:
	gcc task1.c -o process 

2. Execute the compiled file:
	./process [absolute_file_name (*see the basic section)]

3. Check the terminal output and the output written in results-task1.txt

(Task 2) Running SRTN scheduling algorithm:
The SRTN scheduling algorithm is contained in the program file named task2.c.

Functionality:
SRTN is a pre-emptive scheduling algorithm that schedules an order of execution for a process based on the shortest remaining time of a process with a time quantum. 
To simulate this behavior, there is a function named checkShortestRemainTime that determines which process in the READY or RUNNING state has the shortest remaining time. 
In the shortRemainTimeSched function, it loads all the current processes where the entry time of the process is less than equal to the time that a program keeps counting for. 
From the processes loaded, it determines which process has to be executed based on the decision-making algorithm with a consideration of time quantum. If a process execution finishes before the time quantum then we use checkAvailableProcess that determines which process has the earliest entry time and puts the process with the earliest entry time into an execution order until the time quantum hits. 


Assumption:
- An entry time and remaining time(=service time) of a process should be less than 10000
- There are at most 100 processes
- The processes listed in order of arrival time in the given text file
- Since we use strong comparison, if there are two processes, holding the earliest entry times and shortest remaining time then the last one will be chosen. 
- There is no process with a service time that is less than equal to 0.
- There is no process with an entry time that is less than 0. 
- There is no process with a deadline time that is less than equal to 0.  

There are some steps required to run the program, which is shown below:

1. Compile the task1 file:
	gcc task2.c -o process 

2. Execute the compiled file:
	./process [absolute_file_name (*see the basic section)]

3. Check the terminal output and the output written in results-task2.txt



(Task 3) Running Deadline based scheduling algorithm:
A deadline-based scheduling algorithm is contained in the program file named task3.c

Algorithm Design Decision:
The primary objective for this algorithm is to maximize the number of deadlines meets which is determined if a process turnaround time is less than equal to the deadline value that a process holds.
Turnaround time is calculated by subtracting entry time from completion time. Hence, when designing the algorithm, we decided to put a strong focus on the completion time because the turnaround time decreases as the completion time decreases, which increases the probability of meeting the deadline given in a process. 
In order to achieve this, we have decided to prioritize a process with the shortest deadline time to be executed first. This is to decrease the completion time it takes to finish the process execution and ideally finish the execution before the next process entry time so that the next process can be executed without delay. 
Hence, we have decided to create a decision-making algorithm for the scheduling algorithm that determines the shortest deadline time of a process that needs to be executed in order to maximize the number of processes that meet the deadline time. We have decided

Functionality:
A deadline-based scheduling algorithm is a non-preemptive scheduling algorithm that schedules an order of process execution based on the deadline value that each process holds. 
Based on the algorithm design decision, there is a function named findShortestDeadline that determines which process in the READY state has the shortest deadline time. 
In the deadLineSched function, it loads all the current processes where the entry time of the process is less than equal to the time that a program keeps counting for. 
From the processes loaded, it determines which process has to be executed based on the decision-making algorithm and execute the targeted process accordingly until the remaining time becomes
equal to 0, indicating the end of process execution. 

Assumption:
- A deadline time value of a process should be less than 10000
- There are at most 100 processes
- The processes listed in order of arrival time in the given text file
- Since we use strong comparison, if there are two processes, holding the shortest deadline times then the last one will be chosen. 
- There is no process with a service time that is less than equal to 0.
- There is no process with an entry time that is less than 0.
- There is no process with a deadline time that is less than equal to 0.  

There are some steps required to run the program, which is shown below;

1. Compile the task1 file:
	gcc task3.c -o process 

2. Execute the compiled file:
	./process [absolute_file_name (*see the basic section)]

3. Check the terminal output and the output written in results-task3.txt

Example test case:

[process_name] [Entry Time] [Service Time] [Deadline]
	A	    0		2		2
	B	    1		2		3
	C	    2		1		5
	D	    3		2		3

For the example test case, my deadline-based algorithm is capable of maximizing the number of processes that meet their specified deadlines. 

The procedure of the deadline based algorithm is shown below;

At time = 0:
Process A enters into the scheduling program with a state of READY and since it has the shortest deadline, the program executes process A that is now in the RUNNING state. The remaining time of process A becomes 1 (= 2-1).

At time = 1:
Process B enters into the scheduling program with a state of READY. However, process A is still 
In the RUNNING state, the program continues to execute process A, which makes the remaining time to be 0 and process A exits.

At time = 2:
Process C enters. Since process, B has the shortest deadline (B.deadline < C.deadline), process B is executed. Process B's remaining time becomes 1. 

At time = 3:
Process D enters. Since process B is still in the RUNNING state, it continues to run. 
The remaining time of process B becomes 0 and process B exits. 

At time = 4:
Process D has the shortest deadline and hence it is now in the running state and the program 
executes process D. 

... This goes on until all the processes are in an EXIT state. (Ends at time = 6) and it outputs all the required information in the results-task3.txt file. In the output file, you should be able to see that all the processes have a value of 1 in the Deadline Met column. Hence, it maximized the number of processes that meet a specified deadline value. 

The effectiveness for maximizing the number of processes that meet a deadline value becomes clear if you compare the output you run with FCFS and SRTN with time quantum = 3 when you run the algorithms with the example test case. 

For the output of the FCFS algorithm, some processes do not meet the deadline value as it runs process C first at time = 4 due to the decision-making algorithm choosing the earliest entry time. 
Similarly, for the output of SRTN with time quantum = 3, some processes do not meet the deadline value due to the completion time of process B and process C entering first instead of process D, which is caused by its decision-making algorithms. 

From the comparison of other process scheduling algorithms and the deadline-based algorithm, we can confidently state that it effectively maximizes the number of processes that meet a specified deadline value as it executes a process with the shortest deadline value. 