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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;

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

Skybox skybox;

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

	std::vector<std::string> skyboxFaces;
	
	skyboxFaces.push_back("Textures/Skybox/lado2.tga"); //bien 
	skyboxFaces.push_back("Textures/Skybox/lado4.tga"); //bien
	skyboxFaces.push_back("Textures/Skybox/abajo.tga");
	skyboxFaces.push_back("Textures/Skybox/arriba.tga"); // bien 
	skyboxFaces.push_back("Textures/Skybox/lado1.tga"); //bien
	skyboxFaces.push_back("Textures/Skybox/lado3.tga"); // bien 

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir LUZ DEL SOL 
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.5f, 0.3f,
		0.0f, 0.0f, -1.0f);
	////contador de luces puntuales
	//unsigned int pointLightCount = 0;
	////Declaración de primer luz puntual
	//pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f,
	//	0.0f, 2.5f, 1.5f,
	//	0.3f, 0.2f, 0.1f);
	//pointLightCount++;

	//unsigned int spotLightCount = 0;
	////linterna
	//spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
	//	0.0f, 2.0f,
	//	0.0f, 0.0f, 0.0f,
	//	0.0f, -1.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	5.0f);
	//spotLightCount++;

	////luz de helicóptero
	//spotLights[1] = SpotLight(1.0f, 0.0f, 1.0f,
	//	0.0f, 2.0f,
	//	-20.0f, 6.0f, -1.0,
	//	0.0f, -5.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	30.0f);
	//spotLightCount++;

	////luz de faro
	////Luz 1 del carro
	//spotLights[2] = SpotLight(0.0f, 1.0f, 1.0f,
	//	0.0f, 2.0f,
	//	6.1f, -1.0f, -1.0f,
	//	1.0f, -0.5f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	30.0f);
	//spotLightCount++;

	////Luz 2 del carro
	//spotLights[3] = SpotLight(1.0f, 1.0f, 0.0f,
	//	0.0f, 2.0f,
	//	-20.0f, 2.0f, -2.0f,
	//	-1.0f, -0.5f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	30.0f);
	//spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
		
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
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

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
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

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);


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
		model = glm::translate(model, glm::vec3(-7.5f, 0.0f, 0.5f));
		model = glm::rotate(model, glm::radians(-42.3f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		vaca.RenderModel();
		
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
		
		//caballos
		model = glm::mat4(1.0);
		if (camera.getIsometric()) model = camera.ConfIsometric(model);
		model = glm::translate(model, glm::vec3(11.0f, 0.0f, -3.0f));
		model = glm::rotate(model, -204.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		caballo.RenderModel();
		
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
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		baulTapa.RenderModel();

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
		model = glm::translate(model, glm::vec3(22.0f, 0.0f, 20.0f));
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
		
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
