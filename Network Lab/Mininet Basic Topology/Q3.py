from mininet.topo import Topo
from mininet.net import Mininet
from mininet.nodelib import NAT
from mininet.log import setLogLevel
from mininet.cli import CLI
from mininet.util import dumpNodeConnections
from mininet.util import irange
import thread
from time import sleep
import mininet.node

class InternetTopo(Topo):
    "Single switch connected to n hosts."
    def __init__(self, n=2, **opts):
        Topo.__init__(self, **opts)


        # set up inet switch
        switchS1 = self.addSwitch('s1')
        switchS2 = self.addSwitch('s2')
        for i in range(2):
            host = self.addHost(chr(ord('s') + i))
            self.addLink(switchS2,host)

        publicIntf = "nat-eht2"
        privateIntf = "nat-eht1"
        localIP = '192.168.0.1'
        localSubnet = '192.168.0.0/24'
        natParams = { 'ip' : '192.168.0.1/24' }
        nat = self.addNode('nat',cls=NAT,subnet=localSubnet,inetIntf=publicIntf, localIntf=privateIntf)
        self.addLink(nat,switchS2,intfName1=publicIntf)
        self.addLink(nat,switchS1,intfName1=privateIntf, params1=natParams)

        for i in range(1,4):
            host = self.addHost('h%d'%i,ip='192.168.0.10%d/24' % i,defaultRoute='via 192.168.0.1')
            self.addLink(host,switchS1)

def server(net):
        publicServer = net.get('s')
        print("ip of server s is %s" % publicServer.IP())
        publicServer.cmd('tcpdump -i s-eth0 > connection.txt')

def server1(net):
    publicServer = net.get('t')
    print("ip of server t is %s" % publicServer.IP())
    publicServer.cmd('tcpdump -i t-eth0 > connection1.txt')

def run():
    topo = InternetTopo()
    net = Mininet(topo=topo)
    net.start()
    nat = net.get('nat')
    print("Detailed Connections :")
    dumpNodeConnections(net.hosts)
    print("Nat ip of public interface i.e eth2 is %s"%nat.IP())
    print("Nat ip of private interface is : 192.168.0.1")

    CLI(net)
    privateHost = net.get('h1')
    thread.start_new_thread( server, (net, ) )
    thread.start_new_thread( server1, (net, ) )
    for i in range(1,4):
        host = net.get('h%d'%i)
        print("Ip of node h%d is %s" % (i,host.IP()))
        sIp = net.get('s').IP()
        tIp = net.get('t').IP()
        print("Node h%d is pinging server s" %i)
        privateHost.cmd('ping -c3 %s'%sIp)
        print("Node h%d is pinging server t" %i)
        privateHost.cmd('ping -c3 %s'%tIp)
    net.stop()
    
if __name__ == '__main__':
    setLogLevel('info')
    run()
    print("Network activity at server s")
    with open("connection.txt") as f:
        contents = f.readlines()
    contents = [x.strip() for x in contents]
    for i in range(len(contents)):
        print(contents[i])

    print("Network activity at server t :")
    with open("connection1.txt") as f:
        contents = f.readlines()
    contents = [x.strip() for x in contents]
    for i in range(len(contents)):
        print(contents[i])
