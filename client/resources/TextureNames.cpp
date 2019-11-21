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


r_xhrer::r_xhrer()
{
	
	bulletBeige = TextureDef{
			"tanks_1/Bullets/bulletBeige",
			GPU_Rect{234, 912, 28, 14}
		};
	textures.push_back(&bulletBeige);
	bulletBeigeSilver = TextureDef{
			"tanks_1/Bullets/bulletBeigeSilver",
			GPU_Rect{266, 912, 28, 14}
		};
	textures.push_back(&bulletBeigeSilver);
	bulletBeigeSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletBeigeSilver_outline",
			GPU_Rect{522, 554, 36, 22}
		};
	textures.push_back(&bulletBeigeSilver_outline);
	bulletBeige_outline = TextureDef{
			"tanks_1/Bullets/bulletBeige_outline",
			GPU_Rect{874, 0, 36, 22}
		};
	textures.push_back(&bulletBeige_outline);
	bulletBlue = TextureDef{
			"tanks_1/Bullets/bulletBlue",
			GPU_Rect{362, 912, 28, 14}
		};
	textures.push_back(&bulletBlue);
	bulletBlueSilver = TextureDef{
			"tanks_1/Bullets/bulletBlueSilver",
			GPU_Rect{394, 912, 28, 14}
		};
	textures.push_back(&bulletBlueSilver);
	bulletBlueSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletBlueSilver_outline",
			GPU_Rect{874, 238, 36, 22}
		};
	textures.push_back(&bulletBlueSilver_outline);
	bulletBlue_outline = TextureDef{
			"tanks_1/Bullets/bulletBlue_outline",
			GPU_Rect{874, 394, 36, 22}
		};
	textures.push_back(&bulletBlue_outline);
	bulletGreen = TextureDef{
			"tanks_1/Bullets/bulletGreen",
			GPU_Rect{490, 912, 28, 14}
		};
	textures.push_back(&bulletGreen);
	bulletGreenSilver = TextureDef{
			"tanks_1/Bullets/bulletGreenSilver",
			GPU_Rect{522, 912, 28, 14}
		};
	textures.push_back(&bulletGreenSilver);
	bulletGreenSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletGreenSilver_outline",
			GPU_Rect{874, 446, 36, 22}
		};
	textures.push_back(&bulletGreenSilver_outline);
	bulletGreen_outline = TextureDef{
			"tanks_1/Bullets/bulletGreen_outline",
			GPU_Rect{874, 420, 36, 22}
		};
	textures.push_back(&bulletGreen_outline);
	bulletRed = TextureDef{
			"tanks_1/Bullets/bulletRed",
			GPU_Rect{586, 912, 28, 14}
		};
	textures.push_back(&bulletRed);
	bulletRedSilver = TextureDef{
			"tanks_1/Bullets/bulletRedSilver",
			GPU_Rect{632, 844, 28, 14}
		};
	textures.push_back(&bulletRedSilver);
	bulletRedSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletRedSilver_outline",
			GPU_Rect{370, 486, 36, 22}
		};
	textures.push_back(&bulletRedSilver_outline);
	bulletRed_outline = TextureDef{
			"tanks_1/Bullets/bulletRed_outline",
			GPU_Rect{874, 368, 36, 22}
		};
	textures.push_back(&bulletRed_outline);
	bulletSilver = TextureDef{
			"tanks_1/Bullets/bulletSilver",
			GPU_Rect{458, 912, 28, 14}
		};
	textures.push_back(&bulletSilver);
	bulletSilverSilver = TextureDef{
			"tanks_1/Bullets/bulletSilverSilver",
			GPU_Rect{298, 912, 28, 14}
		};
	textures.push_back(&bulletSilverSilver);
	bulletSilverSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletSilverSilver_outline",
			GPU_Rect{874, 342, 36, 22}
		};
	textures.push_back(&bulletSilverSilver_outline);
	bulletSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletSilver_outline",
			GPU_Rect{874, 316, 36, 22}
		};
	textures.push_back(&bulletSilver_outline);
	bulletYellow = TextureDef{
			"tanks_1/Bullets/bulletYellow",
			GPU_Rect{202, 912, 28, 14}
		};
	textures.push_back(&bulletYellow);
	bulletYellowSilver = TextureDef{
			"tanks_1/Bullets/bulletYellowSilver",
			GPU_Rect{700, 844, 28, 14}
		};
	textures.push_back(&bulletYellowSilver);
	bulletYellowSilver_outline = TextureDef{
			"tanks_1/Bullets/bulletYellowSilver_outline",
			GPU_Rect{874, 290, 36, 22}
		};
	textures.push_back(&bulletYellowSilver_outline);
	bulletYellow_outline = TextureDef{
			"tanks_1/Bullets/bulletYellow_outline",
			GPU_Rect{874, 264, 36, 22}
		};
	textures.push_back(&bulletYellow_outline);
	
};

r_jr3fj::r_jr3fj()
{
	
	barrelGreen_side = TextureDef{
			"tanks_1/Obstacles/barrelGreen_side",
			GPU_Rect{596, 794, 64, 46}
		};
	textures.push_back(&barrelGreen_side);
	barrelGreen_side_damaged = TextureDef{
			"tanks_1/Obstacles/barrelGreen_side_damaged",
			GPU_Rect{528, 794, 64, 46}
		};
	textures.push_back(&barrelGreen_side_damaged);
	barrelGreen_up = TextureDef{
			"tanks_1/Obstacles/barrelGreen_up",
			GPU_Rect{806, 446, 50, 50}
		};
	textures.push_back(&barrelGreen_up);
	barrelGrey_sde_rust = TextureDef{
			"tanks_1/Obstacles/barrelGrey_sde_rust",
			GPU_Rect{806, 0, 64, 46}
		};
	textures.push_back(&barrelGrey_sde_rust);
	barrelGrey_side = TextureDef{
			"tanks_1/Obstacles/barrelGrey_side",
			GPU_Rect{732, 794, 64, 46}
		};
	textures.push_back(&barrelGrey_side);
	barrelGrey_up = TextureDef{
			"tanks_1/Obstacles/barrelGrey_up",
			GPU_Rect{806, 554, 50, 50}
		};
	textures.push_back(&barrelGrey_up);
	barrelRed_side = TextureDef{
			"tanks_1/Obstacles/barrelRed_side",
			GPU_Rect{664, 794, 64, 46}
		};
	textures.push_back(&barrelRed_side);
	barrelRed_up = TextureDef{
			"tanks_1/Obstacles/barrelRed_up",
			GPU_Rect{806, 500, 50, 50}
		};
	textures.push_back(&barrelRed_up);
	oil = TextureDef{
			"tanks_1/Obstacles/oil",
			GPU_Rect{112, 106, 98, 98}
		};
	textures.push_back(&oil);
	sandbagBeige = TextureDef{
			"tanks_1/Obstacles/sandbagBeige",
			GPU_Rect{370, 414, 46, 68}
		};
	textures.push_back(&sandbagBeige);
	sandbagBrown = TextureDef{
			"tanks_1/Obstacles/sandbagBrown",
			GPU_Rect{0, 512, 46, 68}
		};
	textures.push_back(&sandbagBrown);
	
};

