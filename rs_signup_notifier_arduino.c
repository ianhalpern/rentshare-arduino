/* RS signup notifier
 * -----------
 *
 *
 * (cleft) 2005 D. Cuartielles for K3
 * (cleft) Refactoring and comments 2006 clay.shirky@nyu.edu
 * Modified 2011 ian@ian-halpern.com
 */

// TONES  ==========================================
// Start by defining the relationship between
//       note, period, &  frequency.
#define  note_c1     3830    // 261 Hz
#define  note_d1     3400    // 294 Hz
#define  note_e1     3038    // 329 Hz
#define  note_f1     2864    // 349 Hz
#define  note_g1     2550    // 392 Hz
#define  note_a1     2272    // 440 Hz
#define  note_as1     2172    // 440 Hz
#define  note_b1     2028    // 493 Hz
#define  note_c2     1912    // 523 Hz
#define  note_d2 1700
#define  note_e2 1519
#define  note_f2 1432
#define  note_g2 1275
#define  note_a2 1136
#define  note_as2 1075
#define  note_b2 1014
#define  note_c3 956
// Define a special note, 'R', to represent a rest
#define  note_r     0

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
int melody[] = {  note_c2*2,  note_c3*2,  note_a1*2,  note_a2*2, note_as1*2, note_as2*2, note_r };
int beats[]  = { 16, 10, 16, 10, 16, 10, 32 };
int MAX_COUNT = sizeof(melody) / 2; // Melody length, for looping.

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

void playMelody() {
  // Set up a counter to pull from melody[] and beats[]
  for (int i=0; i<MAX_COUNT; i++) {
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
                if ( incomingByte == 115 ) {
                  digitalWrite(ledPin, HIGH);   // sets the LED on
                  delay(1000);                  // waits for a second
                  digitalWrite(ledPin, LOW);    // sets the LED off
                  playMelody();
                }
	}

  
}
