//
// Create 10 'Hello' message, 1 per second, print and discard.
//

// LIMIT 10: Send 10 packets
// STOP true: Stop the click program after sending the packets
// DATA "hello": Send out the hello data
// RATE 1: Send 1 packet per second
// Discard: Throw the packet away

RatedSource(DATA "Hello", RATE 1, LIMIT 10, STOP true)
		->Print(DATA, CONTENTS ASCII)
		->Discard;

