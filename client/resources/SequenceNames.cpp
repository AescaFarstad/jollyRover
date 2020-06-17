#include <SequenceNames.h>

SequenceNames::SequenceNames(TextureNames& textures)
{
	explosion.name = "explosion";
	explosion.frames = {
		textures.tanks_2.explosion1,
		textures.tanks_2.explosion2,
		textures.tanks_2.explosion3,
		textures.tanks_2.explosion4,
		textures.tanks_2.explosion5
		};
		
	explosionSmoke.name = "explosionSmoke";
	explosionSmoke.frames = {
		textures.tanks_2.explosionSmoke1,
		textures.tanks_2.explosionSmoke2,
		textures.tanks_2.explosionSmoke3,
		textures.tanks_2.explosionSmoke4,
		textures.tanks_2.explosionSmoke5
		};
		
	greenPathDash.name = "greenPathDash";
	greenPathDash.frames = {
		textures.art.path.green.b1,
		textures.art.path.green.b2,
		textures.art.path.green.b3
		};
		
	greenPathDot.name = "greenPathDot";
	greenPathDot.frames = {
		textures.art.path.green.s1,
		textures.art.path.green.s2,
		textures.art.path.green.s3
		};
		
	redPathDash.name = "redPathDash";
	redPathDash.frames = {
		textures.art.path.red.b1,
		textures.art.path.red.b2,
		textures.art.path.red.b3,
		textures.art.path.red.b4
		};
		
	redPathDot.name = "redPathDot";
	redPathDot.frames = {
		textures.art.path.red.s1,
		textures.art.path.red.s2,
		textures.art.path.red.s3
		};
		
	grayPathDash.name = "grayPathDash";
	grayPathDash.frames = {
		textures.art.path.gray.b1,
		textures.art.path.gray.b2,
		textures.art.path.gray.b3
		};
		
	grayPathDot.name = "grayPathDot";
	grayPathDot.frames = {
		textures.art.path.gray.s1,
		textures.art.path.gray.s2,
		textures.art.path.gray.s3
		};
		
	fire.name = "fire";
	fire.frames = {
		textures.td.fire2,
		textures.td.fire3,
		textures.td.fire4
		};
}