#include "gn_buff.h"
#include <stdio.h>


int
main()
{
	char buff[20] = {0};

	gn_buff_t *bf = rn_buff_new(10);
	rn_buff_write(bf, "hello world", 11);
	buff[rn_buff_read(bf, buff, 20)] = 0;
	printf("%s\n", buff);

}
