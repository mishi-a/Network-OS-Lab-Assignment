import matplotlib.pyplot as plt
import queue as Q
import math
import random

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
    def __init__(self, sid, bs):
        self.sourceId = sid
        self.bs = bs


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
def calculateAvgQueueSize(nSource, bs, bss, pktLength,simTime,rate):
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

    pktTot = nSource
    lastLeftTime = 0.0
    avgSize = 0
    tCount = 0
    # Simulating for fixed time
    # Event0 = generation of packet
    # Event1 = reaching Queue time
    # Event2 = leaving queue time
    # Event3 = reaching sink time
    curTime = 0
    simTime = 5000
    pkt_reach_sink = 0
    while (pkt_reach_sink < simTime):
        avgSize+=swich.qSize
        tCount+=1
        x = pq.get()
        pid = x.pId
        curTime = x.curTime
        # print(curTime)
        # Event 0 -> (Event0,Event1)
        if x.eId == 0:
            nextTim = nextTime(rate)
            pq.put(EVENT(0, pktTot, curTime + nextTim))
            packet.append(DATAPACKET(pktTot, curTime + nextTim, packet[pid].sourceId))
            pktTot = pktTot + 1
            pq.put(EVENT(1, pid, curTime + pktLength / bs))
            packet[pid].qInTime = curTime + pktLength / bs
        # Event 1 -> Event2
        elif x.eId == 1:
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
            # pcount = pcount + 1

    return avgSize/tCount


def main():
    print("Enter 0 to use default value or 1 to use own")
    resp = int(input())
    if resp == 0:
        # nsource = number of source
        nSource = 4

        # bs = bandwidth between source and switch in bit
        bs = 2e6

        # bss = bandwidth between switch and sink in bit
        bss = 4.8e6
        # pktLength = size of each packet in bit
        pktLength = 1.2e4

        # grate = packet genrate
        grate = 1

        # simulation time
        simTime = 5000
    else:

        nSource = int(input("Enter Number of Source :"))
        bs = float(input("Enter bandwdth between Source and switch(bs) in bit:"))
        bss= float(input("Enter bandwidthbetween switch and sink(bss) in bit:"))
        pktLength = int(input("Enter packet length in bit(pktlength) :"))
        simTime = int(input("Enter Simulation time:"))

    # x and y holds value of lamda and queue Size
    x = []
    y = []

    # creating source object
    source = []
    for i in range(nSource):
        source.append(DATASOURCE(i, bs))

    lamda = 4
    totIter = 20

    while lamda < 1000:
        x.append(lamda)
        iter = 0
        qSize = 0
        while iter < totIter:
            qSize +=calculateAvgQueueSize(nSource,bs,bss,pktLength,simTime,lamda/nSource)
            iter+=1
        print(lamda,qSize/totIter)
        y.append(qSize/totIter)
        lamda+=4


    s = "nSource="+str(nSource)+",bs(bit)="+str(bs)+"\n pktlength="+str(pktLength)+" bit\n bss(bit) ="+ str(bss)+"\nlambda = 4-1000\n"
    # plotting curve
    plt.plot(x, y)
    plt.xlabel("lambda")
    plt.ylabel("Average Queue Size")
    plt.text(4,4,s)
    plt.title("Average Queue Size vs lambda")
    plt.show()


if __name__ == "__main__":
    main()
