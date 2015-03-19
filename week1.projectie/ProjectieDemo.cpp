#include "ProjectieDemo.h"
#include <blib/Renderer.h>
#include <blib/util/FileSystem.h>
#include <blib/Shader.h>
#include <blib/ResourceManager.h>
#include <blib/gl/Vertex.h>

#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
	blib::util::FileSystem::registerHandler(new blib::util::PhysicalFileSystemHandler("."));
	blib::util::FileSystem::registerHandler(new blib::util::PhysicalFileSystemHandler("../blib"));
	(new ProjectieDemo())->start();
	return -1;
}



ProjectieDemo::ProjectieDemo()
{
	appSetup.renderer = blib::AppSetup::GlRenderer;
	appSetup.border = false;
	appSetup.title = "Projectie Demo";
	appSetup.vsync = true;
	appSetup.window.setWidth(1280);
	appSetup.window.setHeight(720);
}


ProjectieDemo::~ProjectieDemo()
{
}

void ProjectieDemo::init()
{
	shader = resourceManager->getResource<blib::Shader>("simplecolor3d");
	shader->bindAttributeLocation("a_position", 0);
	shader->bindAttributeLocation("a_color", 1);
	shader->setUniformName(Uniforms::projectionMatrix, "projectionMatrix", blib::Shader::Mat4);
	shader->setUniformName(Uniforms::cameraMatrix, "cameraMatrix", blib::Shader::Mat4);
	shader->setUniformName(Uniforms::modelMatrix, "modelMatrix", blib::Shader::Mat4);
	shader->finishUniformSetup();
	renderer->renderState.activeShader = shader;

	rot = 0;
	ortho = false;
	fov = 90.0f;
}

void ProjectieDemo::update(double elapsedTime)
{
	if (keyState.isPressed(blib::Key::ESC))
		running = false;


	if (keyState.isPressed(blib::Key::O) && !prevKeyState.isPressed(blib::Key::O))
		ortho = !ortho;
	if (keyState.isPressed(blib::Key::MINUS))
		fov -= elapsedTime * 45;
	if (keyState.isPressed(blib::Key::PLUS))
		fov += elapsedTime * 45;


	rot += 90 * (float)elapsedTime;

	prevKeyState = keyState;
}

void ProjectieDemo::draw()
{
	float s = 1;
	glm::vec4 color(0, 0, 0, 1);
	std::vector<blib::VertexP3C4> boxVerts;
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




	renderer->clear(glm::vec4(1, 1, 1, 1), blib::Renderer::Color | blib::Renderer::Depth);

	renderer->setViewPort(0, 0, 640, 720);
	if (ortho)
		shader->setUniform(Uniforms::projectionMatrix, glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -50.0f, 50.0f));
	else
		shader->setUniform(Uniforms::projectionMatrix, glm::perspective(fov, 640 / 720.0f, 0.1f, 100.0f));
	shader->setUniform(Uniforms::cameraMatrix, glm::lookAt(glm::vec3(1.75f, 2.25f, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	shader->setUniform(Uniforms::modelMatrix, glm::rotate(glm::mat4(), rot, glm::vec3(0,1,0)));
	renderer->drawLines(boxVerts, 10.0f);


	renderer->setViewPort(640, 0, 640, 720);
	shader->setUniform(Uniforms::projectionMatrix, glm::perspective(70.0f, 640 / 720.0f, 0.1f, 100.0f));
	shader->setUniform(Uniforms::cameraMatrix, glm::lookAt(glm::vec3(3.75f, 4.25f, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	shader->setUniform(Uniforms::modelMatrix, glm::rotate(glm::mat4(), rot, glm::vec3(0, 1, 0)));
	renderer->drawLines(boxVerts, 10.0f);



	renderer->setViewPort(0, 0, 1280, 720);

}