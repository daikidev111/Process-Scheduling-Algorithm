/*
 * Author: Daiki Kubo
 * Student ID: 30523346
 * Description of the program:
 * The program simulates how a CPU scheduling algorithm works. 
 * The program consists of a scheduling algorithm named FCFS, which schedules 
 * processes based on which process comes first into the ready queue. 
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


int count = 0; // count is the number of processes scanned from the input file 
int timer = 0; // timer is the time that a scheduling process runs.
pcb_t *process = NULL; // pointer to a pcb_t type of array

int fcfsDecisionMaking(int timer, int process_index) {
/*
 * Function: fcfsDecisionMaking
 * --------------------------------
 * Function Description:
 * This function acts as a decision making algorithm for the first come first served scheduling algorithm.
 * It loops through all the processes that are available in the program and retrieve the one with the earliest 
 * entry time.
 *
 * Parameter:
 * timer: time of the program running
 * process_index: a number of processes available. e.g. process_index = 1 then process 0 and process 1 are available.
 *
 * Return int:
 * It returns a variable named current that is an index of the process with the earliest entry time and the process will
 * be executed. 
 *
 */
	int current = -1;
	int earliest = 10000;

	for (int i = 0; i < process_index; i++) {
		if (process[i].state == READY) {
			if (earliest >= process[i].entryTime) {
				earliest = process[i].entryTime;
				current = i;
			}
		}
	}
	return current;
}


void fcfsScheduling(FILE *file) {
/*
 * Function: fcfsScheduling
 * ---------------------
 *  Function description:
 *  The function serves a purpose of scheduling CPU processes based on the entry time value that each process holds to determine what process came into the ready queue first.
 *  The scheduler first loads the file content and store the important information into an instance of pcb if the entry time is
 *  the same as the timer variable.
 *  Then, we keep checking the current variable and if it is -1 then this means that there is no process that is running, hence
 *  we find a process that needs to be run with a use of decision making algorithm called fcfsDecisionMaking(). 
 *  If the current variable is not equal to -1 then this means that there is a process running and hence we decrement the remaining time by 1. If the remaining time is less than equal to 1 then it indicates that the process will end the execution at the time. So, we calculate the necessary inforamtions that have to be outputted and write the information to the output file. 
 *
 *  Parameter:
 *  *file: a pointer variable of FILE object that has the file information.
 *
 *  Returns: void
 *  It does not return anything. 
*/

	// while the checkFinish is 0, increase the timer val
	int current = -1; // to indicate no process index for execution assigned
	int process_index = 0;

	process = (pcb_t *)malloc(count * sizeof(pcb_t));

	for (timer = 0; checkFinish(process, count) == 0; timer++) {

		int process_name_len_max = 11;
		char process_name_arr[process_name_len_max];
		int entry_time, service_time, deadline_time;
		
		fseek(file, 0, SEEK_SET);

		while (fscanf(file, "%s %d %d %d\n", process_name_arr, &entry_time, &service_time, &deadline_time) != EOF) {
			if (timer == entry_time) { // if the timer is the same as the entry_time of the process
				// load all the necessary inforamtion into a pcb instance
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

		// if there is a running process
		if (current != -1) {
			if (process[current].remainingTime <= 1) { // if the remaining time is less than equal to one
				process[current].remainingTime = 0;
				process[current].completeTime = timer + 1;
				calcSchedCriteria(current, timer, process, count, 1); // calculate the necessary informaiton required for output
				current = -1;
			} else {
				process[current].remainingTime -= 1; // decrement the remaining time by one 
			}
		} else if (current == -1) { // if there is no running process
			current = fcfsDecisionMaking(timer, process_index); // invoke the decision making algorithm
			if (current != -1) {
				process[current].state = RUNNING; // change it to running
				process[current].startTime = timer; // records the start time of the process
				printf("Time %d: %s is in the running state \n", timer, process[current].process_name);

				if (process[current].remainingTime <= 1) {
					process[current].remainingTime = 0;
					process[current].completeTime = timer + 1;
					calcSchedCriteria(current, timer, process, count, 1);
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
 * This function calls all the necessary functions to initiate FCFS scheduling algorithm.
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
	} else if (argc == 1) { // if no input given
		file_name = "processes.txt";
	} else {
		write(1, "Invalid Argument(s).\n", 22);
		exit(1);
	}

	file = openFile(file_name); 
	count = printFileContent(file);

	// invoke the scheduling algorithm
	fcfsScheduling(file);
	return 1;
}

