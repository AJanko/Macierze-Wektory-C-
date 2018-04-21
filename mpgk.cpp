#include "mpgk.h"


using namespace std;

class Wektor;
class Matrix;
class Przeksztalcenia;

ProgramMPGK::ProgramMPGK()
{
	wysokoscOkna = 768;
	szerokoscOkna = 1024;
	polozenieOknaX = 100;
	polozenieOknaY = 100;
}

ProgramMPGK::ProgramMPGK(GLint wysokoscOkna, GLint szerokoscOkna, GLint polozenieOknaX, GLint polozenieOknaY)
{
	this->wysokoscOkna = wysokoscOkna;
	this->szerokoscOkna = szerokoscOkna;
	this->polozenieOknaX = polozenieOknaX;
	this->polozenieOknaY = polozenieOknaY;
}

void ProgramMPGK::stworzenieOkna(GLint argc, GLchar** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);
	glutInitWindowPosition(polozenieOknaX, polozenieOknaY);
	glutCreateWindow("Program MPGK");
}

void ProgramMPGK::inicjalizacjaGlew()
{
	GLenum wynik = glewInit();
	if (wynik != GLEW_OK)
	{
		std::cerr << "Nie udalo sie zainicjalizowac GLEW. Blad: " << glewGetErrorString(wynik) << std::endl;
		system("pause");
		exit(1);
	}
}

void  ProgramMPGK::wyswietl()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	
	/*GLfloat mJ[4][4];

	mJ[0][0] = 1.0f;  mJ[0][1] = 0.0f;	mJ[0][2] = 0.0f;  mJ[0][3] = 0.0f;
	mJ[1][0] = 0.0f;  mJ[1][1] = 1.0f;	mJ[1][2] = 0.0f;  mJ[1][3] = 0.0f;
	mJ[2][0] = 0.0f;  mJ[2][1] = 0.0f;  mJ[2][2] = 1.0f;  mJ[2][3] = 0.0f;
	mJ[3][0] = 0.0f;  mJ[3][1] = 0.0f;	mJ[3][2] = 0.0f;  mJ[3][3] = 1.0f;*/
	
	//glUniformMatrix4fv(macierzShader, 1, GL_TRUE, &mJ[0][0]);
	
	
	Matrix* mJ;	
	mJ = new Matrix();
	Przeksztalcenia prz;

	/*Matrix* matrixArray[3];
	matrixArray[2] = prz.create4RotationMatrix(30,'X');
	matrixArray[1] = prz.create4ScaleMatrix(0.5);
	matrixArray[0] = prz.create4TranslationMatrix(0.8);
	mJ = prz.multiplyMatrixes(matrixArray, 3);*/
	//90,768,1024-1-10
	//mJ = prz.calculatePerspectiveMatrix(60, 1300, 900, 1, 1000);

	Matrix* matrixArray[4];

	matrixArray[0] = prz.create4RotationMatrix(0, 'Z');
	matrixArray[1] = prz.create4ScaleMatrix(1.0);
	matrixArray[2] = prz.create4TranslationMatrix(0.0f, 0.0f, 7.9f);
	matrixArray[3] = prz.calculatePerspectiveMatrix(90, 768, 1024, -0.1, -10);
	
	mJ = prz.multiplyMatrixes(matrixArray, 4);
	
	
	glUniformMatrix4fv(macierzShader, 1, GL_TRUE, mJ->getMatrix1D());



	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutSwapBuffers();
}

void ProgramMPGK::stworzenieVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

void ProgramMPGK::stworzenieVBO()
{
	GLfloat wierzcholki[9] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wierzcholki), wierzcholki, GL_STATIC_DRAW);
}

void ProgramMPGK::stworzenieCBO()
{
	GLfloat kolory[] =
	{
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f
	};

	glGenBuffers(1, &CBO);
	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(kolory), kolory, GL_STATIC_DRAW);
}

