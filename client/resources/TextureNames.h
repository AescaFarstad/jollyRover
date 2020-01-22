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


// /art/path/gray
class r_5soil : public TextureFolder
{
public:	
	r_5soil();
	
	
	// 16 x 16
	TextureDef s1;
	// 16 x 16
	TextureDef s2;
	// 16 x 16
	TextureDef s3;
	// 25 x 14
	TextureDef b2;
	// 29 x 14
	TextureDef b3;
	// 27 x 14
	TextureDef b1;
};
// /art/path/red
class r_byo84 : public TextureFolder
{
public:	
	r_byo84();
	
	
	// 30 x 34
	TextureDef b1;
	// 32 x 34
	TextureDef b3;
	// 34 x 34
	TextureDef b4;
	// 30 x 30
	TextureDef b2;
	// 18 x 18
	TextureDef s2;
	// 17 x 18
	TextureDef s3;
	// 18 x 17
	TextureDef s1;
};
// /art/path/green
class r_yp1st : public TextureFolder
{
public:	
	r_yp1st();
	
	
	// 22 x 17
	TextureDef s1;
	// 29 x 17
	TextureDef b3;
	// 30 x 17
	TextureDef b1;
	// 23 x 17
	TextureDef s2;
	// 32 x 17
	TextureDef b2;
	// 21 x 17
	TextureDef s3;
};
// /tanks_1/Smoke
class r_hbzjo : public TextureFolder
{
public:	
	r_hbzjo();
	
	
	// 101 x 104
	TextureDef smokeGrey4;
	// 101 x 104
	TextureDef smokeOrange4;
	// 101 x 104
	TextureDef smokeWhite3;
	// 101 x 104
	TextureDef smokeYellow4;
	// 111 x 102
	TextureDef smokeYellow5;
	// 111 x 102
	TextureDef smokeGrey5;
	// 111 x 102
	TextureDef smokeWhite4;
	// 111 x 102
	TextureDef smokeOrange5;
	// 93 x 96
	TextureDef smokeWhite0;
	// 93 x 96
	TextureDef smokeOrange1;
	// 93 x 96
	TextureDef smokeYellow1;
	// 93 x 96
	TextureDef smokeGrey1;
	// 103 x 94
	TextureDef smokeWhite1;
	// 103 x 94
	TextureDef smokeGrey2;
	// 103 x 94
	TextureDef smokeOrange2;
	// 103 x 94
	TextureDef smokeYellow2;
	// 91 x 91
	TextureDef smokeYellow0;
	// 91 x 91
	TextureDef smokeOrange0;
	// 91 x 91
	TextureDef smokeWhite5;
	// 91 x 91
	TextureDef smokeGrey0;
	// 83 x 83
	TextureDef smokeYellow3;
	// 83 x 83
	TextureDef smokeOrange3;
	// 83 x 83
	TextureDef smokeGrey3;
	// 83 x 83
	TextureDef smokeWhite2;
};
// /tanks_1/Obstacles
class r_i9ox9 : public TextureFolder
{
public:	
	r_i9ox9();
	
	
	// 100 x 100
	TextureDef oil;
	// 48 x 70
	TextureDef sandbagBeige;
	// 48 x 70
	TextureDef sandbagBrown;
	// 52 x 52
	TextureDef barrelGrey_up;
	// 52 x 52
	TextureDef barrelRed_up;
	// 52 x 52
	TextureDef barrelGreen_up;
	// 66 x 48
	TextureDef barrelRed_side;
	// 66 x 48
	TextureDef barrelGreen_side_damaged;
	// 66 x 48
	TextureDef barrelGreen_side;
	// 66 x 48
	TextureDef barrelGrey_side;
	// 66 x 48
	TextureDef barrelGrey_sde_rust;
};
// /art/path
class r_mt6ul : public TextureFolder
{
public:	
	r_mt6ul();
	
