import matplotlib.pyplot as plt
import numpy as np
import sys

# import data from the out.csv and store it in a list of lists
def import_data():
    data = []
    with open('out.csv', 'r') as f:
        for line in f:
            data.append(line.split(','))
    return data

data = import_data()

#print(data)
# sort the data by last column
data.sort(key=lambda x: x[-1])

ct=[]
tat=[]
wt=[]
rt=[]
pid=[]
for i in range(len(data)):
    ct.append(float(data[i][0]))
    tat.append(float(data[i][1]))
    wt.append(float(data[i][2]))
    rt.append(float(data[i][3]))
    pid.append(int(data[i][4]))


# plt the data in bar graph
col1 = np.array(ct)
col2 = np.array(tat)
col3 = np.array(wt)
col4 = np.array(rt)

plt.clf()
plt.bar(pid, col1, color='r', label='Completion Time')
plt.bar(pid, col2, color='g', label='Turn Around Time')
plt.bar(pid, col3, color='b', label='Waiting Time')
plt.bar(pid, col4, color='y', label='Response Time')
plt.title(sys.argv[2])
plt.xlabel('Process ID')
plt.ylabel('Time')
plt.grid(True)
plt.title(sys.argv[2])
plt.legend(['Completion Time', 'Turn Around Time', 'Waiting Time', 'Response Time'])
plt.legend(loc='upper left')
plt.savefig('plots/'+sys.argv[1], dpi=1000)
