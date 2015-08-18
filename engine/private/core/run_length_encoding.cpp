/*
SDLEngine
Matt Hoyle
*/
#include "run_length_encoding.h"
#include "kernel/assert.h"

namespace Core
{
	RunLengthEncoder::RunLengthEncoder()
		: m_repeatCount(0)
		, m_byteToWrite(0)
	{
	}

	RunLengthEncoder::~RunLengthEncoder()
	{
	}
	
	void RunLengthEncoder::Flush(std::vector<uint8_t>& outBuffer)
	{
		int32_t repeatsToWrite = m_repeatCount;
		while (repeatsToWrite > 0)
		{
			uint8_t repeatCount = repeatsToWrite > 255 ? 255 : (uint8_t)repeatsToWrite;
			outBuffer.push_back(repeatCount);
			outBuffer.push_back(m_byteToWrite);
			repeatsToWrite -= repeatCount;
		}
		m_repeatCount = 0;
	}

	void RunLengthEncoder::WriteData(const uint8_t* inBuffer, size_t inBufferSize, std::vector<uint8_t>& outBuffer)
	{
		SDE_ASSERT(inBufferSize > 0);

		if (m_repeatCount == 0)
		{
			m_byteToWrite = *inBuffer;
			m_repeatCount = 0;
		}

		uint8_t firstValue = m_byteToWrite;
		for (size_t inOffset = 0; inOffset < inBufferSize; ++inOffset)
		{
			uint8_t thisValue = *(inBuffer + inOffset);
			if (thisValue == firstValue)
			{
				m_repeatCount++;
			}
			else
			{
				int32_t repeatsToWrite = m_repeatCount;
				while (repeatsToWrite > 0)
				{
					uint8_t repeatCount = repeatsToWrite > 255 ? 255 : (uint8_t)repeatsToWrite;
					outBuffer.push_back(repeatCount);
					outBuffer.push_back(firstValue);
					repeatsToWrite -= repeatCount;
				}
				firstValue = thisValue;
				m_repeatCount = 1;
			}
		}

		m_byteToWrite = firstValue;
	}

	RunLengthDecoder::RunLengthDecoder()
	{

	}

	RunLengthDecoder::~RunLengthDecoder()
	{

	}

	void RunLengthDecoder::ReadData(const uint8_t* inBuffer, size_t inBufferSize, std::vector<uint8_t>& outBuffer)
	{
		SDE_ASSERT(inBuffer != nullptr);
		SDE_ASSERT(inBufferSize > 2);

		for (size_t offs = 0; offs < inBufferSize; offs += 2)
		{
			uint8_t repeatCount = inBuffer[offs];
			uint8_t thisChar = inBuffer[offs + 1];
			outBuffer.resize(outBuffer.size() + repeatCount);
			memset(&outBuffer[outBuffer.size() - repeatCount], thisChar, repeatCount);
		}
	}
}