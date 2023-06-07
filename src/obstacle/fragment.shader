#version 330

in vec2 TexCoord;
out vec4 FragColor;
in vec3 ourColor;

void main( ) 
{
	float y = TexCoord.x;
	
	vec4 white = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);

	float step1 = 0.0;
	float step2 = 0.1;
	float step3 = 0.8;
	
	vec4 color = mix(yellow, white, smoothstep(step1, step2, y));
	color = mix(color, yellow, smoothstep(step2, step3, y));

	gl_FragColor = color;
}