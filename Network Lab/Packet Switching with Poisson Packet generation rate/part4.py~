        import matplotlib.pyplot as plt
import queue as Q
import math
import random

pDropList = []
fixedQueueSize=10
# class for pcket
class DATAPACKET:
    def __init__(self, pid=0, gtime=0.0, sourceid=0):
        self.packetId = pid
        self.sourceId = sourceid
        self.gentime = gtime
        self.qInTIme = -1
        self.qOutTime = -1
        self.sinkReachTime = -1


# class for source
class DATASOURCE:
    def __init__(self, sid, bs,grate):
        self.sourceId = sid
        self.bs = bs
        self.rate = grate


# class for switch
class DATASWITCH:
    def __init__(self, bwidth):
        self.bss = bwidth
        self.qSize = 0


# class for various Event
class EVENT:
    def __init__(self, eid, pid, t):
        self.eId = eid
        self.pId = pid
        self.curTime = t

    def __lt__(self, other):
        return self.curTime < other.curTime

def nextTime(rateParameter):
        return -math.log(1.0 - random.uniform(0,1)) / rateParameter

# function to calculate avg delay
def calculateAvgPDrop(nSource, bs, bss, pktLength,simTime,source):
    packet = []
    avgQueuingDelay = 0.0
    swich = DATASWITCH(bss)

    # pq is priority queue on the basis of event current time
    pq = Q.PriorityQueue()

    # generating first packet from every source at an fixed interval
    ttime = 0.0
    for i in range(nSource):
        packet.append(DATAPACKET(i, ttime, i))
        pq.put(EVENT(0, i, ttime))
        ttime += 0.000001

    pDrop =[]
    pArrived = []
    for i in range(len(source)):
        pDrop.append(0)
        pArrived.append(0)

    pktTot = nSource
    lastLeftTime = 0.0

    # Simulating for fixed time
    # Event0 = generation of packet
    # Event1 = reaching Queue time
    # Event2 = leaving queue time
    # Event3 = reaching sink time
    curTime = 0
    pkt_reach_sink = 0
    simTime = 5000
    while (pkt_reach_sink < simTime):
        x = pq.get()
        pid = x.pId
        curTime = x.curTime
        sourceId = packet[pid].sourceId
        # Event 0 -> (Event0,Event1)
        if x.eId == 0:
            nextTim = nextTime(source[sourceId].rate)
            pq.put(EVENT(0, pktTot, curTime + nextTim))
            packet.append(DATAPACKET(pktTot, curTime + nextTim, packet[pid].sourceId))
            pktTot = pktTot + 1
            pq.put(EVENT(1, pid, curTime + pktLength / bs))
            packet[pid].qInTime = curTime + pktLength / bs
        # Event 1 -> Event2
        elif x.eId == 1:
            pArrived[sourceId]+=1
            if swich.qSize >= fixedQueueSize:
                pDrop[sourceId]+=1
            else:
                rtime = (swich.qSize * pktLength) / bss
                tx = 0
                if packet[pid].qInTime - lastLeftTime < (pktLength / bss):
                    tx = max(0, (pktLength / bss) - (packet[pid].qInTime - lastLeftTime))
                if lastLeftTime == 0:
                    tx = 0
                pq.put(EVENT(2, pid, curTime + rtime + tx))
                packet[pid].qOutTime = curTime + rtime + tx
                swich.qSize = swich.qSize + 1
        # Event2 -> Event3
        elif x.eId == 2:
            swich.qSize = swich.qSize - 1
            lastLeftTime = curTime
            sTime = curTime + pktLength / bss
            packet[pid].sinkReachTime = sTime
            pq.put(EVENT(3, pid, sTime))
        else:
            pkt_reach_sink+=1

    for i in range(nSource):
        #print(pDrop[i],pArrived[i],end=" ")
        pDropList[i].append(pDrop[i]/pArrived[i])

def main():
    print("Enter 0 to use default value or 1 to use own")
    resp = int(input())
    if resp == 0:
        # nsource = number of source
        nSource = 4

        # bs = bandwidth between source and switch in bit
        bs = 2*12000*16

        # bss = bandwidth between switch and sink in bit
        bss = 5e4
        # pktLength = size of each packet in bit
        pktLength = 1.2e4

        # grate = packet genrate
        grate = [0.9,2,4,16]
        # simulation time
        simTime = 5000
    else:
        grate = []
        nSource = int(input("Enter Number of Source :"))
        bs = float(input("Enter bandwdth between Source and switch(bs) in bit:"))
        bss= float(input("Enter bandwidth between switch and sink(bss) in bit:"))
        pktLength = int(input("Enter packet length in bit(pktlength) :"))
        simTime = int(input("Enter Simulation time:"))
        for i in range(nSource):
            print("Source ",i)
            l = int(input("Enter lambda for source "))
            grate.append(l)
   
    # creating source object
    source = []
    for i in range(nSource):
        source.append(DATASOURCE(i, bs,grate[i]))
        pDropList.append([])

    iter = 0
    l = []
    for i in range(nSource):
        l.append("Source "+str(i))
    while iter < 200:
        calculateAvgPDrop(nSource,bs,bss,pktLength,simTime,source)
        print(iter)
        iter+=1
    plt.title("Average packet drop for each source")    
    plt.ylabel("time unit")
    plt.xlabel("sources")
    plt.boxplot(pDropList,labels=l)
    plt.show()

if __name__ == "__main__":
    main()
