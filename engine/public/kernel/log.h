/*
SDLEngine
	Matt Hoyle
*/
#pragma once

namespace LogInternals
{
	void LogImpl(const char* txt, ...);
}

// Standard logging
#define SDE_LOG( ... )		LogInternals::LogImpl(__VA_ARGS__)

// Log with channels
#define SDE_LOGC( channel, ... )		LogInternals::LogImpl(#channel ": "__VA_ARGS__)