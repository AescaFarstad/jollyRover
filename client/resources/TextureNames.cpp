#include <TextureNames.h>

TextureDef* TextureFolder::getTextureByName(const std::string& name)
{
	for(auto& texture : textures)
	{
		if (texture->name == name) 
			return texture;
	}
	for(auto& f : folders)
	{
		auto result = f->getTextureByName(name);
		if (result)
			return result;
	}
	return nullptr;
}


r_5soil::r_5soil()
{
	
	s1 = TextureDef{
			"art/path/gray/s1",
			GPU_Rect{199, 730, 12, 12}
		};
	textures.push_back(&s1);
	s2 = TextureDef{
			"art/path/gray/s2",
			GPU_Rect{215, 730, 12, 12}
		};
	textures.push_back(&s2);
	s3 = TextureDef{
			"art/path/gray/s3",
			GPU_Rect{231, 730, 12, 12}
		};
	textures.push_back(&s3);
	b3 = TextureDef{
			"art/path/gray/b3",
			GPU_Rect{530, 747, 25, 10}
		};
	textures.push_back(&b3);
	b2 = TextureDef{
			"art/path/gray/b2",
			GPU_Rect{559, 747, 21, 10}
		};
	textures.push_back(&b2);
	b1 = TextureDef{
			"art/path/gray/b1",
			GPU_Rect{584, 747, 23, 10}
		};
	textures.push_back(&b1);
	
};

r_byo84::r_byo84()
{
	
	b3 = TextureDef{
			"art/path/red/b3",
			GPU_Rect{729, 576, 28, 30}
		};
	textures.push_back(&b3);
	b4 = TextureDef{
			"art/path/red/b4",
			GPU_Rect{0, 672, 30, 30}
		};
	textures.push_back(&b4);
	b1 = TextureDef{
			"art/path/red/b1",
			GPU_Rect{34, 672, 26, 30}
		};
	textures.push_back(&b1);
	b2 = TextureDef{
			"art/path/red/b2",
			GPU_Rect{282, 672, 26, 26}
		};
	textures.push_back(&b2);
	s3 = TextureDef{
			"art/path/red/s3",
			GPU_Rect{649, 706, 13, 14}
		};
	textures.push_back(&s3);
	s2 = TextureDef{
			"art/path/red/s2",
			GPU_Rect{740, 626, 14, 14}
		};
	textures.push_back(&s2);
	s1 = TextureDef{
			"art/path/red/s1",
			GPU_Rect{740, 644, 14, 13}
		};
	textures.push_back(&s1);
	
};

r_yp1st::r_yp1st()
{
	
	s3 = TextureDef{
			"art/path/green/s3",
			GPU_Rect{737, 18, 17, 13}
		};
	textures.push_back(&s3);
	s2 = TextureDef{
			"art/path/green/s2",
			GPU_Rect{737, 35, 19, 13}
		};
	textures.push_back(&s2);
	s1 = TextureDef{
			"art/path/green/s1",
			GPU_Rect{737, 52, 18, 13}
		};
	textures.push_back(&s1);
	b3 = TextureDef{
			"art/path/green/b3",
			GPU_Rect{0, 730, 25, 13}
		};
	textures.push_back(&b3);
	b2 = TextureDef{
			"art/path/green/b2",
			GPU_Rect{29, 730, 28, 13}
		};
	textures.push_back(&b2);
	b1 = TextureDef{
			"art/path/green/b1",
			GPU_Rect{61, 730, 26, 13}
		};
	textures.push_back(&b1);
	
};

r_hbzjo::r_hbzjo()
{
	
	smokeOrange4 = TextureDef{
			"tanks_1/Smoke/smokeOrange4",
			GPU_Rect{0, 0, 97, 100}
		};
	textures.push_back(&smokeOrange4);
	smokeGrey4 = TextureDef{
			"tanks_1/Smoke/smokeGrey4",
			GPU_Rect{101, 0, 97, 100}
		};
	textures.push_back(&smokeGrey4);
	smokeYellow4 = TextureDef{
			"tanks_1/Smoke/smokeYellow4",
			GPU_Rect{202, 0, 97, 100}
		};
	textures.push_back(&smokeYellow4);
	smokeWhite3 = TextureDef{
			"tanks_1/Smoke/smokeWhite3",
			GPU_Rect{303, 0, 97, 100}
		};
	textures.push_back(&smokeWhite3);
	smokeYellow5 = TextureDef{
			"tanks_1/Smoke/smokeYellow5",
			GPU_Rect{404, 0, 107, 98}
		};
	textures.push_back(&smokeYellow5);
	smokeOrange5 = TextureDef{
			"tanks_1/Smoke/smokeOrange5",
			GPU_Rect{515, 0, 107, 98}
		};
	textures.push_back(&smokeOrange5);
	smokeWhite4 = TextureDef{
			"tanks_1/Smoke/smokeWhite4",
			GPU_Rect{626, 0, 107, 98}
		};
	textures.push_back(&smokeWhite4);
	smokeGrey5 = TextureDef{
			"tanks_1/Smoke/smokeGrey5",
			GPU_Rect{0, 104, 107, 98}
		};
	textures.push_back(&smokeGrey5);
	smokeOrange1 = TextureDef{
			"tanks_1/Smoke/smokeOrange1",
			GPU_Rect{211, 104, 89, 92}
		};
	textures.push_back(&smokeOrange1);
	smokeWhite0 = TextureDef{
			"tanks_1/Smoke/smokeWhite0",
			GPU_Rect{304, 104, 89, 92}
		};
	textures.push_back(&smokeWhite0);
	smokeYellow1 = TextureDef{
			"tanks_1/Smoke/smokeYellow1",
			GPU_Rect{397, 104, 89, 92}
		};
	textures.push_back(&smokeYellow1);
	smokeGrey1 = TextureDef{
			"tanks_1/Smoke/smokeGrey1",
			GPU_Rect{490, 104, 89, 92}
		};
	textures.push_back(&smokeGrey1);
	smokeOrange2 = TextureDef{
			"tanks_1/Smoke/smokeOrange2",
			GPU_Rect{583, 104, 99, 90}
		};
	textures.push_back(&smokeOrange2);
	smokeWhite1 = TextureDef{
			"tanks_1/Smoke/smokeWhite1",
			GPU_Rect{0, 206, 99, 90}
		};
	textures.push_back(&smokeWhite1);
	smokeYellow2 = TextureDef{
			"tanks_1/Smoke/smokeYellow2",
			GPU_Rect{103, 206, 99, 90}
		};
	textures.push_back(&smokeYellow2);
	smokeGrey2 = TextureDef{
			"tanks_1/Smoke/smokeGrey2",
			GPU_Rect{206, 206, 99, 90}
		};
	textures.push_back(&smokeGrey2);
	smokeOrange0 = TextureDef{
			"tanks_1/Smoke/smokeOrange0",
			GPU_Rect{309, 206, 87, 87}
		};
	textures.push_back(&smokeOrange0);
	smokeYellow0 = TextureDef{
			"tanks_1/Smoke/smokeYellow0",
			GPU_Rect{400, 206, 87, 87}
		};
	textures.push_back(&smokeYellow0);
	smokeGrey0 = TextureDef{
			"tanks_1/Smoke/smokeGrey0",
			GPU_Rect{491, 206, 87, 87}
		};
	textures.push_back(&smokeGrey0);
	smokeWhite5 = TextureDef{
			"tanks_1/Smoke/smokeWhite5",
			GPU_Rect{582, 206, 87, 87}
		};
	textures.push_back(&smokeWhite5);
	smokeWhite2 = TextureDef{
			"tanks_1/Smoke/smokeWhite2",
			GPU_Rect{673, 206, 79, 79}
		};
	textures.push_back(&smokeWhite2);
	smokeYellow3 = TextureDef{
			"tanks_1/Smoke/smokeYellow3",
			GPU_Rect{0, 300, 79, 79}
		};
	textures.push_back(&smokeYellow3);
	smokeOrange3 = TextureDef{
			"tanks_1/Smoke/smokeOrange3",
			GPU_Rect{83, 300, 79, 79}
		};
	textures.push_back(&smokeOrange3);
	smokeGrey3 = TextureDef{
			"tanks_1/Smoke/smokeGrey3",
			GPU_Rect{166, 300, 79, 79}
		};
	textures.push_back(&smokeGrey3);
	
};

