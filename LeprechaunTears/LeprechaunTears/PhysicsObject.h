#include <glm.hpp>

class PhysicsObject{
public:
	PhysicsObject();
	~PhysicsObject();
	void update();
	void render();
	void addForce(glm::vec3);
protected:
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float mass;
	void addDrag();
};