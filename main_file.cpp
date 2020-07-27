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
#include "myCube.h"

float speed_x = 0;//[radiany/s]
float speed_y = 0;//[radiany/s]


float speed = PI;
float	stopnie=90.0f, 
		stopnie2=90.0f, 
		angle= glm::radians(stopnie), 
		angle2= glm::radians(stopnie2), 
		radius=5.0f;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
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
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetKeyCallback(window, key_callback);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y, float x, float y , float z) {
	
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości
	
	glm::mat4 M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	
	
	glm::mat4 V = glm::lookAt(glm::vec3(x,y,z), 
							  glm::vec3(0.0f, 0.0f, 0.0f), 
							  glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f); //Wylicz macierz rzutowania

	//Tablica współrzędnych wierzchołków
	
	//drzewo 
	float verts[] = {
		/*
		-4,0,0,1,		-2,0,-3.5,1, //ab
		-2,0,-3.5,1,	2,0,-3.5,1,//bc
		2,0,-3.5,1,		4,0,0,1,//cd
		4,0,0,1,		2,0,3.5,1,//de
		2,0,3.5,1,		-2,0,3.5,1,//ef
		-2,0,3.5,1,		-4,0,0,1,//fa
		*/

		0,30,0,1,
		-4,0,0,1,		-2,0,-3.5,1, 
		2,0,-3.5,1,		4,0,0,1,
		2,0,3.5,1,		-2,0,3.5,1,
		-2,0,3.5,1,		-4,0,0,1,
		-4,0,0,1
		
	};
	int vertexCount = 10;


	spConstant->use();
	glUniformMatrix4fv(spConstant->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spConstant->u("V"), 1, false, glm::value_ptr(V));
	M = glm::scale(M, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(spConstant->u("M"), 1, false, glm::value_ptr(M));
	glUniform4f(spConstant->u("color"), 1, 0, 0, 1);
	glEnableVertexAttribArray(spConstant->a("vertex"));
	glVertexAttribPointer(spConstant->a("vertex"), 4, GL_FLOAT, false, 0, verts);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
	glDisableVertexAttribArray(spConstant->a("vertex"));

	


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
	
	float x, y, z;
	float angle_x = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	float angle_y = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	glfwSetTime(0); //Wyzeruj licznik czasu

	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{

		x = radius * sin(angle2) * cos(angle);
		y = radius * cos(angle2);
		z = radius * sin(angle) * sin(angle2);
		
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window,angle_x,angle_y,x,y,z); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