r_i9ox9::r_i9ox9()
{
	
	oil = TextureDef{
			"tanks_1/Obstacles/oil",
			GPU_Rect{111, 104, 96, 96}
		};
	textures.push_back(&oil);
	sandbagBeige = TextureDef{
			"tanks_1/Obstacles/sandbagBeige",
			GPU_Rect{249, 300, 44, 66}
		};
	textures.push_back(&sandbagBeige);
	sandbagBrown = TextureDef{
			"tanks_1/Obstacles/sandbagBrown",
			GPU_Rect{297, 300, 44, 66}
		};
	textures.push_back(&sandbagBrown);
	barrelRed_up = TextureDef{
			"tanks_1/Obstacles/barrelRed_up",
			GPU_Rect{478, 519, 48, 48}
		};
	textures.push_back(&barrelRed_up);
	barrelGreen_up = TextureDef{
			"tanks_1/Obstacles/barrelGreen_up",
			GPU_Rect{550, 519, 48, 48}
		};
	textures.push_back(&barrelGreen_up);
	barrelGrey_up = TextureDef{
			"tanks_1/Obstacles/barrelGrey_up",
			GPU_Rect{602, 519, 48, 48}
		};
	textures.push_back(&barrelGrey_up);
	barrelGreen_side_damaged = TextureDef{
			"tanks_1/Obstacles/barrelGreen_side_damaged",
			GPU_Rect{49, 576, 62, 44}
		};
	textures.push_back(&barrelGreen_side_damaged);
	barrelGrey_side = TextureDef{
			"tanks_1/Obstacles/barrelGrey_side",
			GPU_Rect{115, 576, 62, 44}
		};
	textures.push_back(&barrelGrey_side);
	barrelGrey_sde_rust = TextureDef{
			"tanks_1/Obstacles/barrelGrey_sde_rust",
			GPU_Rect{233, 576, 62, 44}
		};
	textures.push_back(&barrelGrey_sde_rust);
	barrelRed_side = TextureDef{
			"tanks_1/Obstacles/barrelRed_side",
			GPU_Rect{299, 576, 62, 44}
		};
	textures.push_back(&barrelRed_side);
	barrelGreen_side = TextureDef{
			"tanks_1/Obstacles/barrelGreen_side",
			GPU_Rect{365, 576, 62, 44}
		};
	textures.push_back(&barrelGreen_side);
	
};

r_mt6ul::r_mt6ul()
{
	
	
	folders.push_back(&red);
	folders.push_back(&green);
	folders.push_back(&gray);
};

