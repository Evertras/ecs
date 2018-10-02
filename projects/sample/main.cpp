#include "pch.h"
#include "Game.h"

int main(int argc, char** argv)
{
	Game g;

	if (g.Initialize()) {
		g.Run();
	}

	return 0;
}
