#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform int isFlying;

void main()
{
	vec4 color = texture(texture1, TexCoord);    // black color
    if (isFlying == 1){

        float step1 = 0.4;
        float step2 = 0.6;

	    vec4 white = vec4(1.0, 1.0, 0.0, 0.1);
        float d = distance(TexCoord, vec2(0.5, 0.5));
        
	    color = mix(color, white, smoothstep(step1, step2, d));
        if (d > 0.5) 
        {
            discard;
        }
    }

    else{
        if (color.a < 1.0){   
		    discard;
	    }
    }
    FragColor = color;	
}