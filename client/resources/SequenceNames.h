#pragma once
#include <TextureNames.h>
#include <vector>
 
class SequenceDef 
{	
public:	
	std::string name;
	std::vector<TextureDef> frames;
};

class SequenceNames 
{
public:	
	SequenceNames(TextureNames& textures);
	
	SequenceDef explosion;
	SequenceDef explosionSmoke;
	SequenceDef greenPathDash;
	SequenceDef greenPathDot;
	SequenceDef redPathDash;
	SequenceDef redPathDot;
	SequenceDef grayPathDash;
	SequenceDef grayPathDot;
	
};