/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/


#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
//#include "ziemia2.c"
#include "pien.c"
#include "ziemia3.c"


GLuint ziemia_tex,
pien_tex;
float	stopnie = 90.0f,  // na potem: zrobic jedna jednostke , usunac zmienne stopnie 
stopnie2 = 90.0f,
angle = glm::radians(stopnie),
angle2 = glm::radians(stopnie2),
radius = 5.0f;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


//Funkcja wczytująca teksturę 
GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());



	return tex;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod) {

	if (action == GLFW_PRESS) {

		if (key == GLFW_KEY_LEFT) {

			stopnie = stopnie + 5;
			angle = glm::radians(stopnie);

		}
		if (key == GLFW_KEY_RIGHT) {
			stopnie = stopnie - 5;
			angle = glm::radians(stopnie);
		}
		if (key == GLFW_KEY_UP) {
			stopnie2 = stopnie2 - 5;
			angle2 = glm::radians(stopnie2);
		}
		if (key == GLFW_KEY_DOWN) {
			stopnie2 = stopnie2 + 5;
			angle2 = glm::radians(stopnie2);
		}
		if (key == GLFW_KEY_W) {
			radius--;
		}
		if (key == GLFW_KEY_S) {
			radius++;
		}

	}
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();

	//Wczytanie i import obrazka – w initOpenGLProgram
	ziemia_tex = readTexture("ziemia_tex128.png");
	pien_tex = readTexture("kora128_256.png");
	glClearColor(0, 0.8f, 1, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetKeyCallback(window, key_callback);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float x, float y, float z, float height, int first_level_branch_count) {


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	glm::mat4 V = glm::lookAt(glm::vec3(x, y, z),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania

	spTextured->use();
	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V));

	//ziemia

	glm::mat4 M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	M = glm::translate(M, glm::vec3(0, -2.0f, 0));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, ziemia3Positions);
	glEnableVertexAttribArray(spTextured->a("texCoord"));

	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, ziemia3Texels);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, ziemia_tex);
	glUniform1i(spTextured->u("tex"), 0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glDrawArrays(GL_TRIANGLES, 0, ziemia3Vertices);
	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("textCoord"));


	//pień
	glm::mat4 Pien = glm::mat4(1.0f);

	Pien = glm::translate(Pien, glm::vec3(0, height * 7.0 - 1.9, 0));
	Pien = glm::scale(Pien, glm::vec3(height, height, height));

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(Pien));
	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, pienPositions);
	glEnableVertexAttribArray(spTextured->a("texCoord"));

	float pien_tex_copy[2292];
	memcpy(pien_tex_copy, pienTexels, sizeof(pien_tex_copy));
	for (int i = 0; i < 2292; i++) {
		pien_tex_copy[i] *= pien_tex_copy[i] * 5.0f;
	}

	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, pien_tex_copy);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, pien_tex);
	glUniform1i(spTextured->u("tex"), 0);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

	glDrawArrays(GL_TRIANGLES, 0, pienVertices);
	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("textCoord"));

	//gałęzie

	for (int i = 0; i < first_level_branch_count; i++) {


	}




	glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno
	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów
	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora
	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}
	initOpenGLProgram(window); //Operacje inicjujące


	//Główna pętla

	float	x, y, z,		//zmienne pozycji kamery
		height = 0.0f;	//aktualna wysokość pnia
	glfwSetTime(0);			//Wyzeruj licznik czasu

	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		//wyliczenie aktualnej pozycji kamery
		x = radius * sin(angle2) * cos(angle);
		y = radius * cos(angle2);
		z = radius * sin(angle) * sin(angle2);

		float max_height = 0.2f;					//na potem: powinno być losowane 
		int first_level_branch_count = 5;
		//na potem: powinno być losowane ;ilość gałezi 1 poziomu
		while (height < max_height) {

			x = radius * sin(angle2) * cos(angle);
			y = radius * cos(angle2);
			z = radius * sin(angle) * sin(angle2);
			height += 0.01f * glfwGetTime();
			glfwSetTime(0);
			drawScene(window, x, y, z, height, first_level_branch_count); //Wykonaj procedurę rysującą
			glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
		}

		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, x, y, z, height, first_level_branch_count); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
