#include "animation.hpp"

Animation::Animation(int start, int end, int width, int height, int framesRow, float duration)
	: startFrame(start),
	endFrame(end),
	frameWidth(width),
	frameHeight(height),
	framesPerRow(framesRow),
	frameDuration(duration),
	currentFrame(start),
	accumulator(0.0f),
	isPlaying(true)
{}
void Animation::update(float deltaTime) {
	if (!isPlaying) return;
	
	accumulator += deltaTime;

	while (accumulator >= frameDuration) {
		currentFrame++;
		if (currentFrame >= endFrame) {
			currentFrame = startFrame;
		}
		accumulator -= frameDuration;
	}
}
void Animation::reset() {
	currentFrame = startFrame;
	accumulator = 0.0f;
}
void AnimationSystem::addAnimation(const Animation& animation) {
	animations.push_back(animation);
}
void AnimationSystem::update(float deltaTime) {
	for (auto& anim : animations) {
		anim.update(deltaTime);
	}
}
Animation& AnimationSystem::getAnimation(size_t index) {
	return animations.at(index);
}