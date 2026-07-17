#include "Camera.hpp"
#include "glm/gtc/matrix_transform.hpp"

camera::camera() {
	mEye = glm::vec3(0.0f, 0.0f, 1.0f);
	mViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	mUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
}
glm::mat4 camera::GetViewMatrix() const {
	return glm::lookAt(mEye, mViewDirection, mUpVector);
}
void camera::Zoom(float offset) {
	mEye.z = 1.0f-offset;
}
void camera::Xdir(float offset) {
	mEye.x = offset;
	mViewDirection.x = offset;
}
void camera::Ydir(float offset) {
	mEye.y = offset;
	mViewDirection.y = offset;
}