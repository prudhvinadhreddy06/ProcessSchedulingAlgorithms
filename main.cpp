#include<bits/stdc++.h>
#include <Python.h>
#include<bits/stdc++.h>

using namespace std;

int main(){
    //clear the content of all.csv file
    // ofstream ofs;
    // ofs.open("all.csv", ofstream::out | ofstream::trunc);
    // ofs.close();
    
    // //complie SJFP.cpp and run it
    // system("g++ -Wall -I/usr/include/python3.8 -lpython3.8 SJF_P.cpp -o SJF_P");
    // system("./SJF_P");

    // //complie Priority_P.cpp and run it
    // system("g++ -Wall -I/usr/include/python3.8 -lpython3.8 Priority_P.cpp -o Priority_P");
    // system("./Priority_P");

    //complie FCFS.cpp and run it
    system("g++ -Wall -I/usr/include/python3.8 -lpython3.8 FCFS.cpp -o FCFS");
    system("./FCFS");

    // //complie SJF_NP.cpp and run it
    system("g++ -Wall -I/usr/include/python3.8 -lpython3.8 SJF_NP.cpp -o SJF_NP");
    system("./SJF_NP");

    // //complie Priority_NP.cpp and run it
    system("g++ -Wall -I/usr/include/python3.8 -lpython3.8 Priority_NP.cpp -o Priority_NP");
    system("./Priority_NP");

    // //complie RR.cpp and run it
    system("g++ -Wall -I/usr/include/python3.8 -lpython3.8 RR.cpp -o RR");
    system("./RR");

    // //complie MLF.cpp and run it
    system("g++ -Wall -I/usr/include/python3.8 -lpython3.8 MLF.cpp -o MLF");
    system("./MLF");

    //complie MLFQ.cpp and run it
    system("g++ -Wall -I/usr/include/python3.8 -lpython3.8 MLFQ.cpp -o MLFQ");
    system("./MLFQ");

    //run python file to plot the graph
    system("python3 bar.py");
    //remove all the executable files
    //system("rm FCFS SJF_P SJF_NP Priority_P Priority_NP RR MLF MLFQ");
    system("rm FCFS SJF_NP Priority_NP RR MLF MLFQ");

    return 0;
}