/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/base_types.h"
#include <vector>

namespace Core
{
	class RunLengthEncoder
	{
	public:
		RunLengthEncoder();
		~RunLengthEncoder();
		void WriteData(const uint8_t* inBuffer, size_t inBufferSize, std::vector<uint8_t>& outBuffer);
		void Flush(std::vector<uint8_t>& outBuffer);
	private:
		uint8_t m_byteToWrite;
		int32_t m_repeatCount;
	};

	class RunLengthDecoder
	{
	public:
		RunLengthDecoder();
		~RunLengthDecoder();
		void ReadData(const uint8_t* inBuffer, size_t inBufferSize, std::vector<uint8_t>& outBuffer);
	private:
	};
}