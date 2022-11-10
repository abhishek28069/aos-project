#include <iostream>
#include <bits/stdc++.h>
using namespace std;

struct Process
{
    // populating from input
    int PID;
    string name;
    bool cpu_bound;
    int priority;
    int arrival_time;
    vector<pair<string, int>> burst_times; // example [{"c",2},{"i",3},{"c",4}....]
    // derived variables
    string state;
    int start_time;                            // timestamp of first-time cpu allocation
    int completion_time;                       // timestamp of termination
    int waiting_time;                          // turnaround time - cpu_time
    int turn_around_time;                      // completion time - arrival time
    int response_time;                         // start time - arrival time
    vector<pair<string, int>> remaining_times; // tracking the remainging burst_times
    // constructor
    Process(int _PID, string _name, bool _cpu_bound, int _priority, int _arrival_time, vector<pair<string, int>> _burst_times)
    {
        PID = _PID;
        name = _name;
        cpu_bound = _cpu_bound;
        priority = _priority;
        arrival_time = _arrival_time;
        burst_times = _burst_times;
        remaining_times = _burst_times;
    }
};

void set_waiting(Process P)
{
    P.state = "WAITING";
}
void set_running(Process P)
{
    P.state = "RUNNING";
}
void set_ready(Process P)
{
    P.state = "READY";
}
void set_terminated(Process P)
{
    P.state = "TERMINATED";
}

int main()
{

    return 0;
}