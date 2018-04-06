#include "passes.h"

int main()
{
	initialiseOpTable();
	raw.open("input.txt");
	inter.open("intermediate.txt");
	pass1();
	inter.close();
	in.open("intermediate.txt");
	listing.open("list.txt");
	obj.open("programme.txt");
	pass2();
	return 0;
}
