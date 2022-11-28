# Process Scheduling algorithms simulation

### Dependencies
* g++ compiler
	* `sudo apt install g++`
* python3-dev
	* `sudo apt install python3-dev`
* matplotlib
	* `pip3 install matplotlib`
* numpy
    * `pip3 install numpy`


### Input format
txt file with each row having `PID,name,type,Priority,AT,BT1,I/O,BT2`
<br>eg.input3.txt
```
  0,P1,C,67,22,1,3,6
  1,P2,C,71,9,45,6,4
  2,P3,C,26,12,2,9,2
```

### Run instruction 
```
mkdir -p plots
g++ -Wall -I/usr/include/python3.8 -lpython3.8 SJF_P.cpp -o SJF_P
./SJF_P

g++ -Wall -I/usr/include/python3.8 -lpython3.8 Priority_P.cpp -o Priority_P
./Priority_P

g++ -Wall -I/usr/include/python3.8 -lpython3.8 main.cpp -o main
./main

```
### Output
All the plots will be saved inside `./plots``

### Github link
* https://github.com/prudhvinadhreddy06/ProcessSchedulingAlgorithms
