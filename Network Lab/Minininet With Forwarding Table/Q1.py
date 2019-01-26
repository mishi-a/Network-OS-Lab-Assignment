#!/usr/bin/python
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.nodelib import NAT
from mininet.log import setLogLevel
from mininet.cli import CLI
from mininet.util import dumpNodeConnections
from mininet.util import irange
import thread
from mininet.node import Node
from time import sleep
import mininet.node


class LinuxRouter( Node ):
    "A Node with IP forwarding enabled."

    def config( self, **params ):
        super( LinuxRouter, self).config( **params )
        # Enable forwarding on the router
        self.cmd( 'sysctl net.ipv4.ip_forward=1' )

    def terminate( self ):
        self.cmd( 'sysctl net.ipv4.ip_forward=0' )
        super( LinuxRouter, self ).terminate()

class customTopo(Topo):
        def build( self, **_opts ):
                #R1 = self.addNode( 'r1', cls=LinuxRouter, ip=defaultIP )
                R1 = self.addNode( 'R1', cls=LinuxRouter, ip='10.0.1.1/24',defaultRoute='via 11.0.1.2')
                R2 = self.addNode('R2',cls=LinuxRouter,ip='10.0.2.1/24',defaultRoute='via 11.0.4.2')
                R3 = self.addNode('R3',cls=LinuxRouter,ip='10.0.3.1/24',defaultRoute='via 11.0.3.1')
                R4 = self.addNode('R4',cls=LinuxRouter,ip='10.0.4.1/24',defaultRoute='via 11.0.2.1')
                switch1 = self.addSwitch('s1')
                switch2 = self.addSwitch('s2')
                H1 = self.addHost('H1',ip='10.0.1.2/24',defaultRoute='via 10.0.1.1')
                H7 = self.addHost('H7',ip='10.0.2.2/24',defaultRoute='via 10.0.2.1')
                for i in range(2,5):
                       h1 = self.addHost('H%d'%i,ip='10.0.3.%d'%i,defaultRoute='via 10.0.3.1')
                       self.addLink(h1,switch1)
                for i in range(5,7):
                       h1 = self.addHost('H%d'%i,ip = '10.0.4.%d'%i,defaultRoute='via 10.0.4.1')
                       self.addLink(h1,switch2)

                self.addLink(switch1,R3,intfName2='R3-eth0',params2={'ip':'10.0.3.1/24'})
                self.addLink(switch2,R4,intfName2='R4-eth0',params2={'ip':'10.0.4.1/24'})
                self.addLink(H1,R1,intfName2='R1-eth0',params2={'ip' : '10.0.1.1/24'})
                self.addLink(H7,R2,intfName2='R2-eth0',params2={'ip':'10.0.2.1/24'})

                #connection among routers
                self.addLink(R1,R2,intfName1='R1-eth1',intfName2='R2-eth1',params1={'ip':'11.0.1.1/24'},params2={'ip':'11.0.1.2/24'})
                self.addLink(R3,R4,intfName1='R3-eth1',intfName2='R4-eth1',params1={'ip':'11.0.2.1/24'},params2={'ip':'11.0.2.2/24'})
                self.addLink(R1,R3,intfName1='R1-eth2',intfName2='R3-eth2',params1={'ip':'11.0.3.1/24'},params2={'ip':'11.0.3.2/24'})
                self.addLink(R2,R4,intfName1='R2-eth2',intfName2='R4-eth2',params1={'ip':'11.0.4.1/24'},params2={'ip':'11.0.4.2/24'})

def configureR3subnetHost(host):
    host.cmd('route add -net 10.0.2.0 netmask 255.255.255.0 gw 10.0.3.1')
    host.cmd('route add -net 10.0.1.0 netmask 255.255.255.0 gw 10.0.3.1')
    host.cmd('route add -net 10.0.4.0 netmask 255.255.255.0 gw 10.0.3.1')

def configureR4subnetHost(host):
    host.cmd('route add -net 10.0.2.0 netmask 255.255.255.0 gw 10.0.4.1')
    host.cmd('route add -net 10.0.1.0 netmask 255.255.255.0 gw 10.0.4.1')
    host.cmd('route add -net 10.0.3.0 netmask 255.255.255.0 gw 10.0.4.1')

def configureRouter(routerNo,router):
    with open("R{}_FwdingTbl.txt".format(routerNo)) as f:
        contents = f.readlines()
    contents = [x.strip() for x in contents]
    for i in range(1,len(contents)):
        routeInfo = contents[i]
        routeInfo = routeInfo.replace('\t'," ")
        routeInfo = routeInfo.split()
        #print(routeInfo)
        router.cmd('route add -net {} netmask {} gw {}'.format(routeInfo[0],routeInfo[2],routeInfo[1]))


def createTopo():
        topo = customTopo()
        net = Mininet(topo=topo)
        net.start()
        #configure host
        for i in range(2,7):
            if(i<5):
                configureR3subnetHost(net.get('H%i'%i))
            else:
                configureR4subnetHost(net.get('H%i'%i))

        #configure Router
        for i in range(1,5):
            print("configuring Router %i......."%i)
            configureRouter(i,net.get('R%i'%i))
        CLI(net)
        net.pingAll()
        net.stop()

if __name__ == '__main__':
        setLogLevel('info')
        createTopo()

