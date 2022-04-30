#include "Engine/Engine.h"

int main()
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    srand((unsigned int)time(0));

	Engine engine({ 1280, 720 });
	engine.run();

	return 0;
}