void ProgramMPGK::stworzenieProgramu()
{
	programZShaderami = glCreateProgram();

	if (programZShaderami == 0)
	{
		std::cerr << "Blad podczas tworzenia progrmu shaderow." << std::endl;
		system("pause");
		exit(1);
	}

	GLchar * vertexShader =
		"	#version 330\n																			\
			layout (location = 0) in vec3 polozenie;												\
			layout (location = 1) in vec4 kolorPrzychodzacyVS;										\
			uniform mat4 macierz;																	\
			out vec4 kolorWychodzacyVS;																\
			void main()																				\
			{																						\
				 gl_Position = macierz * vec4(polozenie, 1.0);										\
				 kolorWychodzacyVS = kolorPrzychodzacyVS;											\
			}";

	GLchar * fragmentShader =
		"	#version 330\n									\
			in vec4 kolorWychodzacyVS;						\
			out vec4 kolorWychodzacyFS;						\
			void main()										\
			{												\
				kolorWychodzacyFS = kolorWychodzacyVS;	    \
			}";

	dodanieDoProgramu(programZShaderami, vertexShader, GL_VERTEX_SHADER);
	dodanieDoProgramu(programZShaderami, fragmentShader, GL_FRAGMENT_SHADER);

	GLint linkowanieOK = 0;
	glLinkProgram(programZShaderami);
	glGetProgramiv(programZShaderami, GL_LINK_STATUS, &linkowanieOK);
	if (linkowanieOK == GL_FALSE)
	{
		GLint dlugoscLoga = 0;
		glGetProgramiv(programZShaderami, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar> log(dlugoscLoga);
		glGetProgramInfoLog(programZShaderami, dlugoscLoga, NULL, &log[0]);
		std::cerr << "Blad podczas linkowania programu shaderow." << std::endl;
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); ++i)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteProgram(programZShaderami);
		system("pause");
		exit(1);
	}

	GLint walidacjaOK = 0;
	glValidateProgram(programZShaderami);
	glGetProgramiv(programZShaderami, GL_VALIDATE_STATUS, &walidacjaOK);
	if (walidacjaOK == GL_FALSE)
	{
		GLint dlugoscLoga = 0;
		glGetProgramiv(programZShaderami, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar> log(dlugoscLoga);
		glGetProgramInfoLog(programZShaderami, dlugoscLoga, NULL, &log[0]);
		std::cerr << "Blad podczas walidacji programu shaderow." << std::endl;
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); ++i)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteProgram(programZShaderami);
		system("pause");
		exit(1);
	}

	glUseProgram(programZShaderami);

	macierzShader = glGetUniformLocation(programZShaderami, "macierz");
	if (macierzShader == -1)
	{
		std::cerr << "Nie znaleziono zmiennej uniform." << std::endl;
		system("pause");
		exit(1);
	}
}

GLuint ProgramMPGK::dodanieDoProgramu(GLuint programZShaderami, const GLchar * tekstShadera, GLenum typShadera)
{
	GLuint shader = glCreateShader(typShadera);

	GLchar * typShaderaTekst = typShadera == 35633 ? "vertex" : "fragment";

	if (shader == 0)
	{
		std::cerr << "Blad podczas tworzenia " << typShaderaTekst << " shadera." << std::endl;
		system("pause");
		exit(0);
	}

	const GLchar * tekstShaderaTab[1];
	tekstShaderaTab[0] = tekstShadera;
	GLint dlugoscShadera[1];
	dlugoscShadera[0] = strlen(tekstShadera);
	glShaderSource(shader, 1, tekstShaderaTab, dlugoscShadera);

	glCompileShader(shader);
	GLint kompilacjaOK = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &kompilacjaOK);
	if (kompilacjaOK == GL_FALSE)
	{
		GLint dlugoscLoga = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar> log(dlugoscLoga);
		glGetShaderInfoLog(shader, dlugoscLoga, NULL, &log[0]);
		std::cerr << "Blad podczas kompilacji " << typShaderaTekst << " shadera." << std::endl;
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); ++i)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteShader(shader);
		system("pause");
		exit(1);
	}

	glAttachShader(programZShaderami, shader);
	return shader;
}

