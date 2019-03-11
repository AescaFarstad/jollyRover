#include <PlayerTest.h>



PlayerTest::PlayerTest()
{
	x = 0;
	y = 0;
	speedX = 0;
	speedY = 0;

	buttonDown_Backward = false;
	buttonDown_Forward = false;
	buttonDown_Left = false;
	buttonDown_Right = false;
}


PlayerTest::~PlayerTest()
{
}

void PlayerTest::deserialize(SerializationStream & stream)
{
	Serializer::read(x, stream);
	Serializer::read(y, stream);
	Serializer::read(speedX, stream);
	Serializer::read(speedY, stream);

	Serializer::read(buttonDown_Left, stream);
	Serializer::read(buttonDown_Right, stream);
	Serializer::read(buttonDown_Forward, stream);
	Serializer::read(buttonDown_Backward, stream);

	Serializer::read(login, stream);

	Serializer::readVector(activeCars, stream);
}

void PlayerTest::serialize(SerializationStream & stream) const
{
	Serializer::write(x, stream);
	Serializer::write(y, stream);
	Serializer::write(speedX, stream);
	Serializer::write(speedY, stream);
				
	Serializer::write(buttonDown_Left, stream);
	Serializer::write(buttonDown_Right, stream);
	Serializer::write(buttonDown_Forward, stream);
	Serializer::write(buttonDown_Backward, stream);
				
	Serializer::write(login, stream);

	Serializer::writeVector(activeCars, stream);
}
