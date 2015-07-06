/*
SDLEngine
Matt Hoyle
*/
#include "file_io.h"
#include "assert.h"
#include <fstream>

namespace Kernel
{
	namespace FileIO
	{
		bool LoadTextFromFile(const char* fileSrcPath, std::string& resultBuffer)
		{
			SDE_ASSERT(fileSrcPath != nullptr, "Invalid source path");
			SDE_ASSERT(strlen(fileSrcPath) != 0, "Invalid source path");

			resultBuffer.clear();
			std::ifstream fileStream(fileSrcPath, std::ios::in);
			if (!fileStream.is_open())
			{
				return false;
			}

			char oneLine[512] = { '\0' };
			while (!fileStream.eof())
			{
				fileStream.getline(oneLine, 512);
				resultBuffer += oneLine;
				resultBuffer += "\n";
			}
			return true;
		}
	}
}