r_ir8mj::r_ir8mj()
{
	
	smokeGrey0 = TextureDef{
			"tanks_1/Smoke/smokeGrey0",
			GPU_Rect{430, 0, 88, 88}
		};
	textures.push_back(&smokeGrey0);
	smokeGrey1 = TextureDef{
			"tanks_1/Smoke/smokeGrey1",
			GPU_Rect{0, 414, 90, 94}
		};
	textures.push_back(&smokeGrey1);
	smokeGrey3 = TextureDef{
			"tanks_1/Smoke/smokeGrey3",
			GPU_Rect{430, 92, 80, 80}
		};
	textures.push_back(&smokeGrey3);
	smokeGrey2 = TextureDef{
			"tanks_1/Smoke/smokeGrey2",
			GPU_Rect{326, 0, 100, 92}
		};
	textures.push_back(&smokeGrey2);
	smokeGrey4 = TextureDef{
			"tanks_1/Smoke/smokeGrey4",
			GPU_Rect{0, 210, 98, 102}
		};
	textures.push_back(&smokeGrey4);
	smokeGrey5 = TextureDef{
			"tanks_1/Smoke/smokeGrey5",
			GPU_Rect{0, 0, 108, 100}
		};
	textures.push_back(&smokeGrey5);
	smokeOrange0 = TextureDef{
			"tanks_1/Smoke/smokeOrange0",
			GPU_Rect{278, 414, 88, 88}
		};
	textures.push_back(&smokeOrange0);
	smokeOrange1 = TextureDef{
			"tanks_1/Smoke/smokeOrange1",
			GPU_Rect{326, 290, 90, 94}
		};
	textures.push_back(&smokeOrange1);
	smokeOrange2 = TextureDef{
			"tanks_1/Smoke/smokeOrange2",
			GPU_Rect{198, 316, 100, 92}
		};
	textures.push_back(&smokeOrange2);
	smokeOrange3 = TextureDef{
			"tanks_1/Smoke/smokeOrange3",
			GPU_Rect{430, 176, 80, 80}
		};
	textures.push_back(&smokeOrange3);
	smokeOrange4 = TextureDef{
			"tanks_1/Smoke/smokeOrange4",
			GPU_Rect{214, 104, 98, 102}
		};
	textures.push_back(&smokeOrange4);
	smokeOrange5 = TextureDef{
			"tanks_1/Smoke/smokeOrange5",
			GPU_Rect{102, 210, 108, 100}
		};
	textures.push_back(&smokeOrange5);
	smokeWhite0 = TextureDef{
			"tanks_1/Smoke/smokeWhite0",
			GPU_Rect{326, 192, 90, 94}
		};
	textures.push_back(&smokeWhite0);
	smokeWhite1 = TextureDef{
			"tanks_1/Smoke/smokeWhite1",
			GPU_Rect{326, 96, 100, 92}
		};
	textures.push_back(&smokeWhite1);
	smokeWhite2 = TextureDef{
			"tanks_1/Smoke/smokeWhite2",
			GPU_Rect{430, 344, 80, 80}
		};
	textures.push_back(&smokeWhite2);
	smokeWhite3 = TextureDef{
			"tanks_1/Smoke/smokeWhite3",
			GPU_Rect{214, 210, 98, 102}
		};
	textures.push_back(&smokeWhite3);
	smokeWhite4 = TextureDef{
			"tanks_1/Smoke/smokeWhite4",
			GPU_Rect{214, 0, 108, 100}
		};
	textures.push_back(&smokeWhite4);
	smokeWhite5 = TextureDef{
			"tanks_1/Smoke/smokeWhite5",
			GPU_Rect{186, 414, 88, 88}
		};
	textures.push_back(&smokeWhite5);
	smokeYellow0 = TextureDef{
			"tanks_1/Smoke/smokeYellow0",
			GPU_Rect{94, 414, 88, 88}
		};
	textures.push_back(&smokeYellow0);
	smokeYellow1 = TextureDef{
			"tanks_1/Smoke/smokeYellow1",
			GPU_Rect{0, 316, 90, 94}
		};
	textures.push_back(&smokeYellow1);
	smokeYellow2 = TextureDef{
			"tanks_1/Smoke/smokeYellow2",
			GPU_Rect{94, 316, 100, 92}
		};
	textures.push_back(&smokeYellow2);
	smokeYellow3 = TextureDef{
			"tanks_1/Smoke/smokeYellow3",
			GPU_Rect{430, 260, 80, 80}
		};
	textures.push_back(&smokeYellow3);
	smokeYellow4 = TextureDef{
			"tanks_1/Smoke/smokeYellow4",
			GPU_Rect{112, 0, 98, 102}
		};
	textures.push_back(&smokeYellow4);
	smokeYellow5 = TextureDef{
			"tanks_1/Smoke/smokeYellow5",
			GPU_Rect{0, 106, 108, 100}
		};
	textures.push_back(&smokeYellow5);
	
};

