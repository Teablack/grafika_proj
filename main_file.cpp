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

float speed = PI;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

void moj_torus(float angle) {
	//1torus
		glm::mat4 M1 = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M1 = glm::translate(M1, glm::vec3(-1.05f, 0.0f, 0.0f));
		M1 = glm::rotate(M1, angle, glm::vec3(0.0f, 0.0f, 1.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi Y
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M1)); //Załaduj do programu cieniującego macierz modelu

		Models::torus.drawSolid(); //Narysuj obiekt

	//2torus
		glm::mat4 M2 = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
		M2 = glm::translate(M2, glm::vec3(1.05f, 0.0f, 0.0f));
		M2 = glm::rotate(M2, -angle, glm::vec3(0.0f, 0.0f, 1.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi Y
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M2)); //Załaduj do programu cieniującego macierz modelu

		Models::torus.drawSolid(); //Narysuj obiekt 

	//kostki
		for (int i = 0; i < 12; i++) {
			glm::mat4 Mk = M1;
			Mk = glm::rotate(Mk, glm::radians(30.0f * i), glm::vec3(0.0f, 0.0f, 1.0f));
			Mk = glm::translate(Mk, glm::vec3(1.0f, 0.0f, 0.0f));
			Mk = glm::scale(Mk, glm::vec3(0.1f, 0.1f, 0.1f));

			glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mk)); //Załaduj do programu cieniującego macierz modelu
			Models::cube.drawSolid(); //Narysuj obiekt
		}
		for (int i = 0; i < 12; i++) {
			glm::mat4 Mk = M2;
			Mk = glm::rotate(Mk, glm::radians(30.0f * i + 15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			Mk = glm::translate(Mk, glm::vec3(1.0f, 0.0f, 0.0f));
			Mk = glm::scale(Mk, glm::vec3(0.1f, 0.1f, 0.1f));

			glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mk)); //Załaduj do programu cieniującego macierz modelu
			Models::cube.drawSolid(); //Narysuj obiekt
		}


}

void torusy6(float angle) {

	for (int i = 0; i < 6; i++) {
		glm::mat4 M1 = glm::mat4(1.0f);

		M1 = glm::rotate(M1, glm::radians(60.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
		M1 = glm::translate(M1, glm::vec3(0.0f, 0.0f, 2.0f));
		if (i % 2 == 0) M1 = glm::rotate(M1, -angle, glm::vec3(0.0f, 0.0f, 1.0f));
		else M1 = glm::rotate(M1, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M1));
		Models::torus.drawSolid(); //Narysuj obiekt

		for (int j = 0; j < 12; j++) {
			glm::mat4 Mk = M1;
			Mk = glm::rotate(Mk, glm::radians(30.0f * j), glm::vec3(0.0f, 0.0f, 1.0f));
			Mk = glm::translate(Mk, glm::vec3(1.0f, 0.0f, 0.0f));
			Mk = glm::scale(Mk, glm::vec3(0.1f, 0.1f, 0.1f));

			glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Mk)); //Załaduj do programu cieniującego macierz modelu
			Models::cube.drawSolid(); //Narysuj obiekt
		}
	}
}

void uklad_planet(float angle) {
	//słonce
	glm::mat4 M1 = glm::mat4(1.0f);
	glUniform4f(spLambert->u("color"), 1.0f, 1.0f, 0.0f, 0.0f); //kolor obiektu
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(M1));
	Models::sphere.drawSolid();

	//ziemia
	glm::mat4 Z = M1;
	Z = glm::rotate(Z, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	Z = glm::translate(Z, glm::vec3(0.0f, 0.0f, 2.0f));
	Z = glm::scale(Z, glm::vec3(0.3f, 0.3f, 0.3f));

	glUniform4f(spLambert->u("color"), 0.0f, 1.0f, 0.0f, 0.0f); //kolor obiektu
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(Z));
	Models::sphere.drawSolid();

	//satelita

	glm::mat4 S = Z;
	S = glm::rotate(S, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	S = glm::translate(S, glm::vec3(0.0f, 0.0f, 2.0f));
	S = glm::scale(S, glm::vec3(0.3f, 0.3f, 0.3f));

	glUniform4f(spLambert->u("color"), 0.9f, 0.9f, 0.9f, 0.0f); //kolor obiektu
	glUniformMatrix4fv(spLambert->u("M"), 1, false, glm::value_ptr(S));
	Models::sphere.drawSolid();
}
//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle) {
	//************Tutaj umieszczaj kod rysujący obraz******************

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości
	
	
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 2.0f, -6.0f),
				  glm::vec3(0.0f, 0.0f, 0.0f),
			      glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania

	spLambert->use(); //Aktywuj program cieniujący
	glUniform4f(spLambert->u("color"), 0, 1, 0, 1); //Ustaw kolor rysowania obiektu
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku


	//moj_torus(angle);
	//torusy6(angle);
	uklad_planet(angle);


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
	float angle = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	glfwSetTime(0); //Wyzeruj licznik czasu
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle += speed * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window,angle); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
