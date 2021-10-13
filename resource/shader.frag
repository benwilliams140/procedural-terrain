#version 330 

in vec3 vertexPos;
in vec3 vertexNormal;
in vec3 lightVector;
in vec2 f_uv;

uniform sampler2D texture_map;

uniform vec3 ambient_color = vec3(0.75);
uniform vec3 diffuse_color = vec3(0.1, 0.5, 0.05);
uniform vec3 specular_color = vec3(0.90, 0.90, 0.55);

uniform float shine = 5.0f;

uniform float coefA = 1;
uniform float coefD = 1;
uniform float coefS = 0.1;

void main(void)
{
	vec3 L = normalize(lightVector);
	vec3 V = normalize(-vertexPos);
	vec3 N = normalize(vertexNormal);
	vec3 R = -L + 2 * dot(L, N) * N;

	vec3 ambient, diffuse, specular;

	ambient = coefA * ambient_color;
	diffuse = coefD * diffuse_color * max(dot(L, N), 0.0f);
	specular = coefS * specular_color * pow(max(dot(R, V), 0.0f), shine);

	vec4 color = texture(texture_map, f_uv);
	color *= vec4((ambient+diffuse+specular), 1.0);
	gl_FragColor = color;
}
