/* ----------------------------------------------------------------------------
 *         SAM Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2016, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "spi-nor.h"
#include "compiler.h"
#include <stdlib.h>

/*----------------------------------------------------------------------------
 *        Local Constants
 *----------------------------------------------------------------------------*/

/** Array of recognized SPI NOR devices. */
static const struct _spi_nor_desc spi_nor_devices[] = {
	/* Name          JEDECID    PgSz         Total Size  Flags */
	{ "AT25DF021",   0x0000431f, 256,        256 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "AT25DF041A",  0x0001441f, 256,        512 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "AT26DF081A",  0x0001451f, 256,   1 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "AT26DF0161",  0x0000461f, 256,   2 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "AT26DF161A",  0x0001461f, 256,   2 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "AT25DF161",   0x0002461f, 256,   2 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "AT25DF321",   0x0000471f, 256,   4 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "AT25DF321A",  0x0001471f, 256,   4 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "AT26DF641",   0x0000481f, 256,   8 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "AT25DF512B",  0x0000651f, 256,         64 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K },
	{ "AT25DF512B",  0x0001651f, 256,         64 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K },
	/* Manufacturer: ST/Micron */
	{ "M25P05",      0x00102020, 256,         64 * 1024, SPINOR_FLAG_ERASE_64K },
	{ "M25P10",      0x00112020, 256,        128 * 1024, SPINOR_FLAG_ERASE_64K },
	{ "M25P20",      0x00122020, 256,        256 * 1024, SPINOR_FLAG_ERASE_64K },
	{ "M25P40",      0x00132020, 256,        512 * 1024, SPINOR_FLAG_ERASE_64K },
	{ "M25P80",      0x00142020, 256,   1 * 1024 * 1024, SPINOR_FLAG_ERASE_64K },
	{ "M25P16",      0x00152020, 256,   2 * 1024 * 1024, SPINOR_FLAG_ERASE_64K },
	{ "M25P32",      0x00162020, 256,   4 * 1024 * 1024, SPINOR_FLAG_ERASE_64K },
	{ "M25P64",      0x00172020, 256,   8 * 1024 * 1024, SPINOR_FLAG_ERASE_64K },
	{ "N25Q032A",    0x0016ba20, 256,   4 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K | SPINOR_FLAG_QUAD },
	{ "N25Q064A",    0x0017ba20, 256,   8 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K | SPINOR_FLAG_QUAD },
	{ "N25Q128A",    0x0018ba20, 256,  16 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K | SPINOR_FLAG_QUAD },
	{ "N25Q256A",    0x0019ba20, 256,  32 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K | SPINOR_FLAG_QUAD },
	/* Manufacturer: Windbond */
	{ "W25X10",      0x001130ef, 256,        128 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K },
	{ "W25X20",      0x001230ef, 256,        256 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K },
	{ "W25X40",      0x001330ef, 256,        512 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K },
	{ "W25X80",      0x001430ef, 256,   1 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K },
	{ "W25Q256",     0x001940ef, 256,  32 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K},
	/* Manufacturer: Macronix */
	{ "MX25L512",    0x001020c2, 256,         64 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "MX25L4005",   0x001320c2, 256,        512 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "MX25L8005",   0x001420c2, 256,       1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "MX25L3205",   0x001620c2, 256,   4 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "MX25L6405",   0x001720c2, 256,   8 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "MX25L12835F", 0x001820c2, 256,  16 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "MX25L51245G", 0x001a20c2, 256,  64 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K | SPINOR_FLAG_QUAD },
	{ "MX66L1G45G",  0x001b20c2, 256, 128 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K | SPINOR_FLAG_QUAD },
	/* Manufacturer: SST */
	{ "SST25VF032",  0x004a25bf, 256,   4 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "SST25VF064",  0x004b25bf, 256,   8 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "SST25VF040B", 0x008d25bf, 256,        512 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	{ "SST25VF080B", 0x008e25bf, 256,       1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_32K | SPINOR_FLAG_ERASE_64K },
	/* Manufacturer: Spansion */
	{ "S25FL116K",   0x00154001, 256,   2 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K | SPINOR_FLAG_QUAD },
	{ "S25FL132K",   0x00164001, 256,   4 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K | SPINOR_FLAG_QUAD },
	{ "S25FL164K",   0x00174001, 256,   8 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K | SPINOR_FLAG_QUAD },
	{ "S25FL128S",   0x00182001, 256,  16 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K | SPINOR_FLAG_QUAD },
	{ "S25FL256S",   0x00190201, 256,  32 * 1024 * 1024, SPINOR_FLAG_ERASE_4K | SPINOR_FLAG_ERASE_64K | SPINOR_FLAG_QUAD },
	{ "S25FL512S",   0x00200201, 256, 256 * 1024 * 1024, SPINOR_FLAG_ERASE_256K | SPINOR_FLAG_QUAD },
};

/*----------------------------------------------------------------------------
 *        Public Functions
 *----------------------------------------------------------------------------*/

const struct _spi_nor_desc *spi_nor_find(uint32_t jedec_id)
{
	int i;

	/* Look for a supported flash */
	for (i = 0; i < ARRAY_SIZE(spi_nor_devices); i++) {
		const struct _spi_nor_desc *desc = &spi_nor_devices[i];
		if (jedec_id == desc->jedec_id)
			return desc;
	}

	return NULL;
}