#include "stdafx.h"
#include "LoginApp.h"

int main(int narg, const char** args)
{
	App* app = new LoginApp(narg, args);

	if (!app->initialize())
	{
		delete app;
		return 1;
	}

	app->run();
	app->finish();

	delete app;
	return 0;
}