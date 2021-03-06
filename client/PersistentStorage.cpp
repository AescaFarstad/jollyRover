#include <PersistentStorage.h>
#include <BinarySerializer.h>
#include <fstream>

namespace S 
{
	PersistentStorage persistentStorage;
}

PersistentStorage::PersistentStorage()
{
	m_isReady = false;
	m_isInitialized = false;
	savedState.isValid_ = false;
}

void PersistentStorage::p_init()
{
	std::ifstream file("out/save/save.data", std::ios::binary);
	if (!file.is_open()) 
		return;
	
	file.seekg (0, file.end);
	int32_t length = file.tellg();
	file.seekg (0, file.beg);

	char* buffer = new char [length];
	file.read(buffer, length);
	
	BinarySerializer bs(buffer, length);
	
	bool hasSavedState;
	bs.read(hasSavedState);
	
	if (hasSavedState)
	{
		bs.read(savedState);
	}
	
	file.close();
	delete[] buffer;
}

void PersistentStorage::p_commit()
{
	BinarySerializer bs;
	bs.write(savedState.isValid_);
	
	if (savedState.isValid_)
		bs.write(savedState);
		
	std::ofstream file("out/save/save.data");

    if(file.is_open())
    {
		auto data = bs.dumpAll();
		file.write(data.data(), data.size());
		file.close();
    }
	else
	{
		S::log.add("Failed to save persistent data", {LOG_TAGS::ERROR_});
	}
}

#ifndef __EMSCRIPTEN__
bool PersistentStorage::isReady()
{
	return m_isReady;
}

void PersistentStorage::init()
{
	p_init();
	m_isReady = true;
	m_isInitialized = true;
}

void PersistentStorage::commit()
{
	p_commit();
}

#else
#include <emscripten.h>

void PersistentStorage::init()
{
	S::log.add("Init file system", {LOG_TAGS::FILE_SYSTEM});
	EM_ASM(
		FS.mkdir('out/save');
		FS.mount(IDBFS,{},'out/save');
		Module.print("Start File sync..");
		Module.syncdone = 0;
		FS.syncfs(true, function(err) {
						assert(!err);
						Module.print("End file sync..");
						Module.syncdone = 1;
		});
	);
}

void PersistentStorage::commit()
{
	S::log.add("Save persistent storage", {LOG_TAGS::FILE_SYSTEM});
	m_isReady = false;
	p_commit();	
	EM_ASM(
			Module.print("Start File sync..");
			Module.syncdone = 0;
			FS.syncfs(false, function(err) {
							assert(!err);
							Module.print("End File sync..");
							Module.syncdone = 1;
							});
	);
}


bool PersistentStorage::isReady()
{
	if (m_isReady)
		return m_isReady;
	m_isReady = emscripten_run_script_int("Module.syncdone") == 1;
	if (m_isReady)
	{
		S::log.add("Persistent file system ready", {LOG_TAGS::FILE_SYSTEM});
		if (!m_isInitialized)
		{
			p_init();
			m_isInitialized = true;
		}
	}
	return m_isReady;
}
#endif


void PersistentStorage::clean()
{
	savedState.isValid_ = false;
	commit();
}
