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

#plot sub plots for each pie chart for completion time, turn around time, waiting time and response time with respect to process id and store all of them in a single image
fig, axs = plt.subplots(2, 2)
axs[0, 0].pie(ct, labels=pid,  startangle=90)
axs[0, 0].set_title('Completion Time')
axs[0, 1].pie(tat, labels=pid, startangle=90)
axs[0, 1].set_title('Turn Around Time')
axs[1, 0].pie(wt, labels=pid,startangle=90)
axs[1, 0].set_title('Waiting Time')
axs[1, 1].pie(rt, labels=pid,  startangle=90)
axs[1, 1].set_title('Response Time')
fig.suptitle(sys.argv[2])
plt.savefig('plots/'+sys.argv[1], dpi=1000)
