#include "Application.h"

int main(void)
{
	Application* app = new Application(800,600);
	app->init();
	app->run();
}