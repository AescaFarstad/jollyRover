#pragma once
#include <string>
#include <vector>
#include <SDL_gpu.h>

class TextureDef 
{	
public:	
	std::string name;
	GPU_Rect rect;
};

class TextureFolder 
{	
public:	
	std::string name;
	std::vector<TextureFolder*> folders;
	std::vector<TextureDef*> textures;
	
	TextureDef* getTextureByName(const std::string& name);
};


// /tanks_1/Bullets
class r_xhrer : public TextureFolder
{
public:	
	r_xhrer();
	
	
	// 32 x 18
	TextureDef bulletBeige;
	// 32 x 18
	TextureDef bulletBeigeSilver;
	// 40 x 26
	TextureDef bulletBeigeSilver_outline;
	// 40 x 26
	TextureDef bulletBeige_outline;
	// 32 x 18
	TextureDef bulletBlue;
	// 32 x 18
	TextureDef bulletBlueSilver;
	// 40 x 26
	TextureDef bulletBlueSilver_outline;
	// 40 x 26
	TextureDef bulletBlue_outline;
	// 32 x 18
	TextureDef bulletGreen;
	// 32 x 18
	TextureDef bulletGreenSilver;
	// 40 x 26
	TextureDef bulletGreenSilver_outline;
	// 40 x 26
	TextureDef bulletGreen_outline;
	// 32 x 18
	TextureDef bulletRed;
	// 32 x 18
	TextureDef bulletRedSilver;
	// 40 x 26
	TextureDef bulletRedSilver_outline;
	// 40 x 26
	TextureDef bulletRed_outline;
	// 32 x 18
	TextureDef bulletSilver;
	// 32 x 18
	TextureDef bulletSilverSilver;
	// 40 x 26
	TextureDef bulletSilverSilver_outline;
	// 40 x 26
	TextureDef bulletSilver_outline;
	// 32 x 18
	TextureDef bulletYellow;
	// 32 x 18
	TextureDef bulletYellowSilver;
	// 40 x 26
	TextureDef bulletYellowSilver_outline;
	// 40 x 26
	TextureDef bulletYellow_outline;
};
// /tanks_1/Obstacles
class r_jr3fj : public TextureFolder
{
public:	
	r_jr3fj();
	
	
	// 68 x 50
	TextureDef barrelGreen_side;
	// 68 x 50
	TextureDef barrelGreen_side_damaged;
	// 54 x 54
	TextureDef barrelGreen_up;
	// 68 x 50
	TextureDef barrelGrey_sde_rust;
	// 68 x 50
	TextureDef barrelGrey_side;
	// 54 x 54
	TextureDef barrelGrey_up;
	// 68 x 50
	TextureDef barrelRed_side;
	// 54 x 54
	TextureDef barrelRed_up;
	// 102 x 102
	TextureDef oil;
	// 50 x 72
	TextureDef sandbagBeige;
	// 50 x 72
	TextureDef sandbagBrown;
};
// /tanks_1/Smoke
class r_ir8mj : public TextureFolder
{
public:	
	r_ir8mj();
	
	
	// 92 x 92
	TextureDef smokeGrey0;
	// 94 x 98
	TextureDef smokeGrey1;
	// 84 x 84
	TextureDef smokeGrey3;
	// 104 x 96
	TextureDef smokeGrey2;
	// 102 x 106
	TextureDef smokeGrey4;
	// 112 x 104
	TextureDef smokeGrey5;
	// 92 x 92
	TextureDef smokeOrange0;
	// 94 x 98
	TextureDef smokeOrange1;
	// 104 x 96
	TextureDef smokeOrange2;
	// 84 x 84
	TextureDef smokeOrange3;
	// 102 x 106
	TextureDef smokeOrange4;
	// 112 x 104
	TextureDef smokeOrange5;
	// 94 x 98
	TextureDef smokeWhite0;
	// 104 x 96
	TextureDef smokeWhite1;
	// 84 x 84
	TextureDef smokeWhite2;
	// 102 x 106
	TextureDef smokeWhite3;
	// 112 x 104
	TextureDef smokeWhite4;
	// 92 x 92
	TextureDef smokeWhite5;
	// 92 x 92
	TextureDef smokeYellow0;
	// 94 x 98
	TextureDef smokeYellow1;
	// 104 x 96
	TextureDef smokeYellow2;
	// 84 x 84
	TextureDef smokeYellow3;
	// 102 x 106
	TextureDef smokeYellow4;
	// 112 x 104
	TextureDef smokeYellow5;
};
// /tanks_2
class r_alp0p : public TextureFolder
{
public:	
	r_alp0p();
	
	
	// 34 x 26
	TextureDef barrelBlack_side;
	// 30 x 30
	TextureDef barrelBlack_top;
	// 34 x 26
	TextureDef barrelGreen_side;
	// 30 x 30
	TextureDef barrelGreen_top;
	// 34 x 26
	TextureDef barrelRed_side;
	// 30 x 30
	TextureDef barrelRed_top;
	// 34 x 26
	TextureDef barrelRust_side;
	// 30 x 30
	TextureDef barrelRust_top;
	// 34 x 34
	TextureDef barricadeMetal;
	// 34 x 34
	TextureDef barricadeWood;
	// 16 x 10
	TextureDef bulletBlue1;
	// 20 x 14
	TextureDef bulletBlue1_outline;
	// 18 x 14
	TextureDef bulletBlue2;
	// 20 x 10
	TextureDef bulletBlue3;
	// 24 x 14
	TextureDef bulletBlue3_outline;
	// 16 x 10
	TextureDef bulletDark1;
	// 22 x 18
	TextureDef bulletBlue2_outline;
	// 20 x 14
	TextureDef bulletDark1_outline;
	// 18 x 14
	TextureDef bulletDark2;
	// 22 x 18
	TextureDef bulletDark2_outline;
	// 20 x 10
	TextureDef bulletDark3;
	// 24 x 14
	TextureDef bulletDark3_outline;
	// 16 x 10
	TextureDef bulletGreen1;
	// 20 x 14
	TextureDef bulletGreen1_outline;
	// 18 x 14
	TextureDef bulletGreen2;
	// 22 x 18
	TextureDef bulletGreen2_outline;
	// 20 x 10
	TextureDef bulletGreen3;
	// 24 x 14
	TextureDef bulletGreen3_outline;
	// 16 x 10
	TextureDef bulletRed1;
	// 20 x 14
	TextureDef bulletRed1_outline;
	// 18 x 14
	TextureDef bulletRed2;
	// 22 x 18
	TextureDef bulletRed2_outline;
	// 20 x 10
	TextureDef bulletRed3;
	// 24 x 14
	TextureDef bulletRed3_outline;
	// 16 x 10
	TextureDef bulletSand1;
	// 20 x 14
	TextureDef bulletSand1_outline;
	// 18 x 14
	TextureDef bulletSand2;
	// 22 x 18
	TextureDef bulletSand2_outline;
	// 20 x 10
	TextureDef bulletSand3;
	// 24 x 14
	TextureDef bulletSand3_outline;
	// 34 x 34
	TextureDef crateMetal;
	// 34 x 34
	TextureDef crateMetal_side;
	// 34 x 34
	TextureDef crateWood;
	// 34 x 34
	TextureDef crateWood_side;
	// 66 x 66
	TextureDef explosion1;
	// 62 x 62
	TextureDef explosion2;
	// 70 x 70
	TextureDef explosion3;
	// 50 x 52
	TextureDef explosion4;
	// 58 x 58
	TextureDef explosion5;
	// 66 x 66
	TextureDef explosionSmoke1;
	// 62 x 62
	TextureDef explosionSmoke2;
	// 70 x 70
	TextureDef explosionSmoke3;
	// 50 x 52
	TextureDef explosionSmoke4;
	// 58 x 58
	TextureDef explosionSmoke5;
	// 22 x 54
	TextureDef fenceRed;
	// 22 x 58
	TextureDef fenceYellow;
	// 56 x 56
	TextureDef oilSpill_large;
	// 20 x 20
	TextureDef oilSpill_small;
	// 28 x 38
	TextureDef sandbagBeige;
	// 34 x 48
	TextureDef sandbagBeige_open;
	// 28 x 38
	TextureDef sandbagBrown;
	// 34 x 48
	TextureDef sandbagBrown_open;
	// 30 x 26
	TextureDef shotLarge;
	// 34 x 22
	TextureDef shotOrange;
	// 44 x 28
	TextureDef shotRed;
	// 32 x 14
	TextureDef shotThin;
	// 28 x 20
	TextureDef specialBarrel1;
	// 32 x 24
	TextureDef specialBarrel1_outline;
	// 30 x 18
	TextureDef specialBarrel2;
	// 34 x 22
	TextureDef specialBarrel2_outline;
	// 34 x 16
	TextureDef specialBarrel3;
	// 38 x 20
	TextureDef specialBarrel3_outline;
	// 38 x 16
	TextureDef specialBarrel4;
	// 42 x 20
	TextureDef specialBarrel4_outline;
	// 32 x 18
	TextureDef specialBarrel5;
	// 36 x 22
	TextureDef specialBarrel5_outline;
	// 32 x 14
	TextureDef specialBarrel6;
	// 36 x 18
	TextureDef specialBarrel6_outline;
	// 32 x 14
	TextureDef specialBarrel7;
	// 36 x 18
	TextureDef specialBarrel7_outline;
	// 32 x 18
	TextureDef tankBlue_barrel1;
	// 36 x 22
	TextureDef tankBlue_barrel1_outline;
	// 32 x 14
	TextureDef tankBlue_barrel2;
	// 36 x 18
	TextureDef tankBlue_barrel2_outline;
	// 36 x 18
	TextureDef tankBlue_barrel3_outline;
	// 32 x 14
	TextureDef tankBlue_barrel3;
	// 54 x 54
	TextureDef tankBody_bigRed;
	// 58 x 58
	TextureDef tankBody_bigRed_outline;
	// 44 x 44
	TextureDef tankBody_blue;
	// 48 x 48
	TextureDef tankBody_blue_outline;
	// 42 x 44
	TextureDef tankBody_dark;
	// 62 x 54
	TextureDef tankBody_darkLarge;
	// 66 x 58
	TextureDef tankBody_darkLarge_outline;
	// 46 x 48
	TextureDef tankBody_dark_outline;
	// 42 x 44
	TextureDef tankBody_green;
	// 46 x 48
	TextureDef tankBody_green_outline;
	// 74 x 64
	TextureDef tankBody_huge;
	// 78 x 68
	TextureDef tankBody_huge_outline;
	// 42 x 40
	TextureDef tankBody_red;
	// 46 x 44
	TextureDef tankBody_red_outline;
	// 42 x 44
	TextureDef tankBody_sand;
	// 46 x 48
	TextureDef tankBody_sand_outline;
	// 36 x 22
	TextureDef tankDark_barrel1_outline;
	// 32 x 18
	TextureDef tankDark_barrel1;
	// 36 x 18
	TextureDef tankDark_barrel2_outline;
	// 32 x 14
	TextureDef tankDark_barrel2;
	// 32 x 14
	TextureDef tankDark_barrel3;
	// 36 x 18
	TextureDef tankDark_barrel3_outline;
	// 32 x 18
	TextureDef tankGreen_barrel1;
	// 36 x 22
	TextureDef tankGreen_barrel1_outline;
	// 32 x 14
	TextureDef tankGreen_barrel2;
	// 36 x 18
	TextureDef tankGreen_barrel2_outline;
	// 32 x 14
	TextureDef tankGreen_barrel3;
	// 36 x 18
	TextureDef tankGreen_barrel3_outline;
	// 32 x 18
	TextureDef tankRed_barrel1;
	// 36 x 22
	TextureDef tankRed_barrel1_outline;
	// 32 x 14
	TextureDef tankRed_barrel2;
	// 36 x 18
	TextureDef tankRed_barrel2_outline;
	// 32 x 14
	TextureDef tankRed_barrel3;
	// 36 x 18
	TextureDef tankRed_barrel3_outline;
	// 32 x 18
	TextureDef tankSand_barrel1;
	// 36 x 22
	TextureDef tankSand_barrel1_outline;
	// 32 x 14
	TextureDef tankSand_barrel2;
	// 36 x 18
	TextureDef tankSand_barrel2_outline;
	// 32 x 14
	TextureDef tankSand_barrel3;
	// 36 x 18
	TextureDef tankSand_barrel3_outline;
	// 58 x 58
	TextureDef tank_bigRed;
	// 52 x 48
	TextureDef tank_dark;
	// 52 x 48
	TextureDef tank_blue;
	// 66 x 58
	TextureDef tank_darkLarge;
	// 52 x 48
	TextureDef tank_green;
	// 82 x 68
	TextureDef tank_huge;
	// 52 x 44
	TextureDef tank_red;
	// 52 x 48
	TextureDef tank_sand;
	// 58 x 46
	TextureDef tracksDouble;
	// 58 x 46
	TextureDef tracksLarge;
	// 58 x 42
	TextureDef tracksSmall;
	// 70 x 70
	TextureDef treeBrown_large;
	// 16 x 14
	TextureDef treeBrown_leaf;
	// 42 x 42
	TextureDef treeBrown_small;
	// 28 x 32
	TextureDef treeBrown_twigs;
	// 70 x 70
	TextureDef treeGreen_large;
	// 16 x 14
	TextureDef treeGreen_leaf;
	// 42 x 42
	TextureDef treeGreen_small;
	// 28 x 32
	TextureDef treeGreen_twigs;
	// 54 x 50
	TextureDef wireCrooked;
	// 76 x 24
	TextureDef wireStraight;
};
// /td
class r_hbzjo : public TextureFolder
{
public:	
	r_hbzjo();
	
	
	// 70 x 70
	TextureDef fire1;
	// 70 x 70
	TextureDef fire2;
	// 70 x 70
	TextureDef fire3;
	// 70 x 70
	TextureDef fire4;
	// 70 x 70
	TextureDef mount1;
	// 70 x 70
	TextureDef mount2;
	// 70 x 70
	TextureDef mount3;
	// 70 x 70
	TextureDef mount4;
	// 70 x 70
	TextureDef plane_gray;
	// 70 x 70
	TextureDef plane_green;
	// 70 x 70
	TextureDef plane_shadow_green;
	// 70 x 70
	TextureDef plane_shadow_gray;
	// 70 x 70
	TextureDef rock1;
	// 70 x 70
	TextureDef rock2;
	// 70 x 70
	TextureDef rock3;
	// 70 x 70
	TextureDef rocket_large;
	// 70 x 70
	TextureDef rocket_small;
	// 70 x 70
	TextureDef soldier1;
	// 70 x 70
	TextureDef soldier2;
	// 70 x 70
	TextureDef soldier3;
	// 70 x 70
	TextureDef soldier4;
	// 70 x 70
	TextureDef tankTurret_beige;
	// 70 x 70
	TextureDef tankTurret_green;
	// 70 x 70
	TextureDef tank_base_green;
	// 70 x 70
	TextureDef towerDefense_tile015;
	// 70 x 70
	TextureDef tank_base_beige;
	// 70 x 70
	TextureDef towerDefense_tile016;
	// 70 x 70
	TextureDef towerDefense_tile017;
	// 70 x 70
	TextureDef towerDefense_tile018;
	// 70 x 70
	TextureDef towerDefense_tile019;
	// 70 x 70
	TextureDef towerDefense_tile020;
	// 70 x 70
	TextureDef towerDefense_tile021;
	// 70 x 70
	TextureDef towerDefense_tile022;
	// 70 x 70
	TextureDef towerDefense_tile272;
	// 70 x 70
	TextureDef towerDefense_tile273;
	// 70 x 70
	TextureDef towerDefense_tile274;
	// 70 x 70
	TextureDef towerDefense_tile275;
	// 70 x 70
	TextureDef towerDefense_tile276;
	// 70 x 70
	TextureDef towerDefense_tile277;
	// 70 x 70
	TextureDef towerDefense_tile278;
	// 70 x 70
	TextureDef towerDefense_tile279;
	// 70 x 70
	TextureDef towerDefense_tile280;
	// 70 x 70
	TextureDef towerDefense_tile281;
	// 70 x 70
	TextureDef towerDefense_tile282;
	// 70 x 70
	TextureDef towerDefense_tile283;
	// 70 x 70
	TextureDef towerDefense_tile284;
	// 70 x 70
	TextureDef towerDefense_tile285;
	// 70 x 70
	TextureDef towerDefense_tile286;
	// 70 x 70
	TextureDef towerDefense_tile287;
	// 70 x 70
	TextureDef towerDefense_tile288;
	// 70 x 70
	TextureDef towerDefense_tile289;
	// 70 x 70
	TextureDef tree1;
	// 70 x 70
	TextureDef tree2;
	// 70 x 70
	TextureDef tree3;
	// 70 x 70
	TextureDef tree4;
	// 70 x 70
	TextureDef towerDefense_tile290;
	// 70 x 70
	TextureDef tree5;
	// 70 x 70
	TextureDef turret1;
	// 70 x 70
	TextureDef turret10;
	// 70 x 70
	TextureDef turret2;
	// 70 x 70
	TextureDef turret3;
	// 70 x 70
	TextureDef turret4;
	// 70 x 70
	TextureDef turret5;
	// 70 x 70
	TextureDef turret6;
	// 70 x 70
	TextureDef turret8;
	// 70 x 70
	TextureDef turret9;
	// 70 x 70
	TextureDef turret7;
};
// /tanks_1
class r_i9ox9 : public TextureFolder
{
public:	
	r_i9ox9();
	
	r_xhrer Bullets;
	r_jr3fj Obstacles;
	r_ir8mj Smoke;
	
};
// 
class TextureNames : public TextureFolder
{
public:	
	TextureNames();
	
	r_alp0p tanks_2;
	r_hbzjo td;
	r_i9ox9 tanks_1;
	
};