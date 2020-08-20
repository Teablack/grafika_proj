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
#include <time.h>  
#include "pien1.c"
#include "ziemia3.c"
#include "galez6.c"
#include "leaf.c"



GLuint	ziemia_tex,
pien_tex,
leaf_tex;
float	stopnie = 90.0f,  // na potem: zrobic jedna jednostke , usunac zmienne stopnie 
stopnie2 = 90.0f,
angle = glm::radians(stopnie),
angle2 = glm::radians(stopnie2),
radius = 5.0f,
angle_x[10],
angle_z[10],
wysokosc[10];
;

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
	glGenerateMipmap(GL_TEXTURE_2D);

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
	leaf_tex = readTexture("leaf256.png");
	glClearColor(0, 0.8f, 1, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetKeyCallback(window, key_callback);
}

void rysujliscie(glm::mat4 Galaz) {

	for (int i = 0; i < 6; i++) {
		glm::mat4 Leaf = Galaz;
		Leaf = translate(Leaf, glm::vec3(0.0f, wysokosc[i]+2.0, 0.0f));
		Leaf = glm::rotate(Leaf, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		Leaf = glm::rotate(Leaf, angle_x[i], glm::vec3(0.0f, 0.0f, 1.0f));
		Leaf = glm::rotate(Leaf, angle_z[i], glm::vec3(1.0f, 0.0f, 0.0f));
		if (angle_x[i]>0) Leaf = glm::translate(Leaf, glm::vec3(0.0f, -1.3f * sin(angle_x[i] + 0.8f), 0.0f)); //yxz
		else Leaf = glm::translate(Leaf, glm::vec3(0.0f, 1.6f * sin(angle_x[i] - 0.8f), 0.0f));

		Leaf = glm::scale(Leaf, glm::vec3(2.1f, 2.1f , 2.1f));
		glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(Leaf));
		glEnableVertexAttribArray(spTextured->a("vertex"));
		glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, leafPositions);
		glEnableVertexAttribArray(spTextured->a("texCoord"));

		glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, leafTexels);
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, leaf_tex);
		glUniform1i(spTextured->u("tex"), 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

		glDrawArrays(GL_TRIANGLES, 0, leafVertices);
		glDisableVertexAttribArray(spTextured->a("vertex"));
		glDisableVertexAttribArray(spTextured->a("textCoord"));
	}
}


void rysujgalezie(glm::mat4 Base, float branch_height, int i, int level) {

	//printf("\n wejscie poziom: %d dlugosc : %f i: %d \n", level, branch_height, i);

	if (level < 4) {

		glm::mat4 Galaz = Base;

		Galaz = translate(Galaz, glm::vec3(0.0f, wysokosc[i] + 2.0f, 0.0f));

		Galaz = glm::rotate(Galaz, angle_x[i], glm::vec3(0.0f, 0.0f, 1.0f));
		Galaz = glm::rotate(Galaz, angle_z[i], glm::vec3(1.0f, 0.0f, 0.0f));
		if (angle_x[i] > 0) {
			if (angle_z[i] > 0) 
				Galaz = glm::translate(Galaz, glm::vec3(0.0f, 13.8f * branch_height * 2.8f * sin(angle_x[i] + 0.8f), -0.9f)); // x>0 z>0
			else Galaz = glm::translate(Galaz, glm::vec3(0.0f, 13.8f * branch_height * 2.8f * sin(angle_x[i] + 0.8f), 0.5f)); // X>0 z<0
		}
		else {
			if (angle_z[i] > 0)
				Galaz = glm::translate(Galaz, glm::vec3(0.0f, -10.0f * 2.8 * branch_height * sin(angle_x[i] - 0.8f), 0.5f)); //x<0 z>0
			else Galaz = glm::translate(Galaz, glm::vec3(0.0f, -8.5f * 2.8 * branch_height * sin(angle_x[i] - 0.8f), -0.8f)); // x<0 z>0
		}
		Galaz = glm::scale(Galaz, glm::vec3(branch_height * 2.8f, branch_height * 2.8f, branch_height * 2.8f));		//dlugosc na srodku

		glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(Galaz));
		glEnableVertexAttribArray(spTextured->a("vertex"));
		glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, galez6Positions);
		glEnableVertexAttribArray(spTextured->a("texCoord"));

		glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, galez6Texels);
		glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, pien_tex);
		glUniform1i(spTextured->u("tex"), 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


		glDrawArrays(GL_TRIANGLES, 0, galez6Vertices);
		glDisableVertexAttribArray(spTextured->a("vertex"));
		glDisableVertexAttribArray(spTextured->a("textCoord"));


		for (int j = 0; j < 4; j++) {


			rysujgalezie(Galaz, branch_height, j + level, level + 1);

		}
	}
	else rysujliscie(Base);

}

