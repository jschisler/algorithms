//  Defining a Bit Field 
struct bitfield { 
	unsigned one : 1; 
	unsigned two : 2; 
	unsigned : 2; 
	unsigned three : 3; 
};

//  OMG I love this
bool power_of_two(unsigned x) { 
	if (!x) 
		return false; 
	
	return (x & (x - 1)) == 0; 
}

unsigned clear_last_bit(unsigned x) { 
	return x & (x - 1); 
}

unsigned lowest_set_bit(unsigned x) { 
	return x & ~(x - 1); 
}

unsigned count_bits_set(unsigned x) { 
	unsigned count = 0; 
	
	for (int index = 0; index < 32; ++index) { 
		if (x & (0x1 << index)) 
			++count; 
	} 
	
	return count; 
}

unsigned count_bits_set_better(unsigned x) { 
	unsigned count = 0; 
	
	while (x) { 
		x = clear_last_bit(x); 
		++count; 
	} 
	
	return count; 
}

/*
	Folding a value over itself another standard bit twiddling trick. In a sense
	, folding is the opposite of clearing the last bit. The result of folding is 
	setting all bits after the most significant.

	Understand what this code is achieving. In the first operation, every bit 
	sets the bit to its immediately right. In the next, every bit sets the bit 
	two over, then the next sets the bit four away, etc. If the top bit is 
	initially set, the first iteration sets two bits. The second sets four bits, 
	and this continues until the last operation successfully sets all bits.
	
	The operation is defined below for 32 bit integers:
*/
unsigned fold_over(unsigned x) { 
	x |= (x >> 1); 
	x |= (x >> 2); 
	x |= (x >> 4); 
	x |= (x >> 8); 
	x |= (x >> 16); 
	
	return x; 
}

/*
	Our first application of folding is to isolate the highest set bit. This 
	is done by masking off all but the most significant bit after folding.
*/
unsigned int highest_set_bit(unsigned int x) {
	x = fold_over(x);

	return (x & ~(x >> 1));
}

//  Calculate log 2x 
unsigned int log_x(unsigned int x) { 
	x = x - 1; 
	x = fold_over(x); 
	x = x + 1; 
	
	for (auto index = 0; index < 32; ++index) { 
		if (x == (0x1 << index)) 
			return index; 
	} 
	
	return 0; 
}

unsigned next_power_of_two_after(unsigned x) {
	x = fold_over(x);
	return x + 1;
}

/*
	A modification of the successive shifting used in folding can be used to 
	reverse the bits of an unsigned integer in a clever way.The idea is to 
	swap the top and bottom halves of an integer, then recursively swap the 
	top and bottom halves of the swapped halves until neighbors are swapped.
	
	Once neighbors are swapped, the bits of the integer have been reversed.
*/
unsigned reverse_bits(unsigned x) { 
	x = ((x & 0xffff0000) >> 16) | ((x & 0x0000ffff) << 16); 
	x = ((x & 0xff00ff00) >> 8)  | ((x & 0x00ff00ff) << 8); 
	x = ((x & 0xf0f0f0f0) >> 4)  | ((x & 0x0f0f0f0f) << 4); 
	x = ((x & 0xcccccccc) >> 2)  | ((x & 0x33333333) << 2); 
	x = ((x & 0xaaaaaaaa) >> 1)  | ((x & 0x55555555) << 1); 
	
	return x; 
}

int multiply(int x, int y) { 
	int product = 0; 
	
	while (y) { 
		product += x << log_x(lowest_set_bit(y));
		y = clear_last_bit(y);
	} 
	
	return product; 
}

int divide(int x, int y) { 
	auto x_neg = x < 0; 
	auto y_neg = y < 0; 
	
	if (x_neg) 
		x = ~x + 1; 
	
	if (y_neg) 
		y = ~y + 1; 
	
	unsigned remainder = 0; 
	unsigned quotient = 0; 
	
	for (auto i = 32; i > 0; --i) { 
		quotient <<= 1; 
		remainder = (remainder << 1) + ((x & (1 << (i - 1))) != 0); 
		
		if (remainder >= y) { 
			quotient |= 1; 
			remainder = remainder - y; 
		} 
	}
	
	if (x_neg != y_neg) 
		quotient = ~quotient + 1; 
	
	return quotient; 
}