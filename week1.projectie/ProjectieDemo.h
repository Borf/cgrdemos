#pragma once

#include <blib/App.h>

namespace blib { class Shader; }

class ProjectieDemo : public blib::App
{
	blib::Shader* shader;
	enum class Uniforms
	{
		projectionMatrix,
		modelMatrix,
		cameraMatrix
	};
	float rot;

	bool ortho;
	float fov;

	blib::KeyState prevKeyState;
public:
	ProjectieDemo();
	~ProjectieDemo();


	virtual void init() override;
	virtual void update(double elapsedTime) override;
	virtual void draw() override;

};

