#include "User.h"
int main()
{
	srand(time(0));
	User test("hi",NULL);
	std::cout << test.getWord()<<" "<<test.getSWC();
}