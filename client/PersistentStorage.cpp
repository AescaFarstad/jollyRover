#include <PersistentStorage.h>
#include <fstream>

namespace S 
{
	PersistentStorage persistentStorage;
}

PersistentStorage::PersistentStorage()
{
	m_isReady = false;
}

bool PersistentStorage::isReady()
{
	return m_isReady;
}

void PersistentStorage::init()
{
	std::ifstream file("out/save.data", std::ios::binary);
	if (file.is_open()) 
	{
		file.seekg (0, file.end);
		int32_t length = file.tellg();
		file.seekg (0, file.beg);

		char* buffer = new char [length];
		file.read(buffer, length);
		
		auto s = SerializationStream::createExp();
		s->write(buffer, length);
		s->seekAbsolute(0);
		
		bool hasSavedState;
		Serializer::read(hasSavedState, *s);
		
		if (hasSavedState)
		{
			savedState = std::make_unique<GameState>();
			savedState->deserialize(*s);			
		}
		
		
		file.close();
		delete[] buffer;
	}
	m_isReady = true;
}

void PersistentStorage::commit()
{
	auto s = SerializationStream::createExp();
	Serializer::write(savedState != nullptr, *s);
	if (savedState != nullptr)
		savedState->serialize(*s);
		
	std::ofstream file("out/save.data");

    if(file.is_open())
    {
		auto data = s->readAll();
		auto length = s->getLength();
		file.write(data, length);
		file.close();
    }
	
}

void PersistentStorage::clean()
{
	savedState = nullptr;
	commit();
}
