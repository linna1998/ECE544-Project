require (library /home/comnetsii/elements/routerport.click);

define ($dev veth1, $in_mac 0e:5a:e3:1e:f3:79, $out_mac 8a:eb:b0:aa:1c:88)

rp::RouterPort(DEV $dev, IN_MAC $in_mac, OUT_MAC $out_mac)

rp	->PacketSender()
	->rp