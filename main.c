#include "gn_buff.h"
#include <stdio.h>
#include <string.h>

int
main()
{
	char buff[200] = {0};

	char *s;

	gn_buff_t *bf = gn_buff_new(5);

	s = "hello world\n";
	ll_buff_write(&bf, s, strlen(s));
		
	s = "my name is yusuf\n";
	ll_buff_write(&bf, s, strlen(s));

	s = "tihs is a buffer test\n";
	ll_buff_write(&bf, s, strlen(s));

	ll_buff_read(&bf, buff, 200);
	printf("%p\n%s\n",bf,  buff);
	return 0;

}
