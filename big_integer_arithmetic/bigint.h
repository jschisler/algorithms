#include <vector>

typedef std::vector<unsigned char> bigint;

bigint operator +(const bigint& lhs, const bigint& rhs) {
	bigint sum; 
	auto carry = 0; 
	auto push_term = [&carry, &sum](unsigned char lhs, unsigned char rhs) { 
		unsigned int value = lhs + rhs + carry; 
		unsigned char term = value & 0xff; 
		carry = (value - term) ? 1 : 0;

		sum.emplace(sum.begin(), term); 
	}; 
	
	auto lhs_itr = lhs.rbegin(); 
	auto rhs_itr = rhs.rbegin(); 
	
	while (lhs_itr != lhs.rend() && rhs_itr != rhs.rend()) { 
		push_term(*lhs_itr++, *rhs_itr++); 
	} 
	
	while (lhs_itr != lhs.rend()) { 
		push_term(*lhs_itr++, 0); 
	} 
	
	while (rhs_itr != rhs.rend()) { 
		push_term(*rhs_itr++, 0); 
	} 
	
	if (carry) 
		push_term(0, 0); 
		
	return sum;
}

struct signed_bigint {
	bool negative = false; 
	bigint value;
};

signed_bigint subtract(bigint lhs, bigint rhs);

signed_bigint operator +(const signed_bigint& lhs, const signed_bigint& rhs) { 
	if (lhs.negative == rhs.negative) { 
		signed_bigint retval; 
		retval.negative = lhs.negative; 
		retval.value = lhs.value + rhs.value; 
		return retval; 
	} 
	
	if (rhs.negative) { 
		return subtract(lhs.value, rhs.value); 
	} 
	
	return subtract(rhs.value, lhs.value); 
}

bigint twos_complement(const bigint& b) { 
	bigint retval(b); 
	
	for (auto& node : retval) { 
		node = ~node; 
	} 
	
	retval = retval + bigint(0x1); 
	
	if (retval.size() > b.size()) { 
		retval.erase(retval.begin()); 
	} 
	
	return retval; 
}

bool msb_set(const bigint& b) { 
	return b[0] & 0x80; 
}

signed_bigint subtract(bigint lhs, bigint rhs) { 
	if (msb_set(lhs)) 
		lhs.emplace(lhs.begin(), 0x00); 
	
	if (msb_set(rhs)) 
		rhs.emplace(rhs.begin(), 0x00); 
	
	while (lhs.size() > rhs.size()) { 
		rhs.emplace(rhs.begin(), 0x00); 
	} 
	
	while (rhs.size() > lhs.size()) { 
		lhs.emplace(lhs.begin(), 0x00); 
	} 
	
	rhs = twos_complement(rhs); 
	
	signed_bigint retval; 
	retval.value = lhs + rhs; 
	
	if (retval.value.size() > lhs.size()) { 
		retval.value.erase(retval.value.begin()); 
	} 
	
	retval.negative = msb_set(retval.value); 
	
	if (retval.negative) { 
		retval.value = twos_complement(retval.value); 
	} 
	
	while (retval.value.size() > 1 && !retval.value[0]) { 
		retval.value.erase(retval.value.begin()); 
	} 
	
	return retval; 
}

bigint operator *(const bigint& lhs, const bigint& rhs) { 
	auto mult_term = [](const bigint& lhs, unsigned char term) { 
		bigint prod; 
		unsigned char carry = 0; 
		
		for (auto itr = lhs.rbegin(); itr != lhs.rend(); ++itr) { 
			unsigned int value = (*itr * term) + carry; 
			prod.emplace(prod.begin(), value & 0xff); 
			carry = value >> 8; 
		} 
		
		if (carry) { 
			prod.emplace(prod.begin(), carry); 
		} 
		
		return prod; 
	}; 
	
	bigint prod; 
	
	for (auto& val : rhs) { 
		bigint term = mult_term(lhs, val); 
		prod.push_back(0x00); 
		prod = prod + term; 
	} 
	
	while (prod.size() > 1 && !prod[0]) { 
		prod.erase(prod.begin()); 
	} 
	
	return prod; 
}