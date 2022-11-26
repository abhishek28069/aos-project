#include <bits/stdc++.h>
using namespace std;

// PID,name,type,Priority,AT,BT

struct Process
{
    // populating from input
    int PID;
    string name;
    bool cpu_bound;
    int priority;
    int arrival_time;
    int burst_time;
    // derived variables
    int start_time;       // timestamp of first-time cpu allocation
    int completion_time;  // timestamp of termination
    int waiting_time;     // turnaround time - cpu_time
    int turn_around_time; // completion time - arrival time
    int response_time;    // start time - arrival time
};

struct cmPtq
{
    bool operator()(struct Process &x, struct Process &y)
    {

        return (y.burst_time < x.burst_time);
    }
};

struct cmpArv
{
    bool operator()(struct Process &x, struct Process &y)
    {

        return (x.arrival_time < y.arrival_time);
    }
};

struct cmpId
{
    bool operator()(struct Process &x, struct Process &y)
    {

        return (x.PID < y.PID);
    }
};

priority_queue<struct Process, vector<struct Process>, cmPtq> Cpt;
priority_queue<struct Process, vector<struct Process>, cmPtq> IOpt;
map<int, int> Arrival;
map<int, pair<bool, int>> Vis;

void ProcADD(int &cur_time, vector<struct Process> &CPU_Proc)
{
    if (CPU_Proc.size() == 0)
        return;
    if (Cpt.empty() && cur_time < CPU_Proc[0].arrival_time)
        cur_time = CPU_Proc[0].arrival_time;
    int idx = 0, siz = CPU_Proc.size();

    while (idx < siz && cur_time >= CPU_Proc[idx].arrival_time)
    {
        Cpt.push(*(CPU_Proc.begin()));

        // cout<<CPU_Proc.begin()->PID<<" ";
        CPU_Proc.erase(CPU_Proc.begin());
        idx++;
    }

    return;
}

void IoADD(int &cur_time, vector<struct Process> &IO_Proc)
{
    if (IO_Proc.size() == 0)
        return;
    if (IOpt.empty() && cur_time < IO_Proc[0].arrival_time)
        cur_time = IO_Proc[0].arrival_time;
    int idx = 0, siz = IO_Proc.size();
    while (idx < siz && cur_time >= IO_Proc[idx].arrival_time)
    {
        IOpt.push(*(IO_Proc.begin()));
        IO_Proc.erase(IO_Proc.begin());
        idx++;
    }

    return;
}

int main()
{
    int n, pd, pr, art, bt;
    bool ci;
    string PID, Priority, AT, BT, Type, Name;
    ifstream inp("ab.csv");
    struct Process obj;
    vector<struct Process> INP;
    while (inp.peek() != EOF)
    {
        getline(inp, PID, ',');
        getline(inp, Name, ',');
        getline(inp, Type, ',');
        getline(inp, Priority, ',');
        getline(inp, AT, ',');
        getline(inp, BT, '\n');
        obj.PID = (stoi(PID));
        obj.name = Name;
        obj.cpu_bound = (stoi(Type));
        obj.priority = (stoi(Priority));
        obj.arrival_time = (stoi(AT));
        obj.burst_time = (stoi(BT));
        INP.push_back(obj);
    }
    inp.close();

    // Sorting on the basis of Arrival Time
    sort(INP.begin(), INP.end(), cmpArv());

    vector<struct Process> CPU_Proc;
    vector<struct Process> IO_Proc;
    vector<struct Process> CPU_Op;
    vector<struct Process> IO_Op;
    vector<struct Process> Ans;
    int idx, cur_time = 0, entr;
    struct Process tp;
    for (idx = 0; idx < INP.size(); idx++)
    {
        if (INP[idx].cpu_bound)
            CPU_Proc.push_back(INP[idx]);
        else
        {
            IO_Proc.push_back(INP[idx]);
        }
        Arrival[INP[idx].arrival_time]++;
        Vis[INP[idx].PID] = make_pair(false, INP[idx].burst_time);
    }
    

    ProcADD(cur_time, CPU_Proc);

    while (!Cpt.empty())
    { 
        tp = Cpt.top();
        Cpt.pop();
        if (Vis[tp.PID].first == false)
        {
            tp.start_time = cur_time;
            Vis[tp.PID].first = true;
        }
        entr = 1;
        while (tp.burst_time > 0 && (Arrival[cur_time] == 0) || entr)
        {
            cur_time++;
            tp.burst_time--;
            entr = 0;
        }

        if (tp.burst_time == 0)
        {
            tp.completion_time = cur_time;
            tp.burst_time = Vis[tp.PID].second;
            tp.turn_around_time = tp.completion_time - tp.arrival_time;
            tp.response_time = tp.start_time - tp.arrival_time;
            tp.waiting_time = tp.turn_around_time - tp.burst_time;
            CPU_Op.push_back(tp);
        }
        else
        {
            Cpt.push(tp);
        }

        ProcADD(cur_time, CPU_Proc);
    }
    cout << endl;
    // code for I/O Process

    cur_time = 0;
    IoADD(cur_time, IO_Proc);
    while (!IOpt.empty())
    {
        tp = IOpt.top();
        IOpt.pop();
        if (Vis[tp.PID].first == false)
        {
            tp.start_time = cur_time;
            Vis[tp.PID].first = true;
        }
        entr = 1;
        while (tp.burst_time > 0 && (Arrival[cur_time] == 0) || entr)
        {
            cur_time++;
            tp.burst_time--;
            entr = 0;
        }

        if (tp.burst_time == 0)
        {
            tp.completion_time = cur_time;
            tp.burst_time = Vis[tp.PID].second;
            tp.turn_around_time = tp.completion_time - tp.arrival_time;
            tp.response_time = tp.start_time - tp.arrival_time;
            tp.waiting_time = tp.turn_around_time - tp.burst_time;
            IO_Op.push_back(tp);
        }
        else
        {
            IOpt.push(tp);
        }

        IoADD(cur_time, CPU_Proc);
    }

    for (idx = 0; idx < CPU_Op.size(); idx++)
        Ans.push_back(CPU_Op[idx]);

    for (idx = 0; idx < IO_Op.size(); idx++)
        Ans.push_back(IO_Op[idx]);

    sort(Ans.begin(), Ans.end(), cmpId());

    for (idx = 0; idx < Ans.size(); idx++)
    {
        cout << Ans[idx].PID << " "; //<<Ans[idx].name<<" "<<Ans[idx].cpu_bound<<" "<<Ans[idx].priority<<" "<<Ans[idx].arrival_time<<" "<<Ans[idx].burst_time<<" ";
        cout << Ans[idx].start_time << " " << Ans[idx].completion_time << " " << Ans[idx].waiting_time << " " << Ans[idx].turn_around_time << " " << Ans[idx].response_time << endl;
    }

    return 0;
}
