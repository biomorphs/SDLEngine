/*
SDLEngine
	Matt Hoyle
*/

// This class is responsible for initialising any platform stuff
class Platform
{
public:
	enum InitResult
	{
		InitOK,
		InitFailed,
	};

	enum ShutdownResult
	{
		ShutdownOK
	};

	static InitResult Initialise(int argc, char* argv[]);
	static ShutdownResult Shutdown();
};