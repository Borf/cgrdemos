#pragma once

#include <blib/App.h>
#include <blib/math/Ray.h>
#include <functional>

namespace blib { class Shader; class Font; }

class Action
{
public:
	std::function<glm::mat4(const glm::mat4&)> func;
	std::string text;
	bool enabled;

	Action(const std::string &text, const std::function<glm::mat4(const glm::mat4&)>& func, bool enabled = true)
	{
		this->text = text;
		this->func = func;
		this->enabled = enabled;
	}
};



class TransformOrderDemo : public blib::App
{
	blib::Shader* shader;
	enum class Uniforms
	{
		projectionMatrix,
		modelMatrix,
		cameraMatrix
	};
	float rot;
	float fov;

	blib::Font* font;

	blib::KeyState prevKeyState;
	blib::MouseState prevMouseState;
	glm::ivec2 mouseClickPos;
	std::vector<Action> actions;
	int dragIndex = -1;
	float dragOffset = 0;
public:
	TransformOrderDemo();
	~TransformOrderDemo();


	virtual void init() override;
	virtual void update(double elapsedTime) override;
	virtual void draw() override;
	void drawText(const glm::vec2 &pos, const std::string &text, const glm::vec4 &color) const;
};

