#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

GLuint VAO;
GLuint VBO;
GLuint CBO;
GLuint macierzShader;

class ProgramMPGK
{
private:
	GLint wysokoscOkna;
	GLint szerokoscOkna;
	GLint polozenieOknaX;
	GLint polozenieOknaY;
	GLuint programZShaderami;

public:
	ProgramMPGK();
	ProgramMPGK(GLint wysokoscOkna, GLint szerokoscOkna, GLint polozenieOknaX, GLint polozenieOknaY);
	void stworzenieOkna(GLint argc, GLchar** argv);
	void inicjalizacjaGlew();
	static void wyswietl();
	void stworzenieVAO();
	void stworzenieVBO();
	void stworzenieCBO();
	void stworzenieProgramu();
	GLuint dodanieDoProgramu(GLuint programZShaderami, const GLchar * tekstShadera, GLenum typShadera);
};


class Wektor {

	float* w;
	int wymiar;


public:

	Wektor();
	Wektor(float w1, float w2, float w3, float w4);
	Wektor(float w1, float w2, float w3);
	Wektor(float w1);
	Wektor(float tab[], int rozmiar);

	void setWektor(float tab[], int rozmiar);
	float* getWektor() const;
	int getWymiar() const;

	float iloczynSkalarny(Wektor wektor);
	Wektor iloczynWektorowy(Wektor wektor);
	float dlugosc();
	void normalizuj();
	void wyswietl();

	Wektor & operator=(const Wektor &wek);
	Wektor operator+(const Wektor &wek);
	Wektor & operator+=(const Wektor & wek);
	Wektor operator-(const Wektor &wek);
	Wektor & operator-=(const Wektor & wek);
	Wektor operator*(const float &number);
	Wektor & operator*=(const float &number);
	friend std::ostream & operator <<(const std::ostream &out, const Wektor& wek);
};


class Matrix {

	float** matrix;
	int wymiar;


public:

	Matrix();
	Matrix(int wymiar);
	Matrix(float** tab, int wymiar);

	void setMatrix(float** tab, int wymiar);
	float** getMatrix() const; 
		float* getMatrix1D();

	float getElement(int x, int y);
	void setElement(float el, int x, int y);
	int getWymiar() const;
	void stworzMacierzJednostkowa(int wymiar);
	void transponujMacierz();

	Matrix & operator=(const Matrix &m);
	Matrix operator+(const Matrix &m);
	Matrix & operator+=(const Matrix &m);
	Matrix operator-(const Matrix &m);
	Matrix & operator-=(const Matrix &m);
	Matrix operator*(const float &number);
	Matrix & operator*=(const float &number);
	Matrix operator*(const Matrix& m);
	Matrix & operator*=(const Matrix& m);
	Wektor operator*(const Wektor& wek);

	friend std::ostream & operator <<(const std::ostream &out, const Matrix& m);
};



class Przeksztalcenia {

	Matrix* matrix;

public:
	Przeksztalcenia();
	Przeksztalcenia(Matrix* matrix);



	/* 3x3 MATRIX */
	Matrix* create3ScaleMatrix(float scale);
	Matrix* create3ScaleMatrix(float scaleX, float scaleY);
	Matrix* create3ScaleMatrix(float scaleArray[]);

	Matrix* create3RotationMatrix(float angle);

	Matrix* create3TranslationMatrix(float translation);
	Matrix* create3TranslationMatrix(float translationX, float translationY);
	Matrix* create3TranslationMatrix(float translationArray[]);


	/* 4x4 MATRIX */
	Matrix* create4ScaleMatrix(float scale);
	Matrix* create4ScaleMatrix(float scaleX, float scaleY, float scaleZ);
	Matrix* create4ScaleMatrix(float scaleArray[]);

	Matrix* create4RotationMatrix(float angle, char axis); /* MUST RETURN INFORMATION ABOUT AXIS (X/Y/Z) */

	Matrix* create4TranslationMatrix(float translation);
	Matrix* create4TranslationMatrix(float translationX, float translationY, float translationZ);
	Matrix* create4TranslationMatrix(float translationArray[]);

	Matrix* multiplyMatrix(Matrix* first, Matrix* second);
	Matrix* multiplyMatrixes(Matrix** matrixes, int number);
	Matrix* calculatePerspectiveMatrix(float fov, float height, float width, float nearZ, float farZ);
	
};