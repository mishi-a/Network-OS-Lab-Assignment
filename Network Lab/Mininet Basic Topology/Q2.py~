#!/usr/bin/python

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel
import thread
from time import sleep
from mininet.cli import CLI
from mininet.link import TCLink

class CreateTopo(Topo):
        def build(self,n):
                switch1 = self.addSwitch('s1');
                switch2 = self.addSwitch('t1');
                for i in range(n):
                        host = self.addHost(chr(ord('a') + i))
                        if i == 0 or i == 1 or i == 4:
                                self.addLink(host,switch1,bw=5, delay='3ms', loss=2,max_queue_size=300)
                        else:
                                self.addLink(host,switch2,bw=5, delay='3ms', loss=2,max_queue_size=300)
                host = self.addHost('u')
                self.addLink(host,switch2,bw=5, delay='3ms', loss=2,max_queue_size=300)
                self.addLink(switch1,switch2,bw=15, delay='2ms')

def customTopo():
        topo = CreateTopo(n=5)
        net = Mininet(topo=topo,link=TCLink)
        net.start()
        #CLI(net)
        print("Detailed Connections :")
        dumpNodeConnections(net.hosts)

        print("Ping Result to all nodes :")
        net.pingAll()
        net.stop()



if __name__ == '__main__':
    setLogLevel( 'info' )
    customTopo()

