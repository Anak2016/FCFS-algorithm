#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

class process
{
public:
	string name;
	int CPUBurstAndIO[100];
	int currentCPUBurst;
	int currentIO;
	string currentState; //Waiting, Executing, IO, Completed
	int responseTime;
	int waitTime;
	int turnAroundTime;
	process *previous;
	process *next;
};

class Algorithm
{
public:
	Algorithm();
	void readyQueue(process initial[], int elements);
	process sendToIO(process current);
	process run(process current, int time);
	process current();
	process last();
	void print(process current, int time);
	void printReadyQueue();
	void printIOQueue();
	void deQueue();
	void deQueueSpecific(process transfer);
	void adjustQueue(int IO);
	void readyQueuePrint();
	void IOQueuePrint();
	bool isEmpty();
	void reduce();
	bool transfer();
	process getFinishIO();
	int getBurst();
	friend bool isCompleted(process transfer);
	bool hasOne();
	
private:
	process *front;
	process *back;
};

Algorithm::Algorithm() {
	front = 0;
	back = 0;
}

void Algorithm::readyQueue(process readyQueue[],int elements) {
	/*process next = readyQueue[0];
	return next;*/
	int i=0;
	int array[] = { 1,2,3,4,5,6,7,8 };
	int num2 = sizeof(array) / sizeof(*array);
	int num = sizeof(readyQueue)/sizeof(*readyQueue);
	process *created;
	do{
		if (front == 0) {
			created = new process;
			created->name = readyQueue[i].name;
			for (int j = 0; readyQueue[i].CPUBurstAndIO[j]!=0; j++) {
				created->CPUBurstAndIO[j] = readyQueue[i].CPUBurstAndIO[j];
			}
			created->currentCPUBurst = readyQueue[i].currentCPUBurst;
			created->currentIO = readyQueue[i].currentIO;
			created->currentState = readyQueue[i].currentState;
			created->responseTime = readyQueue[i].responseTime;
			created->waitTime = readyQueue[i].waitTime;
			created->turnAroundTime = readyQueue[i].turnAroundTime;
			front = back = created;
			i++;
		}

		else {
			created = new process;
			back->next = created;
			created->previous = back;
			back = created;
			created->name = readyQueue[i].name;
			for (int j = 0; readyQueue[i].CPUBurstAndIO[j]!=0; j++) {
				created->CPUBurstAndIO[j] = readyQueue[i].CPUBurstAndIO[j];
			}
			created->currentCPUBurst = readyQueue[i].currentCPUBurst;
			created->currentIO = readyQueue[i].currentIO;
			created->currentState = readyQueue[i].currentState;
			created->responseTime = readyQueue[i].responseTime;
			created->waitTime = readyQueue[i].waitTime;
			created->turnAroundTime = readyQueue[i].turnAroundTime;
			i++;
		}
	} while (i < elements);
}

void Algorithm::deQueue() {
	if (front == 0) {
		return;
	}
	else if (front->next == 0) {
		delete front;
		front = 0;
		back = 0;
	}
	else {
		process *temp = front;
		front = front->next;
		front->previous = 0;
		delete temp;
	}
}

process Algorithm::current() {
	process IDLE{ "IDLE"};
	if (front == 0) {
		return IDLE;
	}
	process *p = front;
	return *p;
}

process Algorithm::last() {
	process *p = back;
	return *p;
}

process Algorithm::run(process current, int time) {
	process *ran = front;
	process IO = sendToIO(*ran);
	int CPUBurst = current.CPUBurstAndIO[0];
	for (int i = current.CPUBurstAndIO[0]; i >= 0; i--) {
		if (time == 0) {
			break;
		}
		else {
			time++;
		}
	}
	print(current, time);
	return *ran;
}

