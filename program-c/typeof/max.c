#define max(x,y) ({ \
	    typeof(x) _x = (x);    \
	    typeof(y) _y = (y);    \
	    (void) (&_x == &_y);        \
	    _x > _y ? _x : _y; })

int main(void)
{
	int a=3;
	float b=0.5;
	max(a,b);

	return 0;
}
