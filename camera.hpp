#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

class camera {
public:
	//Default constructor
	camera();
	//The ultimate View Matrix we will create and return in order to create viewspace
	glm::mat4 GetViewMatrix() const;

	void Zoom(float offset);
	void Xdir(float offset);
	void Ydir(float offset);

private:
	glm::vec3 mEye;
	glm::vec3 mViewDirection;
	glm::vec3 mUpVector;
};
#endif