r_5q2x0::r_5q2x0()
{
	
	bulletRed_outline = TextureDef{
			"tanks_1/Bullets/bulletRed_outline",
			GPU_Rect{492, 672, 34, 20}
		};
	textures.push_back(&bulletRed_outline);
	bulletRedSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletRedSilver_outline",
			GPU_Rect{530, 672, 34, 20}
		};
	textures.push_back(&bulletRedSilver_outline);
	bulletGreen_outline = TextureDef{
			"tanks_1/Bullets/bulletGreen_outline",
			GPU_Rect{600, 672, 34, 20}
		};
	textures.push_back(&bulletGreen_outline);
	bulletGreenSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletGreenSilver_outline",
			GPU_Rect{638, 672, 34, 20}
		};
	textures.push_back(&bulletGreenSilver_outline);
	bulletBlue_outline = TextureDef{
			"tanks_1/Bullets/bulletBlue_outline",
			GPU_Rect{676, 672, 34, 20}
		};
	textures.push_back(&bulletBlue_outline);
	bulletBlueSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletBlueSilver_outline",
			GPU_Rect{714, 672, 34, 20}
		};
	textures.push_back(&bulletBlueSilver_outline);
	bulletBeigeSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletBeigeSilver_outline",
			GPU_Rect{686, 172, 34, 20}
		};
	textures.push_back(&bulletBeigeSilver_outline);
	bulletBeige_outline = TextureDef{
			"tanks_1/Bullets/bulletBeige_outline",
			GPU_Rect{724, 172, 34, 20}
		};
	textures.push_back(&bulletBeige_outline);
	bulletYellow_outline = TextureDef{
			"tanks_1/Bullets/bulletYellow_outline",
			GPU_Rect{125, 706, 34, 20}
		};
	textures.push_back(&bulletYellow_outline);
	bulletYellowSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletYellowSilver_outline",
			GPU_Rect{163, 706, 34, 20}
		};
	textures.push_back(&bulletYellowSilver_outline);
	bulletSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletSilver_outline",
			GPU_Rect{201, 706, 34, 20}
		};
	textures.push_back(&bulletSilver_outline);
	bulletSilverSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletSilverSilver_outline",
			GPU_Rect{239, 706, 34, 20}
		};
	textures.push_back(&bulletSilverSilver_outline);
	bulletYellowSilver = TextureDef{
			"tanks_1/Bullets/bulletYellowSilver",
			GPU_Rect{343, 730, 26, 12}
		};
	textures.push_back(&bulletYellowSilver);
	bulletYellow = TextureDef{
			"tanks_1/Bullets/bulletYellow",
			GPU_Rect{521, 730, 26, 12}
		};
	textures.push_back(&bulletYellow);
	bulletBeige = TextureDef{
			"tanks_1/Bullets/bulletBeige",
			GPU_Rect{34, 747, 26, 12}
		};
	textures.push_back(&bulletBeige);
	bulletBeigeSilver = TextureDef{
			"tanks_1/Bullets/bulletBeigeSilver",
			GPU_Rect{64, 747, 26, 12}
		};
	textures.push_back(&bulletBeigeSilver);
	bulletSilverSilver = TextureDef{
			"tanks_1/Bullets/bulletSilverSilver",
			GPU_Rect{94, 747, 26, 12}
		};
	textures.push_back(&bulletSilverSilver);
	bulletBlue = TextureDef{
			"tanks_1/Bullets/bulletBlue",
			GPU_Rect{158, 747, 26, 12}
		};
	textures.push_back(&bulletBlue);
	bulletSilver = TextureDef{
			"tanks_1/Bullets/bulletSilver",
			GPU_Rect{188, 747, 26, 12}
		};
	textures.push_back(&bulletSilver);
	bulletGreen = TextureDef{
			"tanks_1/Bullets/bulletGreen",
			GPU_Rect{282, 747, 26, 12}
		};
	textures.push_back(&bulletGreen);
	bulletGreenSilver = TextureDef{
			"tanks_1/Bullets/bulletGreenSilver",
			GPU_Rect{312, 747, 26, 12}
		};
	textures.push_back(&bulletGreenSilver);
	bulletBlueSilver = TextureDef{
			"tanks_1/Bullets/bulletBlueSilver",
			GPU_Rect{372, 747, 26, 12}
		};
	textures.push_back(&bulletBlueSilver);
	bulletRed = TextureDef{
			"tanks_1/Bullets/bulletRed",
			GPU_Rect{402, 747, 26, 12}
		};
	textures.push_back(&bulletRed);
	bulletRedSilver = TextureDef{
			"tanks_1/Bullets/bulletRedSilver",
			GPU_Rect{432, 747, 26, 12}
		};
	textures.push_back(&bulletRedSilver);
	
};

r_ir8mj::r_ir8mj()
{
	
	
	folders.push_back(&path);
};

r_xhrer::r_xhrer()
{
	
	rocket_large = TextureDef{
			"td/rocket_large",
			GPU_Rect{345, 300, 64, 64}
		};
	textures.push_back(&rocket_large);
	soldier1 = TextureDef{
			"td/soldier1",
			GPU_Rect{413, 300, 64, 64}
		};
	textures.push_back(&soldier1);
	plane_shadow_gray = TextureDef{
			"td/plane_shadow_gray",
			GPU_Rect{481, 300, 64, 64}
		};
	textures.push_back(&plane_shadow_gray);
	rocket_small = TextureDef{
			"td/rocket_small",
			GPU_Rect{549, 300, 64, 64}
		};
	textures.push_back(&rocket_small);
	plane_shadow_green = TextureDef{
			"td/plane_shadow_green",
			GPU_Rect{617, 300, 64, 64}
		};
	textures.push_back(&plane_shadow_green);
	plane_gray = TextureDef{
			"td/plane_gray",
			GPU_Rect{685, 300, 64, 64}
		};
	textures.push_back(&plane_gray);
	plane_green = TextureDef{
			"td/plane_green",
			GPU_Rect{686, 104, 64, 64}
		};
	textures.push_back(&plane_green);
	fire3 = TextureDef{
			"td/fire3",
			GPU_Rect{0, 383, 64, 64}
		};
	textures.push_back(&fire3);
	fire4 = TextureDef{
			"td/fire4",
			GPU_Rect{68, 383, 64, 64}
		};
	textures.push_back(&fire4);
	fire1 = TextureDef{
			"td/fire1",
			GPU_Rect{136, 383, 64, 64}
		};
	textures.push_back(&fire1);
	soldier3 = TextureDef{
			"td/soldier3",
			GPU_Rect{204, 383, 64, 64}
		};
	textures.push_back(&soldier3);
	soldier4 = TextureDef{
			"td/soldier4",
			GPU_Rect{340, 383, 64, 64}
		};
	textures.push_back(&soldier4);
	soldier2 = TextureDef{
			"td/soldier2",
			GPU_Rect{475, 383, 64, 64}
		};
	textures.push_back(&soldier2);
	tankTurret_green = TextureDef{
			"td/tankTurret_green",
			GPU_Rect{543, 383, 64, 64}
		};
	textures.push_back(&tankTurret_green);
	tank_base_beige = TextureDef{
			"td/tank_base_beige",
			GPU_Rect{611, 383, 64, 64}
		};
	textures.push_back(&tank_base_beige);
	tankTurret_beige = TextureDef{
			"td/tankTurret_beige",
			GPU_Rect{679, 383, 64, 64}
		};
	textures.push_back(&tankTurret_beige);
	tank_base_green = TextureDef{
			"td/tank_base_green",
			GPU_Rect{0, 451, 64, 64}
		};
	textures.push_back(&tank_base_green);
	fire2 = TextureDef{
			"td/fire2",
			GPU_Rect{68, 451, 64, 64}
		};
	textures.push_back(&fire2);
	
};

