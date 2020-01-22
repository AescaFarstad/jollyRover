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
			GPU_Rect{211, 730, 12, 12}
		};
	textures.push_back(&s1);
	s2 = TextureDef{
			"art/path/gray/s2",
			GPU_Rect{227, 730, 12, 12}
		};
	textures.push_back(&s2);
	s3 = TextureDef{
			"art/path/gray/s3",
			GPU_Rect{341, 730, 12, 12}
		};
	textures.push_back(&s3);
	b2 = TextureDef{
			"art/path/gray/b2",
			GPU_Rect{654, 748, 21, 10}
		};
	textures.push_back(&b2);
	b3 = TextureDef{
			"art/path/gray/b3",
			GPU_Rect{679, 748, 25, 10}
		};
	textures.push_back(&b3);
	b1 = TextureDef{
			"art/path/gray/b1",
			GPU_Rect{708, 748, 23, 10}
		};
	textures.push_back(&b1);
	
};

r_byo84::r_byo84()
{
	
	b1 = TextureDef{
			"art/path/red/b1",
			GPU_Rect{733, 576, 26, 30}
		};
	textures.push_back(&b1);
	b3 = TextureDef{
			"art/path/red/b3",
			GPU_Rect{0, 672, 28, 30}
		};
	textures.push_back(&b3);
	b4 = TextureDef{
			"art/path/red/b4",
			GPU_Rect{32, 672, 30, 30}
		};
	textures.push_back(&b4);
	b2 = TextureDef{
			"art/path/red/b2",
			GPU_Rect{258, 672, 26, 26}
		};
	textures.push_back(&b2);
	s2 = TextureDef{
			"art/path/red/s2",
			GPU_Rect{683, 706, 14, 14}
		};
	textures.push_back(&s2);
	s3 = TextureDef{
			"art/path/red/s3",
			GPU_Rect{701, 706, 13, 14}
		};
	textures.push_back(&s3);
	s1 = TextureDef{
			"art/path/red/s1",
			GPU_Rect{193, 730, 14, 13}
		};
	textures.push_back(&s1);
	
};

r_yp1st::r_yp1st()
{
	
	s1 = TextureDef{
			"art/path/green/s1",
			GPU_Rect{36, 730, 18, 13}
		};
	textures.push_back(&s1);
	b3 = TextureDef{
			"art/path/green/b3",
			GPU_Rect{58, 730, 25, 13}
		};
	textures.push_back(&b3);
	b1 = TextureDef{
			"art/path/green/b1",
			GPU_Rect{87, 730, 26, 13}
		};
	textures.push_back(&b1);
	s2 = TextureDef{
			"art/path/green/s2",
			GPU_Rect{117, 730, 19, 13}
		};
	textures.push_back(&s2);
	b2 = TextureDef{
			"art/path/green/b2",
			GPU_Rect{140, 730, 28, 13}
		};
	textures.push_back(&b2);
	s3 = TextureDef{
			"art/path/green/s3",
			GPU_Rect{172, 730, 17, 13}
		};
	textures.push_back(&s3);
	
};

