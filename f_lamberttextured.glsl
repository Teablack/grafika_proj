
#version 330

uniform sampler2D tex;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
in float nl;
in float rv;
in vec2 cord;

void main(void) {
	float ambient = 0.1f;
	pixelColor = vec4(ambient*texture(tex,cord).rgb,1.0f)	+	vec4(nl * texture(tex,cord).rgb,1.0f)		+	vec4(rv,rv,rv,0);
	//                       tu ambient									 //diffuse                            //specular
}
