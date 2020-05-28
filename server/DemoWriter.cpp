#include <DemoWriter.h>
#include <experimental/filesystem>
#include <fstream>
#include <glob.h>
#include <cstdint>
#include <regex>


void DemoWriter::start(std::string name)
{
	m_fileName = S::config.demoPath + name + ".dem";
}

void DemoWriter::end()
{
	
}

void DemoWriter::write(const GameState& state)
{
	BinarySerializer bs;
	bs.write(DEMO_EVENT::STATE);
	bs.write(state);
	write(bs);
}

void DemoWriter::write(const InputMessage& input)
{
	BinarySerializer bs;
	bs.write(DEMO_EVENT::INPUT);
	input.serialize(bs);	
	write(bs);	
}

void DemoWriter::write(BinarySerializer& serializer)
{		
	auto data = serializer.dumpAll();
	
	std::ofstream demo(m_fileName, std::ios_base::app | std::ios_base::out | std::ios_base::binary);
	demo.write(data.data(), data.size());
	demo.close();
}

std::vector<std::string> DemoWriter::getList()
{
	std::vector<std::string> result;
	for (const auto& entry : std::experimental::filesystem::directory_iterator(S::config.demoPath))
		result.push_back(std::experimental::filesystem::path(entry.path()).stem());
	return result;
}

std::vector<char> DemoWriter::getDemo(std::string name)
{
	std::regex demoRegex("[0-9\\-\\[\\]\\s]+");
	auto fullName = S::config.demoPath + name + ".dem";
	if (
		!std::regex_match(name, demoRegex) ||
		!std::experimental::filesystem::exists(fullName)
		)
	{
		S::log.add("No such demo: " + name, {LOG_TAGS::ERROR_});
		return std::vector<char>();
	}
	
    std::vector<char> result;
	std::ifstream demoFile(fullName, std::ios::in|std::ios::binary|std::ios::ate);	
	auto size = demoFile.tellg();
    result.resize(size);
    demoFile.seekg (0, std::ios::beg);
    demoFile.read (&result[0], size);
    demoFile.close();
			   
	BinarySerializer bs(result);
	return result;
}