GLint main(GLint argc, GLchar** argv)
{
	ProgramMPGK obiektMPGK(786, 1024, 100, 100);

	obiektMPGK.stworzenieOkna(argc, argv);
	obiektMPGK.inicjalizacjaGlew();
	obiektMPGK.stworzenieVAO();
	obiektMPGK.stworzenieVBO();
	obiektMPGK.stworzenieCBO();
	obiektMPGK.stworzenieProgramu();
	glutDisplayFunc(obiektMPGK.wyswietl);
	glutIdleFunc(obiektMPGK.wyswietl);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	std::cout << "Wersja GL: " << glGetString(GL_VERSION) << std::endl;


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glutMainLoop();

	return 0;
}





//#include "matrix.h"
//#include "przeksztalcenia.h"

//Matrix::wymiar = 4;

/* MATRIX */
Matrix::Matrix() {}


Matrix::Matrix(int wymiar) {

	this->wymiar = wymiar;
	matrix = new float*[(int)wymiar];
	for (int i = 0; i<wymiar; i++)
		matrix[i] = new float[(int)wymiar];
	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			matrix[i][j] = 0;
}



Matrix::Matrix(float** tab, int wymiar) {

	this->wymiar = wymiar;
	matrix = new float*[wymiar];
	for (int i = 0; i<wymiar; i++)
		matrix[i] = new float[wymiar];

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			matrix[i][j] = tab[i][j];
}



void Matrix::setMatrix(float** tab, int wymiar) {

	this->wymiar = wymiar;

	for (int i = 0; i<wymiar; i++)
		delete[] tab[i];
	delete[] * tab;


	matrix = new float*[wymiar];
	for (int i = 0; i<wymiar; i++)
		matrix[i] = new float[wymiar];

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			matrix[i][j] = tab[i][j];
}



float** Matrix::getMatrix() const { return matrix; }


float* Matrix::getMatrix1D() {

	int k = 0;
	float* mat = new float[wymiar*wymiar];
	for (int i = 0; i < wymiar; i++)
		for (int j = 0; j < wymiar; j++) {
			mat[k] = matrix[i][j];
			k++;
		}
			

	return mat;
}



int Matrix::getWymiar() const { return wymiar; }



Matrix & Matrix::operator=(const Matrix &m) {

	this->setMatrix(m.getMatrix(), m.getWymiar());
	return *this;
}



Matrix Matrix::operator+(const Matrix &m) {

	if (this->getWymiar() != m.getWymiar()) {

		cout << "Macierze posiadaj¹ ró¿ne wymiary" << endl;
		Matrix* matr = new Matrix();
		return *matr;
	}

	float** tempM;
	tempM = new float*[this->wymiar];
	for (int i = 0; i<wymiar; i++)
		tempM[i] = new float[wymiar];

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			tempM[i][j] = this->getMatrix()[i][j] + m.getMatrix()[i][j];


	return Matrix(tempM, this->getWymiar());
}


Matrix & Matrix::operator+=(const Matrix &m) {

	if (this->getWymiar() != m.getWymiar()) {
		cout << "Macierze posiadaj¹ ró¿ne wymiary" << endl;
		Matrix* m = new Matrix();
		return *m;
	}

	float** tempM;
	tempM = new float*[this->wymiar];
	for (int i = 0; i<wymiar; i++)
		tempM[i] = new float[wymiar];

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			tempM[i][j] = this->getMatrix()[i][j] + m.getMatrix()[i][j];

	this->setMatrix(tempM, this->getWymiar());
	return *this;
}



Matrix Matrix::operator-(const Matrix &m) {

	if (this->getWymiar() != m.getWymiar()) {

		cout << "Macierze posiadaj¹ ró¿ne wymiary" << endl;
		Matrix* matr = new Matrix();
		return *matr;
	}

	float** tempM;
	tempM = new float*[this->wymiar];
	for (int i = 0; i<wymiar; i++)
		tempM[i] = new float[wymiar];

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			tempM[i][j] = this->getMatrix()[i][j] - m.getMatrix()[i][j];

	return Matrix(tempM, this->getWymiar());
}