	r_byo84 red;
	r_yp1st green;
	r_5soil gray;
	
};
// /tanks_1/Bullets
class r_5q2x0 : public TextureFolder
{
public:	
	r_5q2x0();
	
	
	// 38 x 24
	TextureDef bulletYellowSilver_outline;
	// 38 x 24
	TextureDef bulletSilver_outline;
	// 38 x 24
	TextureDef bulletSilverSilver_outline;
	// 38 x 24
	TextureDef bulletRed_outline;
	// 38 x 24
	TextureDef bulletRedSilver_outline;
	// 38 x 24
	TextureDef bulletGreen_outline;
	// 38 x 24
	TextureDef bulletGreenSilver_outline;
	// 38 x 24
	TextureDef bulletBlue_outline;
	// 38 x 24
	TextureDef bulletBlueSilver_outline;
	// 38 x 24
	TextureDef bulletBeige_outline;
	// 38 x 24
	TextureDef bulletBeigeSilver_outline;
	// 38 x 24
	TextureDef bulletYellow_outline;
	// 30 x 16
	TextureDef bulletBeige;
	// 30 x 16
	TextureDef bulletBeigeSilver;
	// 30 x 16
	TextureDef bulletBlue;
	// 30 x 16
	TextureDef bulletBlueSilver;
	// 30 x 16
	TextureDef bulletGreenSilver;
	// 30 x 16
	TextureDef bulletGreen;
	// 30 x 16
	TextureDef bulletRed;
	// 30 x 16
	TextureDef bulletRedSilver;
	// 30 x 16
	TextureDef bulletSilver;
	// 30 x 16
	TextureDef bulletYellow;
	// 30 x 16
	TextureDef bulletSilverSilver;
	// 30 x 16
	TextureDef bulletYellowSilver;
};
// /art
class r_ir8mj : public TextureFolder
{
public:	
	r_ir8mj();
	
