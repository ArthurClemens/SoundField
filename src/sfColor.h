#pragma once

#ifndef _COLOR
#define _COLOR

#define DEBUG 0

class sfColor {

public:
	int a, r, g, b;
	sfColor(uint32 hex) {
//		if (DEBUG) displayBits(hex);
	
		a = (hex >> 24) &0xFF;
		r = (hex >> 16) &0xFF;
		g = (hex >> 8) &0xFF;
		b = (hex >> 0) &0xFF;
	}
	
	void displayBits( uint32 value ) {
		const int SHIFT = 8 * sizeof( unsigned ) - 1;
		const unsigned MASK = 1 << SHIFT;
		
		cout << setw( 10 ) << value << " = ";
		
		for ( unsigned i = 1; i <= SHIFT + 1; i++ ) {
			cout << ( value & MASK ? '1' : '0' );
			value <<= 1;
			if ( i % 8 == 0 )
				cout << ' ';
		}
		cout << endl;
	}
};

#endif

/*
 m_alpha = (hex & 0xFF);
 m_blue   = ((hex>>=8) & 0xFF);
 m_green = ((hex>>=8) & 0xFF);
 m_red     = ((hex >>8) & 0xFF);
 
 
 
 var colour:uint = 0xFF33CC99;
 
 var A:Number = colour >> 24 & 0xFF;
 var R:Number = colour >> 16 & 0xFF;
 var G:Number = colour >> 8 & 0xFF;
 var B:Number = colour & 0xFF
 
a = (aRGB >> 24) &0xFF 
r = (aRGB >> 16) &0xFF 
g = (aRGB >> 8) &0xFF 
b = (aRGB >> 0) &0xFF 
*/