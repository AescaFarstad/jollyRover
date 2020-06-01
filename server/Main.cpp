#include <Root.h>
#undef main


#include <Root.h>
int main()
{	
	Root root;
	root.init();
	
	while (!root.mainLoop())
		;
	
	root.quit();

	return 0;
}



