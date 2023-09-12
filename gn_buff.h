
#ifndef GN_BUFF_H
# define GN_BUFF_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define EBUFF	1
#define EEOB	2
#define EFULL	3
#define EEMPTY	4

# ifndef STATIC_GN_BUFF_SIZE
#  define STATIC_GN_BUFF_SIZE
#  define GN_BUFF_NOT_STATIC
# endif

#ifndef LL_BUFF_STD_SIZE
# define LL_BUFF_STD_SIZE 512
#endif 

typedef struct gn_buff_s gn_buff_t;

typedef struct gn_buff_s
{
	size_t		wi;
	size_t		ri;
	size_t		bfs;
	gn_buff_t	*next;
	uint8_t		buff[STATIC_GN_BUFF_SIZE];
} gn_buff_t;

gn_buff_t *gn_buff_new(size_t buff_size);
int	gn_buff_init(gn_buff_t **rb, size_t buff_size);

ssize_t rn_buff_read(gn_buff_t * rb, void *buff, size_t n);
ssize_t rn_buff_write(gn_buff_t * rb, void *buff, size_t n);

ssize_t fw_buff_read(gn_buff_t * rb, void *buff, size_t n);
ssize_t fw_buff_write(gn_buff_t * rb, void *buff, size_t n);

ssize_t	ll_buff_read(gn_buff_t **ll, void *buff, size_t n);
ssize_t	ll_buff_write(gn_buff_t **ll, void *buff, size_t n);
#endif
