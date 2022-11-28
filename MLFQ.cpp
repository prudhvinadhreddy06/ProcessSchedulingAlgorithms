#include<bits/stdc++.h>

#include<unistd.h>
using namespace std;
int timer=0;
class Process{
public:
    int at;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
    int priority;
    int processId;
    int remainingTime;
    int responseTime;
    bool isCompleted;
    bool isStarted;
    bool isArrived;
    int bt1;
    int bt2;
    int i_o;

    Process(int processId, int priority,int arrivalTime, int bt1, int bt2, int i_o){
        this->at=arrivalTime;
        this->arrivalTime = arrivalTime;
        this->burstTime = bt1+bt2;
        this->priority = priority;
        this->processId = processId;
        this->isCompleted = false;
        this->isStarted = false;
        this->remainingTime = bt1;
        waitingTime=0;
        turnAroundTime=0;
        responseTime=0;
        completionTime=0;
        isArrived=false;
        this->bt1=bt1;
        this->bt2=bt2;
        this->i_o=i_o;
    }

};

void addProcessesToArrivedQueue(vector<Process> &processes, queue<Process> &arrivedProcesses, int timer){
    for(int i=0;i<processes.size();i++){
        if(!processes[i].isArrived&&processes[i].arrivalTime <= timer){
            arrivedProcesses.push(processes[i]);
            processes[i].isArrived=true;
        }
    }
}

void addProcessesToreadyQueue(queue<Process> &arrivedProcesses, queue<Process> &queue0,queue<Process> &queue1,queue<Process> &queue2, int timer){
    while(!arrivedProcesses.empty()&&arrivedProcesses.front().arrivalTime<=timer){
        Process p = arrivedProcesses.front();
        arrivedProcesses.pop();
        if(p.priority%3 == 0){
            queue0.push(p);
        }else if(p.priority%3 == 1){
            queue1.push(p);
        }else{
            queue2.push(p);
        }
    }
}

void printArrivedQueue(queue<Process> arrivedProcesses){
    cout<<"Arrived Queue: ";
    while(!arrivedProcesses.empty()){
        cout<<arrivedProcesses.front().processId<<" ";
        arrivedProcesses.pop();
    }
    cout<<endl;
}

void printReadyQueue(queue<Process> queue0,queue<Process> queue1,queue<Process> queue2){
    cout<<"Queue 0: ";
    while(!queue0.empty()){
        cout<<queue0.front().processId<<" ";
        queue0.pop();
    }
    cout<<endl;
    cout<<"Queue 1: ";
    while(!queue1.empty()){
        cout<<queue1.front().processId<<" ";
        queue1.pop();
    }
    cout<<endl;
    cout<<"Queue 2: ";
    while(!queue2.empty()){
        cout<<queue2.front().processId<<" ";
        queue2.pop();
    }
    cout<<endl;
}

Process getProcessFromReadyQueue(queue<Process> &queue0,queue<Process> &queue1,queue<Process> &queue2){
    if(!queue0.empty()){
        Process p = queue0.front();
        queue0.pop();
        return p;
    }else if(!queue1.empty()){
        Process p = queue1.front();
        queue1.pop();
        return p;
    }else if(!queue2.empty()){
        Process p = queue2.front();
        queue2.pop();
        return p;
    }
    return Process(-1,-1,-1,-1,-1,-1);
}

int findNextArrivalTimeofQueue0(vector<Process> processes, int timer){
    int minArrivalTime = INT_MAX;
    for(int i=0;i<processes.size();i++){
        if(processes[i].priority==0 && processes[i].arrivalTime < minArrivalTime && processes[i].arrivalTime >= timer){
            minArrivalTime = processes[i].arrivalTime;
        }
    }
    return minArrivalTime;
}

vector<Process> convertQueueToVector(queue<Process> completedProcess){
    vector<Process> processes;
    while(!completedProcess.empty()){
        processes.push_back(completedProcess.front());
        completedProcess.pop();
    }
    return processes;
}


void schedule(vector<Process> &processes, queue<Process> &arrivedProcesses,queue<Process> &queue0,queue<Process> &queue1,queue<Process> &queue2, int timer){
    addProcessesToArrivedQueue(processes,arrivedProcesses,timer);
    addProcessesToreadyQueue(arrivedProcesses,queue0,queue1,queue2,timer);
    //printArrivedQueue(arrivedProcesses);
    //printReadyQueue(queue0,queue1,queue2);
}

