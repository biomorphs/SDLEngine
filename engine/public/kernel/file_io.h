/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <string>
#include <vector>

// Helpers for loading raw data from external files
namespace Kernel
{
	namespace FileIO
	{
		bool LoadTextFromFile(const char* fileSrcPath, std::string& resultBuffer);
		bool LoadBinaryFile(const char* fileSrcPath, std::vector<uint8_t>& resultBuffer);
	}
}