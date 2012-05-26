#define BIT_ISSET( flags, flag ) ( ( (flags) & (flag) ) == (flag) )

#define SegA 9
#define SegB 10
#define SegC 11
#define SegD 12
#define SegE 13
#define SegF 8
#define SegG 7

#define Dig0 0
#define Dig1 1
#define Dig2 2
#define Dig3 3
#define Dig4 4
#define Dig5 5
#define Dig6 6

#define REFRESH_RATE 1

#define SPACE 36

#define C( c ) ( (c) - 87 )

const char seg_num_to_pin[]    = { SegA, SegB, SegC, SegD, SegE, SegF, SegG };
const char digit_num_to_pin[]  = { Dig0, Dig1, Dig2, Dig3, Dig4, Dig5, Dig6 };
const char seg_num_to_bit[]    = { 0b1000000, 0b0100000, 0b0010000, 0b0001000, 0b0000100, 0b0000010, 0b0000001 };
const char char_to_seg_map[] = {
  /* digits */
  0b1111110 /*0*/, 0b0110000 /*1*/, 0b1101101 /*2*/, 0b1111001 /*3*/, 0b0110011 /*4*/,
  0b1011011 /*5*/, 0b0011111 /*6*/, 0b1110000 /*7*/, 0b1111111 /*8*/, 0b1110011 /*9*/,

  /* letters */
  0b1110111 /*A*/, 0b0011111 /*B*/, 0b1001110 /*C*/, 0b0111101 /*D*/, 0b1001111 /*E*/, 0b1000111 /*F*/,
  0b1111011 /*G*/, 0b0110111 /*H*/, 0b0000110 /*I*/, 0b0111000 /*J*/, 0b1111110 /*K*/, 0b0001110 /*L*/,
  0b0110111 /*M*/, 0b0010101 /*N*/, 0b1111110 /*O*/, 0b1100111 /*P*/, 0b1111110 /*Q*/, 0b0000101 /*R*/,
  0b1011011 /*S*/, 0b1110000 /*T*/, 0b0111110 /*U*/, 0b0111110 /*V*/, 0b1111110 /*W*/, 0b1100111 /*X*/,
  0b0110111 /*Y*/, 0b0110000 /*Z*/,

  /* blank */
  0b0000000 };


char display[7] = { SPACE, SPACE, SPACE, 1, 0, 0, 0 };

void setup() {
	for ( char i = 0; i < sizeof( seg_num_to_pin ); i++ )
		pinMode( seg_num_to_pin[i], OUTPUT );

	for ( char i = 0; i < sizeof( digit_num_to_pin ); i++ )
		pinMode( digit_num_to_pin[i], OUTPUT );

	clear();
}

void clear() {
	for ( char i = 0; i < sizeof( seg_num_to_pin ); i++ )
		digitalWrite( seg_num_to_pin[i], LOW );

	for ( char i = 0; i < sizeof( digit_num_to_pin ); i++ )
		digitalWrite( digit_num_to_pin[i], HIGH );
}

bool digitContainsSeg( char digit, char seg ) {
	return BIT_ISSET( char_to_seg_map[ digit ], seg_num_to_bit[ seg ] );
}

void draw() {
	for ( char seg_num = 0; seg_num < sizeof( seg_num_to_pin ); seg_num++ ) {
		digitalWrite( seg_num_to_pin[ seg_num ], HIGH );

		for ( char j = 0; j < sizeof( display ); j++ )
			if ( digitContainsSeg( display[j], seg_num ) )
				digitalWrite( digit_num_to_pin[j], LOW );

		delay( REFRESH_RATE );
		clear();
	}
}

unsigned int t = 0;

void loop() {
	draw();

	/*
	t++;
	if ( t % 10000 ) {
		for ( char c = 0; c < 1000; c++ ) {
			char pos = 6;
			if ( display[pos] == 9 )
				while ( display[pos] == 9 ) {
					display[pos] = 0;
					pos--;
				}
			display[pos]++;
		//	for ( char i = 0; i < 1000 / ( REFRESH_RATE * 7 ); i++ )
				draw();
		}
	}*/
}