void idle(int timer){
    timer++;
}

void yeild(Process *p){
    p->remainingTime = 0;
}

void preempt(Process *p, queue<Process> &queue){
    p->remainingTime-=8;
    queue.push(*p);
}

int main(){
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
        Process p(stoi(pid),stoi(priority),stoi(at),stoi(bt1),stoi(bt2),stoi(io));
        processes.push_back(p);
        
        i++;
    }
    file.close();

    sort(processes.begin(),processes.end(),[](Process a, Process b){
        return a.arrivalTime < b.arrivalTime;
    });
    
    vector<Process> temp = processes;
    
    queue<Process> arrivedProcesses;
    
    queue<Process> completedProcesses;
    
    queue<Process> queue0;
    
    queue<Process> queue1;
    
    queue<Process> queue2;
    timer = processes[0].arrivalTime;
    

    int no_of_processes = processes.size();
    int no_of_completed_processes = 0;
    int counter_for_queue1 = 0;
    while(timer<1000&&no_of_completed_processes < no_of_processes){
        //cout<<"timer: "<<timer<<endl;
        addProcessesToArrivedQueue(processes,arrivedProcesses,timer);
        
        addProcessesToreadyQueue(arrivedProcesses,queue0,queue1,queue2,timer);
        
        if(queue0.empty()&&queue1.empty()&&queue2.empty()){
            timer++;
            //idle(timer);
            continue;
        }
        while(!queue0.empty()){
            Process *p = &queue0.front();
            counter_for_queue1=0;
            queue0.pop();

            if(!p->isStarted){
                p->isStarted = true;
                p->responseTime = timer - p->arrivalTime;
            }
            if(p->remainingTime<=8){
                timer+=p->remainingTime;
                yeild(p);
                if(p->isCompleted){
                    //cout<<"here"<<endl;
                    p->completionTime=timer;    
                    no_of_completed_processes++;
                    p->turnAroundTime = p->completionTime - p->at;
                    completedProcesses.push(*p);
                }
                else{
                    p->remainingTime=p->bt2;
                    p->isCompleted=true;
                    p->arrivalTime=timer+p->i_o;
                    //cout<<"her1e"<<endl;
                    //update this process in the processes vector
                    for(int i=0;i<processes.size();i++){
                        if(processes[i].processId==p->processId){
                            processes[i].remainingTime=p->bt2;
                            processes[i].isCompleted=true;
                            processes[i].arrivalTime=timer+p->i_o;
                            processes[i].isArrived=false;
                            break;
                        }
                    }
                }                
                schedule(processes,arrivedProcesses,queue0,queue1,queue2,timer);

            }else{
                timer+=8;    
                p->remainingTime-=8;
                queue1.push(*p);
                schedule(processes,arrivedProcesses,queue0,queue1,queue2,timer);
            }
        }
        //cout<<"before q1 timer: "<<timer<<endl;
        if(!queue1.empty()){
            while(!queue1.empty()&&queue0.empty()){
                Process *p = &queue1.front();
                
                if(!p->isStarted){
                    counter_for_queue1 = 0;
                    p->isStarted = true;
                    p->responseTime = timer - p->arrivalTime;
                }
                
                if(p->remainingTime==0){
                    timer+=p->remainingTime;
                    yeild(p);
                    if(p->isCompleted){
                        //cout<<"here"<<endl;
                        p->completionTime=timer;    
                        no_of_completed_processes++;
                        p->turnAroundTime = p->completionTime - p->at;
                        completedProcesses.push(*p);
                    }
                    else{
                        p->remainingTime=p->bt2;
                        p->isCompleted=true;
                        p->arrivalTime=timer+p->i_o;
                        //cout<<"her1e"<<endl;
                        //update this process in the processes vector
                        for(int i=0;i<processes.size();i++){
                            if(processes[i].processId==p->processId){
                                processes[i].remainingTime=p->bt2;
                                processes[i].isCompleted=true;
                                processes[i].arrivalTime=timer+p->i_o;
                                processes[i].isArrived=false;
                                break;
                            }
                        }
                    }  
                    queue1.pop();
                    continue;
                }
                else if(counter_for_queue1==16){
                    queue1.pop();
                    queue2.push(*p);
                    counter_for_queue1 = 0;
                    continue;
                }
                else{
                    p->remainingTime=p->remainingTime-1;
                    counter_for_queue1++;
                    
                }
                timer++;
                schedule(processes,arrivedProcesses,queue0,queue1,queue2,timer);
            }
        }
        
        //cout<<"before queue 2"<<endl;

        if(!queue2.empty()){


            while(!queue2.empty()&&queue0.empty()&&queue1.empty()){
                Process *p = &queue2.front();
                
                if(!p->isStarted){
                    p->isStarted = true;
                    p->responseTime = timer - p->arrivalTime;
                }
                if(p->remainingTime==0){
                    timer+=p->remainingTime;
                    yeild(p);
                    if(p->isCompleted){
                        //cout<<"here"<<endl;
                        p->completionTime=timer;    
                        no_of_completed_processes++;
                        p->turnAroundTime = p->completionTime - p->at;
                        completedProcesses.push(*p);
                    }
                    else{
                        p->remainingTime=p->bt2;
                        p->isCompleted=true;
                        p->arrivalTime=timer+p->i_o;
                        //cout<<"her1e"<<endl;
                        //update this process in the processes vector
                        for(int i=0;i<processes.size();i++){
                            if(processes[i].processId==p->processId){
                                processes[i].remainingTime=p->bt2;
                                processes[i].isCompleted=true;
                                processes[i].arrivalTime=timer+p->i_o;
                                processes[i].isArrived=false;
                                break;
                            }
                        }
                    }  
                    queue2.pop();
                    continue;
                }
                else{
                    p->remainingTime=p->remainingTime-1;
                }
                timer++;
                schedule(processes,arrivedProcesses,queue0,queue1,queue2,timer);
            }
        }
    }
    
    vector<Process> completedProcessesVector = convertQueueToVector(completedProcesses);
    
    for(int i=0;i<completedProcessesVector.size();i++){
        completedProcessesVector[i].waitingTime = completedProcessesVector[i].turnAroundTime - completedProcessesVector[i].burstTime;
    }
    
    // cout<<"completed processes"<<endl;
    // cout<<"process ID"<<"\t\t"<< "priority"<<"\t\t"<< "arrival time"<<"\t\t"<< "burst time"<<"\t\t"<< "completion time"<<"\t\t"<< "turn around time"<<"\t\t"<< "waiting time"<<"\t\t"<< "response time"<<endl;
    // for(int i=0;i<completedProcessesVector.size();i++){
    //     cout<<completedProcessesVector[i].processId<<"\t\t\t"<< completedProcessesVector[i].priority<<"\t\t\t"<< completedProcessesVector[i].at<<"\t\t\t"<< completedProcessesVector[i].burstTime<<"\t\t\t"<< completedProcessesVector[i].completionTime<<"\t\t\t"<< completedProcessesVector[i].turnAroundTime<<"\t\t\t"<< completedProcessesVector[i].waitingTime<<"\t\t\t"<< completedProcessesVector[i].responseTime<<endl;
    // }

    //return 0;
    float completionTime[completedProcessesVector.size()];
	float TAT[completedProcessesVector.size()];
	float waitingTime[completedProcessesVector.size()];
	float responseTime[completedProcessesVector.size()];
    float avgTAT = 0;
    float avgWT = 0;
    float avgRT = 0;
    float avgCT = 0;
    for(int i=0;i<completedProcessesVector.size();i++){
        completionTime[i] = completedProcessesVector[i].completionTime;
        TAT[i] = completedProcessesVector[i].turnAroundTime;
        waitingTime[i] = completedProcessesVector[i].waitingTime-completedProcessesVector[i].i_o;
        responseTime[i] = completedProcessesVector[i].responseTime;
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
        out<<completionTime[i]<<","<<TAT[i]<<","<<waitingTime[i]<<","<<responseTime[i]<<","<<completedProcessesVector[i].processId<<endl;
    }
    out.close();
    system("python3 graph.py MLFQ.jpeg MultilevelFeedbackQueue");
	system("python3 graph1.py MLFQPie.jpeg MultilevelFeedbackQueue");
    system("python3 stack.py MLFQstack.jpeg MultilevelFeedbackQueue");
    remove("out.csv");
	//remove("out.csv");
    //store the average of completion time, turn around time, waiting time, response time int his order of each process in all.csv file dont overwrite the file just append the data
    ofstream all("all.csv",ios::app);
    all<<avgCT<<","<<avgTAT<<","<<avgWT<<","<<avgRT<<endl;
    all.close();

    

    return 0;
}