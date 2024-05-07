#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int burstTime;
    int priority;
    int arrivalTime;
    int waitingTime;
    int turnaroundTime;
} Process;

// Utility function to sort processes by arrival time
int compareArrival(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->arrivalTime - p2->arrivalTime;
}

// Utility function to sort processes by burst time for SJF
int compareBurstTime(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->burstTime - p2->burstTime;
}

// Utility function to sort processes by priority for Priority Scheduling
int comparePriority(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->priority - p2->priority;
}

// Function to calculate waiting time and turnaround time
void calculateMetrics(Process processes[], int n) {
    int total_wt = 0, total_tat = 0;
    printf("ID\tBurst Time\tArrival Time\tPriority\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        processes[i].turnaroundTime = processes[i].burstTime + processes[i].waitingTime;
        total_wt += processes[i].waitingTime;
        total_tat += processes[i].turnaroundTime;
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].burstTime, processes[i].arrivalTime, processes[i].priority, processes[i].waitingTime, processes[i].turnaroundTime);
    }
    printf("Average Waiting Time = %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time = %.2f\n", (float)total_tat / n);
}

// First Come First Serve Algorithm
void FCFS(Process processes[], int n) {
    printf("FCFS:\n");
    qsort(processes, n, sizeof(Process), compareArrival);
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrivalTime)
            currentTime = processes[i].arrivalTime;
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        currentTime += processes[i].burstTime;
    }
    calculateMetrics(processes, n);
}

// Shortest Job First Algorithm (non-preemptive)
void SJF(Process processes[], int n) {
    printf("SJF:\n");
    qsort(processes, n, sizeof(Process), compareBurstTime);
    int completed = 0, currentTime = 0, minIndex;
    while (completed < n) {
        minIndex = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].burstTime > 0) {
                if (minIndex == -1 || processes[i].burstTime < processes[minIndex].burstTime)
                    minIndex = i;
            }
        }
        if (minIndex == -1)
            currentTime++;
        else {
            processes[minIndex].waitingTime = currentTime - processes[minIndex].arrivalTime;
            currentTime += processes[minIndex].burstTime;
            processes[minIndex].burstTime = 0;
            completed++;
        }
    }
    calculateMetrics(processes, n);
}

// Priority Scheduling Algorithm (non-preemptive)
void PriorityScheduling(Process processes[], int n) {
    printf("Priority:\n");
    qsort(processes, n, sizeof(Process), comparePriority);
    int currentTime = 0;
    for (int i = 0; i < n; i++) {
        if (currentTime < processes[i].arrivalTime)
            currentTime = processes[i].arrivalTime;
        processes[i].waitingTime = currentTime - processes[i].arrivalTime;
        currentTime += processes[i].burstTime;
    }
    calculateMetrics(processes, n);
}

// Round Robin Algorithm
void RoundRobin(Process processes[], int n, int quantum) {
    printf("Round Robin:\n");
    qsort(processes, n, sizeof(Process), compareArrival);
    int rem_bt[n];
    for (int i = 0; i < n; i++) {
        rem_bt[i] = processes[i].burstTime;
    }
    int t = 0;
    while (1) {
        int done = 1;
        for (int i = 0; i < n; i++) {
            if (rem_bt[i] > 0) {
                done = 0;
                if (rem_bt[i] > quantum) {
                    t += quantum;
                    rem_bt[i] -= quantum;
                } else {
                    t += rem_bt[i];
                    processes[i].waitingTime = t - processes[i].burstTime - processes[i].arrivalTime;
                    rem_bt[i] = 0;
                }
            }
        }
        if (done == 1)
            break;
    }
    calculateMetrics(processes, n);
}

int main() {
    Process processes[] = {{1, 6, 2, 1}, {2, 8, 3, 1}, {3, 7, 1, 2}, {4, 3, 4, 3}};
    int n = sizeof(processes) / sizeof(processes[0]);
    int quantum = 4;
    FCFS(processes, n);
    SJF(processes, n);
    PriorityScheduling(processes, n);
    RoundRobin(processes, n, quantum);
    return 0;
}
