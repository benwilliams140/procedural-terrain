#version 330

in vec3 vertex;
in vec3 normal;
in vec2 uv;

uniform mat4 projection_mat;
uniform mat4 view_mat;
uniform mat4 world_mat;

uniform vec3 lightPos = vec3(0, 100, 0);

out vec3 vertexPos;
out vec3 vertexNormal;
out vec3 lightVector;
out vec2 f_uv;

void main(void)
{
	vec4 vtxPos = view_mat*world_mat*vec4(vertex, 1);
	vertexPos = vec3(vtxPos);

	vec4 vtxNorm = transpose(inverse(view_mat*world_mat))*vec4(normal, 1);
	vertexNormal = vec3(vtxNorm);

	vec4 vtxLight = view_mat*world_mat*vec4(lightPos, 1.0);
	lightVector = vec3(vtxLight) - vertexPos;

	f_uv = uv;

	gl_Position = projection_mat*vtxPos;
}