import matplotlib.pyplot as plt
import queue as Q

utilNum = 0

pktDrop = 0

# class for pcket
class DATAPACKET:
    def __init__(self,pid=0,gtime=0.0,sourceid=0):
        self.packetId = pid
        self.sourceId = sourceid
        self.gentime = gtime
        self.qInTIme = -1
        self.qOutTime = -1

# class for source
class DATASOURCE:
    def __init__(self,lamda,sid,bs):
        self.genRate = lamda
        self.sourceId = sid
        self.bs = bs

# class for switch
class DATASWITCH:
    def __init__(self,bwidth):
        self.bss = bwidth
        self.qSize = 0

# class for various Event
class EVENT:
    def __init__(self,eid,pid,t):
        self.eId = eid
        self.pId = pid
        self.curTime = t

    def __lt__(self, other):
        return self.curTime < other.curTime

# function to calculate pktLossRate
def calculatePktLossRate(nSource,bs,bss,pktLength,source,simTime,fixedQueueSize):
    packet = []
    avgQueuingDelay = 0.0
    swich = DATASWITCH(bss)
    global pktDrop
    pktDrop = 0
    # pq is priority queue on the basis of event current time
    pq = Q.PriorityQueue()
    ttime = 0.0

    # generating first packet from every source at an fixed interval
    for i in range(nSource):
        packet.append(DATAPACKET(i, ttime, i))
        pq.put(EVENT(0, i, ttime))
        ttime += 0.000001

    pcount = 0
    pktTot = nSource
    lastLeftTime = 0
    executionTime = 0.0
    packetarrived=0
    # Simulating for fixed time
    # Event0 = generation of packet
    # Event1 = reaching Queue time
    # Event2 = leaving queue time
    # Event3 = reaching sink time
    curTime = 0

    while (curTime < simTime):
        x = pq.get()
        pid = x.pId
        curTime = x.curTime
        #executionTime = max(executionTime,curTime)

        # Event 0 -> (Event0,Event1)
        if x.eId == 0:
            rate = source[packet[pid].sourceId].genRate
            pq.put(EVENT(0, pktTot, curTime + 1 / rate))
            packet.append(DATAPACKET(pktTot, curTime + 1 / rate, packet[pid].sourceId))
            pktTot = pktTot + 1
            pq.put(EVENT(1, pid, curTime + pktLength/bs))
            packet[pid].qInTime = curTime + pktLength / bs
        # Event 1 -> Event2,and if queue is full packet drop
        elif x.eId == 1:
            packetarrived = packetarrived+1
            if(swich.qSize<=fixedQueueSize):
                rtime = (swich.qSize * pktLength) / bss
                tx = 0
                if packet[pid].qInTime - lastLeftTime < (pktLength / bss):
                    tx = max(0, (pktLength / bss) - (packet[pid].qInTime - lastLeftTime))
                if lastLeftTime == 0:
                    tx = 0
                pq.put(EVENT(2, pid, curTime + rtime + tx))
                packet[pid].qOutTime = curTime + rtime + tx
                swich.qSize = swich.qSize + 1
            else:
                #counting pkt drop
                pktDrop = pktDrop+1

        # Event2 -> Event3
        elif x.eId == 2:
            swich.qSize = swich.qSize - 1
            lastLeftTime = curTime
            sTime = curTime + pktLength / bss
            pq.put(EVENT(3, pid, sTime))
        else:
            avgQueuingDelay = packet[pid].qOutTime - packet[pid].qInTime + avgQueuingDelay
            pcount = pcount + 1
    #return pktDroprate
    return pktDrop/packetarrived

def main():
    print("Enter 0 to use default value or 1 to use own")
    resp = int(input())
    if resp == 0:
        # nsource = number of source
        nSource = 4
        # bs = bandwidth between source and switch in bit
        bs = 1e2
        # bss = bandwidth between switch and sink in bit
        bsslow = 10
        bsshigh = 500
        # pktLength = size of each packet in bit
        pktLength = 2

        # grate = packet genrate
        grate = 20

        # simulation time
        simTime = 200
        
        #max queue size in switch
        fixedQueueSize = 50
    else :
        nSource = int(input("Enter Number of Source :"))
        bs = float(input("Enter bandwdth between Source and switch(bs) in bit:"))
        bsslow = float(input("Enter bandwidth(lower bound) between switch and sink(bss) in bit:"))
        bsshigh = float(input("Enter bandwidth(upper bound) between switch and sink(bss) in bit:"))
        pktLength = int(input("Enter packet length in bit(pktlength) :"))
        grate = int(input("Enter packet generation rate:"))
        if pktLength * grate >= bs:
            print("pktLength*grater should be less than bs")
            return 0
        simTime = int(input("Enter Simulation time:"))
        fixedQueueSize = int(input("Enter Queue size in switch:"))

    # utilNum = numerator of utilization factor i.e arrival rate
    global utilNum
    # x and y holds value of delay and utilizationfactor
    x = []
    y = []
    source = []
    for i in range(nSource):
        source.append(DATASOURCE(grate, i, bs))
        utilNum = utilNum+grate
    # varying nbss i.e bss for plotting
    nbss = bsslow
    while(nbss<=bsshigh):
        loadfactor=(utilNum*pktLength)/nbss
        pktDropRate = calculatePktLossRate(nSource,bs,nbss,pktLength,source,simTime,fixedQueueSize)
        x.append(loadfactor)
        y.append(pktDropRate)
        print(loadfactor," ",pktDropRate)
        nbss = nbss+(bsshigh-bsslow)/980


    # plotting curve
    plt.plot(x,y)
    plt.xlabel("Utilization Factor")
    plt.ylabel("Packet loss rate")
    plt.text(x[len(x) - 1], y[len(y) - 1], "nSource=4,bs(bit)=1e2,lambda(gen rate)=20,pktlength=2 bit, \n bsslow(bit/s)=10,bsshigh=500")
    plt.title("Packet loss rate vs Utilization Factor")
    plt.show()

if __name__=="__main__":
    main()

