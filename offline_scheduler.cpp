#include <iostream>
#include <queue> 
#include <unistd.h>
#include <sys/wait.h>
/*
 * COSC 3360
 * Arya Haji Taheri
 */
using namespace std;
//struct containing relevant information for each process.
struct  offline_scheduler {
	int exectime = -1,
		priority = -1,
		PID = 1,
		split_process = 0;
	offline_scheduler(int e, int p, int id, int sp) : exectime(e), priority(p), PID(id), split_process(sp) {
	}
	offline_scheduler() {}
};
// compare and sort by priority.
// If same priority, move the splitted executed time to the end of the queue.
struct Compare_Priority {
	bool operator()(offline_scheduler const& p1, offline_scheduler const& p2) {
		return(p1.priority > p2.priority || ((p1.priority == p2.priority) && (p1.exectime < p2.exectime)));
	}
};
int main()
{
	pid_t pid;
	offline_scheduler scheduler_struct;
	std::priority_queue< offline_scheduler, vector< offline_scheduler>, Compare_Priority> pqueue; //	queue for our processes
	int quantum = 0, PID = 0, exectime = 0, priority = 0;

	cin >> quantum;
	cout << "Quantum = " << quantum << endl;

	while (cin >> exectime >> priority) // read info from text.
	{	//split process if exectime > quantom
		if (exectime > quantum) {
			exectime -= quantum;
			pqueue.push(offline_scheduler(quantum, priority, PID, 0));
			pqueue.push(offline_scheduler(exectime, priority, PID, 1));
		}
		else {
			pqueue.push(offline_scheduler(exectime, priority, PID, 1));
		}
		PID++;
	}
	std::priority_queue< offline_scheduler, vector< offline_scheduler>, Compare_Priority> pqueue_copy = pqueue; // copy queue to display scheduling.

	cout << "Scheduling queue: (Process, Exec-time, Priority):\n\n\t";

	//display
	while (!pqueue_copy.empty()) {
		scheduler_struct = pqueue_copy.top();
		cout << "(" << scheduler_struct.PID << "," << scheduler_struct.exectime << "," << scheduler_struct.priority << ") ";
		pqueue_copy.pop();
	} std::cout << "\n\n";

	while (!pqueue.empty()) {
		scheduler_struct = pqueue.top();
		pqueue.pop();
		pid = fork();

		if (pid == 0) { 		// child process

			cout << "Process: " << scheduler_struct.PID << " Exec time = " << scheduler_struct.exectime << ", Priority = " << scheduler_struct.priority << endl;
			sleep(scheduler_struct.exectime); 
			if (scheduler_struct.split_process == 1) { // if last process execution, print it has ended. 
				cout << "Process: " << scheduler_struct.PID << " ends.\n";
			}
			exit(0);
		}
		if (pid != 0) { 		// parent process
			wait(NULL);
		}
	}

	if (pid != 0) {	// parent process
		return 0;
	}
	return -1;
}
