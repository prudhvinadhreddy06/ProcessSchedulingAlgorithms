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
        this->Waiting_time =  Turn_around_time - Burst_time_1 - Burst_time_2 - IO_time;
    }

};





bool operator<(const Process& p1, const Process& p2)
{
    if(p1.Arrival_time_dup == p2.Arrival_time_dup)
        return p1.PID > p2.PID;      //PID
    else 
        return p1.Arrival_time_dup > p2.Arrival_time_dup;
}

class CPUScheduler{
public:

    long long int clock;
    vector<Process> new_q, running_q, waiting_q, terminated_q;
    priority_queue<Process> ready_queue;  //min_heap based on arrival times
    vector<string> gant_chart;


    CPUScheduler(){
        clock = 0;
    }

    //set clock to 0 initially and take input of processes while invoking the constructor
    CPUScheduler(vector<Process>& process_list){
        clock = 0;
        for(auto e : process_list){
            e.set_to_ready_state();
            ready_queue.push(e);
        }

    }

    
    //not required for FCFS
    void preempt(){

    }


    //post-processing
    void terminate(Process& curr_proc){
        curr_proc.set_to_terminated_state();
        curr_proc.Completion_time = clock;
        curr_proc.compute_times();
        terminated_q.push_back(curr_proc);
    }


    //once IO is done, wake up from waiting queue and move to ready queue
    void wake_up(Process& curr_proc, long long int wakeup_time){         //
        curr_proc.Arrival_time_dup = wakeup_time;
        ready_queue.push(curr_proc);
        curr_proc.set_to_ready_state();
    }


    //Move to IO from CPU
    void yield(Process& curr_proc){                                      //
        long long int io_time = curr_proc.IO_time;
        curr_proc.set_to_waiting_state();
        wake_up(curr_proc, clock + io_time);
    }


    //Process is assigned to CPU
    void run(Process& curr_proc){                                        //
        curr_proc.set_to_running_state();

        long long int pid = curr_proc.PID;
        long long int remTime = curr_proc.no_CPU_execs == 0 ? curr_proc.Burst_time_1 : curr_proc.Burst_time_2;
        if(curr_proc.no_CPU_execs == 0){
            curr_proc.set_start_time(clock);
        }

        
        while(remTime > 0){
            string s = "At t = " + to_string(clock) + " - " + to_string(clock+1) + "; Process " + to_string(pid) +  " is executing in CPU.";
            gant_chart.push_back(s);
            remTime--;
            clock++;
        }
        curr_proc.no_CPU_execs++;            //increment number of times executed

        //if num_Execs == 1 => IO is left
        if(curr_proc.no_CPU_execs == 1){
            //send it to IO and push into ready queue again

            yield(curr_proc);
        }
        else if(curr_proc.no_CPU_execs == 2){
            //Process is done BT1, IO1, BT2 => push into terminated state

            terminate(curr_proc);
        }

        //done executing current process, return to schedule()
        return ;
    }


    void idle(){
        //CPU is idle currently for this 1 sec interval
        string s = "At t = " + to_string(clock) + " - " + to_string(clock+1) + "; No Process in Ready Queue";
        gant_chart.push_back(s);

        return ;
    }

    //FCFS scheduler
    void schedule(){

        //check if ready queue has any process, if yes, schedule it based on FCFS
        while(!ready_queue.empty()){
            Process curr_proc = ready_queue.top(); ready_queue.pop();
            long long int curr_proc_at = curr_proc.Arrival_time_dup;

            while(clock < curr_proc_at){
                //invoke idle() as no process in ready queue
                idle();
                clock++;
            }

            run(curr_proc);
        }

        //done executing all the processes, post-processing now
        string s = "At t = " + to_string(clock) + "; All processes are done executing";
        gant_chart.push_back(s);

    }


};


int main(){
    //freopen("output_fcfs.txt", "w", stdout);
    //ifstream file("input_1.txt");
    ifstream file("input3.txt");

    string line;
    vector<Process> processes;
    int i=0;
    while(getline(file,line)){
        stringstream ss(line);
        
        string pid, p_name, ci_bound, priority, at, bt1, io, bt2;
        getline(ss, pid, ' ');
        getline(ss, p_name, ' ');
        getline(ss, ci_bound, ' ');
        getline(ss, priority, ' ');
        getline(ss, at, ' ');
        getline(ss, bt1, ' ');
        getline(ss, io, ' ');
        getline(ss, bt2, ' ');
        
        Process curr_proc(stoll(pid), p_name, ci_bound[0], stoll(priority), stoll(at), stoll(bt1), stoll(io), stoll(bt2));
        processes.push_back(curr_proc);
        
        i++;
    }

    //create fcfs_scheduler object and pass list of all processes to it for constructor 
    CPUScheduler fcfs_scheduler(processes);
    
    //invoke fcfs scheduler
    fcfs_scheduler.schedule();

    //print gant chart
    // for(auto s : fcfs_scheduler.gant_chart){
    //     cout << s << "\n";
    // }
    // cout << "\n";


    // cout << setw(10) << "id" << setw(10) << "name" << setw(10) << "arrival" << setw(10) << "burst1" << setw(10) << "io" << setw(10) << "burst2" << setw(10) << "start" << setw(10) << "resp" << setw(10) << "compl" << setw(10) << "turn" << setw(10) << "wait";
    // cout << "\n";

    // for(auto p : fcfs_scheduler.terminated_q){
    //     cout << setw(10) << p.PID << setw(10) << p.Process_name << setw(10) << p.Arrival_time << setw(10) << p.Burst_time_1 << setw(10) << p.IO_time << setw(10) << p.Burst_time_2 << setw(10) << p.Start_time << setw(10) << p.Response_time << setw(10) << p.Completion_time << setw(10) << p.Turn_around_time << setw(10) << p.Waiting_time << endl;
    // }


    //code for graph plotting
    vector<Process> completedProcessesVector = fcfs_scheduler.terminated_q;
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

    system("python3 graph.py FCFS.jpeg FCFS");
	system("python3 graph1.py FCFSPie.jpeg FCFS");
    system("python3 stack.py FCFSstack.jpeg FCFS");
    remove("out.csv");

    //store the average of completion time, turn around time, waiting time, response time int his order of each process in all.csv file dont overwrite the file just append the data
    ofstream all("all.csv",ios::app);
    all<<"\n"<<avgCT<<","<<avgTAT<<","<<avgWT<<","<<avgRT<<endl;
    all.close();

    return 0;
}


//comment out gant chart output
//graph for num of context switches