r_alp0p::r_alp0p()
{
	
	barrelBlack_side = TextureDef{
			"tanks_2/barrelBlack_side",
			GPU_Rect{874, 652, 30, 22}
		};
	textures.push_back(&barrelBlack_side);
	barrelBlack_top = TextureDef{
			"tanks_2/barrelBlack_top",
			GPU_Rect{874, 532, 26, 26}
		};
	textures.push_back(&barrelBlack_top);
	barrelGreen_side = TextureDef{
			"tanks_2/barrelGreen_side",
			GPU_Rect{874, 704, 30, 22}
		};
	textures.push_back(&barrelGreen_side);
	barrelGreen_top = TextureDef{
			"tanks_2/barrelGreen_top",
			GPU_Rect{562, 554, 26, 26}
		};
	textures.push_back(&barrelGreen_top);
	barrelRed_side = TextureDef{
			"tanks_2/barrelRed_side",
			GPU_Rect{874, 678, 30, 22}
		};
	textures.push_back(&barrelRed_side);
	barrelRed_top = TextureDef{
			"tanks_2/barrelRed_top",
			GPU_Rect{874, 502, 26, 26}
		};
	textures.push_back(&barrelRed_top);
	barrelRust_side = TextureDef{
			"tanks_2/barrelRust_side",
			GPU_Rect{874, 626, 30, 22}
		};
	textures.push_back(&barrelRust_side);
	barrelRust_top = TextureDef{
			"tanks_2/barrelRust_top",
			GPU_Rect{874, 472, 26, 26}
		};
	textures.push_back(&barrelRust_top);
	barricadeMetal = TextureDef{
			"tanks_2/barricadeMetal",
			GPU_Rect{874, 60, 30, 30}
		};
	textures.push_back(&barricadeMetal);
	barricadeWood = TextureDef{
			"tanks_2/barricadeWood",
			GPU_Rect{836, 864, 30, 30}
		};
	textures.push_back(&barricadeWood);
	bulletBlue1 = TextureDef{
			"tanks_2/bulletBlue1",
			GPU_Rect{416, 852, 12, 6}
		};
	textures.push_back(&bulletBlue1);
	bulletBlue1_outline = TextureDef{
			"tanks_2/bulletBlue1_outline",
			GPU_Rect{914, 138, 16, 10}
		};
	textures.push_back(&bulletBlue1_outline);
	bulletBlue2 = TextureDef{
			"tanks_2/bulletBlue2",
			GPU_Rect{856, 806, 14, 10}
		};
	textures.push_back(&bulletBlue2);
	bulletBlue3 = TextureDef{
			"tanks_2/bulletBlue3",
			GPU_Rect{410, 500, 16, 6}
		};
	textures.push_back(&bulletBlue3);
	bulletBlue3_outline = TextureDef{
			"tanks_2/bulletBlue3_outline",
			GPU_Rect{914, 110, 20, 10}
		};
	textures.push_back(&bulletBlue3_outline);
	bulletDark1 = TextureDef{
			"tanks_2/bulletDark1",
			GPU_Rect{432, 852, 12, 6}
		};
	textures.push_back(&bulletDark1);
	bulletBlue2_outline = TextureDef{
			"tanks_2/bulletBlue2_outline",
			GPU_Rect{302, 394, 18, 14}
		};
	textures.push_back(&bulletBlue2_outline);
	bulletDark1_outline = TextureDef{
			"tanks_2/bulletDark1_outline",
			GPU_Rect{914, 152, 16, 10}
		};
	textures.push_back(&bulletDark1_outline);
	bulletDark2 = TextureDef{
			"tanks_2/bulletDark2",
			GPU_Rect{856, 750, 14, 10}
		};
	textures.push_back(&bulletDark2);
	bulletDark2_outline = TextureDef{
			"tanks_2/bulletDark2_outline",
			GPU_Rect{914, 50, 18, 14}
		};
	textures.push_back(&bulletDark2_outline);
	bulletDark3 = TextureDef{
			"tanks_2/bulletDark3",
			GPU_Rect{462, 852, 16, 6}
		};
	textures.push_back(&bulletDark3);
	bulletDark3_outline = TextureDef{
			"tanks_2/bulletDark3_outline",
			GPU_Rect{914, 82, 20, 10}
		};
	textures.push_back(&bulletDark3_outline);
	bulletGreen1 = TextureDef{
			"tanks_2/bulletGreen1",
			GPU_Rect{856, 848, 12, 6}
		};
	textures.push_back(&bulletGreen1);
	bulletGreen1_outline = TextureDef{
			"tanks_2/bulletGreen1_outline",
			GPU_Rect{410, 486, 16, 10}
		};
	textures.push_back(&bulletGreen1_outline);
	bulletGreen2 = TextureDef{
			"tanks_2/bulletGreen2",
			GPU_Rect{856, 764, 14, 10}
		};
	textures.push_back(&bulletGreen2);
	bulletGreen2_outline = TextureDef{
			"tanks_2/bulletGreen2_outline",
			GPU_Rect{914, 14, 18, 14}
		};
	textures.push_back(&bulletGreen2_outline);
	bulletGreen3 = TextureDef{
			"tanks_2/bulletGreen3",
			GPU_Rect{482, 852, 16, 6}
		};
	textures.push_back(&bulletGreen3);
	bulletGreen3_outline = TextureDef{
			"tanks_2/bulletGreen3_outline",
			GPU_Rect{914, 96, 20, 10}
		};
	textures.push_back(&bulletGreen3_outline);
	bulletRed1 = TextureDef{
			"tanks_2/bulletRed1",
			GPU_Rect{806, 854, 12, 6}
		};
	textures.push_back(&bulletRed1);
	bulletRed1_outline = TextureDef{
			"tanks_2/bulletRed1_outline",
			GPU_Rect{914, 166, 16, 10}
		};
	textures.push_back(&bulletRed1_outline);
	bulletRed2 = TextureDef{
			"tanks_2/bulletRed2",
			GPU_Rect{856, 778, 14, 10}
		};
	textures.push_back(&bulletRed2);
	bulletRed2_outline = TextureDef{
			"tanks_2/bulletRed2_outline",
			GPU_Rect{638, 630, 18, 14}
		};
	textures.push_back(&bulletRed2_outline);
	bulletRed3 = TextureDef{
			"tanks_2/bulletRed3",
			GPU_Rect{502, 852, 16, 6}
		};
	textures.push_back(&bulletRed3);
	bulletRed3_outline = TextureDef{
			"tanks_2/bulletRed3_outline",
			GPU_Rect{914, 124, 20, 10}
		};
	textures.push_back(&bulletRed3_outline);
	bulletSand1 = TextureDef{
			"tanks_2/bulletSand1",
			GPU_Rect{822, 854, 12, 6}
		};
	textures.push_back(&bulletSand1);
	bulletSand1_outline = TextureDef{
			"tanks_2/bulletSand1_outline",
			GPU_Rect{502, 496, 16, 10}
		};
	textures.push_back(&bulletSand1_outline);
	bulletSand2 = TextureDef{
			"tanks_2/bulletSand2",
			GPU_Rect{856, 792, 14, 10}
		};
	textures.push_back(&bulletSand2);
	bulletSand2_outline = TextureDef{
			"tanks_2/bulletSand2_outline",
			GPU_Rect{914, 32, 18, 14}
		};
	textures.push_back(&bulletSand2_outline);
	bulletSand3 = TextureDef{
			"tanks_2/bulletSand3",
			GPU_Rect{396, 852, 16, 6}
		};
	textures.push_back(&bulletSand3);
	bulletSand3_outline = TextureDef{
			"tanks_2/bulletSand3_outline",
			GPU_Rect{914, 68, 20, 10}
		};
	textures.push_back(&bulletSand3_outline);
	crateMetal = TextureDef{
			"tanks_2/crateMetal",
			GPU_Rect{874, 128, 30, 30}
		};
	textures.push_back(&crateMetal);
	crateMetal_side = TextureDef{
			"tanks_2/crateMetal_side",
			GPU_Rect{802, 864, 30, 30}
		};
	textures.push_back(&crateMetal_side);
	crateWood = TextureDef{
			"tanks_2/crateWood",
			GPU_Rect{874, 26, 30, 30}
		};
	textures.push_back(&crateWood);
	crateWood_side = TextureDef{
			"tanks_2/crateWood_side",
			GPU_Rect{874, 94, 30, 30}
		};
	textures.push_back(&crateWood_side);
	explosion1 = TextureDef{
			"tanks_2/explosion1",
			GPU_Rect{140, 794, 62, 62}
		};
	textures.push_back(&explosion1);
	explosion2 = TextureDef{
			"tanks_2/explosion2",
			GPU_Rect{272, 794, 58, 58}
		};
	textures.push_back(&explosion2);
	explosion3 = TextureDef{
			"tanks_2/explosion3",
			GPU_Rect{420, 584, 66, 66}
		};
	textures.push_back(&explosion3);
	explosion4 = TextureDef{
			"tanks_2/explosion4",
			GPU_Rect{806, 750, 46, 48}
		};
	textures.push_back(&explosion4);
	explosion5 = TextureDef{
			"tanks_2/explosion5",
			GPU_Rect{806, 166, 54, 54}
		};
	textures.push_back(&explosion5);
	explosionSmoke1 = TextureDef{
			"tanks_2/explosionSmoke1",
			GPU_Rect{206, 794, 62, 62}
		};
	textures.push_back(&explosionSmoke1);
	explosionSmoke2 = TextureDef{
			"tanks_2/explosionSmoke2",
			GPU_Rect{334, 794, 58, 58}
		};
	textures.push_back(&explosionSmoke2);
	explosionSmoke3 = TextureDef{
			"tanks_2/explosionSmoke3",
			GPU_Rect{596, 280, 66, 66}
		};
	textures.push_back(&explosionSmoke3);
	explosionSmoke4 = TextureDef{
			"tanks_2/explosionSmoke4",
			GPU_Rect{806, 802, 46, 48}
		};
	textures.push_back(&explosionSmoke4);
	explosionSmoke5 = TextureDef{
			"tanks_2/explosionSmoke5",
			GPU_Rect{806, 108, 54, 54}
		};
	textures.push_back(&explosionSmoke5);
	fenceRed = TextureDef{
			"tanks_2/fenceRed",
			GPU_Rect{700, 724, 18, 50}
		};
	textures.push_back(&fenceRed);
	fenceYellow = TextureDef{
			"tanks_2/fenceYellow",
			GPU_Rect{302, 316, 18, 54}
		};
	textures.push_back(&fenceYellow);
	oilSpill_large = TextureDef{
			"tanks_2/oilSpill_large",
			GPU_Rect{806, 336, 52, 52}
		};
	textures.push_back(&oilSpill_large);
	oilSpill_small = TextureDef{
			"tanks_2/oilSpill_small",
			GPU_Rect{302, 374, 16, 16}
		};
	textures.push_back(&oilSpill_small);
	sandbagBeige = TextureDef{
			"tanks_2/sandbagBeige",
			GPU_Rect{874, 200, 24, 34}
		};
	textures.push_back(&sandbagBeige);
	sandbagBeige_open = TextureDef{
			"tanks_2/sandbagBeige_open",
			GPU_Rect{560, 584, 30, 44}
		};
	textures.push_back(&sandbagBeige_open);
	sandbagBrown = TextureDef{
			"tanks_2/sandbagBrown",
			GPU_Rect{874, 162, 24, 34}
		};
	textures.push_back(&sandbagBrown);
	sandbagBrown_open = TextureDef{
			"tanks_2/sandbagBrown_open",
			GPU_Rect{630, 654, 30, 44}
		};
	textures.push_back(&sandbagBrown_open);
	shotLarge = TextureDef{
			"tanks_2/shotLarge",
			GPU_Rect{874, 774, 26, 22}
		};
	textures.push_back(&shotLarge);
	shotOrange = TextureDef{
			"tanks_2/shotOrange",
			GPU_Rect{874, 800, 30, 18}
		};
	textures.push_back(&shotOrange);
	shotRed = TextureDef{
			"tanks_2/shotRed",
			GPU_Rect{478, 556, 40, 24}
		};
	textures.push_back(&shotRed);
	shotThin = TextureDef{
			"tanks_2/shotThin",
			GPU_Rect{842, 912, 28, 10}
		};
	textures.push_back(&shotThin);
	specialBarrel1 = TextureDef{
			"tanks_2/specialBarrel1",
			GPU_Rect{402, 388, 24, 16}
		};
	textures.push_back(&specialBarrel1);
	specialBarrel1_outline = TextureDef{
			"tanks_2/specialBarrel1_outline",
			GPU_Rect{702, 700, 28, 20}
		};
	textures.push_back(&specialBarrel1_outline);
	specialBarrel2 = TextureDef{
			"tanks_2/specialBarrel2",
			GPU_Rect{0, 912, 26, 14}
		};
	textures.push_back(&specialBarrel2);
	specialBarrel2_outline = TextureDef{
			"tanks_2/specialBarrel2_outline",
			GPU_Rect{772, 770, 30, 18}
		};
	textures.push_back(&specialBarrel2_outline);
	specialBarrel3 = TextureDef{
			"tanks_2/specialBarrel3",
			GPU_Rect{468, 496, 30, 12}
		};
	textures.push_back(&specialBarrel3);
	specialBarrel3_outline = TextureDef{
			"tanks_2/specialBarrel3_outline",
			GPU_Rect{732, 844, 34, 16}
		};
	textures.push_back(&specialBarrel3_outline);
	specialBarrel4 = TextureDef{
			"tanks_2/specialBarrel4",
			GPU_Rect{430, 496, 34, 12}
		};
	textures.push_back(&specialBarrel4);
	specialBarrel4_outline = TextureDef{
			"tanks_2/specialBarrel4_outline",
			GPU_Rect{596, 630, 38, 16}
		};
	textures.push_back(&specialBarrel4_outline);
	specialBarrel5 = TextureDef{
			"tanks_2/specialBarrel5",
			GPU_Rect{138, 912, 28, 14}
		};
	textures.push_back(&specialBarrel5);
	specialBarrel5_outline = TextureDef{
			"tanks_2/specialBarrel5_outline",
			GPU_Rect{736, 770, 32, 18}
		};
	textures.push_back(&specialBarrel5_outline);
	specialBarrel6 = TextureDef{
			"tanks_2/specialBarrel6",
			GPU_Rect{700, 778, 28, 10}
		};
	textures.push_back(&specialBarrel6);
	specialBarrel6_outline = TextureDef{
			"tanks_2/specialBarrel6_outline",
			GPU_Rect{596, 844, 32, 14}
		};
	textures.push_back(&specialBarrel6_outline);
	specialBarrel7 = TextureDef{
			"tanks_2/specialBarrel7",
			GPU_Rect{914, 0, 28, 10}
		};
	textures.push_back(&specialBarrel7);
	specialBarrel7_outline = TextureDef{
			"tanks_2/specialBarrel7_outline",
			GPU_Rect{770, 844, 32, 14}
		};
	textures.push_back(&specialBarrel7_outline);
	tankBlue_barrel1 = TextureDef{
			"tanks_2/tankBlue_barrel1",
			GPU_Rect{330, 912, 28, 14}
		};
	textures.push_back(&tankBlue_barrel1);
	tankBlue_barrel1_outline = TextureDef{
			"tanks_2/tankBlue_barrel1_outline",
			GPU_Rect{874, 752, 32, 18}
		};
	textures.push_back(&tankBlue_barrel1_outline);
	tankBlue_barrel2 = TextureDef{
			"tanks_2/tankBlue_barrel2",
			GPU_Rect{778, 912, 28, 10}
		};
	textures.push_back(&tankBlue_barrel2);
	tankBlue_barrel2_outline = TextureDef{
			"tanks_2/tankBlue_barrel2_outline",
			GPU_Rect{874, 894, 32, 14}
		};
	textures.push_back(&tankBlue_barrel2_outline);
	tankBlue_barrel3_outline = TextureDef{
			"tanks_2/tankBlue_barrel3_outline",
			GPU_Rect{874, 876, 32, 14}
		};
	textures.push_back(&tankBlue_barrel3_outline);
	tankBlue_barrel3 = TextureDef{
			"tanks_2/tankBlue_barrel3",
			GPU_Rect{714, 912, 28, 10}
		};
	textures.push_back(&tankBlue_barrel3);
	tankBody_bigRed = TextureDef{
			"tanks_2/tankBody_bigRed",
			GPU_Rect{806, 392, 50, 50}
		};
	textures.push_back(&tankBody_bigRed);
	tankBody_bigRed_outline = TextureDef{
			"tanks_2/tankBody_bigRed_outline",
			GPU_Rect{806, 50, 54, 54}
		};
	textures.push_back(&tankBody_bigRed_outline);
	tankBody_blue = TextureDef{
			"tanks_2/tankBody_blue",
			GPU_Rect{478, 512, 40, 40}
		};
	textures.push_back(&tankBody_blue);
	tankBody_blue_outline = TextureDef{
			"tanks_2/tankBody_blue_outline",
			GPU_Rect{266, 864, 44, 44}
		};
	textures.push_back(&tankBody_blue_outline);
	tankBody_dark = TextureDef{
			"tanks_2/tankBody_dark",
			GPU_Rect{634, 864, 38, 40}
		};
	textures.push_back(&tankBody_dark);
	tankBody_darkLarge = TextureDef{
			"tanks_2/tankBody_darkLarge",
			GPU_Rect{806, 282, 58, 50}
		};
	textures.push_back(&tankBody_darkLarge);
	tankBody_darkLarge_outline = TextureDef{
			"tanks_2/tankBody_darkLarge_outline",
			GPU_Rect{462, 794, 62, 54}
		};
	textures.push_back(&tankBody_darkLarge_outline);
	tankBody_dark_outline = TextureDef{
			"tanks_2/tankBody_dark_outline",
			GPU_Rect{412, 864, 42, 44}
		};
	textures.push_back(&tankBody_dark_outline);
	tankBody_green = TextureDef{
			"tanks_2/tankBody_green",
			GPU_Rect{592, 864, 38, 40}
		};
	textures.push_back(&tankBody_green);
	tankBody_green_outline = TextureDef{
			"tanks_2/tankBody_green_outline",
			GPU_Rect{366, 864, 42, 44}
		};
	textures.push_back(&tankBody_green_outline);
	tankBody_huge = TextureDef{
			"tanks_2/tankBody_huge",
			GPU_Rect{522, 0, 70, 60}
		};
	textures.push_back(&tankBody_huge);
	tankBody_huge_outline = TextureDef{
			"tanks_2/tankBody_huge_outline",
			GPU_Rect{50, 512, 74, 64}
		};
	textures.push_back(&tankBody_huge_outline);
	tankBody_red = TextureDef{
			"tanks_2/tankBody_red",
			GPU_Rect{760, 864, 38, 36}
		};
	textures.push_back(&tankBody_red);
	tankBody_red_outline = TextureDef{
			"tanks_2/tankBody_red_outline",
			GPU_Rect{504, 864, 42, 40}
		};
	textures.push_back(&tankBody_red_outline);
	tankBody_sand = TextureDef{
			"tanks_2/tankBody_sand",
			GPU_Rect{550, 864, 38, 40}
		};
	textures.push_back(&tankBody_sand);
	tankBody_sand_outline = TextureDef{
			"tanks_2/tankBody_sand_outline",
			GPU_Rect{458, 864, 42, 44}
		};
	textures.push_back(&tankBody_sand_outline);
	tankDark_barrel1_outline = TextureDef{
			"tanks_2/tankDark_barrel1_outline",
			GPU_Rect{630, 702, 32, 18}
		};
	textures.push_back(&tankDark_barrel1_outline);
	tankDark_barrel1 = TextureDef{
			"tanks_2/tankDark_barrel1",
			GPU_Rect{170, 912, 28, 14}
		};
	textures.push_back(&tankDark_barrel1);
	tankDark_barrel2_outline = TextureDef{
			"tanks_2/tankDark_barrel2_outline",
			GPU_Rect{874, 840, 32, 14}
		};
	textures.push_back(&tankDark_barrel2_outline);
	tankDark_barrel2 = TextureDef{
			"tanks_2/tankDark_barrel2",
			GPU_Rect{874, 912, 28, 10}
		};
	textures.push_back(&tankDark_barrel2);
	tankDark_barrel3 = TextureDef{
			"tanks_2/tankDark_barrel3",
			GPU_Rect{836, 898, 28, 10}
		};
	textures.push_back(&tankDark_barrel3);
	tankDark_barrel3_outline = TextureDef{
			"tanks_2/tankDark_barrel3_outline",
			GPU_Rect{102, 912, 32, 14}
		};
	textures.push_back(&tankDark_barrel3_outline);
	tankGreen_barrel1 = TextureDef{
			"tanks_2/tankGreen_barrel1",
			GPU_Rect{564, 844, 28, 14}
		};
	textures.push_back(&tankGreen_barrel1);
	tankGreen_barrel1_outline = TextureDef{
			"tanks_2/tankGreen_barrel1_outline",
			GPU_Rect{666, 700, 32, 18}
		};
	textures.push_back(&tankGreen_barrel1_outline);
	tankGreen_barrel2 = TextureDef{
			"tanks_2/tankGreen_barrel2",
			GPU_Rect{746, 912, 28, 10}
		};
	textures.push_back(&tankGreen_barrel2);
	tankGreen_barrel2_outline = TextureDef{
			"tanks_2/tankGreen_barrel2_outline",
			GPU_Rect{66, 912, 32, 14}
		};
	textures.push_back(&tankGreen_barrel2_outline);
	tankGreen_barrel3 = TextureDef{
			"tanks_2/tankGreen_barrel3",
			GPU_Rect{618, 912, 28, 10}
		};
	textures.push_back(&tankGreen_barrel3);
	tankGreen_barrel3_outline = TextureDef{
			"tanks_2/tankGreen_barrel3_outline",
			GPU_Rect{664, 844, 32, 14}
		};
	textures.push_back(&tankGreen_barrel3_outline);
	tankRed_barrel1 = TextureDef{
			"tanks_2/tankRed_barrel1",
			GPU_Rect{554, 912, 28, 14}
		};
	textures.push_back(&tankRed_barrel1);
	tankRed_barrel1_outline = TextureDef{
			"tanks_2/tankRed_barrel1_outline",
			GPU_Rect{874, 730, 32, 18}
		};
	textures.push_back(&tankRed_barrel1_outline);
	tankRed_barrel2 = TextureDef{
			"tanks_2/tankRed_barrel2",
			GPU_Rect{802, 898, 28, 10}
		};
	textures.push_back(&tankRed_barrel2);
	tankRed_barrel2_outline = TextureDef{
			"tanks_2/tankRed_barrel2_outline",
			GPU_Rect{528, 844, 32, 14}
		};
	textures.push_back(&tankRed_barrel2_outline);
	tankRed_barrel3 = TextureDef{
			"tanks_2/tankRed_barrel3",
			GPU_Rect{650, 912, 28, 10}
		};
	textures.push_back(&tankRed_barrel3);
	tankRed_barrel3_outline = TextureDef{
			"tanks_2/tankRed_barrel3_outline",
			GPU_Rect{874, 858, 32, 14}
		};
	textures.push_back(&tankRed_barrel3_outline);
	tankSand_barrel1 = TextureDef{
			"tanks_2/tankSand_barrel1",
			GPU_Rect{426, 912, 28, 14}
		};
	textures.push_back(&tankSand_barrel1);
	tankSand_barrel1_outline = TextureDef{
			"tanks_2/tankSand_barrel1_outline",
			GPU_Rect{560, 632, 32, 18}
		};
	textures.push_back(&tankSand_barrel1_outline);
	tankSand_barrel2 = TextureDef{
			"tanks_2/tankSand_barrel2",
			GPU_Rect{682, 912, 28, 10}
		};
	textures.push_back(&tankSand_barrel2);
	tankSand_barrel2_outline = TextureDef{
			"tanks_2/tankSand_barrel2_outline",
			GPU_Rect{874, 822, 32, 14}
		};
	textures.push_back(&tankSand_barrel2_outline);
	tankSand_barrel3 = TextureDef{
			"tanks_2/tankSand_barrel3",
			GPU_Rect{810, 912, 28, 10}
		};
	textures.push_back(&tankSand_barrel3);
	tankSand_barrel3_outline = TextureDef{
			"tanks_2/tankSand_barrel3_outline",
			GPU_Rect{30, 912, 32, 14}
		};
	textures.push_back(&tankSand_barrel3_outline);
	tank_bigRed = TextureDef{
			"tanks_2/tank_bigRed",
			GPU_Rect{806, 224, 54, 54}
		};
	textures.push_back(&tank_bigRed);
	tank_dark = TextureDef{
			"tanks_2/tank_dark",
			GPU_Rect{104, 864, 48, 44}
		};
	textures.push_back(&tank_dark);
	tank_blue = TextureDef{
			"tanks_2/tank_blue",
			GPU_Rect{0, 864, 48, 44}
		};
	textures.push_back(&tank_blue);
	tank_darkLarge = TextureDef{
			"tanks_2/tank_darkLarge",
			GPU_Rect{396, 794, 62, 54}
		};
	textures.push_back(&tank_darkLarge);
	tank_green = TextureDef{
			"tanks_2/tank_green",
			GPU_Rect{156, 864, 48, 44}
		};
	textures.push_back(&tank_green);
	tank_huge = TextureDef{
			"tanks_2/tank_huge",
			GPU_Rect{430, 428, 78, 64}
		};
	textures.push_back(&tank_huge);
	tank_red = TextureDef{
			"tanks_2/tank_red",
			GPU_Rect{314, 864, 48, 40}
		};
	textures.push_back(&tank_red);
	tank_sand = TextureDef{
			"tanks_2/tank_sand",
			GPU_Rect{52, 864, 48, 44}
		};
	textures.push_back(&tank_sand);
	tracksDouble = TextureDef{
			"tanks_2/tracksDouble",
			GPU_Rect{806, 658, 54, 42}
		};
	textures.push_back(&tracksDouble);
	tracksLarge = TextureDef{
			"tanks_2/tracksLarge",
			GPU_Rect{806, 704, 54, 42}
		};
	textures.push_back(&tracksLarge);
	tracksSmall = TextureDef{
			"tanks_2/tracksSmall",
			GPU_Rect{208, 864, 54, 38}
		};
	textures.push_back(&tracksSmall);
	treeBrown_large = TextureDef{
			"tanks_2/treeBrown_large",
			GPU_Rect{198, 512, 66, 66}
		};
	textures.push_back(&treeBrown_large);
	treeBrown_leaf = TextureDef{
			"tanks_2/treeBrown_leaf",
			GPU_Rect{856, 820, 12, 10}
		};
	textures.push_back(&treeBrown_leaf);
	treeBrown_small = TextureDef{
			"tanks_2/treeBrown_small",
			GPU_Rect{676, 864, 38, 38}
		};
	textures.push_back(&treeBrown_small);
	treeBrown_twigs = TextureDef{
			"tanks_2/treeBrown_twigs",
			GPU_Rect{874, 562, 24, 28}
		};
	textures.push_back(&treeBrown_twigs);
	treeGreen_large = TextureDef{
			"tanks_2/treeGreen_large",
			GPU_Rect{268, 512, 66, 66}
		};
	textures.push_back(&treeGreen_large);
	treeGreen_leaf = TextureDef{
			"tanks_2/treeGreen_leaf",
			GPU_Rect{856, 834, 12, 10}
		};
	textures.push_back(&treeGreen_leaf);
	treeGreen_small = TextureDef{
			"tanks_2/treeGreen_small",
			GPU_Rect{718, 864, 38, 38}
		};
	textures.push_back(&treeGreen_small);
	treeGreen_twigs = TextureDef{
			"tanks_2/treeGreen_twigs",
			GPU_Rect{874, 594, 24, 28}
		};
	textures.push_back(&treeGreen_twigs);
	wireCrooked = TextureDef{
			"tanks_2/wireCrooked",
			GPU_Rect{806, 608, 50, 46}
		};
	textures.push_back(&wireCrooked);
	wireStraight = TextureDef{
			"tanks_2/wireStraight",
			GPU_Rect{326, 388, 72, 20}
		};
	textures.push_back(&wireStraight);
	
};