process Algorithm::sendToIO(process current) {
	process *created;
	if (front == 0) {
		created = new process;
		created->name = current.name;
		for (int j = 0; current.CPUBurstAndIO[j] != 0; j++) {
			created->CPUBurstAndIO[j] = current.CPUBurstAndIO[j];
		}
		created->currentCPUBurst = current.currentCPUBurst;
		created->currentIO = current.currentIO;
		created->currentState = current.currentState;
		created->responseTime = current.responseTime;
		created->waitTime = current.waitTime;
		created->turnAroundTime = current.turnAroundTime;
		front = back = created;
	}
	else {
		created = new process;
		back->next = created;
		created->previous = back;
		back = created;
		created->name = current.name;
		for (int j = 0; current.CPUBurstAndIO[j] != 0; j++) {
			created->CPUBurstAndIO[j] = current.CPUBurstAndIO[j];
		}
		created->currentCPUBurst = current.currentCPUBurst;
		created->currentIO = current.currentIO;
		created->currentState = current.currentState;
		created->responseTime = current.responseTime;
		created->waitTime = current.waitTime;
		created->turnAroundTime = current.turnAroundTime;
	}
	return *created;
}

void Algorithm::adjustQueue(int IO) {
	process *point = back;
	point->currentState = "IO";
	point->currentIO = IO;
	point->currentCPUBurst = point->currentCPUBurst + 2;
}

void Algorithm::print(process current, int time) {
	cout << "Current Time: "<< time << endl;
	cout << "Now Running: " << current.name << endl;
	cout << "------------------------------------" << endl;
	}

void Algorithm::printReadyQueue() {
		cout << "Ready Queue: Process Burst: " << endl;
		readyQueuePrint();
		cout << endl;
		cout << "------------------------------------" << endl;

	}

void Algorithm::printIOQueue() {
	cout << "Now in I/O:  Process  Remaining I/O Time" << endl;
	IOQueuePrint();
	cout << endl;
	cout << "*********************************************************" << endl;
}

void Algorithm::readyQueuePrint() {
	process *i = front;
	int p = 0;
	if (front == 0) {
		cout << "             [Empty]" << endl;
	}
	else {
		for (i = front; i != back->next; i = i->next) {
			cout << "               " << i->name << "      " << i->CPUBurstAndIO[i->currentCPUBurst] << endl;
		}
	}
}

void Algorithm::IOQueuePrint() {
	process *i = front;
	int p = 0;
	if (front == 0) {
		cout << "             [Empty]" << endl;
	}
	else {
		for (i = front; i != back->next; i = i->next) {
			cout << "               " << i->name << "      " << i->currentIO << endl;
		}
	}
}

void Algorithm::reduce() {
	if (front == 0) {
		return;
	}
	else if (front->next == 0) {
		front->currentIO--;
	}
	else {
		for (process *p = front; p != 0; p = p->next) {
			p->currentIO--;
		}
	}
}

bool Algorithm::isEmpty() {
	if (front == 0) {
		return true;
	}
	else {
		return false;
	}
}

bool Algorithm::transfer() {
	 process *p = front;
	if (isEmpty()==false) {
		for (p = front; p != 0; p = p->next) {
			if (p->currentIO == 0) {
				return true;
			}
		}
		return false;
	}
	return false;
}

process Algorithm::getFinishIO() {
	for (process *p = front; p != 0; p = p->next) {
		if (p->currentIO == 0) {
			return *p;
		}
	}
}

int Algorithm::getBurst() {
	int p = 0;
	return p;
}

void Algorithm::deQueueSpecific(process transfer) {
	process *p = front;
	if (front == 0) {
		return;
	}
	else if (front->next == 0) {
		delete p;
		front = 0;
	}
	else {
		for (p = front; p != 0; p = p->next) {
			if (p==front && p->name == transfer.name) {
				process *q = p->next;
				q->previous = 0;
				front = q;
				delete p;
				p = q;
				break;
			}
			else if (p==back && p->name == transfer.name) {
				process *q = p->previous;
				q->next = 0;
				back = q;
				delete p;
				p = q;
				break;
			}
			else if(p->name==transfer.name){//problem here
				process *q = p->previous;
				q->next = p->next;
				q->next->previous = q;
				delete p;
				p = q;
				break;
			}
		}
	}
	//p->name == transfer.name

	/*process *temp = front;
	front = front->next;
	delete temp;*/
}

bool isCompleted(process transfer) {
	if (transfer.CPUBurstAndIO[transfer.currentCPUBurst + 1] == 0) {
		return true;
	}
	return false;
}

