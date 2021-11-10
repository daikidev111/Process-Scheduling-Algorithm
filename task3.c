/*
 * Author: Daiki Kubo
 * Student ID: 30523346
 * Description of the program:
 * The program simulates how a CPU scheduling algorithm works. 
 * The program consists of a scheduling algorithm I invented, which schedules 
 * processes based on a deadline time that each process holds. 
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

int count = 0; // count is the number of processes that exist in the file 
int timer = 0;
pcb_t *process = NULL; // global variable that points to an array of processes

int findShortestDeadline(int timer, int process_index) {
/*
 * Function: findShortestDeadline
 * ---------------------
 *  Function description:
 *  This function initally loops with a loop count being dependent on the process_index, which
 *  is a number of process that has been arrived in the scheduling program.
 *  Then, in the loop, we check if the process state is READY or not. If the process state is ready then 
 *  we compare the shortest_deadline that we have so far with the process's deadline time. 
 *  If the deadline time is less than equal to the deadline time that the process holds then we update the 
 *  shortest_deadline value.
 *
 *  Parameter:
 *  timer: a timer is a integer variable that has the current time value of the scheduling.
 *  process_index: a process_index is a integer variable that has the number of processes, which have entered in the system.
 *
 *  Returns: Int
 *  The function returns current variable that represents which process should be executed next and if there is no 
 *  process to be executed then we simply return -1. 
*/
	int current = -1;
	int shortest_deadline = 10000; // assuming that all the processes has a deadline value less than equal to 10000

	for (int i = 0; i < process_index; i++) { // looping through from 0 to a number of processes that have arrived.
		if (process[i].state == READY) {
			if (shortest_deadline >= process[i].deadLine) {
				shortest_deadline = process[i].deadLine; // update the shortest deadline time
				current = i; // update the current value
			}
		}
	}
	return current; // return a process index that has to be executed next
}


void deadLineSched(FILE *file) {
/*
 * Function: deadLineSched
 * ---------------------
 *  Function description:
 *  deadLineSched function serves a purpose of scheduling CPU processes based on the deadline value that each process holds. 
 *  The scheduler first loads the file content and store the important information into an instance of pcb if the entry time is
 *  the same as the timer variable.
 *  Then, we keep checking the current variable and if it is -1 then this means that there is no process that is running, hence
 *  we find a process that needs to be run with a use of decision making algorithm called findShortestDeadline(). 
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
		fseek(file, 0, SEEK_SET); // set the file offset to be at 0

		while (fscanf(file, "%s %d %d %d\n", process_name_arr, &entry_time, &service_time, &deadline_time) != EOF) {
			if (timer == entry_time) { // if the timer value is the same as entry_time
				strcpy(process[process_index].process_name, process_name_arr); // copy the loaded process_name into the process name of pcb instance.
				process[process_index].entryTime = entry_time;
				process[process_index].serviceTime = service_time;
				process[process_index].deadLine = deadline_time;
				process[process_index].remainingTime = service_time;
				// switch the state to READY
				process[process_index].state = READY;
				printf("Time %d: %s has entered the system \n", timer, process[process_index].process_name);

				// increment the process_index by one 
				process_index += 1;
			}
		}

		// if there is a running process
		if (current != -1) {
			if (process[current].remainingTime <= 1) {
				process[current].remainingTime = 0;
				process[current].completeTime = timer + 1;
				calcSchedCriteria(current, timer, process, count, 3);
				current = -1;
			} else {
				process[current].remainingTime -= 1; // decrement the remaining time
			}
		} else if (current == -1) { // if there is no running process
			// invoke the decision making algorithm 
			current = findShortestDeadline(timer, process_index);

			if (current != -1) {
				process[current].state = RUNNING; // change it to running
				process[current].startTime = timer;
				printf("Time %d: %s is in the running state \n", timer, process[current].process_name);

				if (process[current].remainingTime <= 1) {
					process[current].remainingTime = 0;
					process[current].completeTime = timer + 1;
					calcSchedCriteria(current, timer, process, count, 3); // calculate the wt, ta, and dlMet
					current = -1;
				} else {
					process[current].remainingTime -= 1;
				}
			}
		}
	}
	fclose(file); // close the opened file
	free(process); // free the malloc

	return;
}


int main(int argc, char *argv[])
{
/*
 * Function: main
 * -------------------------
 * Function Description:
 * This function calls all the necessary functions to initiate deadline based scheduling algorithm.
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

	if (argc == 2) { // if a file is given in the input retrieve the file name
		file_name = argv[1];
	} else if (argc == 1) { // if no user input given
		file_name = "processes.txt";
	} else {
		write(1, "Invalid Argument(s).\n", 22);
		exit(1);
	}

	file = openFile(file_name);  // open the file 
	count = printFileContent(file); // print the file content 

	deadLineSched(file); // call the scheduling algorithm
	return 1;
}

