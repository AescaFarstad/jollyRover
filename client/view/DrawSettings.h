struct DrawSettings
{
	bool layer1 = true;
	bool remnants = true;
	bool layer2 = true;
	bool creeps = true;
	bool cars = true;
	bool projectiles = true;
	bool layer3 = true;
	bool formations_D = false;
	bool connections_D = false;
	bool explosions = true;
	bool debug_D = false;
	bool threat_D = false;
	bool obstacles_D;
	bool input = true;
	bool hud = true;
	bool fps_D = false;
	bool timing_D = false;
	bool scores = true;
};

namespace S
{
	extern DrawSettings drawSettings;
};
