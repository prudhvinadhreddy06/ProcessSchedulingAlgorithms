#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;



class Process{
public:
    long long int PID;      //
    string Process_name;    //
    char C_I_bound;         //
    long long int Priority;     //
    long long int Arrival_time;     //
    long long int Arrival_time_dup;     //
    long long int Burst_time_1;     //
    long long int IO_time;     //
    long long int Burst_time_2;     //
    int no_CPU_execs;               //no. of times executed by CPU 

    string Process_state;           //
    long long int Start_time;       
    long long int Completion_time;
    long long int Waiting_time;
    long long int Turn_around_time;    
    long long int Response_time;

    long long int Burst_time_2_or;
    long long int Burst_time_1_or;  
    bool set_s_time=false;

    Process(long long int pid, string p_name, char ci_bound, long long int priority, long long int at, long long int bt1, long long int io, long long int bt2){
        this->PID = pid;
        this->Process_name = p_name;
        this->C_I_bound = ci_bound;
        this->Priority = priority;
        this->Arrival_time = at;
        this->Arrival_time_dup = at;
        this->Burst_time_1 = bt1;
        this->IO_time = io;
        this->Burst_time_2 = bt2;
        this->no_CPU_execs = 0;

        this->Process_state = "NEW";
        this->Start_time = -1;          //default garbage values
        this->Completion_time = -1;     
        this->Waiting_time = -1;            //time spent in ready queue
        this->Turn_around_time = -1;        //Completion time - arrival time
        this->Response_time = -1;           //Start time - arrival time

        this->Burst_time_1_or = bt1;
        this->Burst_time_2_or = bt2;
    }


    void set_to_ready_state(){                  //
        this->Process_state = "READY";
    }

    void set_to_running_state(){                //
        this->Process_state = "RUNNING";
    }

    void set_to_terminated_state(){             //
        this->Process_state = "TERMINATED";
    }

    void set_to_waiting_state(){                //waiting for IO
        this->Process_state = "WAITING";
    }

    void set_start_time(long long int st){      //
        this->Start_time = st;
    }


    void set_completion_time(long long int ct){     //
        this->Completion_time = ct;
    }


    void compute_times(){
        this->Response_time = Start_time - Arrival_time;
        this->Turn_around_time = Completion_time - Arrival_time;
        this->Waiting_time =  Turn_around_time - Burst_time_1_or - Burst_time_2_or-IO_time;
    }

    void set_burst_time(long long int time)
    {
        if(this->no_CPU_execs==0)
        {
            this->Burst_time_1=time;
        }
        else if(this->no_CPU_execs==1)
        {
            this->Burst_time_2=time;
        }
    }

};


bool operator<(const Process& p1, const Process& p2)
{
    if(p1.Arrival_time_dup == p2.Arrival_time_dup)
        return p1.PID > p2.PID;      //PID
    else 
        return p1.Arrival_time_dup > p2.Arrival_time_dup;
}


class CPUScheduler_RR
{
public:

    long long int clock;
    deque<Process> rr_queue, terminated_q;;
    priority_queue<Process> queue;
    vector<string> gant_chart;
    int time_quantun;



    CPUScheduler_RR()
    {
        clock = 0;
        time_quantun=0;
    }

    //set clock to 0 initially and take input of processes while invoking the constructor
    CPUScheduler_RR(vector<Process>& process_list,int time){
        clock = 0;
        time_quantun=time;
        for(auto e : process_list){
            e.set_to_ready_state();
            queue.push(e);
        }

    }

    

    void preempt(Process curr_proc)
    {
        while (!queue.empty())
                {
                    Process temp = queue.top();
                    if (temp.Arrival_time_dup <= clock)
                    {
                        rr_queue.push_back(temp);
                         queue.pop();
                    } 
                    else
                    break;
                }
        rr_queue.push_back(curr_proc);
    }

    void terminate(Process& curr_proc){                      //
        //post-processing
        ////cout<<curr_proc.PID<<"  terminated called at "<<clock <<endl;
        curr_proc.set_to_terminated_state();
        curr_proc.Completion_time = clock;
        curr_proc.compute_times();
        terminated_q.push_back(curr_proc);

    }

