attribute vec3 a_position;
attribute vec4 a_color;

uniform mat4 projectionMatrix;
uniform mat4 cameraMatrix;
uniform mat4 modelMatrix;

varying vec4 color;

void main()
{
	color = a_color;

	gl_Position = projectionMatrix * cameraMatrix * modelMatrix * vec4(a_position,1.0);
}