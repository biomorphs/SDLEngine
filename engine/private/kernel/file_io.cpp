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
			fileStream.close();
			return true;
		}

		bool LoadBinaryFile(const char* fileSrcPath, std::vector<uint8_t>& resultBuffer)
		{
			SDE_ASSERT(fileSrcPath != nullptr, "Invalid source path");
			SDE_ASSERT(strlen(fileSrcPath) != 0, "Invalid source path");

			std::ifstream fileStream(fileSrcPath, std::ios::binary | std::ios::in);
			if (!fileStream.is_open())
			{
				return false;
			}

			fileStream.seekg(0, std::ios::end);
			size_t fileSize = fileStream.tellg();
			fileStream.seekg(0, std::ios::beg);

			resultBuffer.resize(fileSize);
			fileStream.read(reinterpret_cast<char*>(resultBuffer.data()), fileSize);
			fileStream.close();

			return true;
		}
	}
}