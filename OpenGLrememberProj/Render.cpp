#include <cstdlib>
#include <ctime>
#include "Render.h"
#include <sstream>
#include <iostream>
#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include "MyOGL.h"
#include "Camera.h"
#include "Light.h"
#include "Primitives.h"
#include "GUItextRectangle.h"
#include <cmath>

#define PI 3.14

bool textureMode = true;
bool lightMode = true;
bool textureReplace = true;
bool orbit = true;
bool stop = false;

class SpaceCamera : public Camera
{
public:

	double camDist;
	double angleX, angleY;

	SpaceCamera()
	{
		camDist = 15; //расстояние до Солнца
		angleX = 1; //углы обзора
		angleY = 1;
	}

	void SetUpCamera()
	{			
		lookPoint.setCoords(0, 0, 0);
		//позиция камеры вычисляется через сферические координаты
		pos.setCoords(camDist * cos(angleY) * cos(angleX),
			camDist * cos(angleY) * sin(angleX),
			camDist * sin(angleY));

		if (cos(angleY) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void SpaceCamera::LookAt()
	{
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}

}  camera;

class SpaceLight : public Light
{
public:
	SpaceLight()
	{
		pos = Vector3(0, 0, 0);
	}

	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);
		glColor3d(0.9, 0.8, 0);
	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 }; //фоновый свет
		GLfloat dif[] = { 1.0, 1.0, 1.0, 1.0 };//рассеяный свет
		GLfloat spec[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		glLightfv(GL_LIGHT0, GL_POSITION, position);//позиция света (Солнце)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
		glEnable(GL_LIGHT0);
	}

} light;

int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL* ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.angleX += 0.01 * dx;
		camera.angleY += -0.01 * dy;
	}
}

void mouseWheelEvent(OpenGL* ogl, int delta)
{
	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01 * delta;
}

void keyDownEvent(OpenGL* ogl, int key)
{
	if (key == 'Q')
	{
		orbit = !orbit;
	}
	if (key == 'A')
	{
		stop = !stop;
	}
}

void keyUpEvent(OpenGL* ogl, int key)
{
}

GLuint texId_Sun;
GLuint texId_Mercury;
GLuint texId_Venus;
GLuint texId_Earth;
GLuint texId_Mars;
GLuint texId_Jupiter;
GLuint texId_Saturn;
GLuint texId_Uranus;
GLuint texId_Neptune;
GLuint texId_Moon;
GLuint texId_Asteroid;

