define($src_ip_addr 192.168.1.1, $dest_ip_addr 192.168.1.2)
define($src_mac_addr 12:5f:21:75:26:d6, $dest_mac_addr 22:d8:5e:0f:70:1a)
define($dev1 veth1, $dev2 veth2)

FromDevice($dev2)		
		->Print("Receiving Packet.")				
		->Strip(14)  //Strip Ether header.
		->Print("Strip Enther Header")		
		->Strip(20)  // Strip IP header.
		->Print("Dev2 Receive Packets", CONTENTS ASCII)
		->Queue
		->IPEncap(4, $dest_ip_addr, $src_ip_addr)
		->EtherEncap(0x0800, $dest_mac_addr, $src_mac_addr)
		->Print("Dev2 Send Packets")
		->ToDevice($dev2)