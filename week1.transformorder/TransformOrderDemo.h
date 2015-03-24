#pragma once

#include <blib/App.h>
#include <blib/math/Ray.h>

namespace blib { class Shader; }

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

	bool ortho;
	float fov;
	float nearPlane;
	float farPlane;

	blib::KeyState prevKeyState;

	blib::math::Ray topleft;
	blib::math::Ray topright;
	blib::math::Ray bottomleft;
	blib::math::Ray bottomright;
	blib::math::Ray center;


public:
	TransformOrderDemo();
	~TransformOrderDemo();


	virtual void init() override;
	virtual void update(double elapsedTime) override;
	virtual void draw() override;

};

