#include "gn_buff.h"

gn_buff_t 	*
gn_buff_new(size_t buff_size)
{
	gn_buff_t	*gb;

#ifdef GN_BUFF_NOT_STATIC 
	gb = malloc(sizeof(*gb) + buff_size);
	if (!gb)
		return (NULL);
	gb->bfs = buff_size;
#else
	(void)buff_size;
	gb = malloc(sizeof(*gb));
	if (!gb)
		return (NULL);
	gb->bfs = STATIC_RN_BUFF_SIZE;
#endif
	gb->wi = 0;
	gb->ri = 0;
	return (gb);
}

int
gn_buff_init(gn_buff_t **gb, size_t buff_size)
{
#ifdef GN_BUFF_NOT_STATIC 
	*gb = malloc(sizeof(**gb) + buff_size);
	if (!*gb)
		return (-1);
	(*gb)->bfs = buff_size;
#else
	(void)buff_size;
	*gb = malloc(sizeof(**gb));
	if (!*gb)
		return (-1);
	(*gb)->bfs = STATIC_RN_BUFF_SIZE;
#endif
	(*gb)->wi = 0;
	(*gb)->ri = 0;
	return (0);
}


ssize_t
rn_buff_read(gn_buff_t *rb, void *buff, size_t n)
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
rn_buff_write(gn_buff_t *rb, void *buff, size_t n)
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

ssize_t
fw_buff_read(gn_buff_t *fb, void *buff, size_t n)
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

ssize_t
fw_buff_write(gn_buff_t *fb, void *buff, size_t n)
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
ll_buff_read(gn_buff_t **ll, void *buff, size_t n)
{
	gn_buff_t	*tmp;
	gn_buff_t	*swap;
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
		ret = fw_buff_read(tmp, buff, n);
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
ll_buff_write(gn_buff_t **ll, void *buff, size_t n)
{
	size_t		i;
	ssize_t		ret;
	gn_buff_t	*tmp;

	if (!ll || !buff)
		return (-EBUFF);
	if (!*ll && 0 > gn_buff_init(ll, LL_BUFF_STD_SIZE))
		return (-EBUFF);
	tmp = *ll;
	while (tmp->wi == tmp->bfs && tmp->next)
		tmp = tmp->next;
	i = n;
	while (n)
	{
		ret = fw_buff_write(tmp, buff, n);
		if (0 > ret)
		{
			if (!tmp->next && 0 > gn_buff_init(&tmp->next, tmp->bfs))
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


void
ll_buff_del(gn_buff_t *ln)
{
	gn_buff_t	*tmp;

	while (ln)
	{
		tmp = ln->next;
		free(ln);
	}	ln = tmp;
}