    void context_switch(){
    }

    void wake_up(Process& curr_proc, long long int wakeup_time){         //
        curr_proc.Arrival_time_dup = wakeup_time;
        if(curr_proc.Burst_time_2!=0)
        {
            queue.push(curr_proc);
            curr_proc.set_to_ready_state();
        }
        else
        {
            ////cout<<"termination in wake up";
            terminate(curr_proc);
        }
    }

    void yield(Process& curr_proc){                                      //
        long long int io_time = curr_proc.IO_time;
        curr_proc.set_to_waiting_state();
        wake_up(curr_proc, clock + io_time);

    }

    void run(Process& curr_proc){   
        ////cout<<"in run"<<curr_proc.PID<<" "<<curr_proc.Burst_time_1<<" "<<curr_proc.Burst_time_2<<endl;                                     //
        curr_proc.set_to_running_state();

        long long int pid = curr_proc.PID;
        long long int remTime = curr_proc.no_CPU_execs == 0 ? curr_proc.Burst_time_1 : curr_proc.Burst_time_2;
        if(curr_proc.no_CPU_execs == 0 && !curr_proc.set_s_time){
            curr_proc.set_start_time(clock);
            curr_proc.set_s_time=true;
        }
        if (remTime >time_quantun)
        {
            int start_clock=clock;
            clock+=time_quantun;
            remTime-=time_quantun;
            curr_proc.set_burst_time(remTime);

            string s = "At t = " + to_string(start_clock) + " - " + to_string(clock) + "; Process " + to_string(pid) +  " is executing in CPU.";
            gant_chart.push_back(s);
            preempt(curr_proc);
        }     
        else
        {
            clock+=remTime;
            string s = "At t = " + to_string(clock-remTime) + " - " + to_string(clock) + "; Process " + to_string(pid) +  " is executing in CPU.";
            gant_chart.push_back(s);
            if(curr_proc.no_CPU_execs==0)
            {
                curr_proc.set_burst_time(0);
                curr_proc.no_CPU_execs++;
                yield(curr_proc);
            }
            else if(curr_proc.no_CPU_execs==1)
            {
                curr_proc.set_burst_time(0);
                ////cout<<"terminate at end";
                terminate(curr_proc);
            }
        }
        context_switch();
        return;
    }

    void idle(){
        //CPU is idle currently for this 1 sec interval
        string s = "At t = " + to_string(clock) + " - " + to_string(clock+1) + "; No Process in Ready Queue";
        gant_chart.push_back(s);

        return ;
    }

    void schedule()
    {

        while(!(rr_queue.empty() && queue.empty()))
        {
                while (!queue.empty())
                {
                    Process temp = queue.top();
                    if (temp.Arrival_time_dup <= clock)
                    {
                        rr_queue.push_back(temp);
                         queue.pop();
                    } 
                    else
                    break;
                }
            if(rr_queue.empty() && !queue.empty())
            {
                idle();
                clock++;
            }
            if(!rr_queue.empty())
            {
                Process cur = rr_queue.front();
                rr_queue.pop_front();
                run(cur);
            }
        

        }
        
        //done executing all the processes, post-processing now
        string s = "At t = " + to_string(clock) + "; All processes are done executing";
        gant_chart.push_back(s);

    }
};

