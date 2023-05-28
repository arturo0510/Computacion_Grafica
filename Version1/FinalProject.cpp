/*
Semestre 2023-2
Animación:
1.- Simple o básica:Por banderas y condicionales
2.- Compleja: Por medio de funciones y algoritmos,Textura Animada.
Adicional.- Texturizado con transparencia usando Blending: Requerimos dibujar lo que está atras primero
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include<time.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;
const float PI = 3.14159265f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture pastoTexture;

// ---------------------------------- VARIABLES DE MODELOS --------------------------------------
//modelos proyecto
Model cabana_madera;
Model roca;
Model valla;
Model arbol2;
Model fuente;
Model caballo;
Model vaca;
Model vela;
Model arbusto;
Model estrella;
Model baulCuerpo;
Model baulTapa;
Model piedras_piso;
Model arco;
Model valla2;
Model molino;
Model helice_molino;
Model lamp;

// caballo
Model caballo_cabeza;
Model caballo_cuerpo;
Model caballo_cola;
Model caballo_colaPunta;
Model caballo_pataFRD;
Model caballo_pataFRU;
Model caballo_pataFLD;
Model caballo_pataFLU;
Model caballo_pataBRD;
Model caballo_pataBRU;
Model caballo_pataBLD;
Model caballo_pataBLU;

// vaca
Model vaca_cuerpo;
Model vaca_cabeza;
Model vaca_cola1;
Model vaca_cola2;
Model vaca_cola3;
Model vaca_pataFR, vaca_pataFL, vaca_pataBR, vaca_pataBL;

// mariposa
Model butterfly_body, butterfly_R, butterfly_L;

// aguila
Model aguila_cuerpo, aguila_r, aguila_l;

// ----------------------------------- VARIABLES DE ANIMACION ----------------------------------
// caballo
float rotCola, rotColaPunta;
float rotPataFLU, rotPataFLD;
bool plusPataFLU, plusPataFLD;
float rotPataFRU, rotPataFRD;
bool plusPataFRU, plusPataFRD;
float rotPataBLU, rotPataBLD;
bool plusPataBLU, plusPataBLD;
float rotPataBRU, rotPataBRD;
bool plusPataBRU, plusPataBRD;
float rotaCabeza, lossTimeC;
int accionC, movCabezaC;
bool condPataFR[2] = { false, false }, condPataFL[2] = { false, false };
bool condPataBR[2] = { false, false }, condPataBL[2] = { false, false };

// vaca
float rotaCola1ZV, rotaCola2ZV, rotaCola3ZV;
float rotaCola1XV, rotaCola2XV, rotaCola3XV;
int accionColaV;
float rotarVaca;
float rotPataVacaFR, rotPataVacaFL, rotPataVacaBR, rotPataVacaBL;
float movVacaX, movVacaZ;

// mariposa
float rotaAlasButterfly, movMariposaX, movMariposaZ, movMariposaY;
float rotaMariposaY, anguloMariposa;

// aguila - baul
float rotaAlasAguila, movAguilaZ, movAguilaY;
float rotaTapaBaul, rotaAguila;
bool animAguila, openTapa, regresaAguila, arribaAlas;

//molino
float mov_mol, mov_molof;
int velocidad_mol;

// ------------------------- CICLO DIA - NOCHE ---------------------------------
int ciclos = 0;
bool dia;

Skybox skyboxdia;
Skybox skyboxnoche;

//materiales
Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

using namespace std;

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

}



void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		//2, 3, 0,
		// right
		3, 4, 5,
		//6, 7, 4,
		// back
		6, 7, 8,
		//10, 11, 8,

		// left
		9, 10, 11,
		//14, 15, 12,
		// bottom
		12, 13, 14,
		//18, 19, 16,
		// top
		15, 16, 17,

		18, 19, 20,

		21, 22, 23
	};

	GLfloat cubo_vertices[] = {
		// 1 cara superior L muerte *
		//x		y		z		S		T			NX		NY		NZ
		0.0f,-0.5f,0.0f,	0.573f,  0.67f,		-1.0f,	1.0f,	-1.0f,	//0
		0.5f,0.0f,0.0f,		0.857f,	0.67f,		-1.0f,	1.0f,	-1.0f,	//1
		0.0f,0.0f,0.5f,		0.714f,	0.99f,		-1.0f,	1.0f,	-1.0f,	//2

		// 2 cara superior L rayo *
		//x		y		z		S		T			NX		NY		NZ
		0.5f,0.0f,0.0f,     0.02f,  0.65f,	    -1.0f,	-1.0f,	-1.0f,	//3
		0.0f,0.5f,0.0f,		0.285f,	0.65f,		-1.0f,	-1.0f,	-1.0f,	//4
		0.0f,0.0f,0.5f,		0.142f,	0.333f,		-1.0f,	-1.0f,	-1.0f,	//5

		// 3 cara superior signo admiracion  L
		//x		y		z		S		T			NX		NY		NZ
		0.0f,0.5f,0.0f,			0.142f,  0.333f,	1.0f,	-1.0f,	-1.0f,	//6
		-0.5f,0.0f,0.0f,		0.428f,	0.333f,		1.0f,	-1.0f,	-1.0f,	//7
		0.0f,0.0f,0.5f,			0.285f,	0.666f,		1.0f,	-1.0f,	-1.0f,	//8

		// 4 cara superior fuego L
		//x		y		z		S		T			NX		NY		NZ
		-0.5f,0.0f,0.0f,		0.285f,  0.65f,		1.0f,	1.0f,	-1.0f,	//9
		0.0f,-0.5f,0.0f,		0.56f,	0.65f,		1.0f,	1.0f,	-1.0f,	//10
		0.0f,0.0f,0.5f,			0.420f,	0.34f,		1.0f,	1.0f,	-1.0f,	//11

		// 1 cara inferior radioactivo L
		//x		y		z		S		T			NX		NY		NZ
		-0.5f,0.0f,0.0f,		0.573f,	0.66f,		1.0f,	1.0f,	1.0f,	//12
		0.0f,-0.5f,0.0f,		0.855f,	0.66f,		1.0f,	1.0f,	1.0f,	//13
		0.0f,0.0f,-0.5f,		0.714f,  0.333f,	1.0f,	1.0f,	1.0f,	//14

		// 2 cara inferior  nadando 
		//x		y		z		S		T			NX		NY		NZ
		0.0f,-0.5f,0.0f,		0.429f, 0.33f,		-1.0f,	1.0f,	1.0f,	//15
		0.5f,0.0f,0.0f,			0.713f,	0.33f,		-1.0f,	1.0f,	1.0f,	//16
		0.0f,0.0f,-0.5f,		0.571f,	0.66f,		-1.0f,	1.0f,	1.0f,	//17

		// 3 cara inferior electrocutar 
		//x		y		z		S		T			NX		NY		NZ
		0.5f,0.0f,0.0f,			0.999f,	0.33f,		-1.0f,	-1.0f,	1.0f,	//18
		0.0f,0.5f,0.0f,			0.719f,  0.33f,		-1.0f,	-1.0f,	1.0f,	//19
		0.0f,0.0f,-0.5f,		0.856f,	0.66f,		-1.0f,	-1.0f,	1.0f,	//20

		// 4 cara inferior  caer 
		//x		y		z		S		T			NX		NY		NZ
		0.0f,0.5f,0.0f,			0.142f,  0.33f,		1.0f,	-1.0f,	1.0f,	//21
		-0.5f,0.0f,0.0f,		0.420f,	0.33f,		1.0f,	-1.0f,	1.0f,	//22
		0.0f,0.0f,-0.5f,		0.283f,	0.01f,		1.0f,	-1.0f,	1.0f,	//23

	};



	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 192, 24);
	meshList.push_back(dado);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

// Animacion caballo
void AnimateHorse() {
	if (rotCola >= 180.0f) rotCola = -180.0f;
	else rotCola += 3.0f * deltaTime;
	if (rotColaPunta >= 180.0f) rotColaPunta = -180.0f;
	else rotColaPunta += 3.0f * deltaTime;
	if (movCabezaC <= 2) {
		if (rotaCabeza >= 180.0f) {
			rotaCabeza = -180.0f;
			movCabezaC = (rand() % 4) + 1;
		}
		else rotaCabeza += 3.0f * deltaTime;
	}
	else {
		if (lossTimeC >= 180.0f) {
			lossTimeC = -180.0f;
			movCabezaC = (rand() % 4) + 1;
		}
		else lossTimeC += 4.0f * deltaTime;
	}


	if (accionC == 1) {
		// Pata delantera izquierda
		if (plusPataFLU) {
			rotPataFLU -= 4.0f * deltaTime;
			if (rotPataFLU <= -180.0f) plusPataFLU = false;
		}
		else {
			rotPataFLU += 4.0f * deltaTime;
			if (rotPataFLU >= 0.0f) {
				plusPataFLU = true;
				rotPataFLU = 0.0f;
				condPataFL[0] = true;
			}
		}
		if (!plusPataFLD) {
			rotPataFLD += 4.0f * deltaTime;
			if (rotPataFLD >= 180.0f) plusPataFLD = true;
		}
		else {
			rotPataFLD -= 4.0f * deltaTime;
			if (rotPataFLD <= 0.0f) {
				plusPataFLD = false;
				rotPataFLD = 0.0f;
				condPataFL[1] = true;
			}
		}
		if (condPataFL[0] && condPataFL[1]) {
			accionC = rand() % 5 + 1;
			condPataFL[0] = false;
			condPataFL[1] = false;
		}
	}
	else if (accionC == 2) {
		// Pata delantera derecha
		if (plusPataFRU) {
			rotPataFRU -= 4.0f * deltaTime;
			if (rotPataFRU <= -180.0f) plusPataFRU = false;
		}
		else {
			rotPataFRU += 4.0f * deltaTime;
			if (rotPataFRU >= 0.0f) {
				plusPataFRU = true;
				rotPataFRU = 0.0f;
				condPataFR[0] = true;
			}
		}
		if (!plusPataFRD) {
			rotPataFRD += 4.0f * deltaTime;
			if (rotPataFRD >= 180.0f) plusPataFRD = true;
		}
		else {
			rotPataFRD -= 4.0f * deltaTime;
			if (rotPataFRD <= 0.0f) {
				plusPataFRD = false;
				rotPataFRD = 0.0f;
				condPataFR[1] = true;
			}
		}
		if (condPataFR[0] && condPataFR[1]) {
			accionC = rand() % 4 + 1;
			condPataFR[0] = false;
			condPataFR[1] = false;
		}
	}
	else if (accionC == 3) {
		// Pata trasera izquierda
		if (plusPataBLU) {
			rotPataBLU -= 4.0f * deltaTime;
			if (rotPataBLU <= -180.0f) plusPataBLU = false;
		}
		else {
			rotPataBLU += 4.0f * deltaTime;
			if (rotPataBLU >= 0.0f) {
				plusPataBLU = true;
				rotPataBLU = 0.0F;
				condPataBL[0] = true;
			}
		}
		if (!plusPataBLD) {
			rotPataBLD += 4.0f * deltaTime;
			if (rotPataBLD >= 180.0f) plusPataBLD = true;
		}
		else {
			rotPataBLD -= 4.0f * deltaTime;
			if (rotPataBLD <= 0.0f) {
				plusPataBLD = false;
				rotPataBLD = 0.0f;
				condPataBL[1] = true;
			}
		}
		if (condPataBL[0] && condPataBL[1]) {
			accionC = rand() % 5 + 1;
			condPataBL[0] = false;
			condPataBL[1] = false;
		}
	}
	else {
		// Pata trasera derecha
		if (plusPataBRU) {
			rotPataBRU -= 4.0f * deltaTime;
			if (rotPataBRU <= -180.0f) plusPataBRU = false;
		}
		else {
			rotPataBRU += 4.0f * deltaTime;
			if (rotPataBRU >= 0.0f) {
				plusPataBRU = true;
				rotPataBRU = 0.0f;
				condPataBR[0] = true;
			}
		}
		if (!plusPataBRD) {
			rotPataBRD += 4.0f * deltaTime;
			if (rotPataBRD >= 180.0f) plusPataBRD = true;
		}
		else {
			rotPataBRD -= 4.0f * deltaTime;
			if (rotPataBRD <= 0.0f) {
				plusPataBRD = false;
				rotPataBRD = 0.0f;
				condPataBR[1] = true;
			}
		}
		if (condPataBR[0] && condPataBR[1]) {
			accionC = rand() % 5 + 1;
			condPataBR[0] = false;
			condPataBR[1] = false;
		}
	}
}

void AnimateCow() {
	// Mover cola a los lados
	if (accionColaV <= 4) {
		if (rotaCola2ZV >= 180.0f) rotaCola2ZV = -180.0f;
		else rotaCola2ZV += 3.0f * deltaTime;
		if (rotaCola3ZV >= 180.0f) rotaCola3ZV = -180.0f;
		else rotaCola3ZV += 3.0f * deltaTime;
		if (rotaCola1ZV >= 180.0f) rotaCola1ZV = -180.0f;
		else if (rotaCola1ZV >= -1.0f && rotaCola1ZV < 0.0f){
			rotaCola1ZV = 0.0f;
			rotaCola2ZV = 0.0f;
			rotaCola3ZV = 0.0f;
			accionColaV = (rand() % 6) + 1;
		}
		else rotaCola1ZV += 3.0f * deltaTime;
	}
	else {		// alzar la cola
		if (rotaCola1ZV >= 180.0f) rotaCola1ZV = -180.0f;
		else rotaCola1ZV += 1.5f * deltaTime;
		if (rotaCola2ZV >= 180.0f) rotaCola2ZV = -180.0f;
		else rotaCola2ZV += 1.5f * deltaTime;
		if (rotaCola3ZV >= 180.0f) rotaCola3ZV = -180.0f;
		else rotaCola3ZV += 1.5f * deltaTime;
		
		if (rotaCola2XV >= 180.0f) rotaCola2XV = 0.0f;
		else rotaCola2XV += 3.0f * deltaTime;
		if (rotaCola3XV >= 180.0f) rotaCola3XV = 0.0f;
		else rotaCola3XV += 3.0f * deltaTime;
		if (rotaCola1XV >= 180.0f) {
			rotaCola1XV = 0.0f;
			rotaCola2XV = 0.0f;
			rotaCola3XV = 0.0f;
			accionColaV = (rand() % 6) + 1;
		}
		else rotaCola1XV += 3.0f * deltaTime;
	}
	if (rotPataVacaFL >= 180.0f) rotPataVacaFL = -180.0f;
	else rotPataVacaFL += 3.0f * deltaTime;
	if (rotPataVacaBR >= 180.0f) rotPataVacaBR = -180.0f;
	else rotPataVacaBR += 3.0f * deltaTime;
	if (rotPataVacaBL <= -180.0f) rotPataVacaBL = 180.0f;
	else rotPataVacaBL -= 3.0f * deltaTime;
	if (rotPataVacaFR <= -180.0f) rotPataVacaFR = 180.0f;
	else rotPataVacaFR -= 3.0f * deltaTime;
	rotarVaca += 0.15f * deltaTime;
	movVacaX = 5 * cos(glm::radians(rotarVaca));
	movVacaZ = 5 * sin(glm::radians(rotarVaca));
}

void AnimateButterfly() {
	if (rotaAlasButterfly >= 180.0f) rotaAlasButterfly = -180.0f;
	else rotaAlasButterfly += 10.0f * deltaTime;

	//3sin(2t) 0.0 < t < 2pi
	if (anguloMariposa >= 180.0f) anguloMariposa = -180.0f;
	else anguloMariposa += 0.5f * deltaTime;

	movMariposaX = 8 * glm::sin(glm::radians(2 * anguloMariposa)) * glm::cos(glm::radians(anguloMariposa));
	movMariposaZ = 8 * glm::sin(glm::radians(2 * anguloMariposa)) * glm::sin(glm::radians(anguloMariposa));

	movMariposaY = (movMariposaX * movMariposaZ) / 4;

}

void AnimateEagle() {
	if (mainWindow.getabrirBaul()) {
		if (openTapa) {
			if (rotaTapaBaul >= 90.0f) {
				rotaTapaBaul = 90.0f;
				animAguila = true;
			}
			else rotaTapaBaul += 3.0f * deltaTime;
		}

		if (animAguila) {
			if (!regresaAguila) {

				if (arribaAlas) {
					if (rotaAlasAguila >= 30.0f) {
						rotaAlasAguila = 30.0f;
						arribaAlas = false;
					}
					else rotaAlasAguila += 10.0f * deltaTime;
				}
				else {
					if (rotaAlasAguila <= -30.0f) {
						rotaAlasAguila = -30.0f;
						arribaAlas = true;
					}
					else rotaAlasAguila -= 10.0f * deltaTime;
				}

				if (movAguilaZ >= 20.0f) movAguilaZ = 20.0f;
				else movAguilaZ += 0.2f * deltaTime;
				if (movAguilaY >= 10.0f) movAguilaY = 10.0f;
				else movAguilaY += 0.1f * deltaTime;

				if (movAguilaZ == 20.0f && movAguilaY == 10.0f) {
					if (rotaAguila >= 180.0f) {
						rotaAguila = 180.0f;
						regresaAguila = true;
					}
					else rotaAguila += 2.0f * deltaTime;
				}
			}

			if (regresaAguila) {
				if (movAguilaZ <= 0.0f) movAguilaZ = 0.0f;
				else movAguilaZ -= 0.3f * deltaTime;
				if (movAguilaY <= 0.0f) movAguilaY = 0.0f;
				else movAguilaY -= 0.15f * deltaTime;

				if (movAguilaZ == 0.0f && movAguilaY == 0.0f) {
					if (rotaAguila <= 0.0f) {
						rotaAguila = 0.0f;
						regresaAguila = false;
						openTapa = false;
						animAguila = false;
						rotaAlasAguila = 0.0f;
					}
					else { 
						rotaAguila -= 2.0f * deltaTime;
						if (arribaAlas) {
							if (rotaAlasAguila >= 30.0f) {
								rotaAlasAguila = 30.0f;
								arribaAlas = false;
							}
							else rotaAlasAguila += 10.0f * deltaTime;
						}
						else {
							if (rotaAlasAguila <= -30.0f) {
								rotaAlasAguila = -30.0f;
								arribaAlas = true;
							}
							else rotaAlasAguila -= 10.0f * deltaTime;
						}
					}
				}
			}
		}

		if (!openTapa) {
			if (rotaTapaBaul <= 0.0f) {
				rotaTapaBaul = 0.0f;
				openTapa = true;
				mainWindow.setabriBaul(false);
			}
			else rotaTapaBaul -= 3.0f * deltaTime;
		}
	}
}

void Animatemol() {
	if (velocidad_mol == 1) mov_molof = 0.2;
	else if (velocidad_mol == 2) mov_molof = 0.4;
	else if (velocidad_mol == 3) mov_molof = 0.6;
	else if (velocidad_mol == 4) mov_molof = 0.8;
	else mov_molof = 1.0f;
	if (mov_mol >= 360.0f) {
		mov_mol = 0.0f;
		velocidad_mol = (rand() % 5) + 1;
	}
	mov_mol += mov_molof * deltaTime;
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();
	srand(time(NULL));

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/abajo.tga");
	pisoTexture.LoadTextureA();
	pastoTexture = Texture("Textures/pasto.png");
	pastoTexture.LoadTextureA();
	
	cabana_madera = Model();
	cabana_madera.LoadModel("Models/cabana.obj");
	roca = Model();
	roca.LoadModel("Models/roca.obj");
	valla = Model();
	valla.LoadModel("Models/valla.obj");
	arbol2 = Model();
	arbol2.LoadModel("Models/arbol4.obj");
	fuente = Model();
	fuente.LoadModel("Models/fuente.obj");
	caballo = Model();
	caballo.LoadModel("Models/caballo.obj");
	vaca = Model();
	vaca.LoadModel("Models/vaca.obj");
	vela = Model();
	vela.LoadModel("Models/vela.obj");
	arbusto = Model();
	arbusto.LoadModel("Models/arbusto.obj");
	estrella = Model();
	estrella.LoadModel("Models/estrella.obj");
	baulCuerpo = Model();
	baulCuerpo.LoadModel("Models/baul_cuerpo.obj");
	baulTapa = Model();
	baulTapa.LoadModel("Models/baul_tapa.obj");
	piedras_piso = Model();
	piedras_piso.LoadModel("Models/piedras_piso.obj");
	arco = Model();
	arco.LoadModel("Models/arco.obj");
	valla2 = Model();
	valla2.LoadModel("Models/valla2.obj");
	molino = Model();
	molino.LoadModel("Models/molino.obj");
	helice_molino = Model();
	helice_molino.LoadModel("Models/helice-molino.obj");
	lamp = Model();
	lamp.LoadModel("Models/lamp.obj");

	caballo_cuerpo = Model();
	caballo_cuerpo.LoadModel("Models/caballo_cuerpo.obj");
	caballo_cola = Model();
	caballo_cola.LoadModel("Models/caballo_Cola.obj");
	caballo_colaPunta = Model();
	caballo_colaPunta.LoadModel("Models/caballo_ColaPunta.obj");
	caballo_pataFRD = Model();
	caballo_pataFRD.LoadModel("Models/caballo_pataDelDerAba.obj");
	caballo_pataFRU = Model();
	caballo_pataFRU.LoadModel("Models/caballo_pataDelDerArr.obj");
	caballo_pataFLD = Model();
	caballo_pataFLD.LoadModel("Models/caballo_pataDelizqAba.obj");
	caballo_pataFLU = Model();
	caballo_pataFLU.LoadModel("Models/caballo_pataDelizqArr.obj");
	caballo_pataBRD = Model();
	caballo_pataBRD.LoadModel("Models/caballo_pataTraDerAba.obj");
	caballo_pataBRU = Model();
	caballo_pataBRU.LoadModel("Models/caballo_pataTraDerArr.obj");
	caballo_pataBLD = Model();
	caballo_pataBLD.LoadModel("Models/caballo_pataTraIzqAba.obj");
	caballo_pataBLU = Model();
	caballo_pataBLU.LoadModel("Models/caballo_pataTraIzqArr.obj");
	caballo_cabeza = Model();
	caballo_cabeza.LoadModel("Models/caballo_cabeza.obj");
	
	vaca_cuerpo = Model();
	vaca_cuerpo.LoadModel("Models/vaca_cuerpo.obj");
	vaca_cabeza = Model();
	vaca_cabeza.LoadModel("Models/vaca_cabeza.obj");
	vaca_cola1 = Model();
	vaca_cola1.LoadModel("Models/vaca_cola1.obj");
	vaca_cola2 = Model();
	vaca_cola2.LoadModel("Models/vaca_cola2.obj");
	vaca_cola3 = Model();
	vaca_cola3.LoadModel("Models/vaca_cola3.obj");
	vaca_pataFR = Model();
	vaca_pataFR.LoadModel("Models/vaca_pataFR.obj");
	vaca_pataFL = Model();
	vaca_pataFL.LoadModel("Models/vaca_pataFL.obj");
	vaca_pataBR = Model();
	vaca_pataBR.LoadModel("Models/vaca_pataBR.obj");
	vaca_pataBL = Model();
	vaca_pataBL.LoadModel("Models/vaca_pataBL.obj");

	butterfly_body = Model();
	butterfly_body.LoadModel("Models/butterfly_body.obj");
	butterfly_R = Model();
	butterfly_R.LoadModel("Models/butterfly_Right.obj");
	butterfly_L = Model();
	butterfly_L.LoadModel("Models/butterfly_Left.obj");

	aguila_cuerpo = Model();
	aguila_cuerpo.LoadModel("Models/aguila_cuerpo.obj");
	aguila_r = Model();
	aguila_r.LoadModel("Models/aguila_Right.obj");
	aguila_l = Model();
	aguila_l.LoadModel("Models/aguila_left.obj");

	std::vector<std::string> skyboxFacesdia;
	
	skyboxFacesdia.push_back("Textures/Skybox/lado2.tga"); //bien 
	skyboxFacesdia.push_back("Textures/Skybox/lado4.tga"); //bien
	skyboxFacesdia.push_back("Textures/Skybox/abajo.tga");
	skyboxFacesdia.push_back("Textures/Skybox/arriba.tga"); // bien 
	skyboxFacesdia.push_back("Textures/Skybox/lado1.tga"); //bien
	skyboxFacesdia.push_back("Textures/Skybox/lado3.tga"); // bien

	skyboxdia = Skybox(skyboxFacesdia);

	std::vector<std::string> skyboxFacesnoche;

	skyboxFacesnoche.push_back("Textures/Skybox/lado2-noche.tga"); //bien 
	skyboxFacesnoche.push_back("Textures/Skybox/lado4-noche.tga"); //bien
	skyboxFacesnoche.push_back("Textures/Skybox/abajo-noche.tga");
	skyboxFacesnoche.push_back("Textures/Skybox/arriba-noche.tga"); // bien 
	skyboxFacesnoche.push_back("Textures/Skybox/lado1-noche.tga"); //bien
	skyboxFacesnoche.push_back("Textures/Skybox/lado3-noche.tga"); // bien 

	skyboxnoche = Skybox(skyboxFacesnoche);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// ----------------------------- ILUMINACION ----------------------------------------
	//luz direccional, sólo 1 y siempre debe de existir LUZ DEL SOL 
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.3f,
		0.0f, 0.0f, -1.0f);
	
	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(0.0f, 1.0f, 1.0f,
		0.35f, 2.0f,
		12.0f, 0.1f, 12.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 0.0f, 0.0f,
		0.35f, 2.0f,
		-21.8f, 4.7f, -15.4f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[2] = PointLight(1.0f, 0.0f, 0.0f,
		0.4f, 2.0f,
		21.8f, 4.7f, 15.4f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[3] = PointLight(1.0f, 0.0f, 0.0f,
		0.4f, 2.0f,
		21.8f, 4.7f, 0.4f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[4] = PointLight(1.0f, 0.0f, 0.0f,
		0.4f, 2.0f,
		21.8f, 4.7f, -15.4f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[5] = PointLight(1.0f, 1.0f, 0.0f,
		1.5f, 2.0f,
		0.4f, 6.7f, 4.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(0.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		-5.0f, 0.0f, 10.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	// ------------------------------ VARIABLES ANIMACION ----------------------------
	// First sirve para evitar valores extraños en la primera ejecución
	bool first = true;

	// Caballo
	rotCola = 0.0f;
	rotColaPunta = 0.0f;
	rotPataFLU = 0.0f; rotPataFLD = 0.0f;
	plusPataFLU = true; plusPataFLD = false;
	rotPataFRU = 0.0f; rotPataFRD = 0.0f;
	plusPataFRU = true; plusPataFRD = false;
	rotPataBLU = 0.0f; rotPataBLD = 0.0f;
	plusPataBLU = true; plusPataBLD = false;
	rotPataBRU = 0.0f; rotPataBRD = 0.0f;
	plusPataBRU = true; plusPataBRD = false;
	rotaCabeza = 0.0f;
	lossTimeC = 0.0f;

	srand(time(NULL));
	accionC = (rand() % 5) + 1;
	movCabezaC = (rand() % 4) + 1;

	// Vaca
	rotaCola1ZV = 0.0f; rotaCola2ZV = 0.0f; rotaCola3ZV = 0.0f;
	rotaCola1XV = 0.0f; rotaCola2XV = 0.0f; rotaCola3XV = 0.0f;
	accionColaV = (rand() % 6) + 1;
	rotarVaca = 0.0f;
	rotPataVacaFR = 0.0f; rotPataVacaFL = 0.0f; rotPataVacaBR = 0.0f; rotPataVacaBL = 0.0f;
	movVacaX = 10.0f; movVacaZ = 0.0f;

	// mariposa
	rotaAlasButterfly = 0.0f;
	movMariposaX = 0.0f; movMariposaZ = 0.0f;

	// aguila
	openTapa = true;
	regresaAguila = false;
	animAguila = false;
	rotaAguila = 0.0f; rotaTapaBaul = 0.0f; movAguilaY = 0.0f; movAguilaZ = 0.0f;
	rotaAlasAguila = 0.0f;
	arribaAlas = true;

	//molino
	mov_mol = 0.0f; mov_molof = 0.2f;
	velocidad_mol = (rand() % 5) + 1;
	// ------------- ILUMINACION -----------------
	ciclos = 0;
	dia = true;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	GLfloat  ambientcolorLocation = 0, positionLocation_p1 = 0;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		if (ciclos >= CLOCKS_PER_SEC) {
			if (dia) dia = false;
			else dia = true;
			ciclos = 0;
		}
		else ciclos++;
		if (dia) mainLight.setIntensity(1.0f);
		else mainLight.setIntensity(0.4f);

		// Obtener tipo de camara
		//    Camara normal
		if (camera.getIsometric() == false) {
			projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
		}
		else {
			projection = glm::ortho(-camera.getZoom(), camera.getZoom(), -camera.getZoom(), camera.getZoom(), -30.0f, 30.f);
		}

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		AnimateHorse();
		AnimateCow();
		AnimateButterfly();
		AnimateEagle();
		Animatemol();

		if (first) {
			rotCola = 0.0f;
			rotColaPunta = 0.0f;
			rotPataFLU = 0.0f;
			rotPataFLD = 0.0f;
			rotPataFRU = 0.0f;
			rotPataFRD = 0.0f;
			rotPataBLU = 0.0f;
			rotPataBLD = 0.0f;
			rotPataBRU = 0.0f;
			rotPataBRD = 0.0f;
			rotaCabeza = 0.0f;
			lossTimeC = 0.0f;
			rotaCola1ZV = 0.0f; rotaCola2ZV = 0.0f; rotaCola3ZV = 0.0f;
			rotaCola1XV = 0.0f; rotaCola2XV = 0.0f; rotaCola3XV = 0.0f;
			rotPataVacaFR = 0.0f; rotPataVacaFL = 0.0f; rotPataVacaBR = 0.0f; rotPataVacaBL = 0.0f;
			movVacaX = 10.0f; movVacaZ = 0.0f; rotarVaca = 0.0f;
			rotaAlasButterfly = 0.0f;
			movMariposaX = 0.0f; movMariposaZ = 0.0f, movMariposaY = 0.0f;
			rotaAguila = 0.0f; rotaTapaBaul = 0.0f; movAguilaY = 0.0f; movAguilaZ = 0.0f; rotaAlasAguila = 0.0f;
			first = false;
		}

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glUniform3f(ambientcolorLocation, 1.0f, 0.0f, 0.0f);
		glUniform3f(positionLocation_p1, -22.5f, 4.7f, 15.4f);

		if (dia) {
			skyboxdia.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else {

			skyboxnoche.DrawSkybox(camera.calculateViewMatrix(), projection);
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		/*glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());*/
		//spotLights[0].SetPos(glm::vec3(1.0f, 0.0f, 1.0f));

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		if (!dia) shaderList[0].SetPointLights(pointLights, pointLightCount);
		else shaderList[0].SetPointLights(pointLights, 1);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f / 10.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cabana_madera.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-22.0f, 0.0f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		roca.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-22.0f, 0.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla.RenderModel();
				
		//vacas
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-10.0f + movVacaX, 1.05484f, 0.658324f + movVacaZ));
		model = glm::rotate(model, glm::radians(-rotarVaca), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca_cuerpo.RenderModel();
		modelaux = model;
		
		model = glm::translate(model, glm::vec3(0.0f, 0.972919f - 1.05484f, 1.26853f - 0.658324f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca_cabeza.RenderModel();
		
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.83722f - 1.05484f, - 1.30268f - 0.658324f));
		model = glm::rotate(model, glm::sin(glm::radians(rotaCola1ZV)) / 4, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::sin(glm::radians(rotaCola1XV)) / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca_cola1.RenderModel();
		
		model = glm::translate(model, glm::vec3(0.0f, 1.47964f - 1.83722f, - 1.36475f + 1.30268f));
		model = glm::rotate(model, glm::sin(glm::radians(rotaCola2ZV)) / 4, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::sin(glm::radians(rotaCola2XV)) / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca_cola2.RenderModel();
		
		model = glm::translate(model, glm::vec3(0.0f, 0.766128f - 1.47964f, - 1.37366f + 1.36475f));
		model = glm::rotate(model, glm::sin(glm::radians(rotaCola3ZV)) / 4, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::sin(glm::radians(rotaCola3XV)) / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca_cola3.RenderModel();
		
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f - 0.428158f, 0.774082f - 1.05484f, 0.613384f - 0.658324f));
		model = glm::rotate(model, glm::sin(glm::radians(-rotPataVacaFR)) / 4, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca_pataFR.RenderModel();
		
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f + 0.413954f, 0.783064f - 1.05484f, 0.625003f - 0.658324f));
		model = glm::rotate(model, glm::sin(glm::radians(-rotPataVacaFL)) / 4, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca_pataFL.RenderModel();
		
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f - 0.50673f, 0.999737f - 1.05484f, - 0.948187f - 0.658324f));
		model = glm::rotate(model, glm::sin(glm::radians(-rotPataVacaBR)) / 4, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca_pataBR.RenderModel();
		
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f + 0.486654f, 1.00201f - 1.05484f, - 0.903786f - 0.658324f));
		model = glm::rotate(model, glm::sin(glm::radians(-rotPataVacaBL)) / 4, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca_pataBL.RenderModel();
		
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 17.8f));
		model = glm::rotate(model, glm::radians(43.6f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca.RenderModel();
		
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, -15.0f));
		model = glm::rotate(model, glm::radians(-118.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca.RenderModel();
		
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-22.3f, 0.0f, 10.3f));
		model = glm::rotate(model, 63.8f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca.RenderModel();
		
		// ------------------------- caballo ------------------------------------
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(11.0f + 0.0f, 2.1501f, -3.0f + 0.55453f));
		model = glm::rotate(model, -204.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_cuerpo.RenderModel();
		modelaux = model;

		// Cola
		model = glm::translate(model, glm::vec3(0.0f, 2.204f - 2.1501f, -0.55453f - 1.25617f));
		model = glm::rotate(model, glm::sin(glm::radians(rotCola)) / 6, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_cola.RenderModel();
		
		model = glm::translate(model, glm::vec3(0.0f, 1.93774f - 2.204f, 1.25617f - 1.4958f));
		model = glm::rotate(model, glm::sin(glm::radians(rotColaPunta)) / 4, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_colaPunta.RenderModel();

		// cabeza
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.11506f - 2.204f, 1.25726f - 0.55453f));
		model = glm::rotate(model, glm::sin(glm::radians(rotaCabeza)) / 8, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_cabeza.RenderModel();

		// pata delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f - 0.229705f, 1.56881f - 2.1501f, 0.732868f - 0.55453f));
		model = glm::rotate(model, PI * sin(glm::radians(rotPataFRU)) / 3, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_pataFRU.RenderModel();

		model = glm::translate(model, glm::vec3(0.229705f - 0.247778f, 0.784822f - 1.56881f, 0.654164f - 0.732868f));
		model = glm::rotate(model, PI * sin(glm::radians(rotPataFRD)) / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_pataFRD.RenderModel();

		// pata delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f + 0.270485f, 1.57759f - 2.1501f, 0.691724f - 0.55453f));
		model = glm::rotate(model, PI * sin(glm::radians(rotPataFLU)) / 3, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_pataFLU.RenderModel();

		model = glm::translate(model, glm::vec3(0.273987f - 0.270485f, 0.8176f - 1.57759f, 0.661656f - 0.691724f));
		model = glm::rotate(model, PI * sin(glm::radians(rotPataFLD)) / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_pataFLD.RenderModel();

		// pata trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f - 0.28793f, 1.72511f - 2.1501f, -0.55453f - 0.898112f));
		model = glm::rotate(model, PI * sin(glm::radians(rotPataBRU)) / 3.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_pataBRU.RenderModel();

		model = glm::translate(model, glm::vec3(0.28793f - 0.210473f, 0.85076f - 1.72511f, 0.898112f - 1.1546f));
		model = glm::rotate(model, PI * sin(glm::radians(rotPataBRD)) / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_pataBRD.RenderModel();

		// pata trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f + 0.274319f, 1.72279f - 2.1501f, -0.55453f - 0.91318f));
		model = glm::rotate(model, PI * sin(glm::radians(rotPataBLU)) / 3.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_pataBLU.RenderModel();

		model = glm::translate(model, glm::vec3(0.216699f - 0.274319f, 0.874732 - 1.72279f, 0.91318f - 1.16716f));
		model = glm::rotate(model, PI * sin(glm::radians(rotPataBLD)) / 2, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo_pataBLD.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-13.0f, 0.0f, -14.0f));
		model = glm::rotate(model, -82.4f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo.RenderModel();
		
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-13.2f, 0.0f, 15.2f));
		model = glm::rotate(model, 28.4f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo.RenderModel();
		
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(8.0f, 0.0f, 13.5f));
		model = glm::rotate(model, 60.5f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo.RenderModel();
		
		//Fuente 
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(12.0f, 0.0f, 12.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		fuente.RenderModel();

		//vela
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(0.0f, 2.3465f, 8.9925f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vela.RenderModel();
		
		// estrella
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(0.76f, 7.1461f, 4.34f));
		model = glm::rotate(model, 90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estrella.RenderModel();

		// baul
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(0.25f, 0.01f, 3.4f));
		modelaux = model;
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		baulCuerpo.RenderModel();
		
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.06f, 0.8f));
		model = glm::rotate(model, glm::radians(rotaTapaBaul), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		baulTapa.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(0.247015f, 0.5f + movAguilaY, 3.6825f - movAguilaZ));
		model = glm::rotate(model, glm::radians(rotaAguila), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aguila_cuerpo.RenderModel();
		modelaux = model;

		model = glm::translate(model, glm::vec3(-0.08183f, 0.05386f, 0.083179f));
		model = glm::rotate(model, glm::radians(rotaAlasAguila), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aguila_r.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.08183f, 0.05386f, 0.083179f));
		model = glm::rotate(model, glm::radians(-rotaAlasAguila), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aguila_l.RenderModel();
		
		// arco de piedra 
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-16.5318f, 0.0f, -24.3136f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arco.RenderModel();

		// arco de valla
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(8.66738f, 0.451263f, -24.9079f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		valla2.RenderModel();

		// molino
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(15.9798f, 4.84844f, -7.16334f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		molino.RenderModel();

		// lampara 
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(21.8f, 0.0f, -15.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp.RenderModel();

		// lampara
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(21.8f, 0.0f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp.RenderModel();

		// lampara
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(21.8f, 0.0f, 15.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp.RenderModel();

		// lampara
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-21.8f, 0.0f, -15.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp.RenderModel();

		// molino
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(15.9798f, 8.7f, -7.16334f));
		model = glm::rotate(model, mov_mol * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		helice_molino.RenderModel();

		// ------------------- ELEMENTOS CON TRANSPARENCIA -----------------------------
		// arboles
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-22.0f, 0.0f, 6.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		arbol2.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(22.0f, 0.0f, 5.0f));
		model = glm::rotate(model, -35 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol2.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(22.0f, 0.0f, 13.0f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol2.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(22.0f, 0.0f, 20.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol2.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(22.0f, 0.0f, -3.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol2.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(22.0f, 0.0f, -11.0f));
		model = glm::rotate(model, 225 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol2.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(22.0f, 0.0f, -20.0f));
		model = glm::rotate(model, -225 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol2.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-22.0f, 0.0f, -11.0f));
		model = glm::rotate(model, -200 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol2.RenderModel();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-22.0f, 0.0f, 20.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol2.RenderModel();
		
		// paja
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -20.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto.RenderModel();
		
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-20.5f, 0.0f, -1.0f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto.RenderModel();
		
		// piedras del piso
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-2.8854f, 0.001f, 2.0531f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piedras_piso.RenderModel();

		// mariposa
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, 15.0f));
		model = glm::translate(model, glm::vec3(movMariposaX, movMariposaY, movMariposaZ));
		model = glm::rotate(model, glm::radians(anguloMariposa), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		butterfly_body.RenderModel();
		modelaux = model;

		model = glm::rotate(model, PI * sin(glm::radians(rotaAlasButterfly)) / 4, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		butterfly_R.RenderModel();

		model = modelaux;
		model = glm::rotate(model, PI * sin(glm::radians(-rotaAlasButterfly)) / 4, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		butterfly_L.RenderModel();

		// pasto
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pastoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-15.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pastoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 5.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pastoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pastoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
