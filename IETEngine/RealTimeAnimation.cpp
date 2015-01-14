
#include <math.h> 
#include "PhysicsController.h"
 


float rot_speed = 50.0f; // 50 radians per second
int totalAnimationTime;
 
int main(int argc, char* argv[])
{ 
	auto controller = new PhysicsController();

	controller->Init(argc, argv);

	controller->Run();

	return 0;
}
