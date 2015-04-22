#include "ProjectieDemo.h"
#include <blib/Renderer.h>
#include <blib/util/FileSystem.h>
#include <blib/Shader.h>
#include <blib/ResourceManager.h>
#include <blib/gl/Vertex.h>
#include <blib/Window.h>

#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char* argv[])
{
	blib::util::FileSystem::registerHandler(new blib::util::PhysicalFileSystemHandler("."));
	blib::util::FileSystem::registerHandler(new blib::util::PhysicalFileSystemHandler("../blib"));
	(new ProjectieDemo())->start();
	return -1;
}



ProjectieDemo::ProjectieDemo() : 
	topleft(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), 
	bottomright(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), 
	topright(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)), 
	bottomleft(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)),
	center(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0))

{
	appSetup.renderer = blib::AppSetup::GlRenderer;
	appSetup.border = true;
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
	renderer->renderState.depthTest = true;

	rot = 0;
	ortho = false;
	fov = 60.0f;
	nearPlane = 0.1f;
	farPlane = 7.5f;
}

void ProjectieDemo::update(double elapsedTime)
{
	if (keyState.isPressed(blib::Key::ESC))
		running = false;


	if (keyState.isPressed(blib::Key::O) && !prevKeyState.isPressed(blib::Key::O))
		ortho = !ortho;
	if (keyState.isPressed(blib::Key::MINUS))
		fov -= (float)elapsedTime * 45;
	if (keyState.isPressed(blib::Key::PLUS))
		fov += (float)elapsedTime * 45;


	rot += 90 * (float)elapsedTime;


	glm::mat4 cam = glm::lookAt(glm::vec3(1.75f, 2.25f, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 proj;
	if (ortho)
		proj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -50.0f, 50.0f);
	else
		proj = glm::perspective(fov, 640 / 720.0f, 0.1f, 100.0f);


	renderer->setViewPort(0, 0, window->getWidth()/2, window->getHeight());
	renderer->unproject(glm::vec2(0, 0), NULL, &topleft, cam, proj);
	renderer->unproject(glm::vec2(window->getWidth() / 2, 0), NULL, &topright, cam, proj);
	renderer->unproject(glm::vec2(0, window->getHeight()), NULL, &bottomleft, cam, proj);
	renderer->unproject(glm::vec2(window->getWidth() / 2, window->getHeight()), NULL, &bottomright, cam, proj);
	renderer->unproject(glm::vec2(window->getWidth() / 2 / 2, window->getHeight() / 2), NULL, &center, cam, proj);
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

	std::vector<blib::VertexP3C4> axis;
	axis.push_back(blib::VertexP3C4(glm::vec3(-1000, 0, 0), glm::vec4(1, 0, 0, 1)));
	axis.push_back(blib::VertexP3C4(glm::vec3(1000, 0, 0), glm::vec4(1, 0, 0, 1)));
	axis.push_back(blib::VertexP3C4(glm::vec3(0, -1000, 0), glm::vec4(0, 1, 0, 1)));
	axis.push_back(blib::VertexP3C4(glm::vec3(0, 1000, 0), glm::vec4(0, 1, 0, 1)));
	axis.push_back(blib::VertexP3C4(glm::vec3(0, 0, -1000), glm::vec4(0, 0, 1, 1)));
	axis.push_back(blib::VertexP3C4(glm::vec3(0, 0, 1000), glm::vec4(0, 0, 1, 1)));




	renderer->clear(glm::vec4(1, 1, 1, 1), blib::Renderer::Color | blib::Renderer::Depth);

	renderer->setViewPort(0, 0, window->getWidth() / 2, window->getHeight());
	if (ortho)
		shader->setUniform(Uniforms::projectionMatrix, glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -50.0f, 50.0f));
	else
		shader->setUniform(Uniforms::projectionMatrix, glm::perspective(fov, 640 / 720.0f, nearPlane, farPlane));
	shader->setUniform(Uniforms::cameraMatrix, glm::lookAt(glm::vec3(1.75f, 2.25f, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	shader->setUniform(Uniforms::modelMatrix, glm::rotate(glm::mat4(), rot, glm::vec3(0, 1, 0)));
	renderer->drawLines(boxVerts, 10.0f);
	shader->setUniform(Uniforms::modelMatrix, glm::mat4());
	renderer->drawLines(axis, 1.0f);



	renderer->setViewPort(window->getWidth()/2, 0, window->getWidth() / 2, window->getHeight());
	shader->setUniform(Uniforms::projectionMatrix, glm::perspective(70.0f, 640 / 720.0f, 0.1f, 100.0f));
	shader->setUniform(Uniforms::cameraMatrix, glm::lookAt(glm::vec3(3.75f, 4.25f, 4), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(), rot*0.1f, glm::vec3(0, 1, 0)));
	shader->setUniform(Uniforms::modelMatrix, glm::rotate(glm::mat4(), rot, glm::vec3(0, 1, 0)));
	renderer->drawLines(boxVerts, 10.0f);


	std::vector<blib::VertexP3C4> frustumVerts;
	color.r = 1;

	float f = farPlane;
	float n = nearPlane;
	if (ortho)
	{
		f = 50;
		n = -50;
	}

	frustumVerts.push_back(blib::VertexP3C4(topleft.origin + topleft.dir * n, color));
	frustumVerts.push_back(blib::VertexP3C4(topright.origin + topright.dir * n, color));
	frustumVerts.push_back(blib::VertexP3C4(topright.origin + topright.dir * n, color));
	frustumVerts.push_back(blib::VertexP3C4(bottomright.origin + bottomright.dir * n, color));
	frustumVerts.push_back(blib::VertexP3C4(bottomright.origin + bottomright.dir * n, color));
	frustumVerts.push_back(blib::VertexP3C4(bottomleft.origin + bottomleft.dir * n, color));
	frustumVerts.push_back(blib::VertexP3C4(bottomleft.origin + bottomleft.dir * n, color));
	frustumVerts.push_back(blib::VertexP3C4(topleft.origin + topleft.dir * n, color));

	frustumVerts.push_back(blib::VertexP3C4(topleft.origin + topleft.dir * f, color));
	frustumVerts.push_back(blib::VertexP3C4(topright.origin + topright.dir * f, color));
	frustumVerts.push_back(blib::VertexP3C4(topright.origin + topright.dir * f, color));
	frustumVerts.push_back(blib::VertexP3C4(bottomright.origin + bottomright.dir * f, color));
	frustumVerts.push_back(blib::VertexP3C4(bottomright.origin + bottomright.dir * f, color));
	frustumVerts.push_back(blib::VertexP3C4(bottomleft.origin + bottomleft.dir * f, color));
	frustumVerts.push_back(blib::VertexP3C4(bottomleft.origin + bottomleft.dir * f, color));
	frustumVerts.push_back(blib::VertexP3C4(topleft.origin + topleft.dir * f, color));


	frustumVerts.push_back(blib::VertexP3C4(topleft.origin + topleft.dir * n, color));
	frustumVerts.push_back(blib::VertexP3C4(topleft.origin + topleft.dir * f, color));

	frustumVerts.push_back(blib::VertexP3C4(topright.origin + topright.dir * n, color));
	frustumVerts.push_back(blib::VertexP3C4(topright.origin + topright.dir * f, color));

	frustumVerts.push_back(blib::VertexP3C4(bottomleft.origin + bottomleft.dir * n, color));
	frustumVerts.push_back(blib::VertexP3C4(bottomleft.origin + bottomleft.dir * f, color));

	frustumVerts.push_back(blib::VertexP3C4(bottomright.origin + bottomright.dir * n, color));
	frustumVerts.push_back(blib::VertexP3C4(bottomright.origin + bottomright.dir * f, color));

	shader->setUniform(Uniforms::modelMatrix, glm::mat4());
	renderer->drawLines(axis, 1.0f);
	renderer->drawLines(frustumVerts, 7.0f);



	renderer->setViewPort(0, 0, window->getWidth(), window->getHeight());

}
