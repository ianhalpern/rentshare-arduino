/* RS signup notifier
 * -----------
 *
 *
 * (cleft) 2005 D. Cuartielles for K3
 * (cleft) Refactoring and comments 2006 clay.shirky@nyu.edu
 * Modified 2011 ian@ian-halpern.com
 */

#define C 0
#define D 1
#define E 2
#define F 3
#define G 4
#define A 5
#define B 6
#define R 7

#define NOTE(n,o) (notes[n]/o)
#define SHARP(n,o) ((notes[n] - (notes[n] - notes[n+1])/2 )/o)
#define FLAT(n,o) ((notes[n] + (notes[n+1] - notes[n])/2 )/o)
#define REST 0

int notes[] = {
	3830,    // C - 261 Hz
	3400,    // D - 294 Hz
	3038,    // E - 329 Hz
	2864,    // F - 349 Hz
	2550,    // G - 392 Hz
	2272,    // A - 440 Hz
	2028    // B- 493 Hz
};


// SETUP ============================================
// Set up speaker on a PWM pin (digital 9, 10 or 11)
int speakerOut = 9;
// Do we want debugging on serial out? 1 for yes, 0 for no

int ledPin = 13;                 // LED connected to digital pin 13
int incomingByte = 0;

void setup() {
  pinMode(speakerOut, OUTPUT);
  pinMode(ledPin, OUTPUT);      // sets the digital pin as output
  Serial.begin(9600);
}

// MELODY and TIMING  =======================================
//  melody[] is an array of notes, accompanied by beats[],
//  which sets each note's relative length (higher #, longer note)
// int melody_mario[] = {  note_c2*2,  note_c3*2,  note_a1*2,  note_a2*2, note_as1*2, note_as2*2, note_r };
int melody_mario[] = {  NOTE(C,1), NOTE(C,2),  NOTE(A,0.5), NOTE(A,1), SHARP(A,0.5), SHARP(A,1), REST };
int beats_mario[]  = { 16, 10, 16, 10, 16, 10, 32 };

// int melody_gadget[] = {  note_c2*2,  note_d2*2,  note_e2*2,  note_f2*2, note_g2*2, note_e2*2, note_fs2*2, note_d2*2, note_f2*2, note_e2*2, note_r };
int melody_gadget[] = { NOTE(C,1), NOTE(D,1), FLAT(E,1), NOTE(F,1), NOTE(G,1), FLAT(E,1), SHARP(F,1), NOTE(D,1),
					    NOTE(F,1), FLAT(E,1), NOTE(C,1), NOTE(D,1), FLAT(E,1), NOTE(F,1), NOTE(G,1), NOTE(C,2), FLAT(B,0.5), REST };
int beats_gadget[]  = { 12, 12, 12, 12, 24, 24, 24, 24, 24, 24, 12, 12, 12, 12, 24, 24, 32, 32 };

int MAX_COUNT = sizeof(melody_mario) / 2; // Melody length, for looping.

// Set overall tempo
long tempo = 10000;
// Set length of pause between notes
int pause = 1000;
// Loop variable to increase Rest length
int rest_count = 100; //<-BLETCHEROUS HACK; See NOTES

// Initialize core variables
int tone_ = 0;
int beat = 0;
long duration  = 0;

// PLAY TONE  ==============================================
// Pulse the speaker to play a tone for a particular duration
void playTone() {
  long elapsed_time = 0;
  if (tone_ > 0) { // if this isn't a Rest beat, while the tone has
    //  played less long than 'duration', pulse speaker HIGH and LOW
    while (elapsed_time < duration) {

      digitalWrite(speakerOut,HIGH);
      delayMicroseconds(tone_ / 2);

      // DOWN
      digitalWrite(speakerOut, LOW);
      delayMicroseconds(tone_ / 2);

      // Keep track of how long we pulsed
      elapsed_time += (tone_);
    }
  }
  else { // Rest beat; loop times delay
    for (int j = 0; j < rest_count; j++) { // See NOTE on rest_count
      delayMicroseconds(duration);
    }
  }
}

void playMelody( int melody[], int beats[], int count ) {
  // Set up a counter to pull from melody[] and beats[]
  for (int i=0; i<count; i++) {
    tone_ = melody[i];
    beat = beats[i];

    duration = beat * tempo; // Set up timing

    playTone();
    // A pause between notes...
    delayMicroseconds(pause);
  }
}
// LET THE WILD RUMPUS BEGIN =============================
void loop() {
  	if (Serial.available() > 0) {
		// read the incoming byte:
		incomingByte = Serial.read();

		// say what you got:
		Serial.print("I received: ");
		Serial.println(incomingByte, DEC);
                if ( incomingByte == 116 ) {
                  digitalWrite(ledPin, HIGH);   // sets the LED on
                  delay(1000);                  // waits for a second
                  digitalWrite(ledPin, LOW);    // sets the LED off
                  playMelody( melody_mario, beats_mario, sizeof(melody_mario) / 2 );
                }
                else if ( incomingByte == 112 ) {
                  digitalWrite(ledPin, HIGH);   // sets the LED on
                  delay(1000);                  // waits for a second
                  digitalWrite(ledPin, LOW);    // sets the LED off
                  playMelody( melody_gadget, beats_gadget, sizeof(melody_gadget) / 2 );
                }
	}


}

