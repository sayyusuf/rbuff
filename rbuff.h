#ifndef RBUFF_H
#define RBUFF_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#if defined(__GNUC__)

#ifndef _Nonnull
#define _Nonnull __attribute__((nonnull))
#endif

#ifndef _Nullable
#define _Nullable
#endif

#endif

#define EBUFF	1
#define EEOB	2
#define EFULL	3
#define EEMPTY	4

#define LL_BUFF_STD_SIZE	512

typedef struct rbuff_s rbuff_t;

typedef struct rbuff_s
{
	size_t		wi;
	size_t		ri;
	size_t		bfs;
	rbuff_t		*next;
	uint8_t		buff[];
} rbuff_t;

rbuff_t	*
rbuff_new(size_t buff_size);

int
rbuff_init(rbuff_t **rb, void *_Nullable ptr, size_t n);


ssize_t
rbuff_read(rbuff_t * rb, void *buff, size_t n);

ssize_t
rbuff_write(rbuff_t * rb, void *buff, size_t n);


ssize_t
lbuff_read(rbuff_t **ll, void *buff, size_t n);

ssize_t
lbuff_write(rbuff_t **ll, void *buff, size_t n);

ssize_t
lbuff_size(rbuff_t *ll);

void
lbuff_del(rbuff_t *ln);


#ifdef __cplusplus
}
#endif

#endif