Matrix & Matrix::operator-=(const Matrix &m) {

	if (this->getWymiar() != m.getWymiar()) {
		cout << "Macierze posiadaj¹ ró¿ne wymiary" << endl;
		Matrix* m = new Matrix();
		return *m;
	}

	float** tempM;
	tempM = new float*[this->wymiar];
	for (int i = 0; i<wymiar; i++)
		tempM[i] = new float[wymiar];

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			tempM[i][j] = this->getMatrix()[i][j] - m.getMatrix()[i][j];

	this->setMatrix(tempM, this->getWymiar());
	return *this;
}



Matrix Matrix::operator*(const float &number) {

	float** tempM;
	tempM = new float*[this->wymiar];
	for (int i = 0; i<wymiar; i++)
		tempM[i] = new float[wymiar];

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			tempM[i][j] = this->getMatrix()[i][j] * number;

	return Matrix(tempM, this->getWymiar());
}



Matrix & Matrix::operator*=(const float &number) {

	float** tempM;
	tempM = new float*[this->wymiar];
	for (int i = 0; i<wymiar; i++)
		tempM[i] = new float[wymiar];

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			tempM[i][j] = this->getMatrix()[i][j] * number;

	this->setMatrix(tempM, this->getWymiar());
	return *this;
}



Matrix Matrix::operator*(const Matrix& m) {

	if (this->getWymiar() != m.getWymiar()) {

		cout << "Macierze posiadaj¹ ró¿ne wymiary" << endl;
		Matrix* matr = new Matrix();
		return *matr;
	}

	float** tempM;
	tempM = new float*[this->wymiar];
	for (int i = 0; i<wymiar; i++)
		tempM[i] = new float[wymiar];

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			tempM[i][j] = 0;


	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			for (int k = 0; k<wymiar; k++)
				tempM[i][j] += this->getMatrix()[i][k] * m.getMatrix()[k][j];

	return Matrix(tempM, this->getWymiar());
}



Matrix & Matrix::operator*=(const Matrix& m) {

	if (this->getWymiar() != m.getWymiar()) {

		cout << "Macierze posiadaj¹ ró¿ne wymiary" << endl;
		Matrix* temp = new Matrix();
		return *temp;
	}

	float** tempM;
	tempM = new float*[this->wymiar];
	for (int i = 0; i<wymiar; i++)
		tempM[i] = new float[wymiar];

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			tempM[i][j] = 0;

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++)
			for (int k = 0; k<wymiar; k++)
				tempM[i][j] += this->getMatrix()[i][k] * m.getMatrix()[k][j];

	this->setMatrix(tempM, this->getWymiar());
	return *this;
}


Wektor Matrix::operator*(const Wektor& wek) {

	if (this->getWymiar() != wek.getWymiar()) {

		cout << "Macierze posiadaj¹ ró¿ne wymiary" << endl;
		Wektor* wektor = new Wektor();
		return *wektor;
	}

	float* tempM;
	tempM = new float[this->getWymiar()];

	for (int i = 0; i<this->getWymiar(); i++)
		for (int j = 0; j<this->getWymiar(); j++)
			tempM[i] += this->getMatrix()[j][i] * wek.getWektor()[i];

	return Wektor(tempM, this->getWymiar());
}



ostream & operator <<(ostream &out, const Matrix& m) {

	for (int i = 0; i<m.getWymiar(); i++) {
		for (int j = 0; j<m.getWymiar(); j++)
			out << m.getMatrix()[i][j] << "   ";
		out << endl;
	}

	return out;
}



float Matrix::getElement(int x, int y) {

	if (x >= wymiar || y >= wymiar || x<0 || y<0) {
		cout << "Dany element wychodzi poza zakres macierzy." << endl;
		float* fl;
		fl = NULL;
		return *fl;
	}

	return this->getMatrix()[y][x];
}



