#pragma once

template <class SystemType>
class Program {
public:
	SystemType* system;
	inline Program(int screenWidth, int screenHeight, const char* programName) {
		system = new SystemType(screenWidth, screenHeight, programName);
	}
	inline ~Program() { delete system; }
};