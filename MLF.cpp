#include <bits/stdc++.h>
using namespace std;

class Process;
class MLQ;
class ATComparator;
//class BTComparator;
class MLQScheduler;
class Process {
public:
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;
    int waitingTime;
    int completionTime;
    int responseTime;
    int t_burstTime;
    bool isCompleted;
    bool isStarted;
    int Waiting_time;
    int Turn_around_time;
    int startTime;
    int ioTime;
    int brustTime2;
    int t_arrivalTime;
    int noofexe;
   // bool isPreempted;


    int P_run(int quantum,int start_time) {
        //cout<<"process is running"<<pid<<endl;
        if (burstTime <= quantum) {
            ////cout<<"start time "<<start_time<<endl;
            completionTime = start_time + burstTime;
            burstTime = 0;
            
            if(!isStarted) {
                this->startTime=start_time;
                isStarted = true;
            }
            if(noofexe==1)
            {
                compute_times();
                isCompleted = true;
            }
            else
            {
                yield(completionTime);
                burstTime=brustTime2;
                noofexe++;
            }
            return completionTime;
        }
        else {
            burstTime -= quantum;
            ////cout<<pid<<" "<<burstTime<<endl;
            if(!isStarted) {
                this->startTime = start_time;
                isStarted = true;
            }
            return start_time + quantum;
        }

    }

    void yield(int time){                                      //
        long long int io_time =this->ioTime;
        //curr_proc.set_to_waiting_state();
        wake_up(time+ io_time);

    }
    void wake_up(long long int wakeup_time){         //
        this->arrivalTime=wakeup_time;
        // if(curr_proc.Burst_time_2!=0)
        // {
        //     queue.push(curr_proc);
        //     curr_proc.set_to_ready_state();
        // }
        // else
        // {
        //     ////cout<<"termination in wake up";
        //     terminate(curr_proc);
        // }
    }

    int run(int start_time){
        if(!isStarted||isStarted) {
            this->startTime = start_time;
            isStarted = true;
        }
        completionTime = start_time + burstTime;
        burstTime = 0;
        isCompleted = true;
        return completionTime;
    }

    Process(){
        pid = 0;
        arrivalTime = 0;
        burstTime = 0;
        priority = 0;
        waitingTime = 0;
        completionTime = 0;
        responseTime = 0;
        t_burstTime = 0;
        isCompleted = false;
        isStarted = false;
       // isPreempted = false;
    }

    Process(int pid, int arrivalTime, int burstTime,int io,int bt2, int priority) {
        this->pid = pid;
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime;
        this->priority = priority;
        this->waitingTime = 0;
        this->completionTime = 0;
        this->responseTime = -1;
        this->t_burstTime = burstTime;
        this->isCompleted = false;
        this->isStarted = false;
        this->Turn_around_time=0;
        this->Waiting_time=0;
        this->startTime=0;
        this->ioTime=io;
        this->brustTime2=bt2;
        this->t_arrivalTime=this->arrivalTime;
        this->noofexe=0;
       // this->isPreempted = false;
      //  //cout << setw(10) << this->pid << setw(10) << this->arrivalTime << setw(10) << this->t_burstTime << setw(10) << this->ioTime << setw(10) << this->brustTime2 << setw(10) << this->startTime << setw(10) << this->responseTime << setw(10) << this->completionTime << setw(10) << this->Turn_around_time << setw(10) << this->Waiting_time << endl;

    }

     void compute_times(){
        this->responseTime =startTime-t_arrivalTime;
        this->Turn_around_time = completionTime -t_arrivalTime;
        this->Waiting_time =  Turn_around_time - t_burstTime-brustTime2-ioTime;
        arrivalTime=t_arrivalTime;
        ////cout << setw(10) << this->pid << setw(10) << this->arrivalTime << setw(10) << this->t_burstTime << setw(10) << this->ioTime << setw(10) << this->brustTime2 << setw(10) << this->startTime << setw(10) << this->responseTime << setw(10) << this->completionTime << setw(10) << this->Turn_around_time << setw(10) << this->Waiting_time << endl;
       // completedProcessesVector.push_back(this);

    }

};

class ATComparator {
public:
    bool operator()(const Process* p1, const Process* p2) {
        return p1->arrivalTime > p2->arrivalTime;
    }
};

class RRScheduler {
public :
    priority_queue<Process*, vector<Process*>, ATComparator> newQueue;
    int quantum;
    queue<Process*> arrived_queue;
    int time;
    queue<Process*> compledted_queue;

