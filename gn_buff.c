#include "gn_buff.h"

gn_buff_t *
rn_buff_new(size_t buff_size)
{
	gn_buff_t *rb;

#ifdef GN_BUFF_NOT_STATIC 
	rb = malloc(sizeof(*rb) + buff_size);
	if (!rb)
		return (NULL);
	rb->buff_size = buff_size;
#else
	(void)buff_size;
	rb = malloc(sizeof(*rb));
	if (!rb)
		return (NULL);
	rb->buff_size = STATIC_RN_BUFF_SIZE;
#endif
	rb->write_i = 0;
	rb->read_i = 0;
	return (rb);
}

ssize_t
rn_buff_read(gn_buff_t *rb, void *buf, size_t n)
{
	size_t data_s;
	size_t i;

	if (!rb || rb->read_i == rb->write_i)
		return (-EBUFF);
	data_s = rb->read_i < rb->write_i?
		rb->write_i - rb->read_i:
		rb->buff_size - rb->read_i + rb->write_i;
    if (!data_s)
        return (-1);
	if (n > data_s)
		n = data_s;
	i = 0;
	while (i < n)
	{
		((uint8_t *)buf)[i++] = rb->buff[rb->read_i++];
		if (rb->read_i >= rb->buff_size)
			rb->read_i = 0;
	}
	return (i);
}

ssize_t
rn_buff_write(gn_buff_t *rb, void *buf, size_t n)
{
	size_t data_s;
	size_t i;

	if (!rb)
		return (-EBUFF);
	data_s = rb->write_i < rb->read_i?
		rb->read_i - rb->write_i - 1:
		rb->buff_size - rb->write_i + rb->read_i - 1;
    if (!data_s)
        return (-EBUFF);
	if (n > data_s)
		n = data_s;
	i = 0;
	while (i < n)
	{
		rb->buff[rb->write_i++] = 	((uint8_t *)buf)[i++];
		if (rb->write_i >= rb->buff_size)
			rb->write_i = 0;
	}
	return (i);
}



