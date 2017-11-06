define($src_ip_addr 192.168.1.1, $dest_ip_addr 192.168.1.2)
define($src_mac_addr 02:67:1e:2b:a3:27, $dest_mac_addr 3e:60:c2:99:00:68)
define($dev1 veth1, $dev2 veth2)

FromDevice($dev2)
		//Strip Ether header.		
		->Print("Receiving Packet.")
		->Strip(14)
		->Print("Strip Enther Header")
		// Strip IP header.
		->Strip(20)
		->Print("Receiving Packets from Dev2", CONTENTS ASCII)
		->Queue
		->IPEncap(4, $dest_ip_addr, $src_ip_addr)
		->EtherEncap(0x0800, $dest_mac_addr, $src_mac_addr)
		->Print("Sending Packets to Dev2")
		->ToDevice($dev2)