void Matrix::setElement(float el, int x, int y) {

	if (x >= wymiar || y >= wymiar || x<0 || y<0)
		cout << "Dany element wychodzi poza zakres macierzy." << endl;
	else
		this->matrix[y][x] = el;
}



void Matrix::stworzMacierzJednostkowa(int wymiar) {

	float** tempM;
	tempM = new float*[wymiar];
	for (int i = 0; i<wymiar; i++)
		tempM[i] = new float[wymiar];

	for (int i = 0; i<wymiar; i++)
		for (int j = 0; j<wymiar; j++) {
			if (i == j)
				tempM[i][j] = 1;
			else
				tempM[i][j] = 0;
		}

	this->setMatrix(tempM, wymiar);
}



void Matrix::transponujMacierz() {

	float** tempM;
	tempM = new float*[this->wymiar];
	for (int i = 0; i<wymiar; i++)
		tempM[i] = new float[this->wymiar];

	for (int i = 0; i < wymiar; i++)
		for (int j = 0; j < wymiar; j++)
			tempM[i][j] = this->getMatrix()[j][i];

	this->setMatrix(tempM, this->getWymiar());
}



/* WEKTOR*/
Wektor::Wektor() {}


Wektor::Wektor(float w1, float w2, float w3, float w4) {

	this->wymiar = 4;
	w = new float[wymiar];
	w[0] = w1;
	w[1] = w2;
	w[2] = w3;
	w[3] = w4;
}


Wektor::Wektor(float w1, float w2, float w3) {

	wymiar = 3;
	w = new float[wymiar];
	w[0] = w1;
	w[1] = w2;
	w[2] = w3;
}


Wektor::Wektor(float w1) {

	wymiar = 1;
	w = new float[wymiar];
	w[0] = w1;
}


Wektor::Wektor(float tab[], int rozmiar) {

	wymiar = rozmiar;
	w = new float[wymiar];
	for (int i = 0; i<wymiar; i++)
		w[i] = tab[i];
}


void Wektor::setWektor(float tab[], int rozmiar) {

	wymiar = rozmiar;
	w = new float[wymiar];
	for (int i = 0; i<wymiar; i++)
		w[i] = tab[i];
}


float* Wektor::getWektor() const { return w; }


int Wektor::getWymiar() const { return wymiar; }


float Wektor::iloczynSkalarny(Wektor wektor) {

	if (this->getWymiar() != wektor.getWymiar()) {
		cout << "Nie da siê obliczyæ iloczynu skalarnego wektorów o ró¿nej liczbie wymiarów" << endl;
		return 0;
	}

	float skalar = 0;
	for (int i = 0; i<wymiar; i++)
		skalar += this->getWektor()[i] + wektor.getWektor()[i];

	return skalar;
}


Wektor Wektor::iloczynWektorowy(Wektor wektor) {

	if (this->getWymiar() != wektor.getWymiar()) {
		cout << "Nie da siê obliczyæ iloczynu wektorowego wektorów o ró¿nej liczbie wymiarów" << endl;
		return 0;
	}

	float* wektor_wynikowy = new float[this->getWymiar()];

	if (wymiar == 3) {
		wektor_wynikowy[0] = this->getWektor()[1] * wektor.getWektor()[2] - this->getWektor()[2] * wektor.getWektor()[1];
		wektor_wynikowy[1] = this->getWektor()[0] * wektor.getWektor()[2] - this->getWektor()[2] * wektor.getWektor()[0];
		wektor_wynikowy[2] = this->getWektor()[0] * wektor.getWektor()[1] - this->getWektor()[1] * wektor.getWektor()[0];
	}

	else
		cout << "Wektory posiadaj¹ nieprawid³owy wymiar" << endl;

	return Wektor(wektor_wynikowy, wymiar);
}


float Wektor::dlugosc() {

	float suma = 0;
	for (int i = 0; i<wymiar; i++)
		suma += getWektor()[i] * getWektor()[i];
	return sqrt(suma);
}


