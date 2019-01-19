#include "MainForm.h"

#include <Windows.h>

using namespace getest; // This is your project name

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	MainForm^ form = gcnew MainForm();
	Application::Run(form);
}
