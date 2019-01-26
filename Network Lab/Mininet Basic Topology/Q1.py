#!/usr/bin/python

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
#from threading import Thread
import thread
from time import sleep
from mininet.cli import CLI

class CreateTopo(Topo):
        def build(self,n):
                switch = self.addSwitch('s1')
                host1 = self.addHost('h1')
                host2 = self.addHost('h2')
                self.addLink(host1,switch)
                self.addLink(host2,switch)

def server(net):
        h1 = net.get('h1')
        h1.cmd('iperf -s -p 5111 -i 2 > throughput.txt')

def twoHostSystem():
        topo = CreateTopo(n=2)
        net = Mininet(topo=topo)
        net.start()
        #CLI(net)
        #dumpNodeConnections(net.hosts)
        h1 = net.get('h1')
        h2 = net.get('h2')
        thread.start_new_thread( server, (net, ) )
        h2.cmd('iperf -c 10.0.0.1 -p 5111 -t 20')
        #result = h2.cmd('cat throughput.txt|tr "-" " "|grep "sec" | awk "{print $4,$8}"')
        #print(result)
        

if __name__ == '__main__':
    setLogLevel( 'info' )
    twoHostSystem()
    cnt=0
    print("Required Throughput")
    print("Time Interval          Throughput")
    with open("throughput.txt") as f:
        contents = f.readlines()
    contents = [x.strip() for x in contents]
    for i in range(6,len(contents)):
            line = contents[i].split()
            if i<=9:
                    print("[%s %s %s] ---> %s %s" %(line[2],line[3],line[4],line[7],line[8]))
            else:
                    print("[%s %s] ---> %s %s " %(line[2],line[3],line[4],line[7]))
 
