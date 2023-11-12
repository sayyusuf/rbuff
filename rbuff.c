#ifdef __cplusplus
extern "C" {
#endif

#include "rbuff.h"
#include <stddef.h>

rbuff_t 	*
rbuff_new(size_t buff_size)
{
	rbuff_t	*gb;

	gb = malloc(sizeof(*gb) + buff_size);
	if (!gb)
		return (NULL);
	gb->bfs = buff_size;
	gb->wi = 0;
	gb->ri = 0;
	gb->next = NULL;
	return (gb);
}

int
rbuff_init(rbuff_t **gb, void *_Nullable ptr, size_t n)
{
	if (n < sizeof(rbuff_t) + 2)
		return (-1);
	if (!ptr)
		ptr = malloc(n)
			;
	*gb = ptr;
	(*gb)->bfs = n - sizeof(rbuff_t);
	(*gb)->wi = 0;
	(*gb)->ri = 0;
	(*gb)->next = NULL;
	return (0);
}

ssize_t
rbuff_read(rbuff_t *rb, void *buff, size_t n)
{
	size_t	data_s;
	size_t	i;

	if (!rb || !buff || rb->ri == rb->wi)
		return (-EBUFF);
	data_s = rb->ri < rb->wi?
		rb->wi - rb->ri:
		rb->bfs - rb->ri + rb->wi;
	if (!data_s)
        	return (-EEMPTY);
	if (n > data_s)
		n = data_s;
	i = 0;
	while (i < n)
	{
		((uint8_t *)buff)[i++] = rb->buff[rb->ri++];
		if (rb->ri >= rb->bfs)
			rb->ri = 0;
	}
	return (i);
}

ssize_t
rbuff_write(rbuff_t *rb, void *buff, size_t n)
{
	size_t	data_s;
	size_t	i;

	if (!rb || !buff)
		return (-EBUFF);
	data_s = rb->wi < rb->ri?
		rb->ri - rb->wi - 1:
		rb->bfs - rb->wi + rb->ri - 1;
	if (!data_s)
        	return (-EFULL);
	if (n > data_s)
		n = data_s;
	i = 0;
	while (i < n)
	{
		rb->buff[rb->wi++] = ((uint8_t *)buff)[i++];
		if (rb->wi >= rb->bfs)
			rb->wi = 0;
	}
	return (i);
}

static ssize_t
fw_rbuff_read(rbuff_t *fb, void *buff, size_t n)
{
	size_t	data_s;
	size_t	i;

	if (!fb || !buff)
		return (-EBUFF);
	if (fb->ri == fb->wi && fb->ri == fb->bfs)
		return (-EEOB);
	if (fb->ri == fb->wi)
		return (-EEMPTY);
	data_s = fb->wi - fb->ri;
	n = n < data_s? n: data_s;
	i = 0;
	while (i < n) 
		((uint8_t *)buff)[i++] = fb->buff[fb->ri++];
	return (i);
}

static ssize_t
fw_rbuff_write(rbuff_t *fb, void *buff, size_t n)
{
	size_t	data_s;
	size_t	i;

	if (!fb || !buff)
		return (-EBUFF);
	if (fb->wi == fb->bfs)
		return (-EEOB);
	data_s = fb->bfs - fb->wi;
	n = n < data_s? n: data_s;
	i = 0;
	while (i < n) 
		fb->buff[fb->wi++] = ((uint8_t *)buff)[i++];
	return (i);
}


ssize_t
lbuff_read(rbuff_t **ll, void *buff, size_t n)
{
	rbuff_t	*tmp;
	rbuff_t	*swap;
	size_t		i;
	ssize_t		ret;

	if (!ll || !buff)
		return (-EBUFF);
	if (!*ll || (*ll)->wi == (*ll)->ri)
		return (-EEMPTY);
	tmp = *ll;
	i = n;
	while (n && tmp)
	{
		ret = fw_rbuff_read(tmp, buff, n);
		if (-EEMPTY == ret)
			break;
		else if (0 > ret)
		{
			swap = tmp->next;
			free(tmp);
			tmp = swap;
		}
		else
		{
			n -= ret;
			buff += ret;
		}
	}
	*ll = tmp;
	return (i - n);
}

ssize_t
lbuff_write(rbuff_t **ll, void *buff, size_t n)
{
	size_t		i;
	ssize_t		ret;
	rbuff_t	*tmp;

	if (!ll || !buff)
		return (-EBUFF);
	if (!*ll && 0 > rbuff_init(ll, NULL, LL_BUFF_STD_SIZE))
		return (-EBUFF);
	tmp = *ll;
	while (tmp->wi == tmp->bfs && tmp->next)
		tmp = tmp->next;
	i = n;
	while (n)
	{
		ret = fw_rbuff_write(tmp, buff, n);
		if (0 > ret)
		{
			if (!tmp->next && 0 > rbuff_init(&tmp->next, NULL, tmp->bfs + sizeof(rbuff_t)))
				return (-EBUFF);
			tmp = tmp->next;
		}
		else
		{
			n -= ret;
			buff += ret;
		}
	}
	return (i - n);
}

ssize_t
lbuff_size(rbuff_t *rb)
{
	ssize_t	sz;

	sz = 0;
	if (rb)
		sz -= rb->ri;
	while (rb)
	{
		sz += rb->wi;
		rb = rb->next;
	}
	return (sz);
}

void
lbuff_del(rbuff_t *ln)
{
	rbuff_t	*tmp;

	while (ln)
	{
		tmp = ln->next;
		free(ln);
	}	ln = tmp;
}

#ifdef __cplusplus
}
#endif

