
#ifndef GN_BUFF_H
# define GN_BUFF_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define EBUFF	1

# ifndef STATIC_GN_BUFF_SIZE
#  define STATIC_GN_BUFF_SIZE
#  define GN_BUFF_NOT_STATIC
# endif

typedef struct rn_buff_s gn_buff_t;

typedef struct rn_buff_s
{
	size_t		write_i;
	size_t		read_i;
	size_t		buff_size;
	gn_buff_t	*next;
	uint8_t		buff[STATIC_GN_BUFF_SIZE];
} gn_buff_t;

gn_buff_t *rn_buff_new(size_t buff_size);
ssize_t rn_buff_read(gn_buff_t * rb, void *buf, size_t n);
ssize_t rn_buff_write(gn_buff_t * rb, void *buf, size_t n);


#endif
