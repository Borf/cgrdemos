#include "TransformOrderDemo.h"
#include <blib/Renderer.h>
#include <blib/util/FileSystem.h>
#include <blib/Shader.h>
#include <blib/ResourceManager.h>
#include <blib/gl/Vertex.h>
#include <blib/Window.h>
#include <blib/SpriteBatch.h>
#include <blib/Math.h>
#include <blib/Color.h>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
	blib::util::FileSystem::registerHandler(new blib::util::PhysicalFileSystemHandler("."));
	blib::util::FileSystem::registerHandler(new blib::util::PhysicalFileSystemHandler("../blib"));
	(new TransformOrderDemo())->start();
	return -1;
}



TransformOrderDemo::TransformOrderDemo()
{
	appSetup.renderer = blib::AppSetup::GlRenderer;
	appSetup.border = true;
	appSetup.title = "TransformOrder Demo";
	appSetup.vsync = true;
	appSetup.window.setWidth(1280);
	appSetup.window.setHeight(720);
}


TransformOrderDemo::~TransformOrderDemo()
{
}

void TransformOrderDemo::init()
{
	shader = resourceManager->getResource<blib::Shader>("simplecolor3d");
	shader->bindAttributeLocation("a_position", 0);
	shader->bindAttributeLocation("a_color", 1);
	shader->setUniformName(Uniforms::projectionMatrix, "projectionMatrix", blib::Shader::Mat4);
	shader->setUniformName(Uniforms::cameraMatrix, "cameraMatrix", blib::Shader::Mat4);
	shader->setUniformName(Uniforms::modelMatrix, "modelMatrix", blib::Shader::Mat4);
	shader->finishUniformSetup();
	renderer->renderState.activeShader = shader;
	renderer->renderState.depthTest = true;


	font = resourceManager->getResource<blib::Font>("tahoma64");

	rot = 0;


	actions.push_back(Action("translate(2,0,2)", [](const glm::mat4& mat) { return glm::translate(mat, glm::vec3(2, 0, 2)); }));
	actions.push_back(Action("rotate(rot, 0,1,0)", [this](const glm::mat4& mat) { return glm::rotate(mat, rot, glm::vec3(0, 1, 0)); }));
	actions.push_back(Action("translate(0,0,1)", [](const glm::mat4& mat) { return glm::translate(mat, glm::vec3(0, 0, 1)); }));
	actions.push_back(Action("rotate(-rot, 0,1,0)", [this](const glm::mat4& mat) { return glm::rotate(mat, -rot, glm::vec3(0, 1, 0)); }, false));

}

void TransformOrderDemo::update(double elapsedTime)
{
	if (keyState.isPressed(blib::Key::ESC))
		running = false;

	rot += 90 * (float)elapsedTime;

	
	if (mouseState.leftButton && !prevMouseState.leftButton)
	{
		dragIndex = -1;
		for (size_t i = 0; i < actions.size(); i++)
			if (mouseState.position.y > 150 + i * 50 && mouseState.position.y < 150 + (i + 1) * 50)
			{
				dragIndex = i;
				dragOffset = mouseState.position.y - (150 + i * 50);
			}
		dragIndex = glm::clamp(dragIndex, -1, (int)actions.size() - 1);
		mouseClickPos = mouseState.position;
	}
	if (!mouseState.leftButton && prevMouseState.leftButton && dragIndex != -1)
	{
		if (glm::distance(glm::vec2(mouseClickPos), glm::vec2(mouseState.position)) < 2)
		{
			actions[dragIndex].enabled = !actions[dragIndex].enabled;
		}
		else
		{
			int newIndex = (mouseState.position.y - 150) / 50;
			newIndex = glm::clamp(newIndex, 0, (int)actions.size() - 1);
			std::swap(actions[dragIndex], actions[newIndex]);
		}
		dragIndex = -1;
	}

	prevMouseState = mouseState;
	prevKeyState = keyState;
}



void TransformOrderDemo::draw()
{
	float s = 1;
	glm::vec4 color(0, 0, 0, 1);
	static std::vector<blib::VertexP3C4> boxVerts;
	if (boxVerts.empty())
	{
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, -s, -s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, -s, s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, -s, s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, s, s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, s, s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, s, -s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, s, -s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, -s, -s), color));

		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, -s, -s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, -s, s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, -s, s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, s, s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, s, s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, s, -s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, s, -s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, -s, -s), color));

		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, s, -s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, s, -s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, s, s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, s, s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, -s, -s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, -s, -s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(s, -s, s), color));
		boxVerts.push_back(blib::VertexP3C4(glm::vec3(-s, -s, s), color));
	}
	static std::vector<blib::VertexP3C4> axis;
	if (axis.empty())
	{
		axis.push_back(blib::VertexP3C4(glm::vec3(-1000, 0, 0), glm::vec4(1, 0, 0, 1)));
		axis.push_back(blib::VertexP3C4(glm::vec3(1000, 0, 0), glm::vec4(1, 0, 0, 1)));
		axis.push_back(blib::VertexP3C4(glm::vec3(0, -1000, 0), glm::vec4(0, 1, 0, 1)));
		axis.push_back(blib::VertexP3C4(glm::vec3(0, 1000, 0), glm::vec4(0, 1, 0, 1)));
		axis.push_back(blib::VertexP3C4(glm::vec3(0, 0, -1000), glm::vec4(0, 0, 1, 1)));
		axis.push_back(blib::VertexP3C4(glm::vec3(0, 0, 1000), glm::vec4(0, 0, 1, 1)));
	}



	renderer->clear(glm::vec4(1, 1, 1, 1), blib::Renderer::Color | blib::Renderer::Depth);

	shader->setUniform(Uniforms::projectionMatrix, glm::perspective(45.0f, (float)window->getWidth()/window->getHeight(), 0.1f, 25.0f));
	shader->setUniform(Uniforms::cameraMatrix, glm::lookAt(glm::vec3(6.75f, 6.25f, 7.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));

	glm::mat4 mat;
	for (size_t i = 0; i < actions.size(); i++)
		if (actions[i].enabled)
			mat = actions[i].func(mat);
	shader->setUniform(Uniforms::modelMatrix, mat);

	renderer->drawLines(boxVerts, 10.0f);
	shader->setUniform(Uniforms::modelMatrix, glm::mat4());
	renderer->drawLines(axis, 1.0f);



	spriteBatch->begin();

	for (size_t i = 0; i < actions.size(); i++)
	{
		if (i != dragIndex)
			drawText(glm::vec2(10, 150 + i * 50), actions[i].text, actions[i].enabled ? blib::Color::white : blib::Color::fireEngineRed);
		if (i == dragIndex)
			drawText(glm::vec2(10, mouseState.position.y - dragOffset), actions[i].text, actions[i].enabled ? blib::Color::white : blib::Color::fireEngineRed);
	}


	spriteBatch->draw(font, "https://github.com/Borf/cgrdemos/", blib::math::easyMatrix(glm::vec2(window->getWidth()-275, 5), 0, 0.33f), blib::Color::black);

	spriteBatch->end();

}

void TransformOrderDemo::drawText(const glm::vec2 &pos, const std::string &text, const glm::vec4 &color) const
{
	for (int x = -2; x <= 2; x++)
		for (int y = -2; y <= 2; y++)
			spriteBatch->draw(font, text, blib::math::easyMatrix(pos + glm::vec2(x, y)), glm::vec4(0, 0, 0, 1));
	spriteBatch->draw(font, text, blib::math::easyMatrix(pos), color);

}