    void addProcess(Process* p) {
        if(p->arrivalTime > time) {
            newQueue.push(p);
        }
        else {
            arrived_queue.push(p);
        }
    }

    RRScheduler(int quantum=2, int time=0) {
        this->quantum = quantum;
        this->time = time;
    }

    int get_next_Schedule_time() {
        if(!arrived_queue.empty()) {
            return time;
        }
        else if(!newQueue.empty()) {
            return newQueue.top()->arrivalTime;
        }
        else {
            return -1;
        }
    }

    int run(){
        Process *p = NULL;
        if(arrived_queue.empty()){
            if(newQueue.empty()){
                return time=time+quantum;
            }
            else{
                Process *cp = newQueue.top();
                int t = cp->arrivalTime;
                if(t<=quantum+time){

                    time = t;
                }
                else{
                    time = time+quantum;
                    return t;                    
                }
            }
        }
        else{
            p = arrived_queue.front();
            arrived_queue.pop();
            time = p->P_run(quantum,time);
            ////cout<<"time in "<<time<<endl;
            if(p->noofexe==1 &&  !p->isCompleted)
            newQueue.push(p);
        }
       // //cout<<"before while\n";
        while(!newQueue.empty() && newQueue.top()->arrivalTime <= time){
            Process *cp = newQueue.top();
            if(cp->arrivalTime<=time){
                arrived_queue.push(cp);
                newQueue.pop();
            }
            else{
                break;
            }
        }
       // //cout<<"before if\n";
        ////cout<<p->isCompleted<<" "<<p->noofexe<<" \n";
        if((p!=NULL && !p->isCompleted )&& p->noofexe==0){
           // //cout<<"adding into arrived queue\n";
            arrived_queue.push(p);
        }
       // //cout<<"after if\n";
        if(p!=NULL && p->isCompleted){
            ////cout<<"in completed\n";
            compledted_queue.push(p);
        }
        return time;
    }

    int update_time(int t){
        time = t;
        while(!newQueue.empty() && newQueue.top()->arrivalTime <= time){
            Process *cp = newQueue.top();
            if(cp->arrivalTime<=time){
                arrived_queue.push(cp);
                newQueue.pop();
            }
            else{
                break;
            }
        }
    }

    bool is_wait(){
        return arrived_queue.empty();
    }

    bool is_completed(){
        return compledted_queue.empty();
    }

    bool is_idle(){
        return (newQueue.empty() && arrived_queue.empty());
    }

    void view(){
        //cout<<"time "<<time<<endl;
        //cout<<"New Queue : ";
        priority_queue<Process*, vector<Process*>, ATComparator> temp = newQueue;
        while(!temp.empty()){
            //cout<<temp.top()->pid<<" "<<temp.top()->arrivalTime<<" ";
            temp.pop();
        }
        
        //cout<<endl;
        //cout<<"Arrived Queue : ";
        queue<Process*> temp1 = arrived_queue;
        while(!temp1.empty()){
            //cout<<temp1.front()->pid<<" "<<temp1.front()->arrivalTime<<" ";
            temp1.pop();
        }
        //cout<<endl;
        //cout<<"Completed Queue : ";
        queue<Process*> temp2 = compledted_queue;
        while(!temp2.empty()){
            //cout<<temp2.front()->pid<<" ";
            temp2.pop();
        }
        //cout<<endl;
    }

    queue<Process*> get_completed_queue(){
        return compledted_queue;
    }

    queue<Process*> get_arrived_queue(){
        return arrived_queue;
    }
};

class MLFQScheduler{

    public:
    int levels;
    vector<RRScheduler*> m_queue;
	int clock;
    vector<string> gant_chart;
    int step=8;

    MLFQScheduler(int no_of_queue=3, int t_quantam=2)
    {
		clock=0;
       // //cout<<"inside queue"<<endl;
		for(int i=0;i<no_of_queue;i++)
        {
            RRScheduler *temp;
            if(i==no_of_queue-1)
            m_queue.push_back(new RRScheduler(INT_MAX,clock));
            else
            m_queue.push_back(new RRScheduler(t_quantam+(i*step),clock));
        }
        levels=no_of_queue;
    }

    void insert(int priority,Process* p)
    {
       ////cout<<"inserting in queue "<<priority+1<<endl;
        ////cout<<"size "<<m_queue.size()<<endl;
		m_queue[priority]->addProcess(p);
       // view();
        ////cout<<"\n\n";
	}

    void view()
    {
        for(int i=0;i<levels;i++)
        {
            //cout<<"queue "<<i+1<<endl;
            m_queue[i]->view();
            //cout<<"\n\n";
        }
    }



