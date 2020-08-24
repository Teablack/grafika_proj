

#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lp;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec2 texCoord; //kolor wierzchołka
in vec4 normal; //wektor normalny wierzchołka w przestrzeni modelu

out float nl;
out float rv;
out vec2 cord;

void main(void) {
    
    vec4 l = normalize(V * (lp - M * vertex));//znormalizowany wektor do światła w przestrzeni oka
    vec4 n = normalize(V * M * normal);//znormalizowany wektor normalny w przestrzeni oka
    vec4 r = reflect(-l, n); //Wektor odbity
    vec4 v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); //Wektor do obserwatora w przestrzeni oka

    nl = clamp(dot(n, l),0,1); //Kosinus kąta pomiędzy wektorami n i l.
    rv = pow(clamp(dot(r, v), 0, 1), 25); // Kosinus kąta pomiędzy wektorami r i v podniesiony do 25 potęgi
    cord=texCoord;
    
    gl_Position=P*V*M*vertex;
}
