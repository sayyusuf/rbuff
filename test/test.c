#include <rbuff.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int
main()
{
	char buff[200] = {0};

	char *s;

	rbuff_t *bf = rbuff_new(5);
	assert(bf);

	s = "hello world\n";
	assert(1 < lbuff_write(&bf, s, strlen(s)));
		
	s = "this is a buffer test1\n";
	assert(1 < lbuff_write(&bf, s, strlen(s)));

	s = "this is a buffer test2\n";
	assert(1 < lbuff_write(&bf, s, strlen(s)));
	printf("buffer sizze: %d\n", lbuff_size(bf));
	int ret = lbuff_read(&bf, buff, 200);
	assert(1 < ret);
	buff[ret] = 0;
	printf("%s\n", buff);
	return 0;

}
