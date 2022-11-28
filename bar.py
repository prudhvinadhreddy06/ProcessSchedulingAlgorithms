import matplotlib.pyplot as plt
import numpy as np

# import data from the all.csv and store it in a list of lists
def import_data():
    data = []
    with open('all.csv', 'r') as f:
        for line in f:
            data.append(line.split(','))
    return data

data = import_data()

ct = []
tat = []
wt = []
rt = []

#print(data)
for i in range(len(data)):
    ct.append(float(data[i][0]))
    tat.append(float(data[i][1]))
    wt.append(float(data[i][2]))
    rt.append(float(data[i][3]))

#print the lists
# print(ct)
# print(tat)
# print(wt)
# print(rt)
x_axis = ['SJFP','P_p','FCFS','SJFN','P_n','RR','MLQ','MLFQ']

#plotting the graph
plt.xlabel('Process Scheduling Algorithms')
plt.ylabel('Time')
plt.title('Average Complete Time Comparison')
plt.bar(x_axis,ct, label='Completion Time', color='r')
plt.savefig('plots/'+'AvgCt.png', dpi=1000)

plt.clf()
plt.xlabel('Process Scheduling Algorithms')
plt.ylabel('Time')
plt.title('Average Turn Around Time Comparison')
plt.bar(x_axis,tat, label='Turn Around Time', color='g')
plt.savefig('plots/'+'AvgTat.png', dpi=1000)
plt.clf()

plt.xlabel('Process Scheduling Algorithms')
plt.ylabel('Time')
plt.title('Average Waiting Time Comparison')
plt.bar(x_axis,wt, label='Waiting Time', color='b')
plt.savefig('plots/'+'AvgWt.png', dpi=1000)
plt.clf()

plt.xlabel('Process Scheduling Algorithms')
plt.ylabel('Time')
plt.title('Average Response Time Comparison')
plt.bar(x_axis,rt, label='Response Time', color='y')
plt.savefig('plots/'+'AvgRt.png', dpi=1000)
plt.clf()

#print the lists in tabular form
print('Algorithm\t\tCT\t\tTAT\t\tWT\t\tRT')
for i in range(len(data)):
    print(x_axis[i]+'\t\t'+str(ct[i])+'\t\t'+str(tat[i])+'\t\t'+str(wt[i])+'\t\t'+str(rt[i]))