void Wektor::normalizuj() {

	float* tempWektor = new float[this->getWymiar()];
	float dlugosc = this->dlugosc();
	for (int i = 0; i<this->getWymiar(); i++)
		tempWektor[i] = this->getWektor()[i] / dlugosc;
	this->setWektor(tempWektor, this->getWymiar());
}


void Wektor::wyswietl() {

	cout << "[";
	for (int i = 0; i<this->getWymiar(); i++)
		cout << this->getWektor()[i] << ",";
	cout << "\b]";
}


Wektor & Wektor::operator=(const Wektor &wek) {

	this->setWektor(wek.getWektor(), wek.getWymiar());
	return *this;
}


Wektor Wektor::operator+(const Wektor &wek) {

	if (this->getWymiar() != wek.getWymiar()) {

		cout << "wektory posiadaj¹ ró¿ne wymiary" << endl;
		return NULL;
	}

	float* wektorTab = new float[this->getWymiar()];
	for (int i = 0; i< this->getWymiar(); i++)
		wektorTab[i] = this->getWektor()[i] + wek.getWektor()[i];

	return Wektor(wektorTab, this->getWymiar());
}


Wektor & Wektor::operator+=(const Wektor & wek) {

	if (this->getWymiar() != wek.getWymiar()) {

		cout << "wektory posiadaj¹ ró¿ne wymiary" << endl;
		Wektor* t = new Wektor();
		return *t;
	}

	float* wektorTab = new float[this->getWymiar()];
	for (int i = 0; i< this->getWymiar(); i++)
		wektorTab[i] = this->getWektor()[i] + wek.getWektor()[i];

	this->setWektor(wektorTab, this->getWymiar());
	return *this;
}


Wektor Wektor::operator-(const Wektor &wek) {

	if (this->getWymiar() != wek.getWymiar()) {

		cout << "wektory posiadaj¹ ró¿ne wymiary" << endl;
		return NULL;
	}

	float* wektorTab = new float[this->getWymiar()];
	for (int i = 0; i< this->getWymiar(); i++)
		wektorTab[i] = this->getWektor()[i] - wek.getWektor()[i];

	return Wektor(wektorTab, this->getWymiar());
}



Wektor & Wektor::operator-=(const Wektor & wek) {

	if (this->getWymiar() != wek.getWymiar()) {

		cout << "wektory posiadaj¹ ró¿ne wymiary" << endl;
		Wektor* t = new Wektor();
		return *t;
	}

	float* wektorTab = new float[this->getWymiar()];
	for (int i = 0; i< this->getWymiar(); i++)
		wektorTab[i] = this->getWektor()[i] - wek.getWektor()[i];

	this->setWektor(wektorTab, this->getWymiar());
	return *this;
}



Wektor Wektor::operator*(const float &number) {


	float* wektorTab = new float[this->getWymiar()];
	for (int i = 0; i< this->getWymiar(); i++)
		wektorTab[i] = this->getWektor()[i] * number;

	return Wektor(wektorTab, this->getWymiar());
}



Wektor & Wektor::operator*=(const float &number) {

	float* wektorTab = new float[this->getWymiar()];
	for (int i = 0; i< this->getWymiar(); i++)
		wektorTab[i] = this->getWektor()[i] * number;

	this->setWektor(wektorTab, this->getWymiar());
	return *this;
}


ostream & operator <<(ostream &out, const Wektor& wek) {

	out << "[";
	for (int i = 0; i<wek.getWymiar(); i++) {
		out << wek.getWektor()[i] << ",";
		cout << wek.getWektor()[i];
	}
	out << "]";
	return out;
}




/* PRZEKSZTALCENIA */
Przeksztalcenia::Przeksztalcenia() {}


Przeksztalcenia::Przeksztalcenia(Matrix* matrix) { this->matrix = matrix; }


Matrix* Przeksztalcenia::create3ScaleMatrix(float scale) {

	matrix = new Matrix(3);
	matrix->setElement(scale, 0, 0);
	matrix->setElement(scale, 1, 1);
	matrix->setElement(1, 2, 2);
	return matrix;
}


