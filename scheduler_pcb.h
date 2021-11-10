/*
 * Author: Daiki Kubo
 * Student ID: 30523346
 * Description of the program:
 * This is a .h file that contains a data structure used to 
 * represent a process control block that has vital information for 
 * process scheduling. 
 * Start Date: 1st of October
 * Last modified: 8th of October 
 */

/* enum, used to indicate a process state */
typedef enum {
	READY = 0, RUNNING = 1, EXIT = 2
} process_state_t;


/*
 * A simplified model of process control block, which holds 
 * some vital information upon running process in the system.
 */
typedef struct {
	char process_name[11];
	int entryTime;
	int serviceTime;
	int remainingTime;
	int deadLine;
	int startTime;
	int completeTime;
	int waitTime;
	int turnaroundTime;
	int ddlMet;

	process_state_t state;
} pcb_t;