bool Algorithm::hasOne() {
	if (front != 0 && front->next == 0) {
		return true;
	}
	return false;
}


int main() {//Ready Queue is a linked list

	Algorithm ReadyQueue;
	Algorithm IOQueue;
	process IO;
	int time = 0;
	process p1{ "P1",{ 4, 15, 5, 31, 6, 26, 7, 24, 6, 41, 4, 51, 5, 16, 4 }, 0, 0, "Waiting" };
	process p2{ "P2",{ 9, 28, 11, 22, 15, 21, 12, 28, 8, 34, 11, 34, 9, 29, 10, 31, 7 }, 0, 0, "Waiting" };
	process p3{ "P3",{ 24, 28, 12, 21, 6, 27, 17, 21, 11, 54, 22, 31, 18 }, 0, 0, "Waiting" };
	process	p4{ "P4",{ 15, 35, 14, 41, 16, 45, 18, 51, 14, 61, 13, 54, 16, 61, 15 }, 0, 0, "Waiting" };
	process p5{ "P5",{ 6, 22, 5, 21, 15, 31, 4, 26, 7, 31, 4, 18, 6, 21, 10, 33, 3 }, 0, 0, "Waiting" };
	process p6{ "P6",{ 22, 38, 27, 41, 25, 29, 11, 26, 19, 32, 18, 22, 6, 26, 6 }, 0, 0, "Waiting" };
	process p7{ "P7",{ 4, 36, 7, 31, 6, 32, 5, 41, 4, 42, 7, 39, 6, 33, 5, 34, 6, 21, 9 }, 0, 0, "Waiting" };
	process p8{ "P8",{ 5, 14, 4, 33, 6, 31, 4, 31, 6, 27, 5, 21, 4, 19, 6, 11, 6 }, 0, 0, "Waiting" };
	
	process initial[] = { p1, p2, p3, p4, p5, p6, p7, p8 };
	int elements = sizeof(initial) / sizeof(*initial);
	ReadyQueue.readyQueue(initial, elements);

	process now = ReadyQueue.current();
	process previous = ReadyQueue.current();
	process transfer;

	int j = 0;
	int endTime = 0;
	while (ReadyQueue.isEmpty()!=true || IOQueue.isEmpty()!=true) {
		for (int i = ReadyQueue.current().CPUBurstAndIO[j]; i >= 0; i--) {
			if (time == 0) {
				endTime = ReadyQueue.current().CPUBurstAndIO[j];
				previous = ReadyQueue.current();
				ReadyQueue.deQueue();
				ReadyQueue.print(now, time);
				ReadyQueue.printReadyQueue();
				IOQueue.printIOQueue();
				now = ReadyQueue.current();
				time++;
			}
			else {
				if (time == endTime) {
					if (isCompleted(previous) == false) {
						IOQueue.sendToIO(previous);
						j = IOQueue.last().currentCPUBurst;
						IOQueue.adjustQueue(IOQueue.last().CPUBurstAndIO[j + 1]);
					}
					now = ReadyQueue.current();//returns null
					ReadyQueue.print(now, time);
					previous = ReadyQueue.current();
					ReadyQueue.deQueue();
					ReadyQueue.printReadyQueue();
					IOQueue.printIOQueue();
					endTime = time + now.CPUBurstAndIO[now.currentCPUBurst];
					i = now.CPUBurstAndIO[now.currentCPUBurst];
				}
				time++;
				IOQueue.reduce();
				for (;;) {
					if (IOQueue.transfer() == true) {
						transfer = IOQueue.getFinishIO();
						IOQueue.deQueueSpecific(transfer);
						ReadyQueue.sendToIO(transfer);//same as sendToReadyQueue
						if (ReadyQueue.hasOne()&& i==0) {
							if (now.name == "IDLE") {
								endTime = time;
							}
							else {
								endTime = time + transfer.CPUBurstAndIO[transfer.currentCPUBurst];
							}	
						}
					}
					else {
						break;
					}
				}
			}
		}
	}
	cout << "DONE!" << endl << endl;
	return 0;
}