void initRender(OpenGL* ogl)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	//Загрузка текстуры Земли
	{
		RGBTRIPLE* texData;
		char* texDataChar;
		int texWidth, texHeight;
		OpenGL::LoadBMP("Earth.bmp", &texWidth, &texHeight, &texData);
		OpenGL::RGBtoChar(texData, texWidth, texHeight, &texDataChar);

		glGenTextures(1, &texId_Earth); //Создание текстуры
		glBindTexture(GL_TEXTURE_2D, texId_Earth); //привязка
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDataChar);

		free(texDataChar);
		free(texData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	{
		RGBTRIPLE* texData2;
		char* texDataChar2;
		int texWidth2, texHeight2;
		OpenGL::LoadBMP("Mercury.bmp", &texWidth2, &texHeight2, &texData2);
		OpenGL::RGBtoChar(texData2, texWidth2, texHeight2, &texDataChar2);

		glGenTextures(1, &texId_Mercury);
		glBindTexture(GL_TEXTURE_2D, texId_Mercury);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth2, texHeight2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDataChar2);

		free(texDataChar2);
		free(texData2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	{
		RGBTRIPLE* texData3;
		char* texDataChar3;
		int texWidth3, texHeight3;
		OpenGL::LoadBMP("Venus.bmp", &texWidth3, &texHeight3, &texData3);
		OpenGL::RGBtoChar(texData3, texWidth3, texHeight3, &texDataChar3);

		glGenTextures(1, &texId_Venus);
		glBindTexture(GL_TEXTURE_2D, texId_Venus);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth3, texHeight3, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDataChar3);

		free(texDataChar3);
		free(texData3);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	{
		RGBTRIPLE* texData4;
		char* texDataChar4;
		int texWidth4, texHeight4;
		OpenGL::LoadBMP("Sun.bmp", &texWidth4, &texHeight4, &texData4);
		OpenGL::RGBtoChar(texData4, texWidth4, texHeight4, &texDataChar4);

		glGenTextures(1, &texId_Sun);
		glBindTexture(GL_TEXTURE_2D, texId_Sun);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth4, texHeight4, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDataChar4);

		free(texDataChar4);
		free(texData4);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	{
		RGBTRIPLE* texData5;
		char* texDataChar5;
		int texWidth5, texHeight5;
		OpenGL::LoadBMP("Mars.bmp", &texWidth5, &texHeight5, &texData5);
		OpenGL::RGBtoChar(texData5, texWidth5, texHeight5, &texDataChar5);

		glGenTextures(1, &texId_Mars);
		glBindTexture(GL_TEXTURE_2D, texId_Mars);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth5, texHeight5, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDataChar5);

		free(texDataChar5);
		free(texData5);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	{
		RGBTRIPLE* texData6;
		char* texDataChar6;
		int texWidth6, texHeight6;
		OpenGL::LoadBMP("Jupiter.bmp", &texWidth6, &texHeight6, &texData6);
		OpenGL::RGBtoChar(texData6, texWidth6, texHeight6, &texDataChar6);

		glGenTextures(1, &texId_Jupiter);
		glBindTexture(GL_TEXTURE_2D, texId_Jupiter);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth6, texHeight6, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDataChar6);

		free(texDataChar6);
		free(texData6);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	{
		RGBTRIPLE* texData7;
		char* texDataChar7;
		int texWidth7, texHeight7;
		OpenGL::LoadBMP("Saturn.bmp", &texWidth7, &texHeight7, &texData7);
		OpenGL::RGBtoChar(texData7, texWidth7, texHeight7, &texDataChar7);

		glGenTextures(1, &texId_Saturn);
		glBindTexture(GL_TEXTURE_2D, texId_Saturn);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth7, texHeight7, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDataChar7);

		free(texDataChar7);
		free(texData7);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	{
		RGBTRIPLE* texData8;
		char* texDataChar8;
		int texWidth8, texHeight8;
		OpenGL::LoadBMP("Uranus.bmp", &texWidth8, &texHeight8, &texData8);
		OpenGL::RGBtoChar(texData8, texWidth8, texHeight8, &texDataChar8);

		glGenTextures(1, &texId_Uranus);
		glBindTexture(GL_TEXTURE_2D, texId_Uranus);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth8, texHeight8, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDataChar8);

		free(texDataChar8);
		free(texData8);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	{
		RGBTRIPLE* texData9;
		char* texDataChar9;
		int texWidth9, texHeight9;
		OpenGL::LoadBMP("Neptune.bmp", &texWidth9, &texHeight9, &texData9);
		OpenGL::RGBtoChar(texData9, texWidth9, texHeight9, &texDataChar9);

		glGenTextures(1, &texId_Neptune);
		glBindTexture(GL_TEXTURE_2D, texId_Neptune);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth9, texHeight9, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDataChar9);

		free(texDataChar9);
		free(texData9);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	{
		RGBTRIPLE* texData10;
		char* texDataChar11;
		int texWidth11, texHeight11;
		OpenGL::LoadBMP("Asteroid.bmp", &texWidth11, &texHeight11, &texData10);
		OpenGL::RGBtoChar(texData10, texWidth11, texHeight11, &texDataChar11);

		glGenTextures(1, &texId_Asteroid);
		glBindTexture(GL_TEXTURE_2D, texId_Asteroid);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth11, texHeight11, 0, GL_RGBA, GL_UNSIGNED_BYTE, texDataChar11);

		free(texDataChar11);
		free(texData10);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	glEnable(GL_NORMALIZE);
	glEnable(GL_LINE_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.angleX = -1.3;
	camera.angleY = 0.8;
}

void CreateSphere(float radius, int stacks, int slices, float startX, float startY, float startZ, bool isSun) {
	for (int i = 0; i <= stacks; ++i) {
		float theta1 = i * M_PI / stacks - M_PI / 2.0f;
		float theta2 = (i + 1) * M_PI / stacks - M_PI / 2.0f;

		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= slices; ++j) {
			float phi = j * 2 * M_PI / slices;
			float x1 = cos(phi) * cos(theta1) * radius + startX;
			float y1 = sin(phi) * cos(theta1) * radius + startY;
			float z1 = sin(theta1) * radius + startZ;
			float u1 = (float)j / slices;
			float v1 = 1.0f - (float)i / stacks;
			glTexCoord2f(u1, v1);
			if (isSun)
				glNormal3f(-(x1 - startX), -(y1 - startY), -(z1 - startZ));
			else
				glNormal3f((x1 - startX), (y1 - startY), (z1 - startZ));
			glVertex3f(x1, y1, z1);
			float x2 = cos(phi) * cos(theta2) * radius + startX;
			float y2 = sin(phi) * cos(theta2) * radius + startY;
			float z2 = sin(theta2) * radius + startZ;
			float u2 = (float)j / slices;
			float v2 = 1.0f - (float)(i + 1) / stacks;
			glTexCoord2f(u2, v2);
			if (isSun)
				glNormal3f(-(x2 - startX), -(y2 - startY), -(z2 - startZ));
			else
				glNormal3f((x2 - startX), (y2 - startY), (z2 - startZ));
			glVertex3f(x2, y2, z2);
		}
		glEnd();
	}
}

float GetRandomFloat() {
	return ((float)rand() / RAND_MAX) * 2 - 1;
}

void DrawStars(float starPositions[][3], int numStars, float starColors[][3]) {
	glPointSize(1.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < numStars; ++i) {
		float x = starPositions[i][0];
		float y = starPositions[i][1];
		float z = starPositions[i][2];
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(x, y, z);
	}
	glEnd();
}

void GenerateStars(float starPositions[][3], int numStars, float sphereRadius) {
	for (int i = 0; i < numStars; ++i) {
		float theta = ((float)rand() / RAND_MAX) * 2 * 3.14159f;
		float phi = acos(1 - 2 * ((float)rand() / RAND_MAX));
		starPositions[i][0] = sphereRadius * sin(phi) * cos(theta);
		starPositions[i][1] = sphereRadius * sin(phi) * sin(theta);
		starPositions[i][2] = sphereRadius * cos(phi);
	}
}

void DrawRing(GLfloat innerRadius, GLfloat outerRadius, GLint segments, GLfloat offsetX, GLfloat offsetY, GLfloat offsetZ) {
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= segments; ++i) {
		GLfloat theta = 2.0f * M_PI * float(i) / float(segments);
		GLfloat x = cos(theta);
		GLfloat y = sin(theta);

		glNormal3f(0.0f, 1.0f, 0.0f);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(innerRadius * x + offsetX, innerRadius * y + offsetY, offsetZ);
		glVertex3f(outerRadius * x + offsetX, outerRadius * y + offsetY, offsetZ);
	}
	glEnd();
}

