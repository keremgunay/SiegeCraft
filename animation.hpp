#pragma once
#include <vector>

struct Animation {
	int startFrame = 0;
	int endFrame = 0;
	int currentFrame = 0;
	int frameWidth = 128;
	int frameHeight = 128;
	int framesPerRow = 4;
	float frameDuration = 1.0f / 12.0f;
	float accumulator = 0.0f;
	bool isPlaying = true;

	void update(float deltaTime);
	void reset();

	Animation(int start, int end, int width, int height, int framesRow, float duration);
};
class AnimationSystem {
public:
	void addAnimation(const Animation& animation);

	void update(float deltaTime);

	Animation& getAnimation(size_t index);

	size_t count() const { return animations.size(); }

private:
	std::vector<Animation> animations;
};