void rysujgalezie1poziomu(glm::mat4 Base, float branch_height, int i, int level) {

	glm::mat4 Galaz = Base;

	Galaz = translate(Galaz, glm::vec3(0.0f, wysokosc[i], 0.0f));

	Galaz = glm::rotate(Galaz, angle_x[i], glm::vec3(0.0f, 0.0f, 1.0f));
	Galaz = glm::rotate(Galaz, angle_z[i], glm::vec3(1.0f, 0.0f, 0.0f));
	if (angle_x[i] > 0) Galaz = glm::translate(Galaz, glm::vec3(0.0f, 11.8f * branch_height * 2.8f * sin(angle_x[i] + 0.8f), 0.0f)); //yxz
	else Galaz = glm::translate(Galaz, glm::vec3(0.0f, -12.0f * branch_height * 2.8f * sin(angle_x[i] - 0.8f), 0.0f)); //-yxz // 11.8f * branch_height * 2.8f * sin(angle_z-0.3)

	Galaz = glm::scale(Galaz, glm::vec3(branch_height * 2.8f, branch_height * 2.8f, branch_height * 2.8f));

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(Galaz));
	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, galez6Positions);
	glEnableVertexAttribArray(spTextured->a("texCoord"));

	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, galez6Texels);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, pien_tex);
	glUniform1i(spTextured->u("tex"), 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


	glDrawArrays(GL_TRIANGLES, 0, galez6Vertices);
	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("textCoord"));


	for (int j = 0; j < 5; j++) {

		rysujgalezie(Galaz, branch_height, j, level + 1);

	}


}



//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float x, float y, float z, float height, float max_height, float branch_height, int first_level_branch_count) {


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości

	glm::mat4 V = glm::lookAt(glm::vec3(x, y, z),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);		//Wylicz macierz rzutowania

	spTextured->use();
	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V));

	//ziemia

	glm::mat4 M = glm::mat4(1.0f);										//Zainicjuj macierz modelu macierzą jednostkową
	M = glm::translate(M, glm::vec3(0, -2.0f, 0));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M));
	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, ziemia3Positions);
	glEnableVertexAttribArray(spTextured->a("texCoord"));

	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, ziemia3Texels);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, ziemia_tex);
	glUniform1i(spTextured->u("tex"), 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glDrawArrays(GL_TRIANGLES, 0, ziemia3Vertices);
	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("textCoord"));


	//pień
	glm::mat4 Pien = glm::mat4(1.0f);

	Pien = glm::translate(Pien, glm::vec3(0, height * 12.5 - 1.8, 0));
	Pien = glm::scale(Pien, glm::vec3(height, height, height));

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(Pien));
	glEnableVertexAttribArray(spTextured->a("vertex"));
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, pien1Positions);
	glEnableVertexAttribArray(spTextured->a("texCoord"));

	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, pien1Texels);
	glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, pien_tex);
	glUniform1i(spTextured->u("tex"), 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glDrawArrays(GL_TRIANGLES, 0, pien1Vertices);
	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("textCoord"));


	//gałęzie
	if (height >= max_height / 2.0f) {
		int level = 0;
		for (int i = 0; i < first_level_branch_count; i++) {

			rysujgalezie1poziomu(Pien, branch_height, i, level);

		}


	}

	glfwSwapBuffers(window);				//Skopiuj bufor tylny do bufora przedniego
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
	srand(static_cast<unsigned int>(time(0)));

	float	x, y, z,											//zmienne pozycji kamery
		height = 0.0f,											//aktualna wysokość pnia
		max_height = 0.2f, branch_height = 0.0f;					//na potem: powinno być losowane 
	int first_level_branch_count = rand() % 5 + 5;				//ilość gałezi 1 poziomu wartosci od 3 do 



	for (int i = 0; i < 10; i+=2) {
		

			if (int even = rand() % 2) {
				angle_x[i] = (rand() % 60 + 30) / 100.0f;
				angle_x[i + 1] = (rand() % 60 - 90) / 100.0f;
			}
			else {
				angle_x[i] = (rand() % 60 - 90) / 100.0f;
				angle_x[i + 1] = (rand() % 60 + 30) / 100.0f;
			}

			printf("x %f\n", angle_x[i]);
			printf("x %f\n", angle_x[i+1]);

			if (int even = rand() % 2) {
				angle_z[i] = (rand() % 60 + 30) / 100.0f;
				angle_z[i + 1] = (rand() % 60 - 90) / 100.0f;
			}
			else {
				angle_z[i] = (rand() % 60 - 90) / 100.0f;
				angle_z[i + 1] = (rand() % 60 + 30) / 100.0f;
			}

			printf("z %f\n", angle_z[i]);
			printf("z %f\n", angle_z[i+1]);
			wysokosc[i] = rand() % 11 - 8;

	}


	glfwSetTime(0);			//Wyzeruj licznik czasu

	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		//wyliczenie aktualnej pozycji kamery
		x = radius * sin(angle2) * cos(angle);
		y = radius * cos(angle2);
		z = radius * sin(angle) * sin(angle2);

		while (height < max_height) {

			x = radius * sin(angle2) * cos(angle);
			y = radius * cos(angle2);
			z = radius * sin(angle) * sin(angle2);
			height += 0.015f * glfwGetTime();

			if (height >= max_height / 2.0f && branch_height <= 0.18f) {
				branch_height += 0.01f;  //0.015
				printf("\n%f\n", branch_height);
			}

			glfwSetTime(0);
			drawScene(window, x, y, z, height, max_height, branch_height, first_level_branch_count); //Wykonaj procedurę rysującą
			glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
		}

		if (height >= max_height / 2.0f && branch_height <= 0.18f) {
			branch_height += 0.01f;  //0.015
			printf("\n%f\n", branch_height);
		}


		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, x, y, z, height, max_height, branch_height, first_level_branch_count); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
