#pragma once

#include "Application.h"


extern Devil::Application* Devil::CreateApplication();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	try
	{
		int result{};

		auto app = Devil::CreateApplication();
		result = app->Run();
		delete app;

		return result;
	}
	catch (const Devil::DevilException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "No details avaliable", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	// Here can catch exception return exception error code [TODO]
	return 0;
}