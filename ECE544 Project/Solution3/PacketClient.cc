#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/timer.hh>
#include <click/packet.hh>
#include "Packet.hh"
#include "PacketClient.hh"
CLICK_DECLS

PacketClient::PacketClient() : _timer(this)
{
	sequenceNumber = 1;
}
PacketClient::~PacketClient() {}

int PacketClient::configure(Vector<String> &conf, ErrorHandler *errh)
{
	if (cp_va_kparse(conf, this, errh, "SrcAddr", cpkM, cpUnsigned, &this->srcAddr,
		"DestAdr", cpkM, cpUnsigned, &this->destAddr, cpEnd) < 0) return -1;
	return 0;
}
int PacketClient::initialize(ErrorHandler *errh)
{
	_timer.initialize(this);
	_timer.schedule_now();
	return 0;
}

void PacketClient::run_timer(Timer *timer)
{
	assert(timer == &_timer);
	this->helloMessage();
	this->sendRequest();

	// Wait for 2s, periodically send hello message.
	_timer.reschedule_after_msec(2000);
}

void PacketClient::sendRequest()
{
	// If get the ack, send the next sequence
	// Otherwise, resend the sequence
	if (ackSequenceTable.get(sequenceNumber) == true)
	{
		this->sequenceNumber++;
	}

	WritablePacket *packet = Packet::make(0, 0, sizeof(struct PacketHeader) + 5, 0);
	memset(packet->data(), 0, packet->length());
	struct PacketHeader *format = (struct PacketHeader*) packet->data();
	format->type = 0;
	format->srcAddr = this->srcAddr;
	format->destAddr = this->destAddr;
	format->sequenceNumber = this->sequenceNumber;

	char *data = (char*)(packet->data() + sizeof(struct PacketHeader));
	memcpy(data, "zzzzz", 5);
	output(0).push(packet);
	click_chatter("Send request message sequence %d", this->sequenceNumber);
}

void PacketClient::push(int, Packet *p)
{
	struct PacketHeader *header = (struct PacketHeader*) p->data();
	uint8_t type = header->type;
	uint32_t seq = header->sequenceNumber;
	click_chatter("Receive an ack message sequence %d", seq);
	// If type == 1, then receive an ack message
	if (type == 1)
	{
		if (seq == sequenceNumber)
			ackSequenceTable.set(seq, true);
	}
}

void PacketClient::helloMessage()
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
EXPORT_ELEMENT(PacketClient)