/*
 * Author: Daiki Kubo
 * Student ID: 30523346
 * Description of the program:
 * This is a .h file that contains all the common functionalities used across
 * all the tasks.
 *
 * Start Date: 1st of October
 * Last modified: 8th of October
 */


int checkFinish(pcb_t* process, int count) {
/*
 *  Function description:
 *  This function loops through the number of processes that a file scanner 
 *  retrieves and check every process to check whether or not a particular process
 *  exits the scheduling program or not.
 *
 *  Returns: Int
 *  The function returns 1, which indicates that all the processes exit the scheduling program successfully. 
 *  Otherwise, it returns 0, indicating that some of the process are not exited in the scheduling program yet. 
 */

	int valid_finish = 1;
	for (int j = 0; j < count; j++) {
		if (process[j].state != EXIT) {
			valid_finish = 0;
		}
	}
	return valid_finish;

}

void writeFile(int process_index, pcb_t* process, int count, int task_num) {
/*
 * Function: writeFile
 * --------------------------------
 * Function Description:
 * Write an output to the output file.
 * Since, it writes an output in append mode, we initally clear the file content
 * at the index of 0.
 * At the end of all the process index, we close the file.
 *
 * Parameter:
 * process_index: an index that indicates which process output is going to be wrriten. 
 * count: number of processes
 * process: an array of processes that is available in the scheduling program.
 * task_num: task number to determines which task file is being executed
 *
 * Return void:
 * It does not return anything.
 */
	char *file_name;
	if (task_num == 1) {
		char *file_name_task1 = "results-task1.txt";
		file_name = file_name_task1;
	} 
	else if (task_num == 2) {
		char *file_name_task2 = "results-task2.txt";
		file_name = file_name_task2;
	} else if (task_num == 3) {
		char *file_name_task3 = "results-task3.txt";
		file_name = file_name_task3;
	} else {
		char *file_name_task_unknown = "results-task-unknown.txt";
		file_name = file_name_task_unknown;
		printf("Invalid task number, the output file is results-task-unknown.txt");
	}

	FILE *file_clear_content = fopen(file_name, "w");

	if (process_index == 0) { 
		//since it is in the append mode for file, we reopen the file with write mode that clears the content that a file originally has.
		freopen(file_name, "w", file_clear_content);
		fclose(file_clear_content);
	}

	FILE *file = fopen(file_name, "a");

	fprintf(file, "%s %d %d %d\n", process[process_index].process_name, process[process_index].waitTime, process[process_index].turnaroundTime, process[process_index].ddlMet);

	if (process_index == count) { // when it is writing the last process, then close the file after that.
		fclose(file);
	}

	return;
}

void calcSchedCriteria(int process_index, int timer, pcb_t* process, int count, int task_num) {
	/*
	 * Function calcSchedCriteria
	 * ---------------------------
	 *  Function description:
	 *  This funciton calculates all the required information that need to be outputted after the program finishes.
	 *  Specifically, wait time, turnaround time, and a indicator value whether or not a process met its 
	 *  pre-defined deadline time or not. 
	 *  All of the mentioned information is saved in the corresponding process control block instance.
	 *
	 *  List of parameter:
	 *  process_index: An index of the process that is passed on. 
	 *  timer: a timer count value of the running scheduling program.
	 *  task_num: task number to determines which task file is being executed
	 *
	 *  Return: void
	 *  Nothing is returned from this function.
	 */

	int waitTime = process[process_index].startTime - process[process_index].entryTime;
	process[process_index].waitTime = abs(waitTime);

	process[process_index].turnaroundTime = process[process_index].completeTime - process[process_index].entryTime;

	if (process[process_index].turnaroundTime <= process[process_index].deadLine) {
		process[process_index].ddlMet = 1;
	} else {
		process[process_index].ddlMet = 0;
	}
	writeFile(process_index, process, count, task_num);
	process[process_index].state = EXIT;
	printf("Time %d: %s has finished execution \n", timer, process[process_index].process_name);
	return;
}


int printFileContent(FILE *file) {
/*
 * Function: printFileContent
 * --------------------------------
 * Function Description:
 * Prints out the file content to the terminal by looping through each line of the file.
 *
 * Parameter: 
 * file: a pointer to a file object. 
 *
 * Return int:
 * It returns a number of processes obtained while scanning the file.
 */
	int count = 0;
	int process_name_len_max = 11;
	char process_name[process_name_len_max];
	int entry_time, service_time, deadLine;

	printf("---------------------------------- \n");
	while (fscanf(file, "%s %d %d %d\n", process_name, &entry_time, &service_time, &deadLine) != EOF) {
		count += 1; // retrieve the number of lines in data file
		printf("%s %d %d %d \n", process_name, entry_time, service_time, deadLine);
	}
	printf("---------------------------------- \n");

	return count;
}


FILE* openFile(char *file_name){
/*
 * Function: openFile
 * --------------------------------
 * Function Description:
 * The function is used to open a file 
 *
 * Parameter: 
 * file_name: a char pointer to a file_name that is used for opening the specified file.
 *
 * Return FILE*:
 * Returns a pointer to the file object
 */
	FILE *file;
	file = fopen(file_name, "r");
	if (file == NULL) {
		write(1, "Unable to open the text file\n", 29);
		exit(1);
	}
	return file;
}


