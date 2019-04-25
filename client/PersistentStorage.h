#pragma once
#include <GameState.h>

class PersistentStorage
{
public:
	PersistentStorage();
	
	bool isReady();
	void init();
	void commit();
	void clean();
	
	std::unique_ptr<GameState> savedState;
	
private: 
	bool m_isReady;
	bool m_isInitialized;
	
	void p_init();
	void p_commit();
	
	
};

namespace S 
{
	extern PersistentStorage persistentStorage;
}