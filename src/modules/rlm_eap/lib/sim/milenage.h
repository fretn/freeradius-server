/**
 * @file rlm_eap/lib/sim/milenage.h
 * @brief 3GPP AKA - Milenage algorithm (3GPP TS 35.205, .206, .207, .208)
 *
 * This file implements an example authentication algorithm defined for 3GPP
 * AKA. This can be used to implement a simple HLR/AuC into hlr_auc_gw to allow
 * EAP-AKA to be tested properly with real USIM cards.
 *
 * This implementations assumes that the r1..r5 and c1..c5 constants defined in
 * TS 35.206 are used, i.e., r1=64, r2=0, r3=32, r4=64, r5=96, c1=00..00,
 * c2=00..01, c3=00..02, c4=00..04, c5=00..08. The block cipher is assumed to
 * be AES (Rijndael).
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * @copyright 2017 The FreeRADIUS server project
 * @copyright 2006-2007 <j@w1.fi>
 */
#include <stddef.h>

#define MILENAGE_KI_SIZE	16
#define MILENAGE_OPC_SIZE	16
#define MILENAGE_AMF_SIZE	2
#define MILENAGE_SQN_SIZE	6
#define MILENAGE_AK_SIZE	6

/** Copy a 48bit value from a 64bit integer into a uint8_t buff in big endian byte order
 *
 * There may be fast ways of doing this, but this is the *correct*
 * way, and does not make assumptions about how integers are laid
 * out in memory.
 *
 * @param[out] out	6 byte butter to store value.
 * @param[in] i		integer value.
 * @return pointer to out.
 */
static inline uint8_t *uint48_to_buff(uint8_t out[6], uint64_t i)
{
	out[0] = (i & 0xff0000000000) >> 40;
	out[1] = (i & 0x00ff00000000) >> 32;
	out[2] = (i & 0x0000ff000000) >> 24;
	out[3] = (i & 0x000000ff0000) >> 16;
	out[4] = (i & 0x00000000ff00) >> 8;
	out[5] = (i & 0x0000000000ff);

	return out;
}

/** Convert a 48bit big endian value into a unsigned 64bit integer
 *
 */
static inline uint64_t uint48_from_buff(uint8_t const in[6])
{
	uint64_t i = 0;

	i |= ((uint64_t)in[0]) << 40;
	i |= ((uint64_t)in[1]) << 32;
	i |= ((uint32_t)in[2]) << 24;
	i |= ((uint32_t)in[3]) << 16;
	i |= ((uint16_t)in[4]) << 8;
	i |= in[5];

	return i;
}

int	milenage_umts_generate(uint8_t autn[16], uint8_t ik[16], uint8_t ck[16], uint8_t ak[6], uint8_t res[8],
			       uint8_t const opc[16], uint8_t amf[2], uint8_t const k[16],
			       uint64_t sqn, uint8_t const rand[16]);

int	milenage_auts(uint8_t sqn[6],
		      uint8_t const opc[16], uint8_t const k[16], uint8_t const rand[16], uint8_t const auts[14]);

int	milenage_gsm_generate(uint8_t sres[4], uint8_t kc[8],
			      uint8_t const opc[16], uint8_t const k[16], uint8_t const rand[16]);

int	milenage_check(uint8_t ik[16], uint8_t ck[16], uint8_t *res, size_t *res_len, uint8_t *auts,
		       uint8_t const opc[16], uint8_t const k[16], uint8_t const sqn[6],
		       uint8_t const rand[16], uint8_t const autn[16]);