r_hbzjo::r_hbzjo()
{
	
	smokeGrey4 = TextureDef{
			"tanks_1/Smoke/smokeGrey4",
			GPU_Rect{0, 0, 97, 100}
		};
	textures.push_back(&smokeGrey4);
	smokeOrange4 = TextureDef{
			"tanks_1/Smoke/smokeOrange4",
			GPU_Rect{101, 0, 97, 100}
		};
	textures.push_back(&smokeOrange4);
	smokeWhite3 = TextureDef{
			"tanks_1/Smoke/smokeWhite3",
			GPU_Rect{202, 0, 97, 100}
		};
	textures.push_back(&smokeWhite3);
	smokeYellow4 = TextureDef{
			"tanks_1/Smoke/smokeYellow4",
			GPU_Rect{303, 0, 97, 100}
		};
	textures.push_back(&smokeYellow4);
	smokeYellow5 = TextureDef{
			"tanks_1/Smoke/smokeYellow5",
			GPU_Rect{404, 0, 107, 98}
		};
	textures.push_back(&smokeYellow5);
	smokeGrey5 = TextureDef{
			"tanks_1/Smoke/smokeGrey5",
			GPU_Rect{515, 0, 107, 98}
		};
	textures.push_back(&smokeGrey5);
	smokeWhite4 = TextureDef{
			"tanks_1/Smoke/smokeWhite4",
			GPU_Rect{626, 0, 107, 98}
		};
	textures.push_back(&smokeWhite4);
	smokeOrange5 = TextureDef{
			"tanks_1/Smoke/smokeOrange5",
			GPU_Rect{0, 104, 107, 98}
		};
	textures.push_back(&smokeOrange5);
	smokeWhite0 = TextureDef{
			"tanks_1/Smoke/smokeWhite0",
			GPU_Rect{211, 104, 89, 92}
		};
	textures.push_back(&smokeWhite0);
	smokeOrange1 = TextureDef{
			"tanks_1/Smoke/smokeOrange1",
			GPU_Rect{304, 104, 89, 92}
		};
	textures.push_back(&smokeOrange1);
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
	smokeWhite1 = TextureDef{
			"tanks_1/Smoke/smokeWhite1",
			GPU_Rect{583, 104, 99, 90}
		};
	textures.push_back(&smokeWhite1);
	smokeGrey2 = TextureDef{
			"tanks_1/Smoke/smokeGrey2",
			GPU_Rect{0, 206, 99, 90}
		};
	textures.push_back(&smokeGrey2);
	smokeOrange2 = TextureDef{
			"tanks_1/Smoke/smokeOrange2",
			GPU_Rect{103, 206, 99, 90}
		};
	textures.push_back(&smokeOrange2);
	smokeYellow2 = TextureDef{
			"tanks_1/Smoke/smokeYellow2",
			GPU_Rect{206, 206, 99, 90}
		};
	textures.push_back(&smokeYellow2);
	smokeYellow0 = TextureDef{
			"tanks_1/Smoke/smokeYellow0",
			GPU_Rect{309, 206, 87, 87}
		};
	textures.push_back(&smokeYellow0);
	smokeOrange0 = TextureDef{
			"tanks_1/Smoke/smokeOrange0",
			GPU_Rect{400, 206, 87, 87}
		};
	textures.push_back(&smokeOrange0);
	smokeWhite5 = TextureDef{
			"tanks_1/Smoke/smokeWhite5",
			GPU_Rect{491, 206, 87, 87}
		};
	textures.push_back(&smokeWhite5);
	smokeGrey0 = TextureDef{
			"tanks_1/Smoke/smokeGrey0",
			GPU_Rect{582, 206, 87, 87}
		};
	textures.push_back(&smokeGrey0);
	smokeYellow3 = TextureDef{
			"tanks_1/Smoke/smokeYellow3",
			GPU_Rect{673, 206, 79, 79}
		};
	textures.push_back(&smokeYellow3);
	smokeOrange3 = TextureDef{
			"tanks_1/Smoke/smokeOrange3",
			GPU_Rect{0, 300, 79, 79}
		};
	textures.push_back(&smokeOrange3);
	smokeGrey3 = TextureDef{
			"tanks_1/Smoke/smokeGrey3",
			GPU_Rect{83, 300, 79, 79}
		};
	textures.push_back(&smokeGrey3);
	smokeWhite2 = TextureDef{
			"tanks_1/Smoke/smokeWhite2",
			GPU_Rect{166, 300, 79, 79}
		};
	textures.push_back(&smokeWhite2);
	
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
	barrelGrey_up = TextureDef{
			"tanks_1/Obstacles/barrelGrey_up",
			GPU_Rect{498, 519, 48, 48}
		};
	textures.push_back(&barrelGrey_up);
	barrelRed_up = TextureDef{
			"tanks_1/Obstacles/barrelRed_up",
			GPU_Rect{550, 519, 48, 48}
		};
	textures.push_back(&barrelRed_up);
	barrelGreen_up = TextureDef{
			"tanks_1/Obstacles/barrelGreen_up",
			GPU_Rect{602, 519, 48, 48}
		};
	textures.push_back(&barrelGreen_up);
	barrelRed_side = TextureDef{
			"tanks_1/Obstacles/barrelRed_side",
			GPU_Rect{49, 576, 62, 44}
		};
	textures.push_back(&barrelRed_side);
	barrelGreen_side_damaged = TextureDef{
			"tanks_1/Obstacles/barrelGreen_side_damaged",
			GPU_Rect{115, 576, 62, 44}
		};
	textures.push_back(&barrelGreen_side_damaged);
	barrelGreen_side = TextureDef{
			"tanks_1/Obstacles/barrelGreen_side",
			GPU_Rect{181, 576, 62, 44}
		};
	textures.push_back(&barrelGreen_side);
	barrelGrey_side = TextureDef{
			"tanks_1/Obstacles/barrelGrey_side",
			GPU_Rect{247, 576, 62, 44}
		};
	textures.push_back(&barrelGrey_side);
	barrelGrey_sde_rust = TextureDef{
			"tanks_1/Obstacles/barrelGrey_sde_rust",
			GPU_Rect{313, 576, 62, 44}
		};
	textures.push_back(&barrelGrey_sde_rust);
	
};

r_mt6ul::r_mt6ul()
{
	
	
	folders.push_back(&red);
	folders.push_back(&green);
	folders.push_back(&gray);
};

