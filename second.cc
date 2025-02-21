#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/animation-interface.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

int
main(int argc, char* argv[])
{
    CommandLine cmd(_FILE_);
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NodeContainer nodes;
    nodes.Create(2);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("50Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("5ms"));

    NetDeviceContainer devices;
    devices = pointToPoint.Install(nodes);

    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");

    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    UdpEchoServerHelper echoServer(8080);

    ApplicationContainer serverApps = echoServer.Install(nodes.Get(0));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(20.0));

    UdpEchoClientHelper echoClient(interfaces.GetAddress(0), 8080);
    echoClient.SetAttribute("MaxPackets", UintegerValue(10));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(512));

    ApplicationContainer clientApps = echoClient.Install(nodes.Get(1));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(20.0));

//For ascii trace
AsciiTraceHelper ascii;
pointToPoint.EnableAsciiAll(ascii.CreateFileStream("512.tr"));

//NetAnim
AnimationInterface anim("p2p.xml");

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}



To run this file, let's open a terminal 
	$ cd ns-allinone-3.43/ns-3.43 
	$ ./ns3 run scratch/first.cc  

Open another terminal and go to  
	$ cd ns-allinone-3.43/netanim-3.109/ 
	$ ./NetAnim  
  Open the XML file and check the output in the animator window. 

Open another terminal or use the existing terminal
 	$ cd 
	$ cd tracemetrics-1.4.0
 	$ java -jar tracemetrics.jar
    open tr files 
    
run this code in first terminal (ns-3.43 )
        $ wireshark csma-2-0.pcap
 	static > i/o graph


_________________________________________________________________________________________
save this as filename.plt
.......................
set terminal pdf
set output "filename.pdf"
set title "chart title"
set xlable "x name"
set ylable "y name"
plot "textfilename.txt" using 1:2 with lines title "title"
......................


......................
set terminal pdf
set output "filename.pdf"
set title "chart title"

set boxwidth 0.4
set style full solid

set xlable "x name"
set ylable "y name"

set grid ytics
set xtics rotate by -45 font ",8" nomirror

plot "filename.txt" using 2:xtic(1) title "title name" with boxes lc rgb "red", \
     "filename.txt" using 3:xtic(1) title "title name" with boxes lc rgb "blue" 


sudo apt install gnuplot
gnuplot filename.plt



gnuplot


--------------------------------------------------------------------------------------
set terminal pdf      
set output "SmartPhoneAnalysis.pdf"
set title "Smartphone attributes"
set xlabel "BatteryCapacity(mAh)"
set ylabel "price(USD)"
plot "SmartPhoneData.txt" using 2:4 with lines title "Battery capacity vs price"
replot "SmartPhoneData.txt" using 2:4 with lines title "Screen Size vs price"



set terminal png size 600 ,400
set output "mydata.png"
set title "mydata"
set xlabel "no_of_Days"
set ylabel "speed"
plot "mydata.txt" using 1:2 with linespoint title "speed" lt 4



set terminal pdf
set output "histogram.pdf"
set title "AQI values"
set boxwidth 0.1
set style fill solid
set xlabel "city"
set ylabel "AQI last"
set grid ytics
set xtics rotate by 45
plot "data.txt" using 2:xtic(1) title "AQI last" with boxes lc rgb "blue", \
	""using 3:xtic(1) title "AQ1 current" with boxes lc rgb "red"



-------------------------------------------------------------------------------------

/*
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"                 ////////////////
#include "ns3/netanim-module.h"              /////////////////////

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1   n2   n3   n4
//    point-to-point  |    |    |    |
//                    ================
//                      LAN 10.1.2.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SecondScriptExample");

int
main(int argc, char* argv[])
{
    bool verbose = true;
    uint32_t nCsma = 3;

    CommandLine cmd(_FILE_);
    cmd.AddValue("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
    cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);

    cmd.Parse(argc, argv);

    if (verbose)
    {
        LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

    nCsma = nCsma == 0 ? 1 : nCsma;

    NodeContainer p2pNodes;
    p2pNodes.Create(2);

    NodeContainer csmaNodes;
    csmaNodes.Add(p2pNodes.Get(1));
    csmaNodes.Create(nCsma);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("10Mbps"));  ////////////
    pointToPoint.SetChannelAttribute("Delay", StringValue("3ms"));  /////////////

    NetDeviceContainer p2pDevices;
    p2pDevices = pointToPoint.Install(p2pNodes);

    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("175Mbps"));   ///////////
    csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(500))); ///////////

    NetDeviceContainer csmaDevices;
    csmaDevices = csma.Install(csmaNodes);

    InternetStackHelper stack;
    stack.Install(p2pNodes.Get(0));
    stack.Install(csmaNodes);

    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");   /////////////////
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign(p2pDevices);

    address.SetBase("10.1.2.0", "255.255.255.0");   /////////////////
    Ipv4InterfaceContainer csmaInterfaces;
    csmaInterfaces = address.Assign(csmaDevices);

    UdpEchoServerHelper echoServer(8080); ///////////////

    ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(2)); //////////
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(csmaInterfaces.GetAddress(2), 8080); /////////////
    echoClient.SetAttribute("MaxPackets", UintegerValue(2));    ////////////
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));   ////////
    echoClient.SetAttribute("PacketSize", UintegerValue(2048)); ////////////

    ApplicationContainer clientApps = echoClient.Install(p2pNodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    
    pointToPoint.EnablePcapAll("second");
    csma.EnablePcap("csma", csmaDevices.Get(1), true);    /////////////
    csma.EnablePcap("p2p", p2pDevices.Get(0), true);    /////////////
    
    AnimationInterface anim("Ass03.xml");   ////////
    anim.SetConstantPosition(p2pNodes.Get(0),10.0,10.0);    /////////////
    anim.SetConstantPosition(p2pNodes.Get(1),30.0,20.0);       /////////////
    anim.SetConstantPosition(csmaNodes.Get(1),80.0,30.0);    /////////////
    anim.SetConstantPosition(csmaNodes.Get(2),50.0,40.0);    /////////////
    anim.SetConstantPosition(csmaNodes.Get(3),20.0,50.0);    /////////////
    
    AsciiTraceHelper ascii;    /////////////
    pointToPoint.EnableAsciiAll(ascii.CreateFileStream("p2p.tr"));    /////////////
    csma.EnableAsciiAll(ascii.CreateFileStream("csma.tr"));    /////////////

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}