void DrawOrbit(float radius, int segments) {
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < segments; ++i) {
		float theta = 2.0f * M_PI * float(i) / float(segments);
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);
		glVertex2f(x, y);
	}
	glEnd();
}

struct Asteroid {
	float posX, posY, posZ;
	float size;
};

const int numAsteroids = 250;
Asteroid asteroids[numAsteroids];

bool starsGenerated = false;
const int numStars = 9500;
const int numColors = 9500;
float starPositions[numStars][3];
float starColors[numColors][3];
int scaleFactor = 10;
double speedFactor = 1;

float rotationAngle = 0.0f;

void Render(OpenGL* ogl)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);

	GLfloat amb[] = { 0.2, 0.2, 0.2, 1. };
	GLfloat dif[] = { 1.0, 1.0, 1.0, 1. };
	GLfloat spec[] = { 1.0, 1.0, 1.0, 1. };
	GLfloat sh = 0.1f * 256;

	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		glMaterialf(GL_FRONT, GL_SHININESS, sh);
	glShadeModel(GL_SMOOTH);

	if (speedFactor < 50) {
		if (OpenGL::isKeyPressed('W')) { speedFactor = speedFactor + 1; }
	}

	if (speedFactor > 1) {
		if (OpenGL::isKeyPressed('S')) { speedFactor = speedFactor - 1; }
	}

	if (scaleFactor > 10) {
		if (OpenGL::isKeyPressed('P')) { scaleFactor = scaleFactor - 1; }
	}

	float SunSize = 30.0f / scaleFactor;
	float MercurySize = 9.0f / scaleFactor;
	float VenusSize = 9.0f / scaleFactor;
	float EarthSize = 12.0f / scaleFactor;
	float MarsSize = 9.0f / scaleFactor;
	float JupiterSize = 20.1f / scaleFactor;
	float SaturnSize = 20.8f / scaleFactor;
	float UranusSize = 20.5f / scaleFactor;
	float NeptuneSize = 20.5f / scaleFactor;

	float MercuryOrbitRadius = 60.0f / scaleFactor;
	float VenusOrbitRadius = 140.0f / scaleFactor;
	float EarthOrbitRadius = 220.0f / scaleFactor;
	float MarsOrbitRadius = 300.0f / scaleFactor;
	float beltRadius = 380.0f;
	float beltWidth = 10.0f;
	float JupiterOrbitRadius = 460.0f / scaleFactor;
	float SaturnOrbitRadius = 540.0f / scaleFactor;
	float UranusOrbitRadius = 620.0f / scaleFactor;
	float NeptuneOrbitRadius = 700.0f / scaleFactor;

	if (orbit) {
		glDisable(GL_LIGHTING);
		DrawOrbit(MercuryOrbitRadius, 40);
		DrawOrbit(VenusOrbitRadius, 40);
		DrawOrbit(EarthOrbitRadius, 40);
		DrawOrbit(MarsOrbitRadius, 40);
		DrawOrbit(beltRadius / scaleFactor, 40);
		DrawOrbit(JupiterOrbitRadius, 40);
		DrawOrbit(SaturnOrbitRadius, 40);
		DrawOrbit(UranusOrbitRadius, 40);
		DrawOrbit(NeptuneOrbitRadius, 40);
		glEnable(GL_LIGHTING);
	}

	float MercuryOrbitSpeed = 0.04147f * speedFactor;
	float VenusOrbitSpeed = 0.01622f * speedFactor;
	float EarthOrbitSpeed = 0.01f * speedFactor;
	float MarsOrbitSpeed = 0.005f * speedFactor;
	float JupiterOrbitSpeed = 0.001f * speedFactor;
	float SaturnOrbitSpeed = 0.0009f * speedFactor;
	float UranusOrbitSpeed = 0.0007f * speedFactor;
	float NeptuneOrbitSpeed = 0.0004f * speedFactor;

	static float MercuryAngle = 0.0f;
	static float VenusAngle = 0.0f;
	static float EarthAngle = 0.0f;
	static float MarsAngle = 0.0f;
	static float JupiterAngle = 0.0f;
	static float SaturnAngle = 0.0f;
	static float UranusAngle = 0.0f;
	static float NeptuneAngle = 0.0f;

	float UniversalZ = 0.0;

	float MercuryX = MercuryOrbitRadius * cos(MercuryAngle);
	float MercuryY = MercuryOrbitRadius * sin(MercuryAngle);

	float VenusX = VenusOrbitRadius * cos(VenusAngle);
	float VenusY = VenusOrbitRadius * sin(VenusAngle);

	float EarthX = EarthOrbitRadius * cos(EarthAngle);
	float EarthY = EarthOrbitRadius * sin(EarthAngle);

	float MarsX = MarsOrbitRadius * cos(MarsAngle);
	float MarsY = MarsOrbitRadius * sin(MarsAngle);

	float JupiterX = JupiterOrbitRadius * cos(JupiterAngle);
	float JupiterY = JupiterOrbitRadius * sin(JupiterAngle);

	float SaturnX = SaturnOrbitRadius * cos(SaturnAngle);
	float SaturnY = SaturnOrbitRadius * sin(SaturnAngle);

	float UranusX = UranusOrbitRadius * cos(UranusAngle);
	float UranusY = UranusOrbitRadius * sin(UranusAngle);

	float NeptuneX = NeptuneOrbitRadius * cos(NeptuneAngle);
	float NeptuneY = NeptuneOrbitRadius * sin(NeptuneAngle);

	float SaturnRingInnerRadius = 1.5 / scaleFactor;
	float SaturnRingOuterRadius = 7.5 / scaleFactor;

	float UranusRingInnerRadius = 1.5 / scaleFactor;
	float UranusRingOuterRadius = 7.5 / scaleFactor;

	float SaturnRingX = SaturnOrbitRadius * cos(SaturnAngle);
	float SaturnRingY = SaturnOrbitRadius * sin(SaturnAngle);

	float UranusRingX = UranusOrbitRadius * cos(UranusAngle);
	float UranusRingY = UranusOrbitRadius * sin(UranusAngle);

	static float SunRotationAngle = 0.0f;
	static float MercuryRotationAngle = 0.0f;
	static float VenusRotationAngle = 0.0f;
	static float EarthRotationAngle = 0.0f;
	static float MarsRotationAngle = 0.0f;
	static float JupiterRotationAngle = 0.0f;
	static float SaturnRotationAngle = 0.0f;
	static float UranusRotationAngle = 0.0f;
	static float NeptuneRotationAngle = 0.0f;

	float SunRotationSpeed = 0.3f * speedFactor;
	float MercuryRotationSpeed = 3.0f * speedFactor;
	float VenusRotationSpeed = 3.0f * speedFactor;
	float EarthRotationSpeed = 3.0f * speedFactor;
	float MarsRotationSpeed = 3.0f * speedFactor;
	float JupiterRotationSpeed = 2.0f * speedFactor;
	float SaturnRotationSpeed = 2.0f * speedFactor;
	float UranusRotationSpeed = 2.0f * speedFactor;
	float NeptuneRotationSpeed = 2.0f * speedFactor;

	static float beltRotationAngle = 0.0f;
	float beltRotationSpeed = 0.3f * speedFactor;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDisable(GL_LIGHTING);

	if (starsGenerated == false) {
		GenerateStars(starPositions, numStars, 500.0f);
	}
	DrawStars(starPositions, numStars, starColors);

	starsGenerated = true;

	glEnable(GL_LIGHTING);

	glPushMatrix();
	glRotatef(beltRotationAngle, 0.0f, 0.0f, 1.0f);
	glPopMatrix();

	//Солнце
	glBindTexture(GL_TEXTURE_2D, texId_Sun);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(SunRotationAngle, 0.0f, 0.0f, 1.0f);
	CreateSphere(SunSize, 20, 20, 0, 0, 0, true);
	glPopMatrix();

	//Меркурий
	glBindTexture(GL_TEXTURE_2D, texId_Mercury);
	glPushMatrix();
	glTranslatef(MercuryX, MercuryY, UniversalZ);
	glRotatef(MercuryRotationAngle, 0.0f, 0.0f, 1.0f);
	CreateSphere(MercurySize, 20, 20, 0, 0, 0, false);
	glPopMatrix();
	MercuryAngle += MercuryOrbitSpeed;

	//Венера
	glBindTexture(GL_TEXTURE_2D, texId_Venus);
	glPushMatrix();
	glTranslatef(VenusX, VenusY, UniversalZ);
	glRotatef(VenusRotationAngle, 0.0f, 0.0f, 1.0f);
	CreateSphere(VenusSize, 20, 20, 0, 0, 0, false);
	glPopMatrix();
	VenusAngle += VenusOrbitSpeed;

	//Земля
	glBindTexture(GL_TEXTURE_2D, texId_Earth);
	glPushMatrix();
	glTranslatef(EarthX, EarthY, UniversalZ);
	glRotatef(EarthRotationAngle, 0.0f, 0.0f, 1.0f);
	CreateSphere(EarthSize, 20, 20, 0, 0, 0, false);
	glPopMatrix();
	EarthAngle += EarthOrbitSpeed;

	//Марс
	glBindTexture(GL_TEXTURE_2D, texId_Mars);
	glPushMatrix();
	glTranslatef(MarsX, MarsY, UniversalZ);
	glRotatef(EarthRotationAngle, 0.0f, 0.0f, 1.0f);
	CreateSphere(MarsSize, 20, 20, 0, 0, 0, false);
	glPopMatrix();
	MarsAngle += MarsOrbitSpeed;

	//Юпитер
	glBindTexture(GL_TEXTURE_2D, texId_Jupiter);
	glPushMatrix();
	glTranslatef(JupiterX, JupiterY, UniversalZ);
	glRotatef(JupiterRotationAngle, 0.0f, 0.0f, 1.0f);
	CreateSphere(JupiterSize, 20, 20, 0, 0, 0, false);
	glPopMatrix();
	JupiterAngle += JupiterOrbitSpeed;

	//Сатурн
	glBindTexture(GL_TEXTURE_2D, texId_Saturn);
	glPushMatrix();
	glTranslatef(SaturnX, SaturnY, UniversalZ);
	glRotatef(SaturnRotationAngle, 0.0f, 0.0f, 1.0f);
	CreateSphere(SaturnSize, 20, 20, 0, 0, 0, false);
	glPopMatrix();
	DrawRing(SaturnSize + SaturnRingInnerRadius, SaturnSize + SaturnRingOuterRadius, 30, SaturnRingX, SaturnRingY, 0);
	SaturnAngle += SaturnOrbitSpeed;

	//Уран
	glBindTexture(GL_TEXTURE_2D, texId_Uranus);
	glPushMatrix();
	glTranslatef(UranusX, UranusY, UniversalZ);
	glRotatef(UranusRotationAngle, 0.0f, 0.0f, 1.0f);
	CreateSphere(UranusSize, 20, 20, 0, 0, 0, false);
	glPopMatrix();
	DrawRing(UranusSize + UranusRingInnerRadius, UranusSize + UranusRingOuterRadius, 30, UranusRingX, UranusRingY, 0);
	UranusAngle += UranusOrbitSpeed;

	//Нептун
	glBindTexture(GL_TEXTURE_2D, texId_Neptune);
	glPushMatrix();
	glTranslatef(NeptuneX, NeptuneY, UniversalZ);
	glRotatef(NeptuneRotationAngle, 0.0f, 0.0f, 1.0f);
	CreateSphere(NeptuneSize, 20, 20, 0, 0, 0, false);
	glPopMatrix();
	NeptuneAngle += NeptuneOrbitSpeed;

	if (!stop) {
		beltRotationAngle = beltRotationAngle + beltRotationSpeed;
		SunRotationAngle = SunRotationAngle + SunRotationSpeed;
		MercuryRotationAngle = MercuryRotationAngle + MercuryRotationSpeed;
		VenusRotationAngle = VenusRotationAngle + VenusRotationSpeed;
		EarthRotationAngle = EarthRotationAngle + EarthRotationSpeed;
		MarsRotationAngle = MarsRotationAngle + MarsRotationSpeed;
		JupiterRotationAngle = JupiterRotationAngle + JupiterRotationSpeed;
		SaturnRotationAngle = SaturnRotationAngle + SaturnRotationSpeed;
		UranusRotationAngle = UranusRotationAngle + UranusRotationSpeed;
		NeptuneRotationAngle = NeptuneRotationAngle + NeptuneRotationSpeed;
	}

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);

	GuiTextRectangle rec;
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);

	std::stringstream ss;
	ss << "W/S - Ускорить/Замедлить - " << "Скорость: X" << speedFactor << std::endl;
	ss << "Q - Вкл/Выкл линии орбит планет" << std::endl;
	ss << "A - Вкл/Выкл орбиту планеты" << std::endl;


	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}