	r_mt6ul path;
	
};
// /td
class r_xhrer : public TextureFolder
{
public:	
	r_xhrer();
	
	
	// 68 x 68
	TextureDef fire2;
	// 68 x 68
	TextureDef fire4;
	// 68 x 68
	TextureDef fire3;
	// 68 x 68
	TextureDef plane_gray;
	// 68 x 68
	TextureDef plane_green;
	// 68 x 68
	TextureDef plane_shadow_green;
	// 68 x 68
	TextureDef tank_base_beige;
	// 68 x 68
	TextureDef plane_shadow_gray;
	// 68 x 68
	TextureDef rocket_small;
	// 68 x 68
	TextureDef rocket_large;
	// 68 x 68
	TextureDef tank_base_green;
	// 68 x 68
	TextureDef soldier2;
	// 68 x 68
	TextureDef fire1;
	// 68 x 68
	TextureDef tankTurret_beige;
	// 68 x 68
	TextureDef tankTurret_green;
	// 68 x 68
	TextureDef soldier1;
	// 68 x 68
	TextureDef soldier4;
	// 68 x 68
	TextureDef soldier3;
};
// /tanks_2
class r_jr3fj : public TextureFolder
{
public:	
	r_jr3fj();
	
	
	// 68 x 68
	TextureDef treeBrown_large;
	// 67 x 68
	TextureDef explosion3;
	// 68 x 68
	TextureDef treeGreen_large;
	// 67 x 67
	TextureDef explosionSmoke3;
	// 76 x 66
	TextureDef tankBody_huge_outline;
	// 80 x 66
	TextureDef tank_huge;
	// 64 x 64
	TextureDef explosionSmoke1;
	// 64 x 64
	TextureDef explosion1;
	// 72 x 62
	TextureDef tankBody_huge;
	// 60 x 61
	TextureDef explosionSmoke2;
	// 60 x 61
	TextureDef explosion2;
	// 56 x 57
	TextureDef explosionSmoke5;
	// 56 x 57
	TextureDef explosion5;
	// 64 x 56
	TextureDef tank_darkLarge;
	// 20 x 56
	TextureDef fenceYellow;
	// 56 x 56
	TextureDef tankBody_bigRed_outline;
	// 64 x 56
	TextureDef tankBody_darkLarge_outline;
	// 56 x 56
	TextureDef tank_bigRed;
	// 54 x 54
	TextureDef oilSpill_large;
	// 52 x 52
	TextureDef tankBody_bigRed;
	// 20 x 52
	TextureDef fenceRed;
	// 60 x 52
	TextureDef tankBody_darkLarge;
	// 49 x 50
	TextureDef explosionSmoke4;
	// 49 x 50
	TextureDef explosion4;
	// 52 x 48
	TextureDef wireCrooked;
	// 32 x 46
	TextureDef sandbagBeige_open;
	// 50 x 46
	TextureDef tank_sand;
	// 44 x 46
	TextureDef tankBody_sand_outline;
	// 50 x 46
	TextureDef tank_blue;
	// 44 x 46
	TextureDef tankBody_dark_outline;
	// 50 x 46
	TextureDef tank_green;
	// 32 x 46
	TextureDef sandbagBrown_open;
	// 50 x 46
	TextureDef tank_dark;
	// 46 x 46
	TextureDef tankBody_blue_outline;
	// 44 x 46
	TextureDef tankBody_green_outline;
	// 56 x 45
	TextureDef tracksLarge;
	// 56 x 45
	TextureDef tracksDouble;
	// 40 x 42
	TextureDef tankBody_sand;
	// 44 x 42
	TextureDef tankBody_red_outline;
	// 40 x 42
	TextureDef tankBody_green;
	// 40 x 42
	TextureDef tankBody_dark;
	// 50 x 42
	TextureDef tank_red;
	// 42 x 42
	TextureDef tankBody_blue;
	// 56 x 41
	TextureDef tracksSmall;
	// 40 x 40
	TextureDef treeGreen_small;
	// 40 x 40
	TextureDef treeBrown_small;
	// 40 x 38
	TextureDef tankBody_red;
	// 26 x 36
	TextureDef sandbagBrown;
	// 26 x 36
	TextureDef sandbagBeige;
	// 32 x 32
	TextureDef crateWood;
	// 32 x 32
	TextureDef crateMetal_side;
	// 32 x 32
	TextureDef crateWood_side;
	// 32 x 32
	TextureDef crateMetal;
	// 32 x 32
	TextureDef barricadeWood;
	// 32 x 32
	TextureDef barricadeMetal;
	// 26 x 30
	TextureDef treeGreen_twigs;
	// 26 x 30
	TextureDef treeBrown_twigs;
	// 28 x 28
	TextureDef barrelBlack_top;
	// 28 x 28
	TextureDef barrelGreen_top;
	// 28 x 28
	TextureDef barrelRed_top;
	// 28 x 28
	TextureDef barrelRust_top;
	// 42 x 25
	TextureDef shotRed;
	// 32 x 24
	TextureDef barrelGreen_side;
	// 32 x 24
	TextureDef barrelRust_side;
	// 29 x 24
	TextureDef shotLarge;
	// 32 x 24
	TextureDef barrelBlack_side;
	// 32 x 24
	TextureDef barrelRed_side;
	// 30 x 22
	TextureDef specialBarrel1_outline;
	// 74 x 21
	TextureDef wireStraight;
	// 34 x 20
	TextureDef tankBlue_barrel1_outline;
	// 32 x 20
	TextureDef specialBarrel2_outline;
	// 32 x 20
	TextureDef shotOrange;
	// 34 x 20
	TextureDef specialBarrel5_outline;
	// 34 x 20
	TextureDef tankDark_barrel1_outline;
	// 34 x 20
	TextureDef tankGreen_barrel1_outline;
	// 34 x 20
	TextureDef tankRed_barrel1_outline;
	// 34 x 20
	TextureDef tankSand_barrel1_outline;
	// 40 x 18
	TextureDef specialBarrel4_outline;
	// 26 x 18
	TextureDef specialBarrel1;
	// 18 x 18
	TextureDef oilSpill_small;
	// 36 x 18
	TextureDef specialBarrel3_outline;
	// 30 x 16
	TextureDef tankGreen_barrel1;
	// 34 x 16
	TextureDef tankGreen_barrel2_outline;
	// 34 x 16
	TextureDef tankRed_barrel3_outline;
	// 34 x 16
	TextureDef tankDark_barrel2_outline;
	// 34 x 16
	TextureDef tankDark_barrel3_outline;
	// 34 x 16
	TextureDef tankRed_barrel2_outline;
	// 30 x 16
	TextureDef tankDark_barrel1;
	// 34 x 16
	TextureDef tankBlue_barrel3_outline;
	// 34 x 16
	TextureDef tankSand_barrel3_outline;
	// 34 x 16
	TextureDef tankBlue_barrel2_outline;
	// 34 x 16
	TextureDef specialBarrel7_outline;
	// 30 x 16
	TextureDef tankBlue_barrel1;
	// 34 x 16
	TextureDef specialBarrel6_outline;
	// 34 x 16
	TextureDef tankSand_barrel2_outline;
	// 30 x 16
	TextureDef specialBarrel5;
	// 30 x 16
	TextureDef tankRed_barrel1;
	// 20 x 16
	TextureDef bulletSand2_outline;
	// 20 x 16
	TextureDef bulletGreen2_outline;
	// 30 x 16
	TextureDef tankSand_barrel1;
	// 34 x 16
	TextureDef tankGreen_barrel3_outline;
	// 28 x 16
	TextureDef specialBarrel2;
	// 20 x 16
	TextureDef bulletDark2_outline;
	// 20 x 16
	TextureDef bulletRed2_outline;
	// 20 x 16
	TextureDef bulletBlue2_outline;
	// 32 x 14
	TextureDef specialBarrel3;
	// 36 x 14
	TextureDef specialBarrel4;
	// 30 x 12
	TextureDef specialBarrel7;
	// 14 x 12
	TextureDef treeGreen_leaf;
	// 30 x 12
	TextureDef tankDark_barrel3;
	// 30 x 12
	TextureDef tankGreen_barrel3;
	// 30 x 12
	TextureDef tankRed_barrel2;
	// 30 x 12
	TextureDef tankDark_barrel2;
	// 30 x 12
	TextureDef shotThin;
	// 30 x 12
	TextureDef tankGreen_barrel2;
	// 30 x 12
	TextureDef tankSand_barrel3;
	// 30 x 12
	TextureDef tankSand_barrel2;
	// 18 x 12
	TextureDef bulletBlue1_outline;
	// 22 x 12
	TextureDef bulletSand3_outline;
	// 30 x 12
	TextureDef tankBlue_barrel3;
	// 18 x 12
	TextureDef bulletSand1_outline;
	// 22 x 12
	TextureDef bulletRed3_outline;
	// 22 x 12
	TextureDef bulletBlue3_outline;
	// 16 x 12
	TextureDef bulletSand2;
	// 18 x 12
	TextureDef bulletDark1_outline;
	// 16 x 12
	TextureDef bulletRed2;
	// 14 x 12
	TextureDef treeBrown_leaf;
	// 18 x 12
	TextureDef bulletRed1_outline;
	// 22 x 12
	TextureDef bulletGreen3_outline;
	// 30 x 12
	TextureDef specialBarrel6;
	// 30 x 12
	TextureDef tankBlue_barrel2;
	// 18 x 12
	TextureDef bulletGreen1_outline;
	// 16 x 12
	TextureDef bulletGreen2;
	// 16 x 12
	TextureDef bulletDark2;
	// 22 x 12
	TextureDef bulletDark3_outline;
	// 16 x 12
	TextureDef bulletBlue2;
	// 30 x 12
	TextureDef tankRed_barrel3;
	// 18 x 8
	TextureDef bulletDark3;
	// 18 x 8
	TextureDef bulletGreen3;
	// 18 x 8
	TextureDef bulletBlue3;
	// 14 x 8
	TextureDef bulletDark1;
	// 14 x 8
	TextureDef bulletGreen1;
	// 14 x 8
	TextureDef bulletSand1;
	// 18 x 8
	TextureDef bulletRed3;
	// 14 x 8
	TextureDef bulletBlue1;
	// 18 x 8
	TextureDef bulletSand3;
	// 14 x 8
	TextureDef bulletRed1;
};
// /tanks_1
class r_alp0p : public TextureFolder
{
public:	
	r_alp0p();
	
	r_hbzjo Smoke;
	r_i9ox9 Obstacles;
	r_5q2x0 Bullets;
	
};
// 
class TextureNames : public TextureFolder
{
public:	
	TextureNames();
	
	r_alp0p tanks_1;
	r_xhrer td;
	r_jr3fj tanks_2;
	r_ir8mj art;
	
};