r_jr3fj::r_jr3fj()
{
	
	treeBrown_large = TextureDef{
			"tanks_2/treeBrown_large",
			GPU_Rect{272, 383, 64, 64}
		};
	textures.push_back(&treeBrown_large);
	explosion3 = TextureDef{
			"tanks_2/explosion3",
			GPU_Rect{408, 383, 63, 64}
		};
	textures.push_back(&explosion3);
	treeGreen_large = TextureDef{
			"tanks_2/treeGreen_large",
			GPU_Rect{136, 451, 64, 64}
		};
	textures.push_back(&treeGreen_large);
	explosionSmoke3 = TextureDef{
			"tanks_2/explosionSmoke3",
			GPU_Rect{204, 451, 63, 63}
		};
	textures.push_back(&explosionSmoke3);
	tankBody_huge_outline = TextureDef{
			"tanks_2/tankBody_huge_outline",
			GPU_Rect{271, 451, 72, 62}
		};
	textures.push_back(&tankBody_huge_outline);
	tank_huge = TextureDef{
			"tanks_2/tank_huge",
			GPU_Rect{347, 451, 76, 62}
		};
	textures.push_back(&tank_huge);
	explosion1 = TextureDef{
			"tanks_2/explosion1",
			GPU_Rect{427, 451, 60, 60}
		};
	textures.push_back(&explosion1);
	explosionSmoke1 = TextureDef{
			"tanks_2/explosionSmoke1",
			GPU_Rect{491, 451, 60, 60}
		};
	textures.push_back(&explosionSmoke1);
	tankBody_huge = TextureDef{
			"tanks_2/tankBody_huge",
			GPU_Rect{555, 451, 68, 58}
		};
	textures.push_back(&tankBody_huge);
	explosionSmoke2 = TextureDef{
			"tanks_2/explosionSmoke2",
			GPU_Rect{627, 451, 56, 57}
		};
	textures.push_back(&explosionSmoke2);
	explosion2 = TextureDef{
			"tanks_2/explosion2",
			GPU_Rect{687, 451, 56, 57}
		};
	textures.push_back(&explosion2);
	explosion5 = TextureDef{
			"tanks_2/explosion5",
			GPU_Rect{0, 519, 52, 53}
		};
	textures.push_back(&explosion5);
	explosionSmoke5 = TextureDef{
			"tanks_2/explosionSmoke5",
			GPU_Rect{56, 519, 52, 53}
		};
	textures.push_back(&explosionSmoke5);
	tank_bigRed = TextureDef{
			"tanks_2/tank_bigRed",
			GPU_Rect{112, 519, 52, 52}
		};
	textures.push_back(&tank_bigRed);
	fenceYellow = TextureDef{
			"tanks_2/fenceYellow",
			GPU_Rect{168, 519, 16, 52}
		};
	textures.push_back(&fenceYellow);
	tankBody_bigRed_outline = TextureDef{
			"tanks_2/tankBody_bigRed_outline",
			GPU_Rect{188, 519, 52, 52}
		};
	textures.push_back(&tankBody_bigRed_outline);
	tank_darkLarge = TextureDef{
			"tanks_2/tank_darkLarge",
			GPU_Rect{244, 519, 60, 52}
		};
	textures.push_back(&tank_darkLarge);
	tankBody_darkLarge_outline = TextureDef{
			"tanks_2/tankBody_darkLarge_outline",
			GPU_Rect{308, 519, 60, 52}
		};
	textures.push_back(&tankBody_darkLarge_outline);
	oilSpill_large = TextureDef{
			"tanks_2/oilSpill_large",
			GPU_Rect{372, 519, 50, 50}
		};
	textures.push_back(&oilSpill_large);
	tankBody_bigRed = TextureDef{
			"tanks_2/tankBody_bigRed",
			GPU_Rect{426, 519, 48, 48}
		};
	textures.push_back(&tankBody_bigRed);
	fenceRed = TextureDef{
			"tanks_2/fenceRed",
			GPU_Rect{530, 519, 16, 48}
		};
	textures.push_back(&fenceRed);
	tankBody_darkLarge = TextureDef{
			"tanks_2/tankBody_darkLarge",
			GPU_Rect{654, 519, 56, 48}
		};
	textures.push_back(&tankBody_darkLarge);
	explosion4 = TextureDef{
			"tanks_2/explosion4",
			GPU_Rect{714, 519, 45, 46}
		};
	textures.push_back(&explosion4);
	explosionSmoke4 = TextureDef{
			"tanks_2/explosionSmoke4",
			GPU_Rect{0, 576, 45, 46}
		};
	textures.push_back(&explosionSmoke4);
	wireCrooked = TextureDef{
			"tanks_2/wireCrooked",
			GPU_Rect{181, 576, 48, 44}
		};
	textures.push_back(&wireCrooked);
	tankBody_blue_outline = TextureDef{
			"tanks_2/tankBody_blue_outline",
			GPU_Rect{431, 576, 42, 42}
		};
	textures.push_back(&tankBody_blue_outline);
	tank_green = TextureDef{
			"tanks_2/tank_green",
			GPU_Rect{477, 576, 46, 42}
		};
	textures.push_back(&tank_green);
	sandbagBrown_open = TextureDef{
			"tanks_2/sandbagBrown_open",
			GPU_Rect{527, 576, 28, 42}
		};
	textures.push_back(&sandbagBrown_open);
	tankBody_green_outline = TextureDef{
			"tanks_2/tankBody_green_outline",
			GPU_Rect{559, 576, 40, 42}
		};
	textures.push_back(&tankBody_green_outline);
	sandbagBeige_open = TextureDef{
			"tanks_2/sandbagBeige_open",
			GPU_Rect{603, 576, 28, 42}
		};
	textures.push_back(&sandbagBeige_open);
	tankBody_sand_outline = TextureDef{
			"tanks_2/tankBody_sand_outline",
			GPU_Rect{635, 576, 40, 42}
		};
	textures.push_back(&tankBody_sand_outline);
	tank_dark = TextureDef{
			"tanks_2/tank_dark",
			GPU_Rect{679, 576, 46, 42}
		};
	textures.push_back(&tank_dark);
	tank_blue = TextureDef{
			"tanks_2/tank_blue",
			GPU_Rect{0, 626, 46, 42}
		};
	textures.push_back(&tank_blue);
	tank_sand = TextureDef{
			"tanks_2/tank_sand",
			GPU_Rect{50, 626, 46, 42}
		};
	textures.push_back(&tank_sand);
	tankBody_dark_outline = TextureDef{
			"tanks_2/tankBody_dark_outline",
			GPU_Rect{100, 626, 40, 42}
		};
	textures.push_back(&tankBody_dark_outline);
	tracksDouble = TextureDef{
			"tanks_2/tracksDouble",
			GPU_Rect{144, 626, 52, 41}
		};
	textures.push_back(&tracksDouble);
	tracksLarge = TextureDef{
			"tanks_2/tracksLarge",
			GPU_Rect{200, 626, 52, 41}
		};
	textures.push_back(&tracksLarge);
	tankBody_sand = TextureDef{
			"tanks_2/tankBody_sand",
			GPU_Rect{256, 626, 36, 38}
		};
	textures.push_back(&tankBody_sand);
	tankBody_green = TextureDef{
			"tanks_2/tankBody_green",
			GPU_Rect{296, 626, 36, 38}
		};
	textures.push_back(&tankBody_green);
	tankBody_dark = TextureDef{
			"tanks_2/tankBody_dark",
			GPU_Rect{336, 626, 36, 38}
		};
	textures.push_back(&tankBody_dark);
	tankBody_blue = TextureDef{
			"tanks_2/tankBody_blue",
			GPU_Rect{376, 626, 38, 38}
		};
	textures.push_back(&tankBody_blue);
	tank_red = TextureDef{
			"tanks_2/tank_red",
			GPU_Rect{418, 626, 46, 38}
		};
	textures.push_back(&tank_red);
	tankBody_red_outline = TextureDef{
			"tanks_2/tankBody_red_outline",
			GPU_Rect{468, 626, 40, 38}
		};
	textures.push_back(&tankBody_red_outline);
	tracksSmall = TextureDef{
			"tanks_2/tracksSmall",
			GPU_Rect{512, 626, 52, 37}
		};
	textures.push_back(&tracksSmall);
	treeBrown_small = TextureDef{
			"tanks_2/treeBrown_small",
			GPU_Rect{568, 626, 36, 36}
		};
	textures.push_back(&treeBrown_small);
	treeGreen_small = TextureDef{
			"tanks_2/treeGreen_small",
			GPU_Rect{608, 626, 36, 36}
		};
	textures.push_back(&treeGreen_small);
	tankBody_red = TextureDef{
			"tanks_2/tankBody_red",
			GPU_Rect{648, 626, 36, 34}
		};
	textures.push_back(&tankBody_red);
	sandbagBeige = TextureDef{
			"tanks_2/sandbagBeige",
			GPU_Rect{688, 626, 22, 32}
		};
	textures.push_back(&sandbagBeige);
	sandbagBrown = TextureDef{
			"tanks_2/sandbagBrown",
			GPU_Rect{714, 626, 22, 32}
		};
	textures.push_back(&sandbagBrown);
	crateMetal_side = TextureDef{
			"tanks_2/crateMetal_side",
			GPU_Rect{64, 672, 28, 28}
		};
	textures.push_back(&crateMetal_side);
	crateWood_side = TextureDef{
			"tanks_2/crateWood_side",
			GPU_Rect{96, 672, 28, 28}
		};
	textures.push_back(&crateWood_side);
	crateWood = TextureDef{
			"tanks_2/crateWood",
			GPU_Rect{128, 672, 28, 28}
		};
	textures.push_back(&crateWood);
	crateMetal = TextureDef{
			"tanks_2/crateMetal",
			GPU_Rect{160, 672, 28, 28}
		};
	textures.push_back(&crateMetal);
	barricadeWood = TextureDef{
			"tanks_2/barricadeWood",
			GPU_Rect{192, 672, 28, 28}
		};
	textures.push_back(&barricadeWood);
	barricadeMetal = TextureDef{
			"tanks_2/barricadeMetal",
			GPU_Rect{224, 672, 28, 28}
		};
	textures.push_back(&barricadeMetal);
	treeBrown_twigs = TextureDef{
			"tanks_2/treeBrown_twigs",
			GPU_Rect{256, 672, 22, 26}
		};
	textures.push_back(&treeBrown_twigs);
	treeGreen_twigs = TextureDef{
			"tanks_2/treeGreen_twigs",
			GPU_Rect{312, 672, 22, 26}
		};
	textures.push_back(&treeGreen_twigs);
	barrelRed_top = TextureDef{
			"tanks_2/barrelRed_top",
			GPU_Rect{338, 672, 24, 24}
		};
	textures.push_back(&barrelRed_top);
	barrelRust_top = TextureDef{
			"tanks_2/barrelRust_top",
			GPU_Rect{366, 672, 24, 24}
		};
	textures.push_back(&barrelRust_top);
	barrelGreen_top = TextureDef{
			"tanks_2/barrelGreen_top",
			GPU_Rect{394, 672, 24, 24}
		};
	textures.push_back(&barrelGreen_top);
	barrelBlack_top = TextureDef{
			"tanks_2/barrelBlack_top",
			GPU_Rect{422, 672, 24, 24}
		};
	textures.push_back(&barrelBlack_top);
	shotRed = TextureDef{
			"tanks_2/shotRed",
			GPU_Rect{450, 672, 38, 21}
		};
	textures.push_back(&shotRed);
	barrelGreen_side = TextureDef{
			"tanks_2/barrelGreen_side",
			GPU_Rect{568, 672, 28, 20}
		};
	textures.push_back(&barrelGreen_side);
	barrelRust_side = TextureDef{
			"tanks_2/barrelRust_side",
			GPU_Rect{0, 706, 28, 20}
		};
	textures.push_back(&barrelRust_side);
	barrelBlack_side = TextureDef{
			"tanks_2/barrelBlack_side",
			GPU_Rect{32, 706, 28, 20}
		};
	textures.push_back(&barrelBlack_side);
	barrelRed_side = TextureDef{
			"tanks_2/barrelRed_side",
			GPU_Rect{64, 706, 28, 20}
		};
	textures.push_back(&barrelRed_side);
	shotLarge = TextureDef{
			"tanks_2/shotLarge",
			GPU_Rect{96, 706, 25, 20}
		};
	textures.push_back(&shotLarge);
	specialBarrel1_outline = TextureDef{
			"tanks_2/specialBarrel1_outline",
			GPU_Rect{277, 706, 26, 18}
		};
	textures.push_back(&specialBarrel1_outline);
	wireStraight = TextureDef{
			"tanks_2/wireStraight",
			GPU_Rect{307, 706, 70, 17}
		};
	textures.push_back(&wireStraight);
	tankSand_barrel1_outline = TextureDef{
			"tanks_2/tankSand_barrel1_outline",
			GPU_Rect{381, 706, 30, 16}
		};
	textures.push_back(&tankSand_barrel1_outline);
	specialBarrel5_outline = TextureDef{
			"tanks_2/specialBarrel5_outline",
			GPU_Rect{415, 706, 30, 16}
		};
	textures.push_back(&specialBarrel5_outline);
	tankBlue_barrel1_outline = TextureDef{
			"tanks_2/tankBlue_barrel1_outline",
			GPU_Rect{449, 706, 30, 16}
		};
	textures.push_back(&tankBlue_barrel1_outline);
	specialBarrel2_outline = TextureDef{
			"tanks_2/specialBarrel2_outline",
			GPU_Rect{483, 706, 28, 16}
		};
	textures.push_back(&specialBarrel2_outline);
	tankGreen_barrel1_outline = TextureDef{
			"tanks_2/tankGreen_barrel1_outline",
			GPU_Rect{515, 706, 30, 16}
		};
	textures.push_back(&tankGreen_barrel1_outline);
	tankRed_barrel1_outline = TextureDef{
			"tanks_2/tankRed_barrel1_outline",
			GPU_Rect{549, 706, 30, 16}
		};
	textures.push_back(&tankRed_barrel1_outline);
	tankDark_barrel1_outline = TextureDef{
			"tanks_2/tankDark_barrel1_outline",
			GPU_Rect{583, 706, 30, 16}
		};
	textures.push_back(&tankDark_barrel1_outline);
	shotOrange = TextureDef{
			"tanks_2/shotOrange",
			GPU_Rect{617, 706, 28, 16}
		};
	textures.push_back(&shotOrange);
	specialBarrel3_outline = TextureDef{
			"tanks_2/specialBarrel3_outline",
			GPU_Rect{666, 706, 32, 14}
		};
	textures.push_back(&specialBarrel3_outline);
	oilSpill_small = TextureDef{
			"tanks_2/oilSpill_small",
			GPU_Rect{702, 706, 14, 14}
		};
	textures.push_back(&oilSpill_small);
	specialBarrel4_outline = TextureDef{
			"tanks_2/specialBarrel4_outline",
			GPU_Rect{720, 706, 36, 14}
		};
	textures.push_back(&specialBarrel4_outline);
	specialBarrel1 = TextureDef{
			"tanks_2/specialBarrel1",
			GPU_Rect{737, 0, 22, 14}
		};
	textures.push_back(&specialBarrel1);
	tankSand_barrel2_outline = TextureDef{
			"tanks_2/tankSand_barrel2_outline",
			GPU_Rect{91, 730, 30, 12}
		};
	textures.push_back(&tankSand_barrel2_outline);
	bulletSand2_outline = TextureDef{
			"tanks_2/bulletSand2_outline",
			GPU_Rect{125, 730, 16, 12}
		};
	textures.push_back(&bulletSand2_outline);
	tankSand_barrel3_outline = TextureDef{
			"tanks_2/tankSand_barrel3_outline",
			GPU_Rect{145, 730, 30, 12}
		};
	textures.push_back(&tankSand_barrel3_outline);
	bulletBlue2_outline = TextureDef{
			"tanks_2/bulletBlue2_outline",
			GPU_Rect{179, 730, 16, 12}
		};
	textures.push_back(&bulletBlue2_outline);
	tankGreen_barrel2_outline = TextureDef{
			"tanks_2/tankGreen_barrel2_outline",
			GPU_Rect{247, 730, 30, 12}
		};
	textures.push_back(&tankGreen_barrel2_outline);
	tankRed_barrel3_outline = TextureDef{
			"tanks_2/tankRed_barrel3_outline",
			GPU_Rect{281, 730, 30, 12}
		};
	textures.push_back(&tankRed_barrel3_outline);
	specialBarrel2 = TextureDef{
			"tanks_2/specialBarrel2",
			GPU_Rect{315, 730, 24, 12}
		};
	textures.push_back(&specialBarrel2);
	bulletDark2_outline = TextureDef{
			"tanks_2/bulletDark2_outline",
			GPU_Rect{373, 730, 16, 12}
		};
	textures.push_back(&bulletDark2_outline);
	tankSand_barrel1 = TextureDef{
			"tanks_2/tankSand_barrel1",
			GPU_Rect{393, 730, 26, 12}
		};
	textures.push_back(&tankSand_barrel1);
	tankGreen_barrel1 = TextureDef{
			"tanks_2/tankGreen_barrel1",
			GPU_Rect{423, 730, 26, 12}
		};
	textures.push_back(&tankGreen_barrel1);
	tankDark_barrel3_outline = TextureDef{
			"tanks_2/tankDark_barrel3_outline",
			GPU_Rect{453, 730, 30, 12}
		};
	textures.push_back(&tankDark_barrel3_outline);
	tankDark_barrel2_outline = TextureDef{
			"tanks_2/tankDark_barrel2_outline",
			GPU_Rect{487, 730, 30, 12}
		};
	textures.push_back(&tankDark_barrel2_outline);
	tankDark_barrel1 = TextureDef{
			"tanks_2/tankDark_barrel1",
			GPU_Rect{551, 730, 26, 12}
		};
	textures.push_back(&tankDark_barrel1);
	tankBlue_barrel3_outline = TextureDef{
			"tanks_2/tankBlue_barrel3_outline",
			GPU_Rect{581, 730, 30, 12}
		};
	textures.push_back(&tankBlue_barrel3_outline);
	bulletRed2_outline = TextureDef{
			"tanks_2/bulletRed2_outline",
			GPU_Rect{615, 730, 16, 12}
		};
	textures.push_back(&bulletRed2_outline);
	tankBlue_barrel2_outline = TextureDef{
			"tanks_2/tankBlue_barrel2_outline",
			GPU_Rect{635, 730, 30, 12}
		};
	textures.push_back(&tankBlue_barrel2_outline);
	bulletGreen2_outline = TextureDef{
			"tanks_2/bulletGreen2_outline",
			GPU_Rect{669, 730, 16, 12}
		};
	textures.push_back(&bulletGreen2_outline);
	tankGreen_barrel3_outline = TextureDef{
			"tanks_2/tankGreen_barrel3_outline",
			GPU_Rect{689, 730, 30, 12}
		};
	textures.push_back(&tankGreen_barrel3_outline);
	tankBlue_barrel1 = TextureDef{
			"tanks_2/tankBlue_barrel1",
			GPU_Rect{723, 730, 26, 12}
		};
	textures.push_back(&tankBlue_barrel1);
	specialBarrel7_outline = TextureDef{
			"tanks_2/specialBarrel7_outline",
			GPU_Rect{0, 747, 30, 12}
		};
	textures.push_back(&specialBarrel7_outline);
	specialBarrel6_outline = TextureDef{
			"tanks_2/specialBarrel6_outline",
			GPU_Rect{124, 747, 30, 12}
		};
	textures.push_back(&specialBarrel6_outline);
	tankRed_barrel1 = TextureDef{
			"tanks_2/tankRed_barrel1",
			GPU_Rect{218, 747, 26, 12}
		};
	textures.push_back(&tankRed_barrel1);
	tankRed_barrel2_outline = TextureDef{
			"tanks_2/tankRed_barrel2_outline",
			GPU_Rect{248, 747, 30, 12}
		};
	textures.push_back(&tankRed_barrel2_outline);
	specialBarrel5 = TextureDef{
			"tanks_2/specialBarrel5",
			GPU_Rect{342, 747, 26, 12}
		};
	textures.push_back(&specialBarrel5);
	specialBarrel4 = TextureDef{
			"tanks_2/specialBarrel4",
			GPU_Rect{462, 747, 32, 10}
		};
	textures.push_back(&specialBarrel4);
	specialBarrel3 = TextureDef{
			"tanks_2/specialBarrel3",
			GPU_Rect{498, 747, 28, 10}
		};
	textures.push_back(&specialBarrel3);
	tankBlue_barrel3 = TextureDef{
			"tanks_2/tankBlue_barrel3",
			GPU_Rect{611, 747, 26, 8}
		};
	textures.push_back(&tankBlue_barrel3);
	tankDark_barrel2 = TextureDef{
			"tanks_2/tankDark_barrel2",
			GPU_Rect{641, 747, 26, 8}
		};
	textures.push_back(&tankDark_barrel2);
	tankGreen_barrel2 = TextureDef{
			"tanks_2/tankGreen_barrel2",
			GPU_Rect{671, 747, 26, 8}
		};
	textures.push_back(&tankGreen_barrel2);
	treeBrown_leaf = TextureDef{
			"tanks_2/treeBrown_leaf",
			GPU_Rect{701, 747, 10, 8}
		};
	textures.push_back(&treeBrown_leaf);
	tankDark_barrel3 = TextureDef{
			"tanks_2/tankDark_barrel3",
			GPU_Rect{715, 747, 26, 8}
		};
	textures.push_back(&tankDark_barrel3);
	tankGreen_barrel3 = TextureDef{
			"tanks_2/tankGreen_barrel3",
			GPU_Rect{729, 610, 26, 8}
		};
	textures.push_back(&tankGreen_barrel3);
	shotThin = TextureDef{
			"tanks_2/shotThin",
			GPU_Rect{249, 370, 26, 8}
		};
	textures.push_back(&shotThin);
	tankSand_barrel3 = TextureDef{
			"tanks_2/tankSand_barrel3",
			GPU_Rect{279, 370, 26, 8}
		};
	textures.push_back(&tankSand_barrel3);
	specialBarrel7 = TextureDef{
			"tanks_2/specialBarrel7",
			GPU_Rect{309, 370, 26, 8}
		};
	textures.push_back(&specialBarrel7);
	bulletBlue1_outline = TextureDef{
			"tanks_2/bulletBlue1_outline",
			GPU_Rect{339, 370, 14, 8}
		};
	textures.push_back(&bulletBlue1_outline);
	bulletSand3_outline = TextureDef{
			"tanks_2/bulletSand3_outline",
			GPU_Rect{357, 370, 18, 8}
		};
	textures.push_back(&bulletSand3_outline);
	tankBlue_barrel2 = TextureDef{
			"tanks_2/tankBlue_barrel2",
			GPU_Rect{379, 370, 26, 8}
		};
	textures.push_back(&tankBlue_barrel2);
	bulletBlue2 = TextureDef{
			"tanks_2/bulletBlue2",
			GPU_Rect{409, 370, 12, 8}
		};
	textures.push_back(&bulletBlue2);
	bulletSand1_outline = TextureDef{
			"tanks_2/bulletSand1_outline",
			GPU_Rect{425, 370, 14, 8}
		};
	textures.push_back(&bulletSand1_outline);
	bulletBlue3_outline = TextureDef{
			"tanks_2/bulletBlue3_outline",
			GPU_Rect{443, 370, 18, 8}
		};
	textures.push_back(&bulletBlue3_outline);
	bulletSand2 = TextureDef{
			"tanks_2/bulletSand2",
			GPU_Rect{465, 370, 12, 8}
		};
	textures.push_back(&bulletSand2);
	bulletRed3_outline = TextureDef{
			"tanks_2/bulletRed3_outline",
			GPU_Rect{481, 370, 18, 8}
		};
	textures.push_back(&bulletRed3_outline);
	specialBarrel6 = TextureDef{
			"tanks_2/specialBarrel6",
			GPU_Rect{503, 370, 26, 8}
		};
	textures.push_back(&specialBarrel6);
	bulletRed1_outline = TextureDef{
			"tanks_2/bulletRed1_outline",
			GPU_Rect{533, 370, 14, 8}
		};
	textures.push_back(&bulletRed1_outline);
	tankRed_barrel2 = TextureDef{
			"tanks_2/tankRed_barrel2",
			GPU_Rect{551, 370, 26, 8}
		};
	textures.push_back(&tankRed_barrel2);
	tankSand_barrel2 = TextureDef{
			"tanks_2/tankSand_barrel2",
			GPU_Rect{581, 370, 26, 8}
		};
	textures.push_back(&tankSand_barrel2);
	bulletGreen3_outline = TextureDef{
			"tanks_2/bulletGreen3_outline",
			GPU_Rect{611, 370, 18, 8}
		};
	textures.push_back(&bulletGreen3_outline);
	bulletDark2 = TextureDef{
			"tanks_2/bulletDark2",
			GPU_Rect{633, 370, 12, 8}
		};
	textures.push_back(&bulletDark2);
	tankRed_barrel3 = TextureDef{
			"tanks_2/tankRed_barrel3",
			GPU_Rect{649, 370, 26, 8}
		};
	textures.push_back(&tankRed_barrel3);
	bulletDark1_outline = TextureDef{
			"tanks_2/bulletDark1_outline",
			GPU_Rect{679, 370, 14, 8}
		};
	textures.push_back(&bulletDark1_outline);
	bulletGreen1_outline = TextureDef{
			"tanks_2/bulletGreen1_outline",
			GPU_Rect{697, 370, 14, 8}
		};
	textures.push_back(&bulletGreen1_outline);
	bulletGreen2 = TextureDef{
			"tanks_2/bulletGreen2",
			GPU_Rect{715, 370, 12, 8}
		};
	textures.push_back(&bulletGreen2);
	bulletDark3_outline = TextureDef{
			"tanks_2/bulletDark3_outline",
			GPU_Rect{731, 370, 18, 8}
		};
	textures.push_back(&bulletDark3_outline);
	treeGreen_leaf = TextureDef{
			"tanks_2/treeGreen_leaf",
			GPU_Rect{745, 747, 10, 8}
		};
	textures.push_back(&treeGreen_leaf);
	bulletRed2 = TextureDef{
			"tanks_2/bulletRed2",
			GPU_Rect{747, 451, 12, 8}
		};
	textures.push_back(&bulletRed2);
	bulletDark3 = TextureDef{
			"tanks_2/bulletDark3",
			GPU_Rect{673, 289, 14, 4}
		};
	textures.push_back(&bulletDark3);
	bulletGreen3 = TextureDef{
			"tanks_2/bulletGreen3",
			GPU_Rect{691, 289, 14, 4}
		};
	textures.push_back(&bulletGreen3);
	bulletDark1 = TextureDef{
			"tanks_2/bulletDark1",
			GPU_Rect{747, 463, 10, 4}
		};
	textures.push_back(&bulletDark1);
	bulletRed1 = TextureDef{
			"tanks_2/bulletRed1",
			GPU_Rect{747, 471, 10, 4}
		};
	textures.push_back(&bulletRed1);
	bulletGreen1 = TextureDef{
			"tanks_2/bulletGreen1",
			GPU_Rect{747, 479, 10, 4}
		};
	textures.push_back(&bulletGreen1);
	bulletRed3 = TextureDef{
			"tanks_2/bulletRed3",
			GPU_Rect{709, 289, 14, 4}
		};
	textures.push_back(&bulletRed3);
	bulletSand1 = TextureDef{
			"tanks_2/bulletSand1",
			GPU_Rect{747, 487, 10, 4}
		};
	textures.push_back(&bulletSand1);
	bulletSand3 = TextureDef{
			"tanks_2/bulletSand3",
			GPU_Rect{727, 289, 14, 4}
		};
	textures.push_back(&bulletSand3);
	bulletBlue1 = TextureDef{
			"tanks_2/bulletBlue1",
			GPU_Rect{747, 495, 10, 4}
		};
	textures.push_back(&bulletBlue1);
	bulletBlue3 = TextureDef{
			"tanks_2/bulletBlue3",
			GPU_Rect{745, 289, 14, 4}
		};
	textures.push_back(&bulletBlue3);
	
};

r_alp0p::r_alp0p()
{
	
	
	folders.push_back(&Smoke);
	folders.push_back(&Obstacles);
	folders.push_back(&Bullets);
};

TextureNames::TextureNames()
{
	
	
	folders.push_back(&tanks_1);
	folders.push_back(&td);
	folders.push_back(&tanks_2);
	folders.push_back(&art);
};