Matrix* Przeksztalcenia::create3ScaleMatrix(float scaleX, float scaleY) {

	this->matrix = new Matrix(3);
	matrix->setElement(scaleX, 0, 0);
	matrix->setElement(scaleY, 1, 1);
	matrix->setElement(1, 2, 2);
	return matrix;
}


Matrix* Przeksztalcenia::create3ScaleMatrix(float scaleArray[]) {

	this->matrix = new Matrix(3);
	matrix->setElement(scaleArray[0], 0, 0);
	matrix->setElement(scaleArray[1], 1, 1);
	matrix->setElement(1, 2, 2);
	return matrix;
}


Matrix* Przeksztalcenia::create3RotationMatrix(float angle) {

	matrix = new Matrix(3);
	matrix->setElement(cos(angle), 0, 0);
	matrix->setElement(sin(angle), 1, 0);
	matrix->setElement(-sin(angle), 0, 1);
	matrix->setElement(cos(angle), 1, 1);
	return matrix;
}


Matrix* Przeksztalcenia::create3TranslationMatrix(float translation) {

	matrix = new Matrix(3);
	matrix->setElement(1, 0, 0);
	matrix->setElement(1, 1, 1);
	matrix->setElement(1, 2, 2);
	matrix->setElement(translation, 0, 2);
	matrix->setElement(translation, 1, 2);
	return matrix;
}


Matrix* Przeksztalcenia::create3TranslationMatrix(float translationX, float translationY) {

	matrix = new Matrix(3);
	matrix->setElement(1, 0, 0);
	matrix->setElement(1, 1, 1);
	matrix->setElement(1, 2, 2);
	matrix->setElement(translationX, 0, 2);
	matrix->setElement(translationY, 1, 2);
	return matrix;
}


Matrix* Przeksztalcenia::create3TranslationMatrix(float translationArray[]) {

	matrix = new Matrix(3);
	matrix->setElement(1, 0, 0);
	matrix->setElement(1, 1, 1);
	matrix->setElement(1, 2, 2);
	matrix->setElement(translationArray[0], 0, 2);
	matrix->setElement(translationArray[1], 1, 2);
	return matrix;
}




Matrix* Przeksztalcenia::create4ScaleMatrix(float scale) {

	matrix = new Matrix(4);
	matrix->setElement(scale, 0, 0);
	matrix->setElement(scale, 1, 1);
	matrix->setElement(scale, 2, 2);
	matrix->setElement(1, 3, 3);
	return matrix;
}


Matrix* Przeksztalcenia::create4ScaleMatrix(float scaleX, float scaleY, float scaleZ) {

	matrix = new Matrix(4);
	matrix->setElement(scaleX, 0, 0);
	matrix->setElement(scaleY, 1, 1);
	matrix->setElement(scaleZ, 2, 2);
	matrix->setElement(1, 3, 3);
	return matrix;
}


Matrix* Przeksztalcenia::create4ScaleMatrix(float scaleArray[]) {

	matrix = new Matrix(4);
	matrix->setElement(scaleArray[0], 0, 0);
	matrix->setElement(scaleArray[1], 1, 1);
	matrix->setElement(scaleArray[2], 2, 2);
	matrix->setElement(1, 3, 3);
	return matrix;
}


Matrix* Przeksztalcenia::create4RotationMatrix(float angle, char axis) {

	matrix = new Matrix(4);
	if (axis == 'x' || axis == 'X') {
		matrix->setElement(1, 0, 0);
		matrix->setElement(cos(angle), 1, 1);
		matrix->setElement(-sin(angle), 1, 2);
		matrix->setElement(sin(angle), 2, 1);
		matrix->setElement(cos(angle), 2, 2);
		matrix->setElement(1, 3, 3);
	}

	else if (axis == 'y' || axis == 'Y') {
		matrix->setElement(cos(angle), 0, 0);
		matrix->setElement(sin(angle), 0, 2);
		matrix->setElement(1, 1, 1);
		matrix->setElement(-sin(angle), 2, 0);
		matrix->setElement(cos(angle), 2, 2);
		matrix->setElement(1, 3, 3);
	}

	else if (axis == 'z' || axis == 'Z') {
		matrix->setElement(cos(angle), 0, 0);
		matrix->setElement(-sin(angle), 0, 1);
		matrix->setElement(sin(angle), 1, 0);
		matrix->setElement(cos(angle), 1, 1);
		matrix->setElement(1, 2, 2);
		matrix->setElement(1, 3, 3);
	}

	return matrix;
}


