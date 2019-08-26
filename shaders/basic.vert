#version 330

uniform mat4 projection, model, view;
uniform mat3 normalMatrix;

in vec3 position;
in vec3 normal;
out vec3 normalFrag;

void main()
{
  // Transform matrix to viewspace
  normalFrag = normalMatrix * normal;
	// Transform position from pixel coordinates to clipping coordinates
	gl_Position = projection * view * model * vec4(position, 1.0);
}