r_hbzjo::r_hbzjo()
{
	
	fire1 = TextureDef{
			"td/fire1",
			GPU_Rect{338, 512, 66, 66}
		};
	textures.push_back(&fire1);
	fire2 = TextureDef{
			"td/fire2",
			GPU_Rect{408, 512, 66, 66}
		};
	textures.push_back(&fire2);
	fire3 = TextureDef{
			"td/fire3",
			GPU_Rect{522, 64, 66, 66}
		};
	textures.push_back(&fire3);
	fire4 = TextureDef{
			"td/fire4",
			GPU_Rect{522, 134, 66, 66}
		};
	textures.push_back(&fire4);
	mount1 = TextureDef{
			"td/mount1",
			GPU_Rect{522, 204, 66, 66}
		};
	textures.push_back(&mount1);
	mount2 = TextureDef{
			"td/mount2",
			GPU_Rect{0, 584, 66, 66}
		};
	textures.push_back(&mount2);
	mount3 = TextureDef{
			"td/mount3",
			GPU_Rect{350, 584, 66, 66}
		};
	textures.push_back(&mount3);
	mount4 = TextureDef{
			"td/mount4",
			GPU_Rect{490, 584, 66, 66}
		};
	textures.push_back(&mount4);
	plane_gray = TextureDef{
			"td/plane_gray",
			GPU_Rect{596, 70, 66, 66}
		};
	textures.push_back(&plane_gray);
	plane_green = TextureDef{
			"td/plane_green",
			GPU_Rect{596, 140, 66, 66}
		};
	textures.push_back(&plane_green);
	plane_shadow_green = TextureDef{
			"td/plane_shadow_green",
			GPU_Rect{596, 210, 66, 66}
		};
	textures.push_back(&plane_shadow_green);
	plane_shadow_gray = TextureDef{
			"td/plane_shadow_gray",
			GPU_Rect{596, 350, 66, 66}
		};
	textures.push_back(&plane_shadow_gray);
	rock1 = TextureDef{
			"td/rock1",
			GPU_Rect{596, 420, 66, 66}
		};
	textures.push_back(&rock1);
	rock2 = TextureDef{
			"td/rock2",
			GPU_Rect{596, 560, 66, 66}
		};
	textures.push_back(&rock2);
	rock3 = TextureDef{
			"td/rock3",
			GPU_Rect{0, 654, 66, 66}
		};
	textures.push_back(&rock3);
	rocket_large = TextureDef{
			"td/rocket_large",
			GPU_Rect{0, 724, 66, 66}
		};
	textures.push_back(&rocket_large);
	rocket_small = TextureDef{
			"td/rocket_small",
			GPU_Rect{736, 70, 66, 66}
		};
	textures.push_back(&rocket_small);
	soldier1 = TextureDef{
			"td/soldier1",
			GPU_Rect{736, 140, 66, 66}
		};
	textures.push_back(&soldier1);
	soldier2 = TextureDef{
			"td/soldier2",
			GPU_Rect{736, 420, 66, 66}
		};
	textures.push_back(&soldier2);
	soldier3 = TextureDef{
			"td/soldier3",
			GPU_Rect{736, 490, 66, 66}
		};
	textures.push_back(&soldier3);
	soldier4 = TextureDef{
			"td/soldier4",
			GPU_Rect{0, 794, 66, 66}
		};
	textures.push_back(&soldier4);
	tankTurret_beige = TextureDef{
			"td/tankTurret_beige",
			GPU_Rect{70, 794, 66, 66}
		};
	textures.push_back(&tankTurret_beige);
	tankTurret_green = TextureDef{
			"td/tankTurret_green",
			GPU_Rect{736, 700, 66, 66}
		};
	textures.push_back(&tankTurret_green);
	tank_base_green = TextureDef{
			"td/tank_base_green",
			GPU_Rect{736, 630, 66, 66}
		};
	textures.push_back(&tank_base_green);
	towerDefense_tile015 = TextureDef{
			"td/towerDefense_tile015",
			GPU_Rect{736, 560, 66, 66}
		};
	textures.push_back(&towerDefense_tile015);
	tank_base_beige = TextureDef{
			"td/tank_base_beige",
			GPU_Rect{736, 350, 66, 66}
		};
	textures.push_back(&tank_base_beige);
	towerDefense_tile016 = TextureDef{
			"td/towerDefense_tile016",
			GPU_Rect{736, 280, 66, 66}
		};
	textures.push_back(&towerDefense_tile016);
	towerDefense_tile017 = TextureDef{
			"td/towerDefense_tile017",
			GPU_Rect{736, 210, 66, 66}
		};
	textures.push_back(&towerDefense_tile017);
	towerDefense_tile018 = TextureDef{
			"td/towerDefense_tile018",
			GPU_Rect{630, 724, 66, 66}
		};
	textures.push_back(&towerDefense_tile018);
	towerDefense_tile019 = TextureDef{
			"td/towerDefense_tile019",
			GPU_Rect{560, 724, 66, 66}
		};
	textures.push_back(&towerDefense_tile019);
	towerDefense_tile020 = TextureDef{
			"td/towerDefense_tile020",
			GPU_Rect{490, 724, 66, 66}
		};
	textures.push_back(&towerDefense_tile020);
	towerDefense_tile021 = TextureDef{
			"td/towerDefense_tile021",
			GPU_Rect{420, 724, 66, 66}
		};
	textures.push_back(&towerDefense_tile021);
	towerDefense_tile022 = TextureDef{
			"td/towerDefense_tile022",
			GPU_Rect{350, 724, 66, 66}
		};
	textures.push_back(&towerDefense_tile022);
	towerDefense_tile272 = TextureDef{
			"td/towerDefense_tile272",
			GPU_Rect{280, 724, 66, 66}
		};
	textures.push_back(&towerDefense_tile272);
	towerDefense_tile273 = TextureDef{
			"td/towerDefense_tile273",
			GPU_Rect{128, 512, 66, 66}
		};
	textures.push_back(&towerDefense_tile273);
	towerDefense_tile274 = TextureDef{
			"td/towerDefense_tile274",
			GPU_Rect{210, 724, 66, 66}
		};
	textures.push_back(&towerDefense_tile274);
	towerDefense_tile275 = TextureDef{
			"td/towerDefense_tile275",
			GPU_Rect{140, 724, 66, 66}
		};
	textures.push_back(&towerDefense_tile275);
	towerDefense_tile276 = TextureDef{
			"td/towerDefense_tile276",
			GPU_Rect{70, 724, 66, 66}
		};
	textures.push_back(&towerDefense_tile276);
	towerDefense_tile277 = TextureDef{
			"td/towerDefense_tile277",
			GPU_Rect{666, 630, 66, 66}
		};
	textures.push_back(&towerDefense_tile277);
	towerDefense_tile278 = TextureDef{
			"td/towerDefense_tile278",
			GPU_Rect{666, 560, 66, 66}
		};
	textures.push_back(&towerDefense_tile278);
	towerDefense_tile279 = TextureDef{
			"td/towerDefense_tile279",
			GPU_Rect{666, 490, 66, 66}
		};
	textures.push_back(&towerDefense_tile279);
	towerDefense_tile280 = TextureDef{
			"td/towerDefense_tile280",
			GPU_Rect{666, 420, 66, 66}
		};
	textures.push_back(&towerDefense_tile280);
	towerDefense_tile281 = TextureDef{
			"td/towerDefense_tile281",
			GPU_Rect{666, 350, 66, 66}
		};
	textures.push_back(&towerDefense_tile281);
	towerDefense_tile282 = TextureDef{
			"td/towerDefense_tile282",
			GPU_Rect{666, 280, 66, 66}
		};
	textures.push_back(&towerDefense_tile282);
	towerDefense_tile283 = TextureDef{
			"td/towerDefense_tile283",
			GPU_Rect{666, 210, 66, 66}
		};
	textures.push_back(&towerDefense_tile283);
	towerDefense_tile284 = TextureDef{
			"td/towerDefense_tile284",
			GPU_Rect{666, 140, 66, 66}
		};
	textures.push_back(&towerDefense_tile284);
	towerDefense_tile285 = TextureDef{
			"td/towerDefense_tile285",
			GPU_Rect{666, 70, 66, 66}
		};
	textures.push_back(&towerDefense_tile285);
	towerDefense_tile286 = TextureDef{
			"td/towerDefense_tile286",
			GPU_Rect{736, 0, 66, 66}
		};
	textures.push_back(&towerDefense_tile286);
	towerDefense_tile287 = TextureDef{
			"td/towerDefense_tile287",
			GPU_Rect{666, 0, 66, 66}
		};
	textures.push_back(&towerDefense_tile287);
	towerDefense_tile288 = TextureDef{
			"td/towerDefense_tile288",
			GPU_Rect{560, 654, 66, 66}
		};
	textures.push_back(&towerDefense_tile288);
	towerDefense_tile289 = TextureDef{
			"td/towerDefense_tile289",
			GPU_Rect{490, 654, 66, 66}
		};
	textures.push_back(&towerDefense_tile289);
	tree1 = TextureDef{
			"td/tree1",
			GPU_Rect{420, 654, 66, 66}
		};
	textures.push_back(&tree1);
	tree2 = TextureDef{
			"td/tree2",
			GPU_Rect{350, 654, 66, 66}
		};
	textures.push_back(&tree2);
	tree3 = TextureDef{
			"td/tree3",
			GPU_Rect{280, 654, 66, 66}
		};
	textures.push_back(&tree3);
	tree4 = TextureDef{
			"td/tree4",
			GPU_Rect{210, 654, 66, 66}
		};
	textures.push_back(&tree4);
	towerDefense_tile290 = TextureDef{
			"td/towerDefense_tile290",
			GPU_Rect{140, 654, 66, 66}
		};
	textures.push_back(&towerDefense_tile290);
	tree5 = TextureDef{
			"td/tree5",
			GPU_Rect{70, 654, 66, 66}
		};
	textures.push_back(&tree5);
	turret1 = TextureDef{
			"td/turret1",
			GPU_Rect{596, 490, 66, 66}
		};
	textures.push_back(&turret1);
	turret10 = TextureDef{
			"td/turret10",
			GPU_Rect{596, 0, 66, 66}
		};
	textures.push_back(&turret10);
	turret2 = TextureDef{
			"td/turret2",
			GPU_Rect{280, 584, 66, 66}
		};
	textures.push_back(&turret2);
	turret3 = TextureDef{
			"td/turret3",
			GPU_Rect{210, 584, 66, 66}
		};
	textures.push_back(&turret3);
	turret4 = TextureDef{
			"td/turret4",
			GPU_Rect{140, 584, 66, 66}
		};
	textures.push_back(&turret4);
	turret5 = TextureDef{
			"td/turret5",
			GPU_Rect{70, 584, 66, 66}
		};
	textures.push_back(&turret5);
	turret6 = TextureDef{
			"td/turret6",
			GPU_Rect{522, 484, 66, 66}
		};
	textures.push_back(&turret6);
	turret8 = TextureDef{
			"td/turret8",
			GPU_Rect{522, 414, 66, 66}
		};
	textures.push_back(&turret8);
	turret9 = TextureDef{
			"td/turret9",
			GPU_Rect{522, 344, 66, 66}
		};
	textures.push_back(&turret9);
	turret7 = TextureDef{
			"td/turret7",
			GPU_Rect{522, 274, 66, 66}
		};
	textures.push_back(&turret7);
	
};

r_i9ox9::r_i9ox9()
{
	
	
	folders.push_back(&Bullets);
	folders.push_back(&Obstacles);
	folders.push_back(&Smoke);
};

TextureNames::TextureNames()
{
	
	
	folders.push_back(&tanks_2);
	folders.push_back(&td);
	folders.push_back(&tanks_1);
};
