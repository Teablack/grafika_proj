/*
#version 330
uniform sampler2D tex;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
in vec2 i_tc;
in vec4 nl;
in float rv;

void main(void) {
	
	vec4 color=texture(tex,i_tc);
	pixelColor= vec4(color.rgb*nl,color.a)+vec4(rv,rv,rv,0);
}
*/
#version 330

uniform sampler2D tex;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela
in float nl;
in float rv;
in vec2 cord;

void main(void) {
	
	pixelColor = vec4(nl * texture(tex,cord).rgb,1.0f)+vec4(rv,rv,rv,0); //Wyliczenie modelu oświetlenia (bez ambient)

}
