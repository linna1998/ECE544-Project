//
// Sends 10 'Hello' mesages, 1 per second
// Specified destination address and interface
// Receive messages from givn interface and print

define($src_ip_addr 192.168.1.1, $dest_ip_addr 192.168.1.2)
define($src_mac_addr 02:67:1e:2b:a3:27, $dest_mac_addr 3e:60:c2:99:00:68)
define($dev1 veth1, $dev2 veth2)

RatedSource(DATA "Hello", RATE 1, LIMIT 10, STOP true)
		->Print("Source DATA")
		->IPEncap(4, $src_ip_addr, $dest_ip_addr)
		->Print("Finish IP Encap")
		->EtherEncap(0x0800, $src_mac_addr, $dest_mac_addr)
		->Print("Finish Ether Encap, sending packet to dev1")
		->ToDevice($dev1)

FromDevice($dev2)
		->Print("Receiving Packets from Dev2")
		->Discard;