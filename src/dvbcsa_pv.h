/*

    This file is part of libdvbcsa.

    libdvbcsa is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 2 of the License,
    or (at your option) any later version.

    libdvbcsa is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libdvbcsa; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA

    (c) 2006-2008 Alexandre Becoulet <alexandre.becoulet@free.fr>

*/

#ifndef DVBCSA_PV_H_
# define DVBCSA_PV_H_


#include <stdlib.h>
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include <inttypes.h>
#include <stdint.h>
#include <time.h>

#include <curand.h>
#include <math.h>
#include <assert.h>

#if !defined(DVBCSA_DEBUG) && defined(__GNUC__)
#define DVBCSA_INLINE __attribute__ ((always_inline))
#else
#define DVBCSA_INLINE
#endif

/** types used for type-punning */
#ifdef HAVE_ATTRIBUTE_MAY_ALIAS
typedef uint8_t  dvbcsa_u8_aliasing_t;
typedef uint16_t dvbcsa_u16_aliasing_t;
typedef uint32_t dvbcsa_u32_aliasing_t;
typedef uint64_t dvbcsa_u64_aliasing_t;
#else
typedef uint8_t  dvbcsa_u8_aliasing_t;
typedef uint16_t dvbcsa_u16_aliasing_t;
typedef uint32_t dvbcsa_u32_aliasing_t;
typedef uint64_t dvbcsa_u64_aliasing_t;
#endif

void worddump(char *str, void *data, size_t len, size_t ws);

#define DVBCSA_DATA_SIZE        8
#define DVBCSA_KEYSBUFF_SIZE    56
#define DVBCSA_CWBITS_SIZE      64

typedef uint8_t                 dvbcsa_block_t[DVBCSA_DATA_SIZE];
typedef uint8_t                 dvbcsa_keys_t[DVBCSA_KEYSBUFF_SIZE];

typedef struct dvbcsa_key_s     dvbcsa_key_t;
typedef unsigned char           dvbcsa_cw_t[8];


struct  dvbcsa_key_s
{
    dvbcsa_cw_t           cw;
    dvbcsa_cw_t           cws;    /* nibble swapped CW */
    dvbcsa_keys_t         sch;


};

struct dvbcsa_key_s * __device__   dvbcsa_key_alloc(void);
void  __device__  dvbcsa_key_free(struct dvbcsa_key_s *key);
void  __device__  dvbcsa_key_set(dvbcsa_cw_t cw, struct dvbcsa_key_s *key);
void  __device__  dvbcsa_decrypt(struct dvbcsa_key_s *key, uint8_t *data, unsigned int len);
void  __device__  dvbcsa_block_decrypt(dvbcsa_keys_t key, dvbcsa_block_t in, dvbcsa_block_t out);
void  __device__  dvbcsa_key_schedule_block(uint64_t cw, uint8_t * kk);
void  __device__  dvbcsa_stream_xor(dvbcsa_cw_t cw, dvbcsa_block_t iv, uint8_t *stream, unsigned int len);

DVBCSA_INLINE static inline uint32_t   __device__ dvbcsa_load_le32(uint8_t *p)
{
    return ((uint32_t)p[3] << 24) |
            ((uint32_t)p[2] << 16) |
            ((uint32_t)p[1] << 8 ) |
             (uint32_t)p[0];
}

static uint64_t  __device__ dvbcsa_load_le64(uint8_t *p)
{
   return (uint64_t)( ((uint64_t)p[7] << 56) |
            ((uint64_t)p[6] << 48) |
            ((uint64_t)p[5] << 40) |
            ((uint64_t)p[4] << 32) |
            ((uint64_t)p[3] << 24) |
            ((uint64_t)p[2] << 16) |
            ((uint64_t)p[1] << 8 ) |
             (uint64_t)p[0]
            );
}

DVBCSA_INLINE static inline  __device__ void
dvbcsa_store_le64(uint8_t *p, uint64_t w)
{
    memcpy(p, &w, 8);
}

int csa_cu_decrypt(uint8_t *cw, unsigned char *data);

#endif

