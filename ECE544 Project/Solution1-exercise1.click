//
// Create 10 'Hello' message, 1 per second, print and discard.
//

RatedSource(DATA "Hello", RATE 1, LIMIT 10)
		->Print(DATA, CONTENTS ASCII)
		->Discard;