Matrix* Przeksztalcenia::create4TranslationMatrix(float translation) {

	matrix = new Matrix(4);
	matrix->setElement(1, 0, 0);
	matrix->setElement(1, 1, 1);
	matrix->setElement(1, 2, 2);
	matrix->setElement(1, 3, 3);
	matrix->setElement(translation, 3, 0);
	matrix->setElement(translation, 3, 1);
	matrix->setElement(translation, 3, 2);
	return matrix;
}


Matrix* Przeksztalcenia::create4TranslationMatrix(float translationX, float translationY, float translationZ) {

	matrix = new Matrix(4);
	matrix->setElement(1, 0, 0);
	matrix->setElement(1, 1, 1);
	matrix->setElement(1, 2, 2);
	matrix->setElement(1, 3, 3);
	matrix->setElement(translationX, 3, 0);
	matrix->setElement(translationY, 3, 1);
	matrix->setElement(translationZ, 3, 2);
	return matrix;
}


Matrix* Przeksztalcenia::create4TranslationMatrix(float translationArray[]) {

	matrix = new Matrix(4);
	matrix->setElement(1, 0, 0);
	matrix->setElement(1, 1, 1);
	matrix->setElement(1, 2, 2);
	matrix->setElement(1, 3, 3);
	matrix->setElement(translationArray[0], 3, 0);
	matrix->setElement(translationArray[1], 3, 1);
	matrix->setElement(translationArray[2], 3, 2);
	return matrix;
}



Matrix* Przeksztalcenia::multiplyMatrix(Matrix* first, Matrix* second) {

	if (first->getWymiar() != second->getWymiar()) {
		cout << "Macierze posiadaj¹ ró¿ny wymiar. Mno¿enie nie jest mo¿liwe.";
		Matrix *mat = new Matrix();
		return mat;
	}

	int wymiar = first->getWymiar();
	Matrix* mat = new Matrix(wymiar);

	for (int a = 0; a<wymiar; a++)
		for (int b = 0; b<wymiar; b++)
			for (int c = 0; c<wymiar; c++)
				mat->setElement(mat->getElement(b, a) + first->getElement(c, a)*second->getElement(c, b), b, a); // kolejnosc b,a bo funkcja ta ma odwrotnie parametry niz sa indeksowane tablice
	return mat;
}



Matrix* Przeksztalcenia::multiplyMatrixes(Matrix** matrixes, int number) {

	for (int i = 0; i<number - 1; i++)
		if (matrixes[i]->getWymiar() != matrixes[i + 1]->getWymiar()) {
			Matrix* returnResult = new Matrix();
			return returnResult;
		}

	Matrix* mat;
	mat = multiplyMatrix(matrixes[0], matrixes[1]);

	for (int j = 2; j<number; j++)
		mat = multiplyMatrix(mat, matrixes[j]);

	return mat;
}



Matrix* Przeksztalcenia::calculatePerspectiveMatrix(float fov, float height, float width, float nearZ, float farZ) {    //kat w stopniach

	Matrix* mat;
	mat = new Matrix(4);

	float aspect = width / height;
	
	

	mat->setElement(1/aspect * tan(fov*M_PI / 180 /2), 0, 0);
	mat->setElement(1/tan(fov*M_PI / 180 /2), 1, 1);
	mat->setElement(-(nearZ + farZ) / (nearZ-farZ), 2, 2);
	mat->setElement(2 * nearZ*farZ / (nearZ - farZ), 3, 2);
	mat->setElement(1, 2, 3);

	return mat;
}



