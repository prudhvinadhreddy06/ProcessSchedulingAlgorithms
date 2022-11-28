import sys
import os
import matplotlib.pyplot as plt
import numpy as np

#import data from the out.csv and store it in a list of lists
def import_data():
    data = []
    with open('out.csv', 'r') as f:
        for line in f:
            data.append(line.split(','))
    return data

data = import_data()

# sort the data by last column
data.sort(key=lambda x: x[-1])

#store the ct,tat,wt,rt,pid in separate lists
ct = []
tat = []
wt = []
rt = []
pid = []
for i in range(len(data)):
    ct.append(float(data[i][0]))
    tat.append(float(data[i][1]))
    wt.append(float(data[i][2]))
    rt.append(float(data[i][3]))
    pid.append(int(data[i][4]))


#plotting the graph
plt.scatter(pid,ct, label='Completion Time', color='r', marker='o')
plt.scatter(pid,tat, label='Turn Around Time', color='g', marker='o')
plt.scatter(pid,wt, label='Waiting Time', color='b', marker='o')
plt.scatter(pid,rt, label='Response Time', color='y', marker='o')

plt.plot(pid,ct, label='Completion Time', color='r')
plt.plot(pid,tat, label='Turn Around Time', color='g')
plt.plot(pid,wt, label='Waiting Time', color='b')
plt.plot(pid,rt, label='Response Time', color='y')

plt.xlabel('Process ID')
plt.ylabel('Time')
plt.title(sys.argv[2])
plt.legend(['Completion Time', 'Turn Around Time', 'Waiting Time', 'Response Time'])
plt.grid(True)
plt.savefig('plots/'+sys.argv[1], dpi=1000)