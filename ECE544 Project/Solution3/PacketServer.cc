#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/timer.hh>
#include <click/packet.hh>
#include "Packet.hh"
#include "PacketServer.hh"

CLICK_DECLS
PacketServer::PacketServer() : _timer(this) {}
PacketServer::~PacketServer() {}

int PacketServer::initialize(ErrorHandler *errh)
{
	_timer.initialize(this);
	_timer.schedule_now();
	return 0;
}

int PacketServer::configure(Vector<String> &conf, ErrorHandler *errh)
{
	if (cp_va_kparse(conf, this, errh, "SrcAddr", cpkM, 
		cpUnsigned, &this->srcAddr, cpEnd) < 0) return -1;
	return 0;
}

void PacketServer::run_timer(Timer *timer)
{
	assert(timer == &_timer);
	this->helloMessage();
	
	// Wait for 2s, periodically send hello message.
	_timer.reschedule_after_msec(2000);
}

void PacketServer::push(int, Packet *p)
{
	struct PacketHeader *format = (struct PacketHeader*) p->data();
	// Revice type from 0 to 1
	// Send out the ack message for the given sequenceNumber
	format->type = 1;
	// Exchange src and dest addr
	uint32_t in_addr = format->srcAddr;
	uint32_t out_addr = format->destAddr;
	format->srcAddr = out_addr;
	format->destAddr = in_addr;
	
	char* packetContent = (char*)(p->data() + sizeof(struct PacketHeader));
	memcpy(packetContent, "ack", 3);
	output(0).push(p);
	click_chatter("Send out the ack for message sequence %d", format->sequenceNumber);
}

void PacketServer::helloMessage()
{
	WritablePacket *helloPacket = Packet::make(0, 0, sizeof(struct PacketHeader) + 5, 0);
	memset(helloPacket->data(), 0, helloPacket->length());
	struct PacketHeader *format = (struct PacketHeader*) helloPacket->data();
	format->type = 2;  // Type of hello
	format->srcAddr = this->srcAddr;
	format->destAddr = 1;
	output(0).push(helloPacket);

	click_chatter("Send out hello message.");
}

CLICK_ENDDECLS
EXPORT_ELEMENT(PacketServer)