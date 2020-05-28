#pragma once
#include <Demo.h>
#include <GameState.h>
#include <InputMessage.h>

class DemoWriter {
public:
	void start(std::string name);
	void end();
	void write(const GameState& state);
	void write(const InputMessage& input);	
	std::vector<std::string> getList();
	std::vector<char> getDemo(std::string name);
private:
	std::string m_fileName;
	
	void write(BinarySerializer& serializer);
};