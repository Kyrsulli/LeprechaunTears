#include <glm.hpp>

class PhysicsObject{
public:
	PhysicsObject();
	~PhysicsObject();
	void update();
	void render();
private:
	glm::vec3 position;
	glm::vec3 velocity;
	void addDrag();
};