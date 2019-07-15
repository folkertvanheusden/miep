#include <endian.h>
#include <string.h>

#include "error.h"
#include "debug.h"
#include "memory.h"

memory::memory() : pm(NULL), len(0)
{
}

memory::memory(uint64_t size, bool init) : len(size)
{
	if (size == 0)
		error_exit("memory::memory invalid size");

	pm = new unsigned char[size];

	if (init)
		memset(pm, 0x00, size);
}

memory::memory(unsigned char *p, uint64_t size) : pm(p), len(size)
{
	if (size == 0)
		error_exit("memory::memory invalid size");
}

memory::~memory()
{
	delete [] pm;
}

void memory::read_64b(uint64_t real_addr, uint64_t offset, uint64_t *data)
{
	ASSERT(offset + 7 < len);

	unsigned char *dummy_p = &pm[offset];

	*data = be64toh(*(uint64_t *)dummy_p);
}

void memory::read_32b(uint64_t real_addr, uint64_t offset, uint32_t *data)
{
	ASSERT(offset + 3 < len);

	*data = be32toh(*(uint32_t *)&pm[offset]);
}

void memory::read_16b(uint64_t real_addr, uint64_t offset, uint16_t *data)
{
	ASSERT(offset + 1 < len);

	*data = be16toh(*(uint16_t *)&pm[offset]);
}

void memory::read_8b(uint64_t real_addr, uint64_t offset, uint8_t *data)
{
	ASSERT(offset < len);

	*data = pm[offset];
}

void memory::write_64b(uint64_t real_addr, uint64_t offset, uint64_t data)
{
	ASSERT(offset + 8 <= len);

	*(uint64_t *)&pm[offset] = htobe64(data);
}

void memory::write_32b(uint64_t real_addr, uint64_t offset, uint32_t data)
{
	ASSERT(offset + 4 <= len);

	*(uint32_t *)&pm[offset] = htobe32(data);
}

void memory::write_16b(uint64_t real_addr, uint64_t offset, uint16_t data)
{
	ASSERT(offset + 2 <= len);

	*(uint16_t *)&pm[offset] = htobe16(data);
}

void memory::write_8b(uint64_t real_addr, uint64_t offset, uint8_t data)
{
	ASSERT(offset < len);

	pm[offset] = data;
}
