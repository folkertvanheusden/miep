#include "processor.h"
#include "processor_utils.h"

void processor::special2(uint32_t instruction)
{
	uint8_t clo = instruction & MASK_6B;
	uint8_t rd = get_RD(instruction);
	uint8_t rt = get_RT(instruction);
	uint8_t rs = (instruction >> 21) & MASK_5B;

	int64_t temp_64b = -1;
	int temp;

	switch(clo)
	{
		case 0x02:		// MUL
			temp_64b = int64_t(get_register_32b_signed(rs)) * int64_t(get_register_32b_signed(rt));
			set_register_32b(rd, temp_64b & MASK_32B);
			// LO/HI are said to be unpredictable after this command
			break;

		case 0x1C:		// CLZ
			temp = count_leading_zeros(32, get_register_32b_unsigned(rs));
			if (temp == 255)
				temp = 32;
			set_register_32b(rd, temp);
			set_register_32b(rt, temp);
			break;

		case 0x21:		// CLO
			temp = count_leading_ones(32, get_register_32b_unsigned(rs));
			if (temp == 255)
				temp = 32;
			set_register_32b(rd, temp);
			set_register_32b(rt, temp);
			break;

		default:
			pdc -> dc_log("special2 clo %02x not implemented", clo);
			break;
	}
}
