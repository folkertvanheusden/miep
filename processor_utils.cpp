#include <stdlib.h>

#include "debug.h"
#include "optimize.h"
#include "processor_utils.h"

int64_t untwos_complement(uint64_t value, uint8_t bits)
{
	ASSERT(bits >= 1 && bits <= 64);

        if (IS_BIT_OFF0_SET(bits - 1, value))
                return -((1ll << bits) - value); // FIXME fails with 64 bit?

	return value;
}

uint64_t twos_complement(int64_t value, uint8_t bits)
{
	ASSERT(bits >= 1 && bits <= 64);

	if (value >= 0)
		return value;

	uint64_t mask = MASK_N(bits);

	return value & mask;
}

uint8_t count_leading_ones(uint8_t n_bits, uint64_t value)
{
	ASSERT(n_bits >= 1 && n_bits <= 64);

	uint64_t mask = 1ll << (n_bits - 1);

	uint8_t nr = 255;
	for(int i=n_bits-1; i>=0; i--) {
		if ((value & (1ll << i)) == 0) {
			nr = n_bits - 1 - i;
			break;
		}
	}

	return nr;
}

uint8_t count_leading_zeros(uint8_t n_bits, uint64_t value)
{
	ASSERT(n_bits >= 1 && n_bits <= 64);

	uint64_t mask = 1ll << (n_bits - 1);

	uint8_t nr = 255;
	for(int i=n_bits-1; i>=0; i--) {
		if (value & (1ll << i)) {
			nr = n_bits - 1 - i;
			break;
		}
	}

	return nr;
}

uint64_t rotate_right(uint64_t value, uint8_t n, uint8_t width)
{
	ASSERT(n >= 0 && n <= 64);
	ASSERT(width >= 0 && width <= 64);

	uint64_t right_bits = value & MASK_N(n);

	value >>= n;

	value |= right_bits << (width - n);

	return value;
}

uint64_t sign_extend_8b(uint8_t value)
{
	if (value & 128)
		return uint64_t(value) | ~0xffll;

	return value;
}

uint64_t sign_extend_16b(uint16_t value)
{
	if (value & 32768)
		return uint64_t(value) | ~0xffffll;

	return value;
}

uint64_t sign_extend_32b(uint32_t value)
{
	if (IS_BIT_OFF0_SET(31, value))
		return uint64_t(value) | ~0xffffffffll;

	return value;
}

// FIXME need to optimize this
int64_t sign_extend(int64_t value, uint8_t bits)
{
	ASSERT(bits >= 1 && bits <= 64);

	int mask = 1 << (bits - 1);

	if (value & mask)
	{
		while(bits <= 64)
		{
			value |= mask;

			bits++;
			mask <<= 1;
		}
	}
	else
	{
		while(bits <= 64)
		{
			value &= ~mask;

			bits++;
			mask <<= 1;
		}
	}

	return value;
}

bool test_tc_overflow_32b(int32_t val1, int32_t val2)
{
	int32_t result = val1 + val2;

	if (val1 > 0 && val2 > 0 && result <= 0)
		return true;

	if (val1 < 0 && val2 < 0 && result >= 0)
		return true;

	return false;
}

uint32_t make_cmd_I_TYPE(uint8_t rs, uint8_t rt, uint8_t function, int immediate)
{
	ASSERT(rs >=0 && rs <= 31);
	ASSERT(rt >=0 && rt <= 31);
	ASSERT(function >=0 && function <= 63);
	ASSERT(immediate >=0 && immediate <= 0xffff);

	return (function << 26) |
		(rs << 21) |
		(rt << 16) |
		immediate;
}

uint32_t make_cmd_SPECIAL(uint8_t rt, uint8_t rd, uint8_t sa, uint8_t function, uint8_t extra)
{
	ASSERT(rt >=0 && rt <= 31);
	ASSERT(rd >=0 && rd <= 31);
	ASSERT(sa >=0 && sa <= 31);
	ASSERT(function >=0 && function <= 63);
	ASSERT(extra >=0 && extra <= 31);

	return function |
		(sa << 6) |
		(rd << 11) |
		(rt << 16) |
		(extra << 21);
}

uint32_t make_cmd_J_TYPE(uint8_t function, int target)
{
	ASSERT(function >=0 && function <= 63);
	ASSERT(target >=0 && target <= MASK_26B);

	return (function << 26) |
		target;
}

uint32_t make_cmd_R_TYPE(uint8_t opcode, uint8_t sa, uint8_t rd, uint8_t rt, uint8_t rs, uint8_t function)
{
	return (opcode << 26) |
		(rs << 21) |
		(rt << 16) |
		(rd << 11) |
		(sa << 6) |
		function;
}