    int run()
    {
        int a_clock=clock;
        int i=0;
		for(i=0;i<levels;i++)
        {
			if(!m_queue[i]->is_wait())
            {
				a_clock=m_queue[i]->run();
                string s = "At t = " + to_string(clock) + " - " + to_string(a_clock) + "; Process in queue" + to_string(i+1) +  " is executing in CPU.";
                ////cout<<s<<endl;
                gant_chart.push_back(s);
				break;
			}
		}
		if(i==levels)
        {
            int min_time = INT_MAX;
			for(i=0;i<levels;i++)
            {
				int next_time =m_queue[i]->get_next_Schedule_time();
				if(next_time != -1)
				min_time = min(min_time,next_time);
			}
			if(min_time!=INT_MAX)
            {
                a_clock=min_time;
				//current_time = levels[0]->run_single_time_quantum();
			}
			else
            {
                a_clock++;
			}
            for(int i=clock;i<a_clock;i++)
            {
                idle_run(i);
            }
		}
        
        clock=a_clock;
		for(int i=0;i<levels;i++)
			m_queue[i]->update_time(clock);
		return clock;
	}

    void idle_run(int clock){
        //CPU is idle currently for this 1 sec interval
        string s = "At t = " + to_string(clock) + " - " + to_string(clock+1) + "; No Process in Ready Queue";
        gant_chart.push_back(s);

        return ;
    }

    bool is_idle()
    {
		for(int i=0;i<levels;i++)
        {
            if(!m_queue[i]->is_idle())
            return false;
		}
		return true;
	}



};


int main(){
    //freopen("output_mlq.txt", "w", stdout);
    ifstream file("input3.txt");

    string line;
    vector<Process> processes;
    MLFQScheduler ms(3,8);
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
        Process *curr_proc=new Process(stoi(pid),stoll(at), stoll(bt1),stoi(io),stoll(bt2),stoll(priority));
        processes.push_back(*curr_proc);
        //ms.insert(2-stoi(priority)/4,curr_proc);
        ms.insert(stoi(priority)%3,curr_proc);
        ////cout<<processes[i].()<<" "<<processes[i].priority<<" "<<processes[i].at<<" "<<processes[i].bt<<endl;
        i++;
    }

    
    while(!ms.is_idle())
    {
       
       //ms.view();
		ms.run();
	}

    //print gant chart
    for(auto s : ms.gant_chart){
        //cout << s << "\n";
    }
    //cout << "\n";


    vector<Process*>  completedProcessesVector;
    //cout << setw(10) << "id" << setw(10) << "arrival" << setw(10) << "burst1" << setw(10) << "io" << setw(10) << "burst2" << setw(10) << "start" << setw(10) << "resp" << setw(10) << "compl" << setw(10) << "turn" << setw(10) << "wait";
    //cout << "\n";
    for(int i=0;i<ms.levels;i++)
    {
        
        queue<Process*> temp2 =ms.m_queue[i]->get_completed_queue();
        while(!temp2.empty()){
            Process *p=temp2.front();
           //cout << setw(10) << p->pid << setw(10) << p->arrivalTime << setw(10) << p->t_burstTime << setw(10) << p->ioTime << setw(10) << p->brustTime2 << setw(10) << p->startTime << setw(10) << p->responseTime << setw(10) << p->completionTime << setw(10) << p->Turn_around_time << setw(10) << p->Waiting_time << endl;
            completedProcessesVector.push_back(temp2.front());
            temp2.pop();
        }

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

         completionTime[i] = completedProcessesVector[i]->completionTime;
        TAT[i] = completedProcessesVector[i]->Turn_around_time;
        waitingTime[i] = completedProcessesVector[i]->Waiting_time;
        responseTime[i] = completedProcessesVector[i]->responseTime;
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
            out<<completionTime[i]<<","<<TAT[i]<<","<<waitingTime[i]<<","<<responseTime[i]<<","<<completedProcessesVector[i]->pid<<endl;
            }
            out.close();
            //store the average of completion time, turn around time, waiting time, response time int his order of each process in all.csv file dont overwrite the file just append the data
            ofstream all("all.csv",ios::app);
            all<<avgCT<<","<<avgTAT<<","<<avgWT<<","<<avgRT<<endl;
            all.close();

    system("python3 graph.py MLFPr.jpeg MLF");
	system("python3 graph1.py MLFPie.jpeg MLF");
    system("python3 stack.py MLFStack.jpeg MLF");
    remove("out.csv");

    return 0;
}
