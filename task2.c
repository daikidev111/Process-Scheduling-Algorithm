/*
 * Author: Daiki Kubo
 * Student ID: 30523346
 * Description of the program:
 * The program simulates how a CPU scheduling algorithm works. 
 * The program consists of a scheduling algorithm named SRTN, which schedules 
 * processes based on a remaining time of a process service time. 
 * The behavior of this algorithm is accurately recorded with a terminal output and
 * output file that contains wait time, turnaround time, and an indicator whether or not a process
 * meets the deadline.
 * Start Date: 1st of October
 * Last modified: 8th of October
 */

/* List of libraries used */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "scheduler_pcb.h"
#include "scheduler_function.h"

int count = 0; // a number of processes exist in a file scanned
int timer = 0; // time of the process scheduling running
pcb_t *process = NULL; // a pointer to pcb_t type of process array 

int checkShortestRemainTime(int process_index) {
/*
 * Function: checkShortestRemainTime
 * -----------------------------------
 *  Function Description:
 *  This function is a decision making algorithm for the shortest remain time next algorithm.
 *  It checks which process that is in either ready or running state has the shortest remaining time. 
 *  It records the index of the process that has the shortest remaining time, which is used for the 
 *  scheduling functionality to proceed with the process at the index.
 *
 *  Parameter:
 *  process_index: This indicates how many processes are in the scheduling system, which is used for loop condition.
 *
 *  Return int:
 *  It returns an index of a process that has to be executed next as it has the shortest remaining time in the given conditions.
 */
	int shortest = 10000; // we assume that the remaining time should be less than 10000
	int shortest_elem = -1;
	for (int i = 0; i < process_index; i++) {
		// if the remaining time is less than equal to the shortest and the state is either ready or running 
		if (process[i].remainingTime <= shortest && (process[i].state == READY || process[i].state == RUNNING)) {
			shortest = process[i].remainingTime;
			shortest_elem = i; // record index
		}
	}
	return shortest_elem;
}

int checkAvailableProcess(int timer, int process_index) {
/*
 * Function: checkAvailableProcess
 * ---------------------------------
 * Function Description:
 * This function acts as a decision making algorithm for short process next.
 * If a process finishes during the time quantum, this algorithm is activated to 
 * find a process to be executed next. 
 * It runs a loop to find the earliest entryTime of a process and retrieve the index of the process. 
 *
 * Parameter:
 * timer: a current time of the scheduling program.
 * process_index: This indicates how many processes are in the scheduling system, which is used for loop condition.
 *
 * Return int:
 * It returns an index of a process that has to be executed next as it has the earliest entry time in the given conditions.
 */
	int current = -1;
	int earliest = 10000; // we assume that the entry time is less than 10000

	for (int i = 0; i < process_index; i++) {
		if (process[i].state == READY || process[i].state == RUNNING) {
			// if the earliest is more than equal to the entry time 
			if (earliest >= process[i].entryTime) {
				earliest = process[i].entryTime;
				current = i; // record the index of the process
			}
		}
	}
	return current;
}

void shortRemainTimeSched(int time_quantum, FILE *file) {
/*
 * Function: shortRemainTimeSched
 * ---------------------
 *  Function description:
 *  shortRemainTimeSched function serves a purpose of scheduling CPU processes based on the remaining value that each process holds. 
 *  The scheduler first loads the file content and store the important information into an instance of pcb if the entry time is the same as the timer variable.
 *  It find a process that needs to be run with a use of decision making algorithm called checkShortestRemainingTime() at with a time quantum of 3.
 *  If the current variable is not equal to -1 then this means that there is a process running and hence we decrement the remaining time by 1. If the remaining time is less than equal to 1 then it indicates that the process will end the execution at the time. So, we calculate the necessary inforamtions that have to be outputted and write the information to the output file. 
 *  If the current variable is equal to -1 then it finds a process that needs to be run with a use of decision making algorithm called checkAvailableProcess.
 *
 *  Parameter:
 *  time_quantum: a time quantum required for preemption
 *  *file: a pointer variable of FILE object that has the file information.
 *
 *  Returns: void
 *  It does not return anything. 
*/
	int current = -1; // to indicate no process index for execution assigned
	int process_index = 0;

	process = (pcb_t *)malloc(count * sizeof(pcb_t));

	for (timer = 0; checkFinish(process, count) == 0; timer++) {

		int process_name_len_max = 11;
		char process_name_arr[process_name_len_max];
		int entry_time, service_time, deadline_time;

		fseek(file, 0, SEEK_SET);

		while (fscanf(file, "%s %d %d %d\n", process_name_arr, &entry_time, &service_time, &deadline_time) != EOF) {
			if (timer == entry_time) { // if the timer is the same as the entry time of the process
				strcpy(process[process_index].process_name, process_name_arr);
				process[process_index].entryTime = entry_time;
				process[process_index].serviceTime = service_time;
				process[process_index].deadLine = deadline_time;
				process[process_index].remainingTime = service_time;

				process[process_index].state = READY;
				printf("Time %d: %s has entered the system \n", timer, process[process_index].process_name);
				process_index += 1;
			}
		}
		
		// check every 3 seconds
		if (timer % 3 == 0) { // 0 % 3 = 0 as well
			current = checkShortestRemainTime(process_index);
		}

		if (current != -1) {
			if (process[current].state == READY) { // if the process state is ready
				process[current].state = RUNNING;
				process[current].startTime = timer;
				printf("Time %d: %s is in the running state. \n", timer, process[current].process_name);
			}

			if (process[current].remainingTime <= 1) { // is the remaining time is less than equal to 1
				process[current].remainingTime = 0;
				process[current].completeTime = timer + 1;
				calcSchedCriteria(current, timer, process, count, 2); // calculate information required for output 
				current = -1;
			} else {
				process[current].remainingTime -= 1; 
			}
		} else { // if the process exits before the next time quantum interval then find another
			current = checkAvailableProcess(timer, process_index);
			if (current != -1) {
				if (process[current].state == READY) {
					process[current].state = RUNNING;
					process[current].startTime = timer;
					printf("Time %d: %s is in the running state. \n", timer, process[current].process_name);
				}

				if (process[current].remainingTime <= 1) {
					process[current].remainingTime = 0;
					process[current].completeTime = timer + 1;
					calcSchedCriteria(current, timer, process, count, 2);
					current = -1;
				} else {
					process[current].remainingTime -= 1; 
				}
			}
		}

	}
	fclose(file); // close the file 
	free(process); // free the malloc 
	return;
}


int main(int argc, char *argv[])
{
/*
 * Function: main
 * -------------------------
 * Function Description:
 * This function calls all the necessary functions to initiate shortest remaining time next scheduling algorithm.
 * It accepts an user input of a file name from the command line. If a user input is not given, we 
 * set the file_name to be processes.txt.
 *
 * Parameter:
 * argc: argument count in integer.
 * argv: an array of input from the command line.
 *
 * Return: int
 * It returns 1, indicating that a program runs successfuly.
 *
 */
	char *file_name;
	FILE *file;

	if (argc == 2) { // if the argument count is 2 
		file_name = argv[1];
	} else if (argc == 1) { // if there is no user input given
		file_name = "processes.txt";
	} else {
		write(1, "Invalid Argument(s).\n", 22);
		exit(1);
	}

	file = openFile(file_name); 
	count = printFileContent(file);

	shortRemainTimeSched(3, file); // invoke the scheduling algorithm
	return 1;
}