int main(){
    //freopen("output_rr.txt", "w", stdout);
    ifstream file("input3.txt");

    string line;
    vector<Process> processes;
    int i=0;
    while(getline(file,line)){
        stringstream ss(line);
        //change this
        // string pid,pr,at,bt;
        string pid, p_name, ci_bound, priority, at, bt1, io, bt2;
        getline(ss, pid, ' ');
        getline(ss, p_name, ' ');
        getline(ss, ci_bound, ' ');
        getline(ss, priority, ' ');
        getline(ss, at, ' ');
        getline(ss, bt1, ' ');
        getline(ss, io, ' ');
        getline(ss, bt2, ' ');
        // //cout << stoi(pid) << " " << p_name << " " << ci_bound << " " << stoi(priority) << stoi(at) << stoi(bt1) << "\n";
        Process curr_proc(stoll(pid), p_name, ci_bound[0], stoll(priority), stoll(at), stoll(bt1), stoll(io), stoll(bt2));
        processes.push_back(curr_proc);
        ////cout<<processes[i].getPid()<<" "<<processes[i].priority<<" "<<processes[i].at<<" "<<processes[i].bt<<endl;
        i++;
    }

    //create fcfs_scheduler object and pass list of all processes to it for constructor 
    CPUScheduler_RR RR_scheduler(processes,3);
    
    //invoke fcfs scheduler
    RR_scheduler.schedule();

    //print gant chart
    for(auto s : RR_scheduler.gant_chart){
        //cout << s << "\n";
    }
    //cout << "\n";

    //print process attributes in table format
    // //cout << "PID" << "\t" << "Process_name" << "\t" << "C_I_bound" << "\t";
    // //cout << "Priority" << "\t" << "Arrival_time" << "\t" << "Burst_time1" << "\t" << "IO_Time" << "\t" << "Burst_Time2" << "\n";
    // for(auto p : fcfs_scheduler.terminated_q){
    //     //cout << p.PID << "\t" << p.Process_name << "\t" << p.C_I_bound << "\t";
    //     //cout << p.Priority << "\t" << p.Arrival_time << "\t" << p.Burst_time_1 << "\t" << p.IO_time << "\t" <<
    //     p.Burst_time_2 << "\n";
    // }



vector<Process> completedProcessesVector;
//cout << setw(10) << "id" << setw(10) << "name" << setw(10) << "arrival" << setw(10) << "burst1" << setw(10) << "io" << setw(10) << "burst2" << setw(10) << "start" << setw(10) << "resp" << setw(10) << "compl" << setw(10) << "turn" << setw(10) << "wait";
    //cout << "\n";


    for(auto p : RR_scheduler.terminated_q){
        //cout << setw(10) << p.PID << setw(10) << p.Process_name << setw(10) << p.Arrival_time << setw(10) << p.Burst_time_1_or << setw(10) << p.IO_time << setw(10) << p.Burst_time_2_or << setw(10) << p.Start_time << setw(10) << p.Response_time << setw(10) << p.Completion_time << setw(10) << p.Turn_around_time << setw(10) << p.Waiting_time << endl;
    completedProcessesVector.push_back(p);
    }


float completionTime[completedProcessesVector.size()];

float TAT[completedProcessesVector.size()];

float waitingTime[completedProcessesVector.size()];

float responseTime[completedProcessesVector.size()];

float avgTAT = 0;

float avgWT = 0;

float avgRT = 0;

float avgCT = 0;

for(int i=0;i<completedProcessesVector.size();i++){

completionTime[i] = completedProcessesVector[i].Completion_time;

TAT[i] = completedProcessesVector[i].Turn_around_time;

waitingTime[i] = completedProcessesVector[i].Waiting_time;

responseTime[i] = completedProcessesVector[i].Response_time;

avgTAT += TAT[i];

avgWT += waitingTime[i];

avgRT += responseTime[i];

avgCT += completionTime[i];

}

avgTAT = avgTAT/completedProcessesVector.size();

avgWT = avgWT/completedProcessesVector.size();

avgRT = avgRT/completedProcessesVector.size();

avgCT = avgCT/completedProcessesVector.size();

//store the array of completion time, turn around time, waiting time, response time int his order of each process in out.csv file

ofstream out("out.csv");

//out<<"completion time"<<","<<"turn around time"<<","<<"waiting time"<<","<<"response time"<<endl;

for(int i=0;i<completedProcessesVector.size();i++){

out<<completionTime[i]<<","<<TAT[i]<<","<<waitingTime[i]<<","<<responseTime[i]<<","<<completedProcessesVector[i].PID<<endl;

}

out.close();



//store the average of completion time, turn around time, waiting time, response time int his order of each process in all.csv file dont overwrite the file just append the data

ofstream all("all.csv",ios::app);

all<<avgCT<<","<<avgTAT<<","<<avgWT<<","<<avgRT<<endl;

all.close();
    system("python3 graph.py RRPr.jpeg RR");
	system("python3 graph1.py RRPie.jpeg RR");
    system("python3 stack.py RRStack.jpeg RR");
    remove("out.csv");
    return 0;
}
