/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <string>

// Helpers for loading raw data from external files
namespace Core
{
	bool LoadTextFromFile(const char* fileSrcPath, std::string& resultBuffer);
}