r_5q2x0::r_5q2x0()
{
	
	bulletYellowSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletYellowSilver_outline",
			GPU_Rect{494, 672, 34, 20}
		};
	textures.push_back(&bulletYellowSilver_outline);
	bulletSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletSilver_outline",
			GPU_Rect{532, 672, 34, 20}
		};
	textures.push_back(&bulletSilver_outline);
	bulletSilverSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletSilverSilver_outline",
			GPU_Rect{570, 672, 34, 20}
		};
	textures.push_back(&bulletSilverSilver_outline);
	bulletRed_outline = TextureDef{
			"tanks_1/Bullets/bulletRed_outline",
			GPU_Rect{608, 672, 34, 20}
		};
	textures.push_back(&bulletRed_outline);
	bulletRedSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletRedSilver_outline",
			GPU_Rect{646, 672, 34, 20}
		};
	textures.push_back(&bulletRedSilver_outline);
	bulletGreen_outline = TextureDef{
			"tanks_1/Bullets/bulletGreen_outline",
			GPU_Rect{684, 672, 34, 20}
		};
	textures.push_back(&bulletGreen_outline);
	bulletGreenSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletGreenSilver_outline",
			GPU_Rect{722, 672, 34, 20}
		};
	textures.push_back(&bulletGreenSilver_outline);
	bulletBlue_outline = TextureDef{
			"tanks_1/Bullets/bulletBlue_outline",
			GPU_Rect{686, 172, 34, 20}
		};
	textures.push_back(&bulletBlue_outline);
	bulletBlueSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletBlueSilver_outline",
			GPU_Rect{724, 172, 34, 20}
		};
	textures.push_back(&bulletBlueSilver_outline);
	bulletBeige_outline = TextureDef{
			"tanks_1/Bullets/bulletBeige_outline",
			GPU_Rect{0, 706, 34, 20}
		};
	textures.push_back(&bulletBeige_outline);
	bulletBeigeSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletBeigeSilver_outline",
			GPU_Rect{38, 706, 34, 20}
		};
	textures.push_back(&bulletBeigeSilver_outline);
	bulletYellow_outline = TextureDef{
			"tanks_1/Bullets/bulletYellow_outline",
			GPU_Rect{233, 706, 34, 20}
		};
	textures.push_back(&bulletYellow_outline);
	bulletBeige = TextureDef{
			"tanks_1/Bullets/bulletBeige",
			GPU_Rect{625, 730, 26, 12}
		};
	textures.push_back(&bulletBeige);
	bulletBeigeSilver = TextureDef{
			"tanks_1/Bullets/bulletBeigeSilver",
			GPU_Rect{685, 730, 26, 12}
		};
	textures.push_back(&bulletBeigeSilver);
	bulletBlue = TextureDef{
			"tanks_1/Bullets/bulletBlue",
			GPU_Rect{0, 748, 26, 12}
		};
	textures.push_back(&bulletBlue);
	bulletBlueSilver = TextureDef{
			"tanks_1/Bullets/bulletBlueSilver",
			GPU_Rect{30, 748, 26, 12}
		};
	textures.push_back(&bulletBlueSilver);
	bulletGreenSilver = TextureDef{
			"tanks_1/Bullets/bulletGreenSilver",
			GPU_Rect{94, 748, 26, 12}
		};
	textures.push_back(&bulletGreenSilver);
	bulletGreen = TextureDef{
			"tanks_1/Bullets/bulletGreen",
			GPU_Rect{154, 748, 26, 12}
		};
	textures.push_back(&bulletGreen);
	bulletRed = TextureDef{
			"tanks_1/Bullets/bulletRed",
			GPU_Rect{184, 748, 26, 12}
		};
	textures.push_back(&bulletRed);
	bulletRedSilver = TextureDef{
			"tanks_1/Bullets/bulletRedSilver",
			GPU_Rect{264, 748, 26, 12}
		};
	textures.push_back(&bulletRedSilver);
	bulletSilver = TextureDef{
			"tanks_1/Bullets/bulletSilver",
			GPU_Rect{314, 748, 26, 12}
		};
	textures.push_back(&bulletSilver);
	bulletYellow = TextureDef{
			"tanks_1/Bullets/bulletYellow",
			GPU_Rect{344, 748, 26, 12}
		};
	textures.push_back(&bulletYellow);
	bulletSilverSilver = TextureDef{
			"tanks_1/Bullets/bulletSilverSilver",
			GPU_Rect{374, 748, 26, 12}
		};
	textures.push_back(&bulletSilverSilver);
	bulletYellowSilver = TextureDef{
			"tanks_1/Bullets/bulletYellowSilver",
			GPU_Rect{468, 748, 26, 12}
		};
	textures.push_back(&bulletYellowSilver);
	
};

r_ir8mj::r_ir8mj()
{
	
	
	folders.push_back(&path);
};

r_xhrer::r_xhrer()
{
	
	fire2 = TextureDef{
			"td/fire2",
			GPU_Rect{345, 300, 64, 64}
		};
	textures.push_back(&fire2);
	fire4 = TextureDef{
			"td/fire4",
			GPU_Rect{481, 300, 64, 64}
		};
	textures.push_back(&fire4);
	fire3 = TextureDef{
			"td/fire3",
			GPU_Rect{549, 300, 64, 64}
		};
	textures.push_back(&fire3);
	plane_gray = TextureDef{
			"td/plane_gray",
			GPU_Rect{617, 300, 64, 64}
		};
	textures.push_back(&plane_gray);
	plane_green = TextureDef{
			"td/plane_green",
			GPU_Rect{685, 300, 64, 64}
		};
	textures.push_back(&plane_green);
	plane_shadow_green = TextureDef{
			"td/plane_shadow_green",
			GPU_Rect{686, 104, 64, 64}
		};
	textures.push_back(&plane_shadow_green);
	tank_base_beige = TextureDef{
			"td/tank_base_beige",
			GPU_Rect{0, 383, 64, 64}
		};
	textures.push_back(&tank_base_beige);
	plane_shadow_gray = TextureDef{
			"td/plane_shadow_gray",
			GPU_Rect{68, 383, 64, 64}
		};
	textures.push_back(&plane_shadow_gray);
	rocket_small = TextureDef{
			"td/rocket_small",
			GPU_Rect{136, 383, 64, 64}
		};
	textures.push_back(&rocket_small);
	rocket_large = TextureDef{
			"td/rocket_large",
			GPU_Rect{204, 383, 64, 64}
		};
	textures.push_back(&rocket_large);
	tank_base_green = TextureDef{
			"td/tank_base_green",
			GPU_Rect{272, 383, 64, 64}
		};
	textures.push_back(&tank_base_green);
	soldier2 = TextureDef{
			"td/soldier2",
			GPU_Rect{340, 383, 64, 64}
		};
	textures.push_back(&soldier2);
	fire1 = TextureDef{
			"td/fire1",
			GPU_Rect{475, 383, 64, 64}
		};
	textures.push_back(&fire1);
	tankTurret_beige = TextureDef{
			"td/tankTurret_beige",
			GPU_Rect{543, 383, 64, 64}
		};
	textures.push_back(&tankTurret_beige);
	tankTurret_green = TextureDef{
			"td/tankTurret_green",
			GPU_Rect{679, 383, 64, 64}
		};
	textures.push_back(&tankTurret_green);
	soldier1 = TextureDef{
			"td/soldier1",
			GPU_Rect{0, 451, 64, 64}
		};
	textures.push_back(&soldier1);
	soldier4 = TextureDef{
			"td/soldier4",
			GPU_Rect{68, 451, 64, 64}
		};
	textures.push_back(&soldier4);
	soldier3 = TextureDef{
			"td/soldier3",
			GPU_Rect{136, 451, 64, 64}
		};
	textures.push_back(&soldier3);
	
};

r_jr3fj::r_jr3fj()
{
	
	treeBrown_large = TextureDef{
			"tanks_2/treeBrown_large",
			GPU_Rect{413, 300, 64, 64}
		};
	textures.push_back(&treeBrown_large);
	explosion3 = TextureDef{
			"tanks_2/explosion3",
			GPU_Rect{408, 383, 63, 64}
		};
	textures.push_back(&explosion3);
	treeGreen_large = TextureDef{
			"tanks_2/treeGreen_large",
			GPU_Rect{611, 383, 64, 64}
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
	explosionSmoke1 = TextureDef{
			"tanks_2/explosionSmoke1",
			GPU_Rect{427, 451, 60, 60}
		};
	textures.push_back(&explosionSmoke1);
	explosion1 = TextureDef{
			"tanks_2/explosion1",
			GPU_Rect{491, 451, 60, 60}
		};
	textures.push_back(&explosion1);
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
	explosionSmoke5 = TextureDef{
			"tanks_2/explosionSmoke5",
			GPU_Rect{0, 519, 52, 53}
		};
	textures.push_back(&explosionSmoke5);
	explosion5 = TextureDef{
			"tanks_2/explosion5",
			GPU_Rect{56, 519, 52, 53}
		};
	textures.push_back(&explosion5);
	tank_darkLarge = TextureDef{
			"tanks_2/tank_darkLarge",
			GPU_Rect{112, 519, 60, 52}
		};
	textures.push_back(&tank_darkLarge);
	fenceYellow = TextureDef{
			"tanks_2/fenceYellow",
			GPU_Rect{176, 519, 16, 52}
		};
	textures.push_back(&fenceYellow);
	tankBody_bigRed_outline = TextureDef{
			"tanks_2/tankBody_bigRed_outline",
			GPU_Rect{196, 519, 52, 52}
		};
	textures.push_back(&tankBody_bigRed_outline);
	tankBody_darkLarge_outline = TextureDef{
			"tanks_2/tankBody_darkLarge_outline",
			GPU_Rect{252, 519, 60, 52}
		};
	textures.push_back(&tankBody_darkLarge_outline);
	tank_bigRed = TextureDef{
			"tanks_2/tank_bigRed",
			GPU_Rect{316, 519, 52, 52}
		};
	textures.push_back(&tank_bigRed);
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
			GPU_Rect{478, 519, 16, 48}
		};
	textures.push_back(&fenceRed);
	tankBody_darkLarge = TextureDef{
			"tanks_2/tankBody_darkLarge",
			GPU_Rect{654, 519, 56, 48}
		};
	textures.push_back(&tankBody_darkLarge);
	explosionSmoke4 = TextureDef{
			"tanks_2/explosionSmoke4",
			GPU_Rect{714, 519, 45, 46}
		};
	textures.push_back(&explosionSmoke4);
	explosion4 = TextureDef{
			"tanks_2/explosion4",
			GPU_Rect{0, 576, 45, 46}
		};
	textures.push_back(&explosion4);
	wireCrooked = TextureDef{
			"tanks_2/wireCrooked",
			GPU_Rect{379, 576, 48, 44}
		};
	textures.push_back(&wireCrooked);
	sandbagBeige_open = TextureDef{
			"tanks_2/sandbagBeige_open",
			GPU_Rect{431, 576, 28, 42}
		};
	textures.push_back(&sandbagBeige_open);
	tank_sand = TextureDef{
			"tanks_2/tank_sand",
			GPU_Rect{463, 576, 46, 42}
		};
	textures.push_back(&tank_sand);
	tankBody_sand_outline = TextureDef{
			"tanks_2/tankBody_sand_outline",
			GPU_Rect{513, 576, 40, 42}
		};
	textures.push_back(&tankBody_sand_outline);
	tank_blue = TextureDef{
			"tanks_2/tank_blue",
			GPU_Rect{557, 576, 46, 42}
		};
	textures.push_back(&tank_blue);
	tankBody_dark_outline = TextureDef{
			"tanks_2/tankBody_dark_outline",
			GPU_Rect{607, 576, 40, 42}
		};
	textures.push_back(&tankBody_dark_outline);
	tank_green = TextureDef{
			"tanks_2/tank_green",
			GPU_Rect{651, 576, 46, 42}
		};
	textures.push_back(&tank_green);
	sandbagBrown_open = TextureDef{
			"tanks_2/sandbagBrown_open",
			GPU_Rect{701, 576, 28, 42}
		};
	textures.push_back(&sandbagBrown_open);
	tank_dark = TextureDef{
			"tanks_2/tank_dark",
			GPU_Rect{0, 626, 46, 42}
		};
	textures.push_back(&tank_dark);
	tankBody_blue_outline = TextureDef{
			"tanks_2/tankBody_blue_outline",
			GPU_Rect{50, 626, 42, 42}
		};
	textures.push_back(&tankBody_blue_outline);
	tankBody_green_outline = TextureDef{
			"tanks_2/tankBody_green_outline",
			GPU_Rect{96, 626, 40, 42}
		};
	textures.push_back(&tankBody_green_outline);
	tracksLarge = TextureDef{
			"tanks_2/tracksLarge",
			GPU_Rect{140, 626, 52, 41}
		};
	textures.push_back(&tracksLarge);
	tracksDouble = TextureDef{
			"tanks_2/tracksDouble",
			GPU_Rect{196, 626, 52, 41}
		};
	textures.push_back(&tracksDouble);
	tankBody_sand = TextureDef{
			"tanks_2/tankBody_sand",
			GPU_Rect{252, 626, 36, 38}
		};
	textures.push_back(&tankBody_sand);
	tankBody_red_outline = TextureDef{
			"tanks_2/tankBody_red_outline",
			GPU_Rect{292, 626, 40, 38}
		};
	textures.push_back(&tankBody_red_outline);
	tankBody_green = TextureDef{
			"tanks_2/tankBody_green",
			GPU_Rect{336, 626, 36, 38}
		};
	textures.push_back(&tankBody_green);
	tankBody_dark = TextureDef{
			"tanks_2/tankBody_dark",
			GPU_Rect{376, 626, 36, 38}
		};
	textures.push_back(&tankBody_dark);
	tank_red = TextureDef{
			"tanks_2/tank_red",
			GPU_Rect{416, 626, 46, 38}
		};
	textures.push_back(&tank_red);
	tankBody_blue = TextureDef{
			"tanks_2/tankBody_blue",
			GPU_Rect{466, 626, 38, 38}
		};
	textures.push_back(&tankBody_blue);
	tracksSmall = TextureDef{
			"tanks_2/tracksSmall",
			GPU_Rect{508, 626, 52, 37}
		};
	textures.push_back(&tracksSmall);
	treeGreen_small = TextureDef{
			"tanks_2/treeGreen_small",
			GPU_Rect{564, 626, 36, 36}
		};
	textures.push_back(&treeGreen_small);
	treeBrown_small = TextureDef{
			"tanks_2/treeBrown_small",
			GPU_Rect{604, 626, 36, 36}
		};
	textures.push_back(&treeBrown_small);
	tankBody_red = TextureDef{
			"tanks_2/tankBody_red",
			GPU_Rect{644, 626, 36, 34}
		};
	textures.push_back(&tankBody_red);
	sandbagBrown = TextureDef{
			"tanks_2/sandbagBrown",
			GPU_Rect{684, 626, 22, 32}
		};
	textures.push_back(&sandbagBrown);
	sandbagBeige = TextureDef{
			"tanks_2/sandbagBeige",
			GPU_Rect{710, 626, 22, 32}
		};
	textures.push_back(&sandbagBeige);
	crateWood = TextureDef{
			"tanks_2/crateWood",
			GPU_Rect{66, 672, 28, 28}
		};
	textures.push_back(&crateWood);
	crateMetal_side = TextureDef{
			"tanks_2/crateMetal_side",
			GPU_Rect{98, 672, 28, 28}
		};
	textures.push_back(&crateMetal_side);
	crateWood_side = TextureDef{
			"tanks_2/crateWood_side",
			GPU_Rect{130, 672, 28, 28}
		};
	textures.push_back(&crateWood_side);
	crateMetal = TextureDef{
			"tanks_2/crateMetal",
			GPU_Rect{162, 672, 28, 28}
		};
	textures.push_back(&crateMetal);
	barricadeWood = TextureDef{
			"tanks_2/barricadeWood",
			GPU_Rect{194, 672, 28, 28}
		};
	textures.push_back(&barricadeWood);
	barricadeMetal = TextureDef{
			"tanks_2/barricadeMetal",
			GPU_Rect{226, 672, 28, 28}
		};
	textures.push_back(&barricadeMetal);
	treeGreen_twigs = TextureDef{
			"tanks_2/treeGreen_twigs",
			GPU_Rect{288, 672, 22, 26}
		};
	textures.push_back(&treeGreen_twigs);
	treeBrown_twigs = TextureDef{
			"tanks_2/treeBrown_twigs",
			GPU_Rect{314, 672, 22, 26}
		};
	textures.push_back(&treeBrown_twigs);
	barrelBlack_top = TextureDef{
			"tanks_2/barrelBlack_top",
			GPU_Rect{340, 672, 24, 24}
		};
	textures.push_back(&barrelBlack_top);
	barrelGreen_top = TextureDef{
			"tanks_2/barrelGreen_top",
			GPU_Rect{368, 672, 24, 24}
		};
	textures.push_back(&barrelGreen_top);
	barrelRed_top = TextureDef{
			"tanks_2/barrelRed_top",
			GPU_Rect{396, 672, 24, 24}
		};
	textures.push_back(&barrelRed_top);
	barrelRust_top = TextureDef{
			"tanks_2/barrelRust_top",
			GPU_Rect{424, 672, 24, 24}
		};
	textures.push_back(&barrelRust_top);
	shotRed = TextureDef{
			"tanks_2/shotRed",
			GPU_Rect{452, 672, 38, 21}
		};
	textures.push_back(&shotRed);
	barrelGreen_side = TextureDef{
			"tanks_2/barrelGreen_side",
			GPU_Rect{76, 706, 28, 20}
		};
	textures.push_back(&barrelGreen_side);
	barrelRust_side = TextureDef{
			"tanks_2/barrelRust_side",
			GPU_Rect{108, 706, 28, 20}
		};
	textures.push_back(&barrelRust_side);
	shotLarge = TextureDef{
			"tanks_2/shotLarge",
			GPU_Rect{140, 706, 25, 20}
		};
	textures.push_back(&shotLarge);
	barrelBlack_side = TextureDef{
			"tanks_2/barrelBlack_side",
			GPU_Rect{169, 706, 28, 20}
		};
	textures.push_back(&barrelBlack_side);
	barrelRed_side = TextureDef{
			"tanks_2/barrelRed_side",
			GPU_Rect{201, 706, 28, 20}
		};
	textures.push_back(&barrelRed_side);
	specialBarrel1_outline = TextureDef{
			"tanks_2/specialBarrel1_outline",
			GPU_Rect{271, 706, 26, 18}
		};
	textures.push_back(&specialBarrel1_outline);
	wireStraight = TextureDef{
			"tanks_2/wireStraight",
			GPU_Rect{301, 706, 70, 17}
		};
	textures.push_back(&wireStraight);
	tankBlue_barrel1_outline = TextureDef{
			"tanks_2/tankBlue_barrel1_outline",
			GPU_Rect{375, 706, 30, 16}
		};
	textures.push_back(&tankBlue_barrel1_outline);
	specialBarrel2_outline = TextureDef{
			"tanks_2/specialBarrel2_outline",
			GPU_Rect{409, 706, 28, 16}
		};
	textures.push_back(&specialBarrel2_outline);
	shotOrange = TextureDef{
			"tanks_2/shotOrange",
			GPU_Rect{441, 706, 28, 16}
		};
	textures.push_back(&shotOrange);
	specialBarrel5_outline = TextureDef{
			"tanks_2/specialBarrel5_outline",
			GPU_Rect{473, 706, 30, 16}
		};
	textures.push_back(&specialBarrel5_outline);
	tankDark_barrel1_outline = TextureDef{
			"tanks_2/tankDark_barrel1_outline",
			GPU_Rect{507, 706, 30, 16}
		};
	textures.push_back(&tankDark_barrel1_outline);
	tankGreen_barrel1_outline = TextureDef{
			"tanks_2/tankGreen_barrel1_outline",
			GPU_Rect{541, 706, 30, 16}
		};
	textures.push_back(&tankGreen_barrel1_outline);
	tankRed_barrel1_outline = TextureDef{
			"tanks_2/tankRed_barrel1_outline",
			GPU_Rect{575, 706, 30, 16}
		};
	textures.push_back(&tankRed_barrel1_outline);
	tankSand_barrel1_outline = TextureDef{
			"tanks_2/tankSand_barrel1_outline",
			GPU_Rect{609, 706, 30, 16}
		};
	textures.push_back(&tankSand_barrel1_outline);
	specialBarrel4_outline = TextureDef{
			"tanks_2/specialBarrel4_outline",
			GPU_Rect{643, 706, 36, 14}
		};
	textures.push_back(&specialBarrel4_outline);
	specialBarrel1 = TextureDef{
			"tanks_2/specialBarrel1",
			GPU_Rect{718, 706, 22, 14}
		};
	textures.push_back(&specialBarrel1);
	oilSpill_small = TextureDef{
			"tanks_2/oilSpill_small",
			GPU_Rect{744, 706, 14, 14}
		};
	textures.push_back(&oilSpill_small);
	specialBarrel3_outline = TextureDef{
			"tanks_2/specialBarrel3_outline",
			GPU_Rect{0, 730, 32, 14}
		};
	textures.push_back(&specialBarrel3_outline);
	tankGreen_barrel1 = TextureDef{
			"tanks_2/tankGreen_barrel1",
			GPU_Rect{243, 730, 26, 12}
		};
	textures.push_back(&tankGreen_barrel1);
	tankGreen_barrel2_outline = TextureDef{
			"tanks_2/tankGreen_barrel2_outline",
			GPU_Rect{273, 730, 30, 12}
		};
	textures.push_back(&tankGreen_barrel2_outline);
	tankRed_barrel3_outline = TextureDef{
			"tanks_2/tankRed_barrel3_outline",
			GPU_Rect{307, 730, 30, 12}
		};
	textures.push_back(&tankRed_barrel3_outline);
	tankDark_barrel2_outline = TextureDef{
			"tanks_2/tankDark_barrel2_outline",
			GPU_Rect{357, 730, 30, 12}
		};
	textures.push_back(&tankDark_barrel2_outline);
	tankDark_barrel3_outline = TextureDef{
			"tanks_2/tankDark_barrel3_outline",
			GPU_Rect{391, 730, 30, 12}
		};
	textures.push_back(&tankDark_barrel3_outline);
	tankRed_barrel2_outline = TextureDef{
			"tanks_2/tankRed_barrel2_outline",
			GPU_Rect{425, 730, 30, 12}
		};
	textures.push_back(&tankRed_barrel2_outline);
	tankDark_barrel1 = TextureDef{
			"tanks_2/tankDark_barrel1",
			GPU_Rect{459, 730, 26, 12}
		};
	textures.push_back(&tankDark_barrel1);
	tankBlue_barrel3_outline = TextureDef{
			"tanks_2/tankBlue_barrel3_outline",
			GPU_Rect{489, 730, 30, 12}
		};
	textures.push_back(&tankBlue_barrel3_outline);
	tankSand_barrel3_outline = TextureDef{
			"tanks_2/tankSand_barrel3_outline",
			GPU_Rect{523, 730, 30, 12}
		};
	textures.push_back(&tankSand_barrel3_outline);
	tankBlue_barrel2_outline = TextureDef{
			"tanks_2/tankBlue_barrel2_outline",
			GPU_Rect{557, 730, 30, 12}
		};
	textures.push_back(&tankBlue_barrel2_outline);
	specialBarrel7_outline = TextureDef{
			"tanks_2/specialBarrel7_outline",
			GPU_Rect{591, 730, 30, 12}
		};
	textures.push_back(&specialBarrel7_outline);
	tankBlue_barrel1 = TextureDef{
			"tanks_2/tankBlue_barrel1",
			GPU_Rect{655, 730, 26, 12}
		};
	textures.push_back(&tankBlue_barrel1);
	specialBarrel6_outline = TextureDef{
			"tanks_2/specialBarrel6_outline",
			GPU_Rect{715, 730, 30, 12}
		};
	textures.push_back(&specialBarrel6_outline);
	tankSand_barrel2_outline = TextureDef{
			"tanks_2/tankSand_barrel2_outline",
			GPU_Rect{60, 748, 30, 12}
		};
	textures.push_back(&tankSand_barrel2_outline);
	specialBarrel5 = TextureDef{
			"tanks_2/specialBarrel5",
			GPU_Rect{124, 748, 26, 12}
		};
	textures.push_back(&specialBarrel5);
	tankRed_barrel1 = TextureDef{
			"tanks_2/tankRed_barrel1",
			GPU_Rect{214, 748, 26, 12}
		};
	textures.push_back(&tankRed_barrel1);
	bulletSand2_outline = TextureDef{
			"tanks_2/bulletSand2_outline",
			GPU_Rect{244, 748, 16, 12}
		};
	textures.push_back(&bulletSand2_outline);
	bulletGreen2_outline = TextureDef{
			"tanks_2/bulletGreen2_outline",
			GPU_Rect{294, 748, 16, 12}
		};
	textures.push_back(&bulletGreen2_outline);
	tankSand_barrel1 = TextureDef{
			"tanks_2/tankSand_barrel1",
			GPU_Rect{404, 748, 26, 12}
		};
	textures.push_back(&tankSand_barrel1);
	tankGreen_barrel3_outline = TextureDef{
			"tanks_2/tankGreen_barrel3_outline",
			GPU_Rect{434, 748, 30, 12}
		};
	textures.push_back(&tankGreen_barrel3_outline);
	specialBarrel2 = TextureDef{
			"tanks_2/specialBarrel2",
			GPU_Rect{498, 748, 24, 12}
		};
	textures.push_back(&specialBarrel2);
	bulletDark2_outline = TextureDef{
			"tanks_2/bulletDark2_outline",
			GPU_Rect{526, 748, 16, 12}
		};
	textures.push_back(&bulletDark2_outline);
	bulletRed2_outline = TextureDef{
			"tanks_2/bulletRed2_outline",
			GPU_Rect{546, 748, 16, 12}
		};
	textures.push_back(&bulletRed2_outline);
	bulletBlue2_outline = TextureDef{
			"tanks_2/bulletBlue2_outline",
			GPU_Rect{566, 748, 16, 12}
		};
	textures.push_back(&bulletBlue2_outline);
	specialBarrel3 = TextureDef{
			"tanks_2/specialBarrel3",
			GPU_Rect{586, 748, 28, 10}
		};
	textures.push_back(&specialBarrel3);
	specialBarrel4 = TextureDef{
			"tanks_2/specialBarrel4",
			GPU_Rect{618, 748, 32, 10}
		};
	textures.push_back(&specialBarrel4);
	specialBarrel7 = TextureDef{
			"tanks_2/specialBarrel7",
			GPU_Rect{733, 610, 26, 8}
		};
	textures.push_back(&specialBarrel7);
	treeGreen_leaf = TextureDef{
			"tanks_2/treeGreen_leaf",
			GPU_Rect{749, 730, 10, 8}
		};
	textures.push_back(&treeGreen_leaf);
	tankDark_barrel3 = TextureDef{
			"tanks_2/tankDark_barrel3",
			GPU_Rect{249, 370, 26, 8}
		};
	textures.push_back(&tankDark_barrel3);
	tankGreen_barrel3 = TextureDef{
			"tanks_2/tankGreen_barrel3",
			GPU_Rect{279, 370, 26, 8}
		};
	textures.push_back(&tankGreen_barrel3);
	tankRed_barrel2 = TextureDef{
			"tanks_2/tankRed_barrel2",
			GPU_Rect{309, 370, 26, 8}
		};
	textures.push_back(&tankRed_barrel2);
	tankDark_barrel2 = TextureDef{
			"tanks_2/tankDark_barrel2",
			GPU_Rect{339, 370, 26, 8}
		};
	textures.push_back(&tankDark_barrel2);
	shotThin = TextureDef{
			"tanks_2/shotThin",
			GPU_Rect{369, 370, 26, 8}
		};
	textures.push_back(&shotThin);
	tankGreen_barrel2 = TextureDef{
			"tanks_2/tankGreen_barrel2",
			GPU_Rect{399, 370, 26, 8}
		};
	textures.push_back(&tankGreen_barrel2);
	tankSand_barrel3 = TextureDef{
			"tanks_2/tankSand_barrel3",
			GPU_Rect{429, 370, 26, 8}
		};
	textures.push_back(&tankSand_barrel3);
	tankSand_barrel2 = TextureDef{
			"tanks_2/tankSand_barrel2",
			GPU_Rect{459, 370, 26, 8}
		};
	textures.push_back(&tankSand_barrel2);
	bulletBlue1_outline = TextureDef{
			"tanks_2/bulletBlue1_outline",
			GPU_Rect{489, 370, 14, 8}
		};
	textures.push_back(&bulletBlue1_outline);
	bulletSand3_outline = TextureDef{
			"tanks_2/bulletSand3_outline",
			GPU_Rect{507, 370, 18, 8}
		};
	textures.push_back(&bulletSand3_outline);
	tankBlue_barrel3 = TextureDef{
			"tanks_2/tankBlue_barrel3",
			GPU_Rect{529, 370, 26, 8}
		};
	textures.push_back(&tankBlue_barrel3);
	bulletSand1_outline = TextureDef{
			"tanks_2/bulletSand1_outline",
			GPU_Rect{559, 370, 14, 8}
		};
	textures.push_back(&bulletSand1_outline);
	bulletRed3_outline = TextureDef{
			"tanks_2/bulletRed3_outline",
			GPU_Rect{577, 370, 18, 8}
		};
	textures.push_back(&bulletRed3_outline);
	bulletBlue3_outline = TextureDef{
			"tanks_2/bulletBlue3_outline",
			GPU_Rect{599, 370, 18, 8}
		};
	textures.push_back(&bulletBlue3_outline);
	bulletSand2 = TextureDef{
			"tanks_2/bulletSand2",
			GPU_Rect{621, 370, 12, 8}
		};
	textures.push_back(&bulletSand2);
	bulletDark1_outline = TextureDef{
			"tanks_2/bulletDark1_outline",
			GPU_Rect{637, 370, 14, 8}
		};
	textures.push_back(&bulletDark1_outline);
	bulletRed2 = TextureDef{
			"tanks_2/bulletRed2",
			GPU_Rect{655, 370, 12, 8}
		};
	textures.push_back(&bulletRed2);
	treeBrown_leaf = TextureDef{
			"tanks_2/treeBrown_leaf",
			GPU_Rect{671, 370, 10, 8}
		};
	textures.push_back(&treeBrown_leaf);
	bulletRed1_outline = TextureDef{
			"tanks_2/bulletRed1_outline",
			GPU_Rect{685, 370, 14, 8}
		};
	textures.push_back(&bulletRed1_outline);
	bulletGreen3_outline = TextureDef{
			"tanks_2/bulletGreen3_outline",
			GPU_Rect{703, 370, 18, 8}
		};
	textures.push_back(&bulletGreen3_outline);
	specialBarrel6 = TextureDef{
			"tanks_2/specialBarrel6",
			GPU_Rect{725, 370, 26, 8}
		};
	textures.push_back(&specialBarrel6);
	tankBlue_barrel2 = TextureDef{
			"tanks_2/tankBlue_barrel2",
			GPU_Rect{0, 764, 26, 8}
		};
	textures.push_back(&tankBlue_barrel2);
	bulletGreen1_outline = TextureDef{
			"tanks_2/bulletGreen1_outline",
			GPU_Rect{30, 764, 14, 8}
		};
	textures.push_back(&bulletGreen1_outline);
	bulletGreen2 = TextureDef{
			"tanks_2/bulletGreen2",
			GPU_Rect{48, 764, 12, 8}
		};
	textures.push_back(&bulletGreen2);
	bulletDark2 = TextureDef{
			"tanks_2/bulletDark2",
			GPU_Rect{64, 764, 12, 8}
		};
	textures.push_back(&bulletDark2);
	bulletDark3_outline = TextureDef{
			"tanks_2/bulletDark3_outline",
			GPU_Rect{80, 764, 18, 8}
		};
	textures.push_back(&bulletDark3_outline);
	bulletBlue2 = TextureDef{
			"tanks_2/bulletBlue2",
			GPU_Rect{102, 764, 12, 8}
		};
	textures.push_back(&bulletBlue2);
	tankRed_barrel3 = TextureDef{
			"tanks_2/tankRed_barrel3",
			GPU_Rect{118, 764, 26, 8}
		};
	textures.push_back(&tankRed_barrel3);
	bulletDark3 = TextureDef{
			"tanks_2/bulletDark3",
			GPU_Rect{148, 764, 14, 4}
		};
	textures.push_back(&bulletDark3);
	bulletGreen3 = TextureDef{
			"tanks_2/bulletGreen3",
			GPU_Rect{166, 764, 14, 4}
		};
	textures.push_back(&bulletGreen3);
	bulletBlue3 = TextureDef{
			"tanks_2/bulletBlue3",
			GPU_Rect{184, 764, 14, 4}
		};
	textures.push_back(&bulletBlue3);
	bulletDark1 = TextureDef{
			"tanks_2/bulletDark1",
			GPU_Rect{202, 764, 10, 4}
		};
	textures.push_back(&bulletDark1);
	bulletGreen1 = TextureDef{
			"tanks_2/bulletGreen1",
			GPU_Rect{216, 764, 10, 4}
		};
	textures.push_back(&bulletGreen1);
	bulletSand1 = TextureDef{
			"tanks_2/bulletSand1",
			GPU_Rect{230, 764, 10, 4}
		};
	textures.push_back(&bulletSand1);
	bulletRed3 = TextureDef{
			"tanks_2/bulletRed3",
			GPU_Rect{244, 764, 14, 4}
		};
	textures.push_back(&bulletRed3);
	bulletBlue1 = TextureDef{
			"tanks_2/bulletBlue1",
			GPU_Rect{262, 764, 10, 4}
		};
	textures.push_back(&bulletBlue1);
	bulletSand3 = TextureDef{
			"tanks_2/bulletSand3",
			GPU_Rect{276, 764, 14, 4}
		};
	textures.push_back(&bulletSand3);
	bulletRed1 = TextureDef{
			"tanks_2/bulletRed1",
			GPU_Rect{294, 764, 10, 4}
		};
	textures.push_back(&bulletRed1);
	
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
