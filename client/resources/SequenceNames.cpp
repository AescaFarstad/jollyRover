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
}