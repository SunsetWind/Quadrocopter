#include "Render.h"

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"
#include <math.h>
#include <stdlib.h>

double vr = 1000;
double time = 0;
double t = 0;
double ax = 0;
double ay = 0;
double az = 0;
double vx = 0;
double vy = 0;
double vz = -49.5;
double sx = 0;
double sy = 0;
double suny = 0;
double sunz = 0;
double start[] = { 0, 0, -49.5 };
double finish[] = { 0, 0, -49.5 };
double RSF;

int win = 0;
int lose = 0;

bool day = false;
bool eng = false;
bool retx = false;
bool rety = false;
bool fyp = false;
bool fym = false;
bool fxp = false;
bool fxm = false;
double normal[3];
double pi = acos(-1);
bool textureMode = true;
bool lightMode = true;
bool alpha = false;
bool vd = true;
bool g = false;
bool gamestarted = false;

GLuint texId[3];

GLfloat ambd[] = { 0.2, 0.2, 0.1, 1. };
GLfloat difd[] = { 0.4, 0.65, 0.5, 1. };
GLfloat specd[] = { 0.9, 0.8, 0.3, 1. };
GLfloat shd = 0.1f * 256;

GLfloat specg[] = { 1, 0, 0, 1 };

GLfloat ambn[] = { 0, 0, 0, 1 };
GLfloat ambn1[] = { 0.1, 0.1, 0.1, 1 };
GLfloat difn[] = { 0, 0, 0, 1 };
GLfloat difn1[] = { 0.1, 0.1, 0.1, 1 };
GLfloat specn[] = { 0.1, 0.1, 0.1, 1 };
GLfloat specn1[] = { 0.2, 0.2, 0.2, 1 };
GLfloat shn = 0;
GLfloat shn1 = 0.1f * 256;

//класс для настройки камеры
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;

	
	//значния камеры по умолчанию
	CustomCamera()
	{
		camDist = 20;
		fi1 = 1;
		fi2 = 0.5;
	}

	
	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(vx, vy, vz);

		/*pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));*/

		pos.setCoords(vx + camDist*cos(fi2)*cos(fi1),
			vy + camDist*cos(fi2)*sin(fi1),
			vz + camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		/*if (pos.X() < -46)
			pos = Vector3(-45, pos.Y(), pos.Z());
		else if (pos.X() > 46)
			pos = Vector3(45, pos.Y(), pos.Z());
		if (pos.Y() < -46)
			pos = Vector3(pos.X(), -45, pos.Z());
		else if (pos.Y() > 46)
			pos = Vector3(pos.X(), 45, pos.Z());
			else if (pos.Z() > 50)
			pos = Vector3(pos.X(), pos.Y(), 49);*/
		if (pos.Z() < -50)
			pos = Vector3(pos.X(), pos.Y(), -49);
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //создаем объект камеры


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(0, 0, 49);
	}

	
	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		//glColor3d(0.9, 0.8, 0);
		//Sphere s;
		//s.pos = pos;
		//s.scale = s.scale*0.08;
		//s.Show();
		//
		//if (OpenGL::isKeyPressed('G'))
		//{
		//	glColor3d(0, 0, 0);
		//	//линия от источника света до окружности
		//	glBegin(GL_LINES);
		//	glVertex3d(pos.X(), pos.Y(), pos.Z());
		//	glVertex3d(pos.X(), pos.Y(), 0);
		//	glEnd();

		//	//рисуем окруность
		//	Circle c;
		//	c.pos.setCoords(pos.X(), pos.Y(), 0);
		//	c.scale = c.scale*1.5;
		//	c.Show();
		//}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 1 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1 };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света

//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//двигаем свет по плоскости, в точку где мышь
	/*if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}*/

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 5)
		return;
	if (delta > 0 && camera.camDist >= 30)
		return;

	camera.camDist += 0.005*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'P')
	{
		g = false;
		gamestarted = false;
	}
	if (key == '1')
	{
		g = true;
		RSF = 9;
		start[0] = -200 + rand() % 400;
		start[1] = -200 + rand() % 400;
		finish[0] = -200 + rand() % 400;
		finish[1] = -200 + rand() % 400;
		vx = start[0];
		vy = start[1];
		vz = start[2];
		eng = false;
		gamestarted = false;
	}
	if (key == '2')
	{
		g = true;
		RSF = 6;
		start[0] = -200 + rand() % 400;
		start[1] = -200 + rand() % 400;
		finish[0] = -200 + rand() % 400;
		finish[1] = -200 + rand() % 400;
		vx = start[0];
		vy = start[1];
		vz = start[2];
		eng = false;
		gamestarted = false;
	}
	if (key == '3')
	{
		g = true;
		RSF = 3;
		start[0] = -200 + rand() % 400;
		start[1] = -200 + rand() % 400;
		finish[0] = -200 + rand() % 400;
		finish[1] = -200 + rand() % 400;
		vx = start[0];
		vy = start[1];
		vz = start[2];
		eng = false;
		gamestarted = false;
	}
	if (key == 'L')
	{
		lightMode = !lightMode;
		if (alpha)
		{
			glDisable(GL_BLEND);
			alpha = false;
		}
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
		if (alpha)
		{
			glDisable(GL_BLEND);
			alpha = false;
		}
	}

	if (key == 'R')
	{
		eng = false;
		t = 0;
		camera.fi1 = 1;
		camera.fi2 = 0.5;
		camera.camDist = 20;
		ax = 0;
		ay = 0;
		az = 0;
		vx = 0;
		vy = 0;
		vz = -49.5;
	}
	if (key == 'C')
	{
		if (alpha)
		{
			glDisable(GL_BLEND);
			alpha = false;
		}
		else
		{
			lightMode = false;
			textureMode = false;

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			alpha = true;
		}
	}

	if (key == 'W' && eng)
	{
		if (vz>-49.5 && !fyp && !fym)
		{
			if (az >= -45 && az <= 45 && (vy > 400 - abs(vx) || vx < -400 + abs(vy) || vy < -400 + abs(vx)))
			{
				if (ay>0)
					fyp = true;
				else if (ay < 0)
					fym = true; 
				return;
			}
			else if (az >= 45 && az <= 135 && (vx > 400 - abs(vy) || vy < -400 + abs(vx) || vx < -400 + abs(vy)))
			{
				if (ay>0)
					fyp = true;
				else if (ay < 0)
					fym = true;
				return;
			}
			else if ((az >= 135 && az <= 180 || az >= -180 && az <= -135) && (vy > 400 - abs(vx) || vx > 400 - abs(vy) || vy < -400 + abs(vx)))
			{
				if (ay>0)
					fyp = true;
				else if (ay < 0)
					fym = true;
				return;
			}
			else if (az >= -135 && az <= -45 && (vx > 400 - abs(vy) || vy > 400 - abs(vx) || vx < -400 + abs(vy)))
			{
				if (ay>0)
					fyp = true;
				else if (ay < 0)
					fym = true;
				return;
			}
			else
			{
				if (ay>-7)
				{
					ay -= 0.1;
				}
				sx = ay*0.2*cos(az*pi / 180);
				sy = ay*0.2*sin(az*pi / 180);
				vx += sx;
				vy += sy;
			}
		}
	}
	if (key == 'S'&& eng)
	{
		if (vz>-49.5 && !fyp && !fym)
		{
			if (az >= -45 && az <= 45 && (vy > 400 - abs(vx) || vx > 400 - abs(vy) || vy < -400 + abs(vx)))
			{
				if (ay>0)
					fyp = true;
				else if (ay < 0)
					fym = true;
				return;
			}
			else if (az >= 45 && az <= 135 && (vx > 400 - abs(vy) || vy > 400 - abs(vx) || vx < -400 + abs(vy)))
			{
				if (ay>0)
					fyp = true;
				else if (ay < 0)
					fym = true;
				return;
			}
			else if ((az >= 135 && az <= 180 || az >= -180 && az <= -135) && (vy > 400 - abs(vx) || vx < -400 + abs(vy) || vy < -400 + abs(vx)))
			{
				if (ay>0)
					fyp = true;
				else if (ay < 0)
					fym = true;
				return;
			}
			else if (az >= -135 && az <= -45 && (vx > 400 - abs(vy) || vy < -400 + abs(vx) || vx < -400 + abs(vy)))
			{
				if (ay>0)
					fyp = true;
				else if (ay < 0)
					fym = true;
				return;
			}
			else
			{
				if (ay<7)
				{
					ay += 0.1;
				}
				sx = ay*0.2*cos(az*pi / 180);
				sy = ay*0.2*sin(az*pi / 180);
				vx += sx;
				vy += sy;
			}
		}
	}
	if (key == 'A'&& eng)
	{
		if (vz>-49.5 && !fxp && !fxm)
		{
			if (az >= -45 && az <= 45 && (vx > 400 - abs(vy) || vy < -400 + abs(vx) || vx < -400 + abs(vy)))
			{
				if (ax>0)
					fxp = true;
				else if (ax < 0)
					fxm = true;
				return;
			}
			else if (az >= 45 && az <= 135 && (vy > 400 - abs(vx) || vx > 400 - abs(vy) || vy < -400 + abs(vx)))
			{
				if (ax>0)
					fxp = true;
				else if (ax < 0)
					fxm = true;
				return;
			}
			else if ((az >= 135 && az <= 180 || az >= -180 && az <= -135) && (vx > 400 - abs(vy) || vy > 400 - abs(vx) || vx < -400 + abs(vy)))
			{
				if (ax>0)
					fxp = true;
				else if (ax < 0)
					fxm = true;
				return;
			}
			else if (az >= -135 && az <= -45 && (vy > 400 - abs(vx) || vx < -400 + abs(vy) || vy < -400 + abs(vx)))
			{
				if (ax>0)
					fxp = true;
				else if (ax < 0)
					fxm = true;
				return;
			}
			else
			{
				if (ax<7)
				{
					ax += 0.1;
				}
				sx = ax*0.2*sin(az*pi / 180);
				sy = ax*0.2*cos(az*pi / 180);
				vx += sx;
				vy -= sy;
			}
		}
	}
	if (key == 'D' && eng)
	{
		if (vz>-49.5 && !fxp && !fxm)
		{
			if (az >= -45 && az <= 45 && (vx > 400 - abs(vy) || vy > 400 - abs(vx) || vx < -400 + abs(vy)))
			{
				if (ax > 0)
					fxp = true;
				else if (ax < 0)
					fxm = true;
				return;
			}
			else if (az >= 45 && az <= 135 && (vy > 400 - abs(vx) || vx < -400 + abs(vy) || vy < -400 + abs(vx)))
			{
				if (ax>0)
					fxp = true;
				else if (ax < 0)
					fxm = true;
				return;
			}
			else if ((az >= 135 && az <= 180 || az >= -180 && az <= -135) && (vx > 400 - abs(vy) || vy < -400 + abs(vx) || vx < -400400 + abs(vy)))
			{
				if (ax>0)
					fxp = true;
				else if (ax < 0)
					fxm = true;
				return;
			}
			else if (az >= -135 && az <= -45 && (vy > 400 - abs(vx) || vx > 400 - abs(vy) || vy < -400 + abs(vx)))
			{
				if (ax>0)
					fxp = true;
				else if (ax < 0)
					fxm = true;
				return;
			}
			else
			{
				if (ax>-7)
				{
					ax -= 0.1;
				}
				sx = ax*0.2*sin(az*pi / 180);
				sy = ax*0.2*cos(az*pi / 180);
				vx += sx;
				vy -= sy;
			}
		}
	}
	if (key == 'Q' && eng && vz > -49.5)
	{
		if (ay > 0 && !fxp && !fxm&& !fyp && !fym)
			fyp = true;
		else if (ay < 0 && !fxp && !fxm&& !fyp && !fym)
			fym = true;
		if (ax>0 && !fxp && !fxm&& !fyp && !fym)
			fxp = true;
		else if (ax < 0 && !fxp && !fxm&& !fyp && !fym)
			fxm = true;

		if (az <= 180)
			az += 2;
		if (az > 180)
			az = -178;
	}
	if (key == 'E' && eng && vz > -49.5)
	{
		if (ay > 0 && !fxp && !fxm&& !fyp && !fym)
			fyp = true;
		else if (ay < 0 && !fxp && !fxm&& !fyp && !fym)
			fym = true;
		if (ax>0 && !fxp && !fxm&& !fyp && !fym)
			fxp = true;
		else if (ax < 0 && !fxp && !fxm&& !fyp && !fym)
			fxm = true;

		if (az > -180)
			az -= 2;
		if (az <= -180)
			az = 180;
	}
	if (key == ' ' && eng)
	{
		if (!g)
		{
			if (ay > 0 && !fxp && !fxm&& !fyp && !fym)
				fyp = true;
			else if (ay < 0 && !fxp && !fxm&& !fyp && !fym)
				fym = true;
			if (ax>0 && !fxp && !fxm&& !fyp && !fym)
				fxp = true;
			else if (ax < 0 && !fxp && !fxm&& !fyp && !fym)
				fxm = true;
			if (vz < 50.5)
				vz += 0.5;
		}
	}
	if (key == 'Z' && eng)
	{
		if (!g)
		{
			if (ay>0 && !fxp && !fxm&& !fyp && !fym)
				fyp = true;
			else if (ay < 0 && !fxp && !fxm&& !fyp && !fym)
				fym = true;
			if (ax>0 && !fxp && !fxm&& !fyp && !fym)
				fxp = true;
			else if (ax < 0 && !fxp && !fxm&& !fyp && !fym)
				fxm = true;
			if (vz > -49.5)
				vz -= 0.5;
			else
				vz = -49.5;
		}
	}
	if (key == 'X' && ay==0 && ax==0)
	{
		if (eng)
		{
			eng = false;
		}
		else if (!gamestarted)
		{
			eng = true;
			if (g)
				gamestarted = true;
		}
	}
	else if (key == 'X')
	{
		if (ay>0)
			fyp = true;
		else if (ay < 0)
			fym = true;
		if (ax>0)
			fxp = true;
		else if (ax < 0)
			fxm = true;
	}
	if (key == 39)
	{
		if (vr == 17)
			vr = 33;
		else if (vr == 33)
			vr = 100;
		else if (vr == 100)
			vr *= 5;
		else if (vr == 500)
			vr *= 2;
		else if (vr == 1000)
			vr *= 2;
		else if (vr == 2000)
			vr *= 5;
		else if (vr == 10000)
			vr *= 3;
		else if (vr == 30000)
			vr *= 2;
		else
			return;
		vr = round(vr);
	}
	if (key == 37)
	{
		if (vr == 60000)
			vr /= 2;
		else if (vr == 30000)
			vr /= 3;
		else if (vr == 10000)
			vr /= 5;
		else if (vr == 2000)
			vr /= 2;
		else if (vr == 1000)
			vr /= 2;
		else if (vr == 500)
			vr /= 5;
		else if (vr == 100)
			vr = 33;
		else if (vr == 33)
			vr = 17;
		else
			return;
		vr = round(vr);
	}
	if (key == 40)
	{
		vr = 1000;
	}
	if (key == 38)
	{
		vr = 17;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	if (key == 'W')
	{
		if (ay>0)
			fyp = true;
		else if (ay < 0)
			fym = true;
	}
	if (key == 'S')
	{
		if (ay>0)
			fyp = true;
		else if (ay < 0)
			fym = true;
	}
	if (key == 'A')
	{
		if (ax>0)
			fxp = true;
		else if (ax < 0)
			fxm = true;
	}
	if (key == 'D')
	{
		if (ax>0)
			fxp = true;
		else if (ax < 0)
			fxm = true;
	}
}

//выполняется перед первым рендером
void initRender(OpenGL *ogl)
{	
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);


	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE *texarray;
	RGBTRIPLE *texarray1; //я
	RGBTRIPLE *texarray2;
	RGBTRIPLE *texarray3;

	//массив символов, (высота*ширина*4      4, потомучто   выше, мы указали использовать по 4 байта на пиксель текстуры - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("background.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	/*
	GLuint texId;*/
	//генерируем ИД для текстуры
	glGenTextures(5, texId);
	//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glBindTexture(GL_TEXTURE_2D, texId[0]);

	//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//отчистка памяти
	free(texCharArray);
	free(texarray);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	char *texCharArray1; //я

	OpenGL::LoadBMP("model.bmp", &texW, &texH, &texarray1); //я
	OpenGL::RGBtoChar(texarray1, texW, texH, &texCharArray1); //я

	glBindTexture(GL_TEXTURE_2D, texId[1]); //я

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray1); //я

	free(texCharArray1); //я
	free(texarray1); //я

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	char *texCharArray2; //я

	OpenGL::LoadBMP("pol.bmp", &texW, &texH, &texarray2); //я
	OpenGL::RGBtoChar(texarray2, texW, texH, &texCharArray2); //я

	glBindTexture(GL_TEXTURE_2D, texId[2]); //я

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray2); //я

	free(texCharArray2); //я
	free(texarray2); //я

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	char *texCharArray3; //я

	OpenGL::LoadBMP("startfinish.bmp", &texW, &texH, &texarray3); //я
	OpenGL::RGBtoChar(texarray3, texW, texH, &texCharArray3); //я

	glBindTexture(GL_TEXTURE_2D, texId[3]); //я

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray3); //я

	free(texCharArray3); //я
	free(texarray3); //я

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH);


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
}

void norm(double A[], double B[], double C[], double vec[])
{
	double v1[] = { A[0] - B[0], A[1] - B[1], A[2] - B[2] };
	double v2[] = { C[0] - B[0], C[1] - B[1], C[2] - B[2] };
	vec[0] = v1[1] * v2[2] - v2[1] * v1[2];
	vec[1] = -v1[0] * v2[2] + v2[0] * v1[2];
	vec[2] = v1[0] * v2[1] - v2[0] * v1[1];

	double len = pow((pow(vec[0], 2) + pow(vec[1], 2) + pow(vec[2], 2)), 0.5);
	vec[0] = vec[0] / len;
	vec[1] = vec[1] / len;
	vec[2] = vec[2] / len;
}

void sun(double sy, double sz)
{//радиус и количество полигонов в разные плоскости,на сколько частей резать сферу
	float x1, y1, z1, x2, y2, z2;
	float R = 50;
	int s2 = 90;
	int s1 = 90;
	glTranslated(0, sy, sz);
	for (int i = 0; i < s1; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= s2; j++)
		{
			x1 = R * sin(-2 * M_PI * j / s2) * sin(M_PI * i / s1);
			x2 = R * sin(-2 * M_PI * j / s2) * sin(M_PI * (i + 1) / s1);
			y1 = R * cos(-2 * M_PI * j / s2) * sin(M_PI * i / s1);
			y2 = R * cos(-2 * M_PI * j / s2) * sin(M_PI * (i + 1) / s1);
			z1 = R * cos(M_PI * i / s1);
			z2 = R * cos(M_PI * (i + 1) / s1);
			glVertex3d(x1, y1, z1);
			glVertex3d(x2, y2, z2);
		}
		glEnd();
	}
	glTranslated(0, -sy, -sz);
}

void game(double O1[], double O2[], double R)
{
	glBindTexture(GL_TEXTURE_2D, texId[3]);
	for (int i = 0; i <= 360; i++)
	{
		glBegin(GL_TRIANGLES);
		glTexCoord2d(250.0 / 1024.0, 1 - 250 / 1024.0);
		glVertex3dv(O1);
		glTexCoord2d((250.0 - 250.0 * sin(i*pi / 180)) / 1024.0, 1 - (250.0 + 250.0 * cos(i*pi / 180)) / 1024.0);
		glVertex3d(O1[0] + R*sin(i*pi / 180), O1[1] + R*cos(i*pi / 180), O1[2]);
		glTexCoord2d((250.0 - 250.0 * sin((i + 1)*pi / 180)) / 1024.0, 1 - (250.0 + 250.0 * cos((i + 1)*pi / 180)) / 1024.0);
		glVertex3d(O1[0] + R*sin((i + 1)*pi / 180), O1[1] + R*cos((i + 1)*pi / 180), O1[2]);
		glEnd();

		glBegin(GL_TRIANGLES);
		glTexCoord2d(760.0 / 1024.0, 1 - 250 / 1024.0);
		glVertex3dv(O2);
		glTexCoord2d((760.0 - 250.0 * sin(i*pi / 180)) / 1024.0, 1 - (250.0 + 250.0 * cos(i*pi / 180)) / 1024.0);
		glVertex3d(O2[0] + R*sin(i*pi / 180), O2[1] + R*cos(i*pi / 180), O2[2]);
		glTexCoord2d((760.0 - 250.0 * sin((i + 1)*pi / 180)) / 1024.0, 1 - (250.0 + 250.0 * cos((i + 1)*pi / 180)) / 1024.0l);
		glVertex3d(O2[0] + R*sin((i + 1)*pi / 180), O2[1] + R*cos((i + 1)*pi / 180), O2[2]);
		glEnd();
	}
	for (int i = 0; i <= 360; i++)
	{
		if (gamestarted && vz == -49.5 && vx > O2[0] - R*sin(i*pi / 180) && vx<O2[0] + R*sin(i*pi / 180) && vy>O2[1] - R*cos(i*pi / 180) && vy < O2[1] + R*cos(i*pi / 180))
		{
			g = false;
			gamestarted = false;
			win++;
		}
	}
	for (int i = 0; i <= 360; i++)
	{
		if (gamestarted && vz == -49.5 && (vx < O2[0] - R*sin(i*pi / 180) || vx > O2[0] + R*sin(i*pi / 180) || vy < O2[1] - R*cos(i*pi / 180) || vy > O2[1] + R*cos(i*pi / 180)))
		{
			g = false;
			gamestarted = false;
			lose++;
		}
	}
}

void room()
{
	double a[] = { 100, 100, -50 };
	double b[] = { 100, -100, -50 };
	double c[] = { -100, -100, -50 };
	double d[] = { -100, 100, -50 };
	double A[] = { 100, 100, 50 };
	double B[] = { 100, -100, 50 };
	double C[] = { -100, -100, 50 };
	double D[] = { -100, 100, 50 };

	/*/стены//
	glColor3d(1,1,0);
	glBegin(GL_QUADS);
	norm(a,A,B,normal);
	glNormal3dv(normal);
	glVertex3dv(A);
	glVertex3dv(a);
	glVertex3dv(b);
	glVertex3dv(B);

	norm(b, B, C, normal);
	glNormal3dv(normal);
	glVertex3dv(B);
	glVertex3dv(b); 
	glVertex3dv(c);
	glVertex3dv(C);

	norm(c, C, D, normal);
	glNormal3dv(normal);
	glVertex3dv(C);
	glVertex3dv(c); 
	glVertex3dv(d);
	glVertex3dv(D);

	norm(d, D, A, normal);
	glNormal3dv(normal);
	glVertex3dv(D);
	glVertex3dv(d); 
	glVertex3dv(a);
	glVertex3dv(A);
	glEnd();
	glColor3d(0, 1, 0);
	//стены/*/

	/*/потолок/пол//
	glBegin(GL_QUADS);
	norm(a, b, c, normal);
	glNormal3dv(normal);
	glVertex3dv(a);
	glVertex3dv(b);
	glVertex3dv(c);
	glVertex3dv(d);

	norm(A, B, C, normal);
	glNormal3dv(normal);
	glVertex3dv(A);
	glVertex3dv(B);
	glVertex3dv(C);
	glVertex3dv(D);
	glEnd();
	//потолок/пол/*/

	glBindTexture(GL_TEXTURE_2D, texId[0]);
	double O[] = { 0, 0, 0 };
	double R = 500;
	double r = 505;
	//цилиндр/стены//
	glColor4d(0, 1, 1, 0);
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 360; i++)
	{
		double X[3] = { O[0] + (R*sin(i*pi / 180)), O[1] + (R*cos(i*pi / 180)), O[2] - 51 };
		double Y[3] = { O[0] + (R*sin(i*pi / 180)), O[1] + (R*cos(i*pi / 180)), O[2] + 101 };
		double Z[3] = { O[0] + (R*sin((i + 1)*pi / 180)), O[1] + (R*cos((i + 1)*pi / 180)), O[2] - 51 };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glTexCoord2d(500.0 / 1024.0, 1 - (1.0 + i * 784 / 360) / 1024.0);
		glVertex3d(O[0] + R*sin(i*pi / 180), O[1] + R*cos(i*pi / 180), O[2] - 51);
		glTexCoord2d(512.0 / 1024.0, 1 - (1.0 + i * 785 / 360) / 1024.0);
		glVertex3d(O[0] + R*sin(i*pi / 180), O[1] + R*cos(i*pi / 180), O[2] + 101);
		glTexCoord2d(500.0 / 1024.0, 1 - (1.0 + i * 785 / 360) / 1024.0);
		glVertex3d(O[0] + R*sin((i + 1)*pi / 180), O[1] + R*cos((i + 1)*pi / 180), O[2] - 51);
		glTexCoord2d(512.0 / 1024.0, 1 - (1.0 + i * 784 / 360) / 1024.0);
		glVertex3d(O[0] + R*sin((i + 1)*pi / 180), O[1] + R*cos((i + 1)*pi / 180), O[2] + 101);
	}
	glEnd();
	glColor4d(0, 1, 0, 1);
	//цилиндр/стены//

	/*///
	R = 600;
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 360; i++)
	{
		double X[3] = { O[0] + (R*sin(i*pi / 180)), O[1] + (R*cos(i*pi / 180)), O[2] - 51 };
		double Y[3] = { O[0] + (R*sin(i*pi / 180)), O[1] + (R*cos(i*pi / 180)), O[2] + 101 };
		double Z[3] = { O[0] + (R*sin((i + 1)*pi / 180)), O[1] + (R*cos((i + 1)*pi / 180)), O[2] - 51 };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(O[0] + r*sin(i*pi / 180), O[1] + r*cos(i*pi / 180), O[2] - 51);
		glVertex3d(O[0] + R*sin(i*pi / 180), O[1] + R*cos(i*pi / 180), O[2]);
		glVertex3d(O[0] + r*sin((i + 1)*pi / 180), O[1] + r*cos((i + 1)*pi / 180), O[2]-51);
		glVertex3d(O[0] + R*sin((i + 1)*pi / 180), O[1] + R*cos((i + 1)*pi / 180), O[2]);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 360; i++)
	{
		double X[3] = { O[0] + (R*sin(i*pi / 180)), O[1] + (R*cos(i*pi / 180)), O[2] - 51 };
		double Y[3] = { O[0] + (R*sin(i*pi / 180)), O[1] + (R*cos(i*pi / 180)), O[2] + 101 };
		double Z[3] = { O[0] + (R*sin((i + 1)*pi / 180)), O[1] + (R*cos((i + 1)*pi / 180)), O[2] - 51 };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(O[0] + r*sin(i*pi / 180), O[1] + r*cos(i*pi / 180), O[2] + 101);
		glVertex3d(O[0] + R*sin(i*pi / 180), O[1] + R*cos(i*pi / 180), O[2]);
		glVertex3d(O[0] + r*sin((i + 1)*pi / 180), O[1] + r*cos((i + 1)*pi / 180), O[2] + 101);
		glVertex3d(O[0] + R*sin((i + 1)*pi / 180), O[1] + R*cos((i + 1)*pi / 180), O[2]);
	}
	glEnd();
	///*/
	//цилиндр/потолок/пол//
	/*glBindTexture(GL_TEXTURE_2D, texId[0]);
	
	glBegin(GL_TRIANGLES);
	O[2] = -50;
	for (int i = 0; i < 360; i++)
	{
		double Y[3] = { O[0] + r*sin(i*pi / 180), O[1] + r*cos(i*pi / 180), O[2] };
		double Z[3] = { O[0] + r*sin((i + 1)*pi / 180), O[1] + r*cos((i + 1)*pi / 180), O[2] };
		norm(O, Y, Z, normal);
		glNormal3dv(normal);
		glTexCoord2d(250.0 / 1024.0, 1 - 250.0 / 1024.0);
		glVertex3dv(O);
		glTexCoord2d((250.0 - 250.0 * sin(i*pi / 180)) / 1024.0, 1 - (250.0 + 250 * cos(i*pi / 180)) / 1024.0);
		glVertex3d(O[0] + r*sin(i*pi / 180), O[1] + r*cos(i*pi / 180), O[2]);
		glTexCoord2d((250.0 - 250.0*sin((i + 1)*pi / 180)) / 1024.0, 1 - (250.0 + 250 * cos((i + 1)*pi / 180)) / 1024.0);
		glVertex3d(O[0] + r*sin((i + 1)*pi / 180), O[1] + r*cos((i + 1)*pi / 180), O[2]);
	}
	glEnd();*/

	glBindTexture(GL_TEXTURE_2D, texId[2]);

	/*glBegin(GL_TRIANGLES);
	O[2] = -50;
	for (int i = 0; i < 360; i++)
	{
		double Y[3] = { O[0] + r*sin(i*pi / 180), O[1] + r*cos(i*pi / 180), O[2] };
		double Z[3] = { O[0] + r*sin((i + 1)*pi / 180), O[1] + r*cos((i + 1)*pi / 180), O[2] };
		norm(O, Y, Z, normal);
		glNormal3dv(normal);
		glTexCoord2i(0,0);
		glVertex3dv(O);
		glTexCoord2d(0,50);
		glVertex3d(O[0] + r*sin(i*pi / 180), O[1] + r*cos(i*pi / 180), O[2]);
		glTexCoord2d(50, 50);
		glVertex3d(O[0] + r*sin((i + 1)*pi / 180), O[1] + r*cos((i + 1)*pi / 180), O[2]);
	}
	glEnd();*/

	glBegin(GL_QUADS);
	O[2] = -50;
	int i = 0;
	double Y[3] = { O[0] + r*sin(i*pi / 180), O[1] + r*cos(i*pi / 180), O[2] };
	double Z[3] = { O[0] + r*sin((i + 1)*pi / 180), O[1] + r*cos((i + 1)*pi / 180), O[2] };
	norm(O, Y, Z, normal);
	glNormal3dv(normal);
	glTexCoord2i(0, 0);
	glVertex3d(O[0] + r, O[1] + r, O[2]);
	glTexCoord2i(0, 50);
	glVertex3d(O[0] + r, O[1] - r, O[2]);
	glTexCoord2i(50, 50);
	glVertex3d(O[0] - r, O[1] - r, O[2]);
	glTexCoord2i(50, 0);
	glVertex3d(O[0] - r, O[1] + r, O[2]);
	glEnd();

	glColor4d(0, 1, 1, 0);
	
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glBegin(GL_TRIANGLES);
	O[2] = 100;
	for (int i = 0; i < 360; i++)
	{
		double Y[3] = { O[0] + r*sin(i*pi / 180), O[1] + r*cos(i*pi / 180), O[2] };
		double Z[3] = { O[0] + r*sin((i + 1)*pi / 180), O[1] + r*cos((i + 1)*pi / 180), O[2] };
		norm(Z, Y, O, normal);
		glNormal3dv(normal);
		glTexCoord2d(763.0 / 1024.0, 1 - 250.0 / 1024.0);
		glVertex3dv(O);
		glTexCoord2d((763.0 - 250.0 * sin(i*pi / 180)) / 1024.0, 1 - (250.0 + 250 * cos(i*pi / 180)) / 1024.0);
		glVertex3d(O[0] + r*sin(i*pi / 180), O[1] + r*cos(i*pi / 180), O[2]);
		glTexCoord2d((763.0 - 250.0*sin((i + 1)*pi / 180)) / 1024.0, 1 - (250.0 + 250 * cos((i + 1)*pi / 180)) / 1024.0);
		glVertex3d(O[0] + r*sin((i + 1)*pi / 180), O[1] + r*cos((i + 1)*pi / 180), O[2]);
	}
	glEnd();

	glColor4d(0, 1, 0, 1);

	//цилиндр/потолок/пол//
}

void model()
{
	double a[] = { -2.75, -0.5, -0.25 };
	double a0[] = { -2.25, -0.25, 0.25 };
	double b[] = { -2.75, 0.5, -0.25 };
	double b0[] = { -2.25, 0.25, 0.25 };
	double c[] = { -0.5, -1.25, -0.25 };
	double c0[] = { -0.5, -1, 0.25 };
	double C[] = { -0.5, -0.5, 0.75 };
	double d[] = { -0.5, 1.25, -0.25 };
	double d0[] = { -0.5, 1, 0.25 };
	double D[] = { -0.5, 0.5, 0.75 };
	double e[] = { 1.5, -1.25, -0.25 };
	double e0[] = { 1.5, -1, 0.25 };
	double E[] = { 1.25, -0.5, 0.75 };
	double f[] = { 1.5, 1.25, -0.25 };
	double f0[] = { 1.5, 1, 0.25 };
	double F[] = { 1.25, 0.5, 0.75 };
	double g[] = { 2.5, -0.75, -0.25 };
	double g0[] = { 2, -0.5, 0.25 };
	double G[] = { 2.5, -0.5, 0.25 };
	double h[] = { 2.5, 0.75, -0.25 };
	double h0[] = { 2, 0.5, 0.25 };
	double H[] = { 2.5, 0.5, 0.25 };

	//тело//
	glBegin(GL_QUAD_STRIP);
	norm(d,b,a,normal);
	glNormal3dv(normal);
	glTexCoord2d(448.0 / 1024, 1 - 586.0 / 1024);
	glVertex3dv(a);
	glTexCoord2d(548.0 / 1024, 1 - 586.0 / 1024);
	glVertex3dv(b);
	glTexCoord2d(375.0 / 1024, 1 - 362.0 / 1024);
	glVertex3dv(c);
	glTexCoord2d(621.0 / 1024, 1 - 362.0 / 1024);
	glVertex3dv(d);
	glTexCoord2d(375.0 / 1024, 1 - 163.0 / 1024);
	glVertex3dv(e);
	glTexCoord2d(621.0 / 1024, 1 - 163.0 / 1024);
	glVertex3dv(f);
	glTexCoord2d(423.0 / 1024, 1 - 64.0 / 1024);
	glVertex3dv(g);
	glTexCoord2d(572.0 / 1024, 1 - 64.0 / 1024);
	glVertex3dv(h);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	norm(a, a0, c0, normal);
	glNormal3dv(normal);
	glTexCoord2d(374.0 / 1024, 1 - 597.0 / 1024);
	glVertex3dv(a);
	glTexCoord2d(319.0 / 1024, 1 - 550.0 / 1024);
	glVertex3dv(a0);
	glTexCoord2d(375.0 / 1024, 1 - 362.0 / 1024);
	glVertex3dv(c);
	glTexCoord2d(319.0 / 1024, 1 - 362.0 / 1024);
	glVertex3dv(c0);
	norm(c, c0, e0, normal);
	glNormal3dv(normal);
	glTexCoord2d(375.0 / 1024, 1 - 163.0 / 1024);
	glVertex3dv(e);
	glTexCoord2d(319.0 / 1024, 1 - 163.0 / 1024);
	glVertex3dv(e0);
	norm(g0, g, e, normal);
	glNormal3dv(normal);
	glTexCoord2d(374.0 / 1024, 1 - 54.0 / 1024);
	glVertex3dv(g);
	glTexCoord2d(319.0 / 1024, 1 - 95.0 / 1024);
	glVertex3dv(g0);
	norm(f, h, h0, normal);
	glNormal3dv(normal);
	glTexCoord2d(25.0 / 1024, 1 - 54.0 / 1024);
	glVertex3dv(h);
	glTexCoord2d(80.0 / 1024, 1 - 95.0 / 1024);
	glVertex3dv(h0);
	norm(h, h0, f0, normal);
	glNormal3dv(normal);
	glTexCoord2d(25.0 / 1024, 1 - 163.0 / 1024);
	glVertex3dv(f);
	glTexCoord2d(80.0 / 1024, 1 - 163.0 / 1024);
	glVertex3dv(f0);
	norm(f, f0, d0, normal);
	glNormal3dv(normal);
	glTexCoord2d(25.0 / 1024, 1 - 362.0 / 1024);
	glVertex3dv(d);
	glTexCoord2d(80.0 / 1024, 1 - 362.0 / 1024);
	glVertex3dv(d0);
	norm(d, d0, b0, normal);
	glNormal3dv(normal);
	glTexCoord2d(25.0 / 1024, 1 - 597.0 / 1024);
	glVertex3dv(b);
	glTexCoord2d(80.0 / 1024, 1 - 550.0 / 1024);
	glVertex3dv(b0);
	glEnd();

	glBegin(GL_TRIANGLES);
	norm(g0, G, g, normal);
	glNormal3dv(normal);
	glTexCoord2d(249.0 / 1024, 1 - 99.0 / 1024);
	glVertex3dv(g0);
	glTexCoord2d(249.0 / 1024, 1 - 50.0 / 1024);
	glVertex3dv(G);
	glTexCoord2d(304.0 / 1024, 1 - 50.0 / 1024);
	glVertex3dv(g);

	norm(h, H, h0, normal);
	glNormal3dv(normal);
	glTexCoord2d(95.0 / 1024, 1 - 50.0 / 1024);
	glVertex3dv(h);
	glTexCoord2d(150.0 / 1024, 1 - 50.0 / 1024);
	glVertex3dv(H);
	glTexCoord2d(150.0 / 1024, 1 - 99.0 / 1024);
	glVertex3dv(h0);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	norm(a0, C, c0, normal);
	glNormal3dv(normal);
	glTexCoord2d(319.0 / 1024, 1 - 550.0 / 1024);
	glVertex3dv(a0);
	glTexCoord2d(319.0 / 1024, 1 - 550.0 / 1024);
	glVertex3dv(a0);
	glTexCoord2d(250.0 / 1024, 1 - 363.0 / 1024);
	glVertex3dv(C);
	glTexCoord2d(319.0 / 1024, 1 - 363.0 / 1024);
	glVertex3dv(c0);
	norm(C, E, e0, normal);
	glNormal3dv(normal);
	glTexCoord2d(249.0 / 1024, 1 - 190.0 / 1024);
	glVertex3dv(E);
	glTexCoord2d(319.0 / 1024, 1 - 165.0 / 1024);
	glVertex3dv(e0);
	norm(d0, f0, F, normal);
	glNormal3dv(normal);
	glTexCoord2d(151.0 / 1024, 1 - 190.0 / 1024);
	glVertex3dv(F);
	glTexCoord2d(80.0 / 1024, 1 - 165.0 / 1024);
	glVertex3dv(f0);
	norm(F, D, d0, normal);
	glNormal3dv(normal);
	glTexCoord2d(150.0 / 1024, 1 - 363.0 / 1024);
	glVertex3dv(D);
	glTexCoord2d(80.0 / 1024, 1 - 363.0 / 1024);
	glVertex3dv(d0);
	norm(d0, D, b0, normal);
	glNormal3dv(normal);
	glTexCoord2d(81.0 / 1024, 1 - 550.0 / 1024);
	glVertex3dv(b0);
	glTexCoord2d(81.0 / 1024, 1 - 550.0 / 1024);
	glVertex3dv(b0);
	glEnd();

	glBegin(GL_TRIANGLES);
	norm(e0, E, g0, normal);
	glNormal3dv(normal);
	glTexCoord2d(317.0 / 1024, 1 - 158.0 / 1024);
	glVertex3dv(e0);
	glTexCoord2d(249.0 / 1024, 1 - 189.0 / 1024);
	glVertex3dv(E);
	glTexCoord2d(249.0 / 1024, 1 - 99.0 / 1024);
	glVertex3dv(g0);

	norm(h0, F, f0, normal);
	glNormal3dv(normal);
	glTexCoord2d(150.0 / 1024, 1 - 100.0 / 1024);
	glVertex3dv(h0);
	glTexCoord2d(150.0 / 1024, 1 - 188.0 / 1024);
	glVertex3dv(F);
	glTexCoord2d(83.0 / 1024, 1 - 158.0 / 1024);
	glVertex3dv(f0);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	norm(b, b0, a0, normal);
	glNormal3dv(normal);
	glTexCoord2d(248.0 / 1024, 1 - 613.0 / 1024);
	glVertex3dv(a);
	glTexCoord2d(151.0 / 1024, 1 - 613.0 / 1024);
	glVertex3dv(b);
	glTexCoord2d(223.0 / 1024, 1 - 543.0 / 1024);
	glVertex3dv(a0);
	glTexCoord2d(176.0 / 1024, 1 - 543.0 / 1024);
	glVertex3dv(b0);
	norm(a0, b0, D, normal);
	glNormal3dv(normal);
	glTexCoord2d(249.0 / 1024, 1 - 363.0 / 1024);
	glVertex3dv(C);
	glTexCoord2d(151.0 / 1024, 1 - 363.0 / 1024);
	glVertex3dv(D);
	norm(C, D, F, normal);
	glNormal3dv(normal);
	glTexCoord2d(249.0 / 1024, 1 - 190.0 / 1024);
	glVertex3dv(E);
	glTexCoord2d(151.0 / 1024, 1 - 190.0 / 1024);
	glVertex3dv(F);
	norm(E, F, h0, normal);
	glNormal3dv(normal);
	glTexCoord2d(249.0 / 1024, 1 - 99.0 / 1024);
	glVertex3dv(g0);
	glTexCoord2d(150.0 / 1024, 1 - 100.0 / 1024);
	glVertex3dv(h0);
	norm(g0, h0, H, normal);
	glNormal3dv(normal);
	glTexCoord2d(249.0 / 1024, 1 - 50.0 / 1024);
	glVertex3dv(G);
	glTexCoord2d(150.0 / 1024, 1 - 50.0 / 1024);
	glVertex3dv(H);
	norm(g, G, H, normal);
	glNormal3dv(normal);
	glTexCoord2d(275.0 / 1024, 1 - 0.0 / 1024);
	glVertex3dv(g);
	glTexCoord2d(125.0 / 1024, 1 - 0.0 / 1024);
	glVertex3dv(h);
	glEnd();
	//тело//

	double front1[] = { -2, -0.97, -0.04 };
	double front2[] = { -2, 0.97, -0.04 };
	double back1[] = { 2.5, -0.97, -0.04 };
	double back2[] = { 2.5, 0.97, -0.04 };
	double left1[] = { -0.97, -2, -0.04 };
	double left2[] = { 1.47, -2, -0.04 };
	double right1[] = { -0.97, 2, -0.04 };
	double right2[] = { 1.47, 2, -0.04 };
	double R = 0.03;
	double bot1[] = { 0.5, -2.5, -0.1 };
	double bot2[] = { 1.25, -2.5, -0.1 };
	double bot3[] = { 0.5, 2.5, -0.1 };
	double bot4[] = { 1.25, 2.5, -0.1 };
	double top1[] = { 0.5, -2.5, 0 };
	double top2[] = { 1.25, -2.5, 0 };
	double top3[] = { 0.5, 2.5, 0 };
	double top4[] = { 1.25, 2.5, 0 };
	double top5[] = { 0.5, -1.25, 0.25 };
	double top6[] = { 1.25, -1.25, 0.25 };
	double top7[] = { 0.5, 1.25, 0.25 };
	double top8[] = { 1.25, 1.25, 0.25 };

	//балки//
	glColor3d(0,0,0);

	GLfloat amb[] = { 0, 0, 0, 1 };
	GLfloat dif[] = { 0, 0, 0, 1 };
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 360; i++)
	{
		double Y[3] = { front1[0] + R*sin(i*pi / 180), front1[1], front1[2] + R*cos(i*pi / 180) };
		double Z[3] = { front1[0] + R*sin((i + 1)*pi / 180), front1[1], front1[2] + R*cos((i + 1)*pi / 180) };
		norm(front1, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3dv(front1);
		glVertex3dv(Y);
		glVertex3dv(Z);
	}
	for (int i = 0; i <= 360; i++)
	{
		double Y[3] = { front2[0] + R*sin(i*pi / 180), front2[1], front2[2] + R*cos(i*pi / 180) };
		double Z[3] = { front2[0] + R*sin((i + 1)*pi / 180), front2[1], front2[2] + R*cos((i + 1)*pi / 180) };
		norm(front2, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3dv(front2);
		glVertex3dv(Y);
		glVertex3dv(Z);
	}
	for (int i = 0; i < 360; i++)
	{
		double Y[3] = { back1[0] + R*sin(i*pi / 180), back1[1], back1[2] + R*cos(i*pi / 180) };
		double Z[3] = { back1[0] + R*sin((i + 1)*pi / 180), back1[1], back1[2] + R*cos((i + 1)*pi / 180) };
		norm(back1, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3dv(back1);
		glVertex3dv(Y);
		glVertex3dv(Z);
	}
	for (int i = 0; i <= 360; i++)
	{
		double Y[3] = { back2[0] + R*sin(i*pi / 180), back2[1], back2[2] + R*cos(i*pi / 180) };
		double Z[3] = { back2[0] + R*sin((i + 1)*pi / 180), back2[1], back2[2] + R*cos((i + 1)*pi / 180) };
		norm(back2, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3dv(back2);
		glVertex3dv(Y);
		glVertex3dv(Z);
	}
	for (int i = 0; i <= 360; i++)
	{
		double Y[3] = { left1[0], left1[1] + R*cos(i*pi / 180), left1[2] + R*sin(i*pi / 180) };
		double Z[3] = { left1[0], left1[1] + R*cos((i + 1)*pi / 180), left1[2] + R*sin((i + 1)*pi / 180) };
		norm(left1, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3dv(left1);
		glVertex3dv(Y);
		glVertex3dv(Z);
	}
	for (int i = 0; i <= 360; i++)
	{
		double Y[3] = { left2[0], left2[1] + R*cos(i*pi / 180), left2[2] + R*sin(i*pi / 180) };
		double Z[3] = { left2[0], left2[1] + R*cos((i + 1)*pi / 180), left2[2] + R*sin((i + 1)*pi / 180) };
		norm(left2, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3dv(left2);
		glVertex3dv(Y);
		glVertex3dv(Z);
	}
	for (int i = 0; i <= 360; i++)
	{
		double Y[3] = { right1[0], right1[1] + R*cos(i*pi / 180), right1[2] + R*sin(i*pi / 180) };
		double Z[3] = { right1[0], right1[1] + R*cos((i + 1)*pi / 180), right1[2] + R*sin((i + 1)*pi / 180) };
		norm(right1, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3dv(right1);
		glVertex3dv(Y);
		glVertex3dv(Z);
	}
	for (int i = 0; i <= 360; i++)
	{
		double Y[3] = { right2[0], right2[1] + R*cos(i*pi / 180), right2[2] + R*sin(i*pi / 180) };
		double Z[3] = { right2[0], right2[1] + R*cos((i + 1)*pi / 180), right2[2] + R*sin((i + 1)*pi / 180) };
		norm(right2, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3dv(right2);
		glVertex3dv(Y);
		glVertex3dv(Z);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { front1[0] + R*sin(i*pi / 180), front1[1], front1[2] + R*cos(i*pi / 180) };
		double Y[3] = { front2[0] + R*sin(i*pi / 180), front2[1], front2[2] + R*cos(i*pi / 180) };
		double Z[3] = { front1[0] + R*sin((i + 1)*pi / 180), front1[1], front1[2] + R*cos((i + 1)*pi / 180) };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(front1[0] + R*sin(i*pi / 180), front1[1], front1[2] + R*cos(i*pi / 180));
		glVertex3d(front2[0] + R*sin(i*pi / 180), front2[1], front2[2] + R*cos(i*pi / 180));
		glVertex3d(front1[0] + R*sin((i + 1)*pi / 180), front1[1], front1[2] + R*cos((i + 1)*pi / 180));
		glVertex3d(front2[0] + R*sin((i + 1)*pi / 180), front2[1], front2[2] + R*cos((i + 1)*pi / 180));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { back1[0] + R*sin(i*pi / 180), back1[1], back1[2] + R*cos(i*pi / 180) };
		double Y[3] = { back2[0] + R*sin(i*pi / 180), back2[1], back2[2] + R*cos(i*pi / 180) };
		double Z[3] = { back1[0] + R*sin((i + 1)*pi / 180), back1[1], back1[2] + R*cos((i + 1)*pi / 180) };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(back1[0] + R*sin(i*pi / 180), back1[1], back1[2] + R*cos(i*pi / 180));
		glVertex3d(back2[0] + R*sin(i*pi / 180), back2[1], back2[2] + R*cos(i*pi / 180));
		glVertex3d(back1[0] + R*sin((i + 1)*pi / 180), back1[1], back1[2] + R*cos((i + 1)*pi / 180));
		glVertex3d(back2[0] + R*sin((i + 1)*pi / 180), back2[1], back2[2] + R*cos((i + 1)*pi / 180));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { left1[0], left1[1] + R*cos(i*pi / 180), left1[2] + R*sin(i*pi / 180) };
		double Y[3] = { left2[0], left2[1] + R*cos(i*pi / 180), left2[2] + R*sin(i*pi / 180) };
		double Z[3] = { left1[0], left1[1] + R*cos((i + 1)*pi / 180), left1[2] + R*sin((i + 1)*pi / 180) };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(left1[0], left1[1] + R*cos(i*pi / 180), left1[2] + R*sin(i*pi / 180));
		glVertex3d(left2[0], left2[1] + R*cos(i*pi / 180), left2[2] + R*sin(i*pi / 180));
		glVertex3d(left1[0], left1[1] + R*cos((i + 1)*pi / 180), left1[2] + R*sin((i + 1)*pi / 180));
		glVertex3d(left2[0], left2[1] + R*cos((i + 1)*pi / 180), left2[2] + R*sin((i + 1)*pi / 180));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { right1[0], right1[1] + R*cos(i*pi / 180), right1[2] + R*sin(i*pi / 180) };
		double Y[3] = { right2[0], right2[1] + R*cos(i*pi / 180), right2[2] + R*sin(i*pi / 180) };
		double Z[3] = { right1[0], right1[1] + R*cos((i + 1)*pi / 180), right1[2] + R*sin((i + 1)*pi / 180) };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(right1[0], right1[1] + R*cos(i*pi / 180), right1[2] + R*sin(i*pi / 180));
		glVertex3d(right2[0], right2[1] + R*cos(i*pi / 180), right2[2] + R*sin(i*pi / 180));
		glVertex3d(right1[0], right1[1] + R*cos((i + 1)*pi / 180), right1[2] + R*sin((i + 1)*pi / 180));
		glVertex3d(right2[0], right2[1] + R*cos((i + 1)*pi / 180), right2[2] + R*sin((i + 1)*pi / 180));
	}
	glEnd();
	//балки//

	amb[0] = 0.2;
	amb[1] = 0.2;
	amb[2] = 0.1;
	amb[3] = 1;
	dif[0] = 0.4; 
	dif[1] = 0.65;
	dif[2] = 0.5;
	dif[3] = 1;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);

	//крылья//
	glBegin(GL_QUAD_STRIP);
	norm(bot3,bot1,bot2, normal);
	glNormal3dv(normal);
	glTexCoord2d(625.0 / 1024, 1 - 9.0 / 1024);
	glVertex3dv(bot1);
	glTexCoord2d(699.0 / 1024, 1 - 9.0 / 1024);
	glVertex3dv(bot2);
	glTexCoord2d(625.0 / 1024, 1 - 508.0 / 1024);
	glVertex3dv(bot3);
	glTexCoord2d(699.0 / 1024, 1 - 508.0 / 1024);
	glVertex3dv(bot4);
	norm(bot3, bot4, top4, normal);
	glNormal3dv(normal);
	glTexCoord2d(625.0 / 1024, 1 - 517.0 / 1024);
	glVertex3dv(top3);
	glTexCoord2d(699.0 / 1024, 1 - 517.0 / 1024);
	glVertex3dv(top4);
	norm(top3, top4, top8, normal);
	glNormal3dv(normal);
	glTexCoord2d(806.0 / 1024, 1 - 510.0 / 1024);
	glVertex3dv(top3);
	glTexCoord2d(732.0 / 1024, 1 - 510.0 / 1024);
	glVertex3dv(top4);
	glTexCoord2d(806.0 / 1024, 1 - 387.0 / 1024);
	glVertex3dv(top7);
	glTexCoord2d(732.0 / 1024, 1 - 387.0 / 1024);
	glVertex3dv(top8);
	norm(top7, top8, top6, normal);
	glNormal3dv(normal);
	glTexCoord2d(806.0 / 1024, 1 - 130.0 / 1024);
	glVertex3dv(top5);
	glTexCoord2d(732.0 / 1024, 1 - 130.0 / 1024);
	glVertex3dv(top6);
	norm(top5, top6, top2, normal);
	glNormal3dv(normal);
	glTexCoord2d(806.0 / 1024, 1 - 7.0 / 1024);
	glVertex3dv(top1);
	glTexCoord2d(732.0 / 1024, 1 - 7.0 / 1024);
	glVertex3dv(top2);
	norm(top1, top2, bot2, normal);
	glNormal3dv(normal);
	glTexCoord2d(625.0 / 1024, 1 - 0.0 / 1024);
	glVertex3dv(top1);
	glTexCoord2d(699.0 / 1024, 1 - 0.0 / 1024);
	glVertex3dv(top2);
	glTexCoord2d(625.0 / 1024, 1 - 9.0 / 1024);
	glVertex3dv(bot1);
	glTexCoord2d(699.0 / 1024, 1 - 9.0 / 1024);
	glVertex3dv(bot2);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	norm(top5, top1, bot1, normal);
	glNormal3dv(normal);
	glTexCoord2d(839.0 / 1024, 1 - 9.0 / 1024);
	glVertex3dv(bot1);
	glTexCoord2d(831.0 / 1024, 1 - 9.0 / 1024);
	glVertex3dv(top1);
	glTexCoord2d(839.0 / 1024, 1 - 9.0 / 1024);
	glVertex3dv(bot1);
	glTexCoord2d(808.0 / 1024, 1 - 130.0 / 1024);
	glVertex3dv(top5);
	norm(top7, top5, bot1, normal);
	glNormal3dv(normal);
	glTexCoord2d(839.0 / 1024, 1 - 508.0 / 1024);
	glVertex3dv(bot3);
	glTexCoord2d(808.0 / 1024, 1 - 387.0 / 1024);
	glVertex3dv(top7);
	norm(top3, top7, bot3, normal);
	glNormal3dv(normal);
	glTexCoord2d(839.0 / 1024, 1 - 508.0 / 1024);
	glVertex3dv(bot3);
	glTexCoord2d(831.0 / 1024, 1 - 508.0 / 1024);
	glVertex3dv(top3);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	norm(bot2, top2, top6, normal);
	glNormal3dv(normal);
	glTexCoord2d(699.0 / 1024, 1 - 9.0 / 1024);
	glVertex3dv(bot2);
	glTexCoord2d(708.0 / 1024, 1 - 9.0 / 1024);
	glVertex3dv(top2);
	glTexCoord2d(699.0 / 1024, 1 - 9.0 / 1024);
	glVertex3dv(bot2);
	glTexCoord2d(732.0 / 1024, 1 - 130.0 / 1024);
	glVertex3dv(top6);
	norm(bot2, top6, top8, normal);
	glNormal3dv(normal);
	glTexCoord2d(699.0 / 1024, 1 - 508.0 / 1024);
	glVertex3dv(bot4);
	glTexCoord2d(732.0 / 1024, 1 - 387.0 / 1024);
	glVertex3dv(top8);
	norm(bot4, top8, top4, normal);
	glNormal3dv(normal);
	glTexCoord2d(699.0 / 1024, 1 - 508.0 / 1024);
	glVertex3dv(bot4);
	glTexCoord2d(708.0 / 1024, 1 - 508.0 / 1024);
	glVertex3dv(top4);
	glEnd();
	//крылья//

	amb[0] = 0.0;
	amb[1] = 0.0;
	amb[2] = 0.0;
	amb[3] = 1;
	dif[0] = 0.0;
	dif[1] = 0.0;
	dif[2] = 0.0;
	dif[3] = 1;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);

	double SH1[] = { -0.25, -0.875, -0.25 };
	double SH2[] = { -0.25, 0.875, -0.25 };
	double SH3[] = { 1.25, -0.875, -0.25 };
	double SH4[] = { 1.25, 0.875, -0.25 };
	double sh1[] = { -0.25, -0.875, -0.5 };
	double sh2[] = { -0.25, 0.875, -0.5 };
	double sh3[] = { 1.25, -0.875, -0.5 };
	double sh4[] = { 1.25, 0.875, -0.5 };
	double L1[] = { -1.75, -1, -0.5 };
	double L2[] = { -1.75, -0.75, -0.5 };
	double L3[] = { 2.5, -1, -0.5 };
	double L4[] = { 2.5, -0.75, -0.5 };
	double R1[] = { -1.75, 1, -0.5 };
	double R2[] = { -1.75, 0.75, -0.5 };
	double R3[] = { 2.5, 1, -0.5 };
	double R4[] = { 2.5, 0.75, -0.5 };
	double LR1[] = { -1.75, -1, -0.25 };
	double LR2[] = { -1.75, -0.75, -0.25 };
	double RL1[] = { -1.75, 0.75, -0.25 };
	double RL2[] = { -1.75, 1, -0.25 };
	double rsh = 0.05;
	double r = 0.25;

	//подставка//
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { SH1[0] + rsh*sin(i*pi / 180), SH1[1] + rsh*cos(i*pi / 180), SH1[2] };
		double Y[3] = { sh1[0] + rsh*sin(i*pi / 180), sh1[1] + rsh*cos(i*pi / 180), sh1[2] };
		double Z[3] = { SH1[0] + rsh*sin((i + 1)*pi / 180), SH1[1] + rsh*cos((i + 1)*pi / 180), SH1[2] };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(SH1[0] + rsh*sin(i*pi / 180), SH1[1] + rsh*cos(i*pi / 180), SH1[2]);
		glVertex3d(sh1[0] + rsh*sin(i*pi / 180), sh1[1] + rsh*cos(i*pi / 180), sh1[2]);
		glVertex3d(SH1[0] + rsh*sin((i + 1)*pi / 180), SH1[1] + rsh*cos((i + 1)*pi / 180), SH1[2]);
		glVertex3d(sh1[0] + rsh*sin((i + 1)*pi / 180), sh1[1] + rsh*cos((i + 1)*pi / 180), sh1[2]);
	}
	glEnd();
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { SH2[0] + rsh*sin(i*pi / 180), SH2[1] + rsh*cos(i*pi / 180), SH2[2] };
		double Y[3] = { sh2[0] + rsh*sin(i*pi / 180), sh2[1] + rsh*cos(i*pi / 180), sh2[2] };
		double Z[3] = { SH2[0] + rsh*sin((i + 1)*pi / 180), SH2[1] + rsh*cos((i + 1)*pi / 180), SH2[2] };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(SH2[0] + rsh*sin(i*pi / 180), SH2[1] + rsh*cos(i*pi / 180), SH2[2]);
		glVertex3d(sh2[0] + rsh*sin(i*pi / 180), sh2[1] + rsh*cos(i*pi / 180), sh2[2]);
		glVertex3d(SH2[0] + rsh*sin((i + 1)*pi / 180), SH2[1] + rsh*cos((i + 1)*pi / 180), SH2[2]);
		glVertex3d(sh2[0] + rsh*sin((i + 1)*pi / 180), sh2[1] + rsh*cos((i + 1)*pi / 180), sh2[2]);
	}
	glEnd();
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { SH3[0] + rsh*sin(i*pi / 180), SH3[1] + rsh*cos(i*pi / 180), SH3[2] };
		double Y[3] = { sh3[0] + rsh*sin(i*pi / 180), sh3[1] + rsh*cos(i*pi / 180), sh3[2] };
		double Z[3] = { SH3[0] + rsh*sin((i + 1)*pi / 180), SH3[1] + rsh*cos((i + 1)*pi / 180), SH3[2] };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(SH3[0] + rsh*sin(i*pi / 180), SH3[1] + rsh*cos(i*pi / 180), SH3[2]);
		glVertex3d(sh3[0] + rsh*sin(i*pi / 180), sh3[1] + rsh*cos(i*pi / 180), sh3[2]);
		glVertex3d(SH3[0] + rsh*sin((i + 1)*pi / 180), SH3[1] + rsh*cos((i + 1)*pi / 180), SH3[2]);
		glVertex3d(sh3[0] + rsh*sin((i + 1)*pi / 180), sh3[1] + rsh*cos((i + 1)*pi / 180), sh3[2]);
	}
	glEnd();
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { SH4[0] + rsh*sin(i*pi / 180), SH4[1] + rsh*cos(i*pi / 180), SH4[2] };
		double Y[3] = { sh4[0] + rsh*sin(i*pi / 180), sh4[1] + rsh*cos(i*pi / 180), sh4[2] };
		double Z[3] = { SH4[0] + rsh*sin((i + 1)*pi / 180), SH4[1] + rsh*cos((i + 1)*pi / 180), SH4[2] };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(SH4[0] + rsh*sin(i*pi / 180), SH4[1] + rsh*cos(i*pi / 180), SH4[2]);
		glVertex3d(sh4[0] + rsh*sin(i*pi / 180), sh4[1] + rsh*cos(i*pi / 180), sh4[2]);
		glVertex3d(SH4[0] + rsh*sin((i + 1)*pi / 180), SH4[1] + rsh*cos((i + 1)*pi / 180), SH4[2]);
		glVertex3d(sh4[0] + rsh*sin((i + 1)*pi / 180), sh4[1] + rsh*cos((i + 1)*pi / 180), sh4[2]);
	}
	glEnd();
	//подставка//

	//лыжа//
	glBegin(GL_QUADS);
	norm(L4, L2, L1, normal);
	glNormal3dv(normal);
	glVertex3dv(L1);
	glVertex3dv(L2);
	glVertex3dv(L4);
	glVertex3dv(L3);

	norm(R1, R2, R4, normal);
	glNormal3dv(normal);
	glVertex3dv(R1);
	glVertex3dv(R2);
	glVertex3dv(R4);
	glVertex3dv(R3);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 180; i <= 300; i++)
	{
		double X[3] = { LR1[0] + r*sin(i*pi / 180), LR1[1], LR1[2] + r*cos(i*pi / 180) };
		double Y[3] = { LR2[0] + r*sin(i*pi / 180), LR2[1], LR2[2] + r*cos(i*pi / 180) };
		double Z[3] = { LR1[0] + r*sin((i + 1)*pi / 180), LR1[1], LR1[2] + r*cos((i + 1)*pi / 180) };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(LR1[0] + r*sin(i*pi / 180), LR1[1], LR1[2] + r*cos(i*pi / 180));
		glVertex3d(LR2[0] + r*sin(i*pi / 180), LR2[1], LR2[2] + r*cos(i*pi / 180));
		glVertex3d(LR1[0] + r*sin((i + 1)*pi / 180), LR1[1], LR1[2] + r*cos((i + 1)*pi / 180));
		glVertex3d(LR2[0] + r*sin((i + 1)*pi / 180), LR2[1], LR2[2] + r*cos((i + 1)*pi / 180));
	}
	glEnd();
	glBegin(GL_QUAD_STRIP);
	for (int i = 180; i <= 300; i++)
	{
		double X[3] = { RL1[0] + r*sin(i*pi / 180), RL1[1], RL1[2] + r*cos(i*pi / 180) };
		double Y[3] = { RL2[0] + r*sin(i*pi / 180), RL2[1], RL2[2] + r*cos(i*pi / 180) };
		double Z[3] = { RL1[0] + r*sin((i + 1)*pi / 180), RL1[1], RL1[2] + r*cos((i + 1)*pi / 180) };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(RL1[0] + r*sin(i*pi / 180), RL1[1], RL1[2] + r*cos(i*pi / 180));
		glVertex3d(RL2[0] + r*sin(i*pi / 180), RL2[1], RL2[2] + r*cos(i*pi / 180));
		glVertex3d(RL1[0] + r*sin((i + 1)*pi / 180), RL1[1], RL1[2] + r*cos((i + 1)*pi / 180));
		glVertex3d(RL2[0] + r*sin((i + 1)*pi / 180), RL2[1], RL2[2] + r*cos((i + 1)*pi / 180));
	}
	glEnd();
	glColor3d(0.8, 0.8, 0.8);
	//лыжа//

	amb[0] = 0.2;
	amb[1] = 0.2;
	amb[2] = 0.1;
	amb[3] = 1;
	dif[0] = 0.4;
	dif[1] = 0.65;
	dif[2] = 0.5;
	dif[3] = 1;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
}

void krepeg(double x, double y, double z)
{
	double O[] = { 0, 0, -0.09 };
	double o[] = { 0, 0, 0.03 };
	double R = 0.03;
	
	glTranslated(x, y, z);

	//подшипник//
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 360; i++)
	{
		double Y[3] = { O[0] + R*sin(i*pi / 180), O[1] + R*cos(i*pi / 180), O[2] };
		double Z[3] = { O[0] + R*sin((i + 1)*pi / 180), O[1] + R*cos((i + 1)*pi / 180), O[2] };
		norm(O, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3dv(O);
		glVertex3d(O[0] + R*sin(i*pi / 180), O[1] + R*cos(i*pi / 180), O[2]);
		glVertex3d(O[0] + R*sin((i + 1)*pi / 180), O[1] + R*cos((i + 1)*pi / 180), O[2]);
	}
	for (int i = 0; i <= 360; i++)
	{
		double Y[3] = { o[0] + R*sin(i*pi / 180), o[1] + R*cos(i*pi / 180), o[2] };
		double Z[3] = { o[0] + R*sin((i + 1)*pi / 180), o[1] + R*cos((i + 1)*pi / 180), o[2] };
		norm(o, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3dv(o);
		glVertex3d(o[0] + R*sin(i*pi / 180), o[1] + R*cos(i*pi / 180), o[2]);
		glVertex3d(o[0] + R*sin((i + 1)*pi / 180), o[1] + R*cos((i + 1)*pi / 180), o[2]);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { O[0] + R*sin(i*pi / 180), O[1] + R*cos(i*pi / 180), O[2] };
		double Y[3] = { o[0] + R*sin(i*pi / 180), o[1] + R*cos(i*pi / 180), o[2] };
		double Z[3] = { O[0] + R*sin((i + 1)*pi / 180), O[1] + R*cos((i + 1)*pi / 180), O[2] };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(O[0] + R*sin(i*pi / 180), O[1] + R*cos(i*pi / 180), O[2]);
		glVertex3d(o[0] + R*sin(i*pi / 180), o[1] + R*cos(i*pi / 180), o[2]);
		glVertex3d(O[0] + R*sin((i + 1)*pi / 180), O[1] + R*cos((i + 1)*pi / 180), O[2]);
		glVertex3d(o[0] + R*sin((i + 1)*pi / 180), o[1] + R*cos((i + 1)*pi / 180), o[2]);
	}
	glEnd();
	//подшипник//

	double O0[] = { 0, 0, 0 };
	double O1[] = { 0, 0, -0.08 };
	double o1[] = { 0, 0, 0.04 };
	double R1 = 1;
	double R2 = 1.03;
	
	//обруч//
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { o1[0] + R1*sin(i*pi / 180), o1[1] + R1*cos(i*pi / 180), o1[2] };
		double Y[3] = { O0[0] + R2*sin(i*pi / 180), O0[1] + R2*cos(i*pi / 180), O0[2] };
		double Z[3] = { o1[0] + R1*sin((i + 1)*pi / 180), o1[1] + R1*cos((i + 1)*pi / 180), o1[2] };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(o1[0] + R1*sin(i*pi / 180), o1[1] + R1*cos(i*pi / 180), o1[2]);
		glVertex3d(O0[0] + R2*sin(i*pi / 180), O0[1] + R2*cos(i*pi / 180), O0[2]);
		glVertex3d(o1[0] + R1*sin((i + 1)*pi / 180), o1[1] + R1*cos((i + 1)*pi / 180), o1[2]);
		glVertex3d(O0[0] + R2*sin((i + 1)*pi / 180), O0[1] + R2*cos((i + 1)*pi / 180), O0[2]);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { O1[0] + R2*sin(i*pi / 180), O1[1] + R2*cos(i*pi / 180), O1[2] };
		double Y[3] = { O0[0] + R2*sin(i*pi / 180), O0[1] + R2*cos(i*pi / 180), O0[2] };
		double Z[3] = { O1[0] + R2*sin((i + 1)*pi / 180), O1[1] + R2*cos((i + 1)*pi / 180), O1[2] };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(O1[0] + R2*sin(i*pi / 180), O1[1] + R2*cos(i*pi / 180), O1[2]);
		glVertex3d(O0[0] + R2*sin(i*pi / 180), O0[1] + R2*cos(i*pi / 180), O0[2]);
		glVertex3d(O1[0] + R2*sin((i + 1)*pi / 180), O1[1] + R2*cos((i + 1)*pi / 180), O1[2]);
		glVertex3d(O0[0] + R2*sin((i + 1)*pi / 180), O0[1] + R2*cos((i + 1)*pi / 180), O0[2]);
	}
	glEnd();
	//обруч//

	double R22 = 1.08;
	
	//нижние перемычки//
	glColor3d(0, 0, 0);
	glRotated(45, 0, 0, 1);
	glBegin(GL_QUAD_STRIP);
	for (int i = 360; i >= 0; i--)
	{
		double Y[3] = { O[0] + R*sin(i*pi / 180), O[1] + R*cos(i*pi / 180), O[2] };
		double Z[3] = { O[0] + R*sin((i + 1)*pi / 180), O[1] + R*cos((i + 1)*pi / 180), O[2] };
		norm(O, Y, Z, normal);
		glNormal3dv(normal);
		if (i <= 183 && i >= 177)
		{
			glVertex3d(O1[0] + R22*sin(i*pi / 180), O1[1] + R22*cos(i*pi / 180), O1[2]);
			glVertex3d(O1[0] + R22*sin((180 - i)*pi / 180), O1[1] + R22*cos((180 - i)*pi / 180), O1[2]);
		}
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (int i = 360; i >= 0; i--)
	{
		double Y[3] = { O[0] + R*sin(i*pi / 180), O[1] + R*cos(i*pi / 180), O[2] };
		double Z[3] = { O[0] + R*sin((i + 1)*pi / 180), O[1] + R*cos((i + 1)*pi / 180), O[2] };
		norm(O, Y, Z, normal);
		glNormal3dv(normal);
		if (i <= 93 && i >= 87)
		{
			glVertex3d(O1[0] + R22*sin(i*pi / 180), O1[1] + R22*cos(i*pi / 180), O1[2]);
			glVertex3d(O1[0] + R22*sin((360 - i)*pi / 180), O1[1] + R22*cos((360 - i)*pi / 180), O1[2]);
		}
	}
	glEnd();
	//нижние перемычки//

	glRotated(-45, 0, 0, 1);

	//крепление к обручу//
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= 360; i++)
	{
		double X[3] = { O1[0] + R22*sin((i + 1)*pi / 180), O1[1] + R22*cos((i + 1)*pi / 180), O1[2] };
		double Y[3] = { O0[0] + R22*sin(i*pi / 180), O0[1] + R22*cos(i*pi / 180), O0[2] };
		double Z[3] = { O1[0] + R22*sin(i*pi / 180), O1[1] + R22*cos(i*pi / 180), O1[2] };
		norm(X, Y, Z, normal);
		glNormal3dv(normal);
		glVertex3d(O1[0] + R22*sin(i*pi / 180), O1[1] + R22*cos(i*pi / 180), O1[2]);
		glVertex3d(O0[0] + R22*sin(i*pi / 180), O0[1] + R22*cos(i*pi / 180), O0[2]);
		glVertex3d(O1[0] + R22*sin((i + 1)*pi / 180), O1[1] + R22*cos((i + 1)*pi / 180), O1[2]);
		glVertex3d(O0[0] + R22*sin((i + 1)*pi / 180), O0[1] + R22*cos((i + 1)*pi / 180), O0[2]);
	}
	glEnd();
	glColor3d(0.5, 0.5, 0.5);
	//крепление к обручу//

	glTranslated(-x, -y, -z);
}

void lopost(double x, double y, double z, int a)
{
	double A[3] = { 0 };
	double A0[3] = { 0 };
	double A1[3] = { 0 };
	double O0[3] = { 0 };
	double O1[3] = { 0 };
	double B[3] = { 0 };
	double B0[3] = { 0 };
	double B1[3] = { 0 };
	double O2[3] = { 0 };
	double O3[3] = { 0 };
	if (a)
	{
		A[0] = 1; A[1] = -0.1; A[2] = -0.05;
		A0[0] = 1; A0[1] = 0; A0[2] = 0;
		A1[0] = 1; A1[1] = 0.1; A1[2] = 0;
		B[0] = -1; B[1] = 0.1; B[2] = -0.05;
		B0[0] = -1; B0[1] = 0; B0[2] = 0;
		B1[0] = -1; B1[1] = -0.1; B1[2] = 0;
		O0[0] = 0.1; O0[1] = 0.1; O0[2] = 0;
		O1[0] = 0.1; O1[1] = 0; O1[2] = 0;
		O2[0] = -0.1; O2[1] = -0.1; O2[2] = 0;
		O3[0] = -0.1; O3[1] = 0; O3[2] = 0;
	}
	else
	{
		A[0] = 1; A[1] = -0.1; A[2] = 0;
		A0[0] = 1; A0[1] = 0; A0[2] = 0;
		A1[0] = 1; A1[1] = 0.1; A1[2] = -0.05;
		B[0] = -1; B[1] = 0.1; B[2] = 0;
		B0[0] = -1; B0[1] = 0; B0[2] = 0;
		B1[0] = -1; B1[1] = -0.1; B1[2] = -0.05;
		O0[0] = 0.1; O0[1] = 0; O0[2] = 0;
		O1[0] = 0.1; O1[1] = -0.1; O1[2] = 0; 
		O2[0] = -0.1; O2[1] = 0; O2[2] = 0;
		O3[0] = -0.1; O3[1] = 0.1; O3[2] = 0;
	}

	glTranslated(x, y, z);
	if (a)
		glRotated(-t, 0, 0, 1);
	else
		glRotated(t, 0, 0, 1);

	glBegin(GL_QUADS);

	norm(O1, A0, A, normal);
	glNormal3dv(normal);
	glVertex3dv(A);
	glVertex3dv(A0);
	glVertex3dv(O1);
	glVertex3dv(O1);

	norm(O0, A1, A0, normal);
	glNormal3dv(normal);
	glVertex3dv(A0);
	glVertex3dv(A1);
	glVertex3dv(O0);
	glVertex3dv(O1);

	norm(O3, O0, O1, normal);
	glNormal3dv(normal);
	glVertex3dv(O1);
	glVertex3dv(O0);
	glVertex3dv(O3);
	glVertex3dv(O2);

	norm(O3, O2, B1, normal);
	glNormal3dv(normal);
	glVertex3dv(O3);
	glVertex3dv(O2);
	glVertex3dv(B1);
	glVertex3dv(B0);

	norm(B0, B, O3, normal);
	glNormal3dv(normal);
	glVertex3dv(O3);
	glVertex3dv(O3);
	glVertex3dv(B);
	glVertex3dv(B0);

	glEnd();
	
	if (eng)
	{
		if (!gamestarted)
			t += 5;
		else
		{
			if (vz < 0)
				vz += 0.05;
			t += 5;
		}
	}
	else if (vz > -49.5)
	{
		vz -= 0.05;
		t += 2.5;
	}
	else
		vz = -49.5;
	if (a)
		glRotated(t, 0, 0, 1);
	else
		glRotated(-t, 0, 0, 1); 
	glTranslated(-x, -y, -z);
	
}

//30 раз/с
void Render(OpenGL *ogl) 
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//альфаналожение
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//настройка материала
	
	//фоновая
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambd);
	//дифузная
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difd);
	//зеркальная
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specd);
	//размер блика
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shd);

    //чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH);
	//===================================
	//Прогать тут  

	if (fyp)
	{
		if (ay > 0)
			ay -= 0.1;
		else
		{
			ay = 0;
			fyp = false;
		}
		sx = ay*0.2*cos(az*pi / 180);
		sy = ay*0.2*sin(az*pi / 180);
		vx += sx;
		vy += sy;
		if (az > -45 && az < 45)
		{
			//W
			if ((retx || ay == 0) && vx < -399 + abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ay += 0.2;
					fyp = true;
					retx = true;
				}
			}
			else if (retx && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				retx = false;

			//S
			if ((retx || ay == 0) && vx > 399 - abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ay -= 0.2;
					fym = true;
					retx = true;
				}
			}
			else if (retx && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				retx = false;
		}

		if (az > -135 && az < -45)
		{
			//W
			if ((retx || ay == 0) && vy > 399 - abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ay += 0.2;
					fyp = true;
					retx = true;
				}
			}
			else if (retx && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				retx = false;
			//S
			if ((retx || ay == 0) && vy < -399 + abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ay -= 0.2;
					fym = true;
					retx = true;
				}
			}
			else if (retx && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				retx = false;
		}

		if (az > 45 && az < 135)
		{
			//W
			if ((retx || ay == 0) && vy < -399 + abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ay += 0.2;
					fyp = true;
					retx = true;
				}
			}
			else if (retx && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				retx = false;
			//S
			if ((retx || ay == 0) && vy > 399 - abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ay -= 0.2;
					fym = true;
					retx = true;
				}
			}
			else if (retx && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				retx = false;
		}
		if (az > 135 && az <= 180 || az >= -180 && az < -135)
		{
			//W
			if ((retx || ay == 0) && vx > 399 - abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ay += 0.2;
					fyp = true;
					retx = true;
				}
			}
			else if (retx && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				retx = false;
			//S
			if ((retx || ay == 0) && vx < -399 + abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ay -= 0.2;
					fym = true;
					retx = true;
				}
			}
			else if (retx && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				retx = false;
		}
	}

	if (fym)
	{
		if (ay < 0)
			ay += 0.1;
		else
		{
			ay = 0;
			fym = false;
		}
		sx = ay*0.2*cos(az*pi / 180);
		sy = ay*0.2*sin(az*pi / 180);
		vx += sx;
		vy += sy;
		if (az > -45 && az < 45)
		{
			//W
			if ((retx || ay == 0) && vx < -399 + abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ay += 0.2;
					fyp = true;
					retx = true;
				}
			}
			else if (retx && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				retx = false;

			//S
			if ((retx || ay == 0) && vx > 399 - abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ay -= 0.2;
					fym = true;
					retx = true;
				}
			}
			else if (retx && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				retx = false;
		}

		if (az > -135 && az < -45)
		{
			//W
			if ((retx || ay == 0) && vy > 399 - abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ay += 0.2;
					fyp = true;
					retx = true;
				}
			}
			else if (retx && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				retx = false;
			//S
			if ((retx || ay == 0) && vy < -399 + abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ay -= 0.2;
					fym = true;
					retx = true;
				}
			}
			else if (retx && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				retx = false;
		}

		if (az > 45 && az < 135)
		{
			//W
			if ((retx || ay == 0) && vy < -399 + abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ay += 0.2;
					fyp = true;
					retx = true;
				}
			}
			else if (retx && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				retx = false;
			//S
			if ((retx || ay == 0) && vy > 399 - abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ay -= 0.2;
					fym = true;
					retx = true;
				}
			}
			else if (retx && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				retx = false;
		}
		if (az > 135 && az <= 180 || az >= -180 && az < -135)
		{
			//W
			if ((retx || ay == 0) && vx > 399 - abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ay += 0.2;
					fyp = true;
					retx = true;
				}
			}
			else if (retx && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				retx = false;
			//S
			if ((retx || ay == 0) && vx < -399 + abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ay -= 0.2;
					fym = true;
					retx = true;
				}
			}
			else if (retx && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				retx = false;
		}
	}

	if (fxp)
	{
		if (ax > 0)
			ax -= 0.1;
		else
		{
			ax = 0;
			fxp = false;
		}
		sx = ax*0.2*sin(az*pi / 180);
		sy = ax*0.2*cos(az*pi / 180);
		vx += sx;
		vy -= sy;
		if (az > -45 && az < 45)
		{
			//A
			if ((rety || ax == 0) && vy < -399 + abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ax -= 0.2;
					fxm = true;
					rety = true;
				}
			}
			else if (rety && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				rety = false;
			//D
			if ((rety || ax == 0) && vy > 399 - abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ax += 0.2;
					fxp = true;
					rety = true;
				}
			}
			else if (rety && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				rety = false;
		}
		if (az > 45 && az < 135)
		{
			//A
			if ((rety || ax == 0) && vx > 399 - abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ax -= 0.2;
					fxm = true;
					rety = true;
				}
			}
			else if (rety && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				rety = false;
			//D
			if ((rety || ax == 0) && vx < -399 + abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ax += 0.2;
					fxp = true;
					rety = true;
				}
			}
			else if (rety && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				rety = false;
		}
		if (az > -135 && az < -45)
		{
			//A
			if ((rety || ax == 0) && vx < -399 + abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ax -= 0.2;
					fxm = true;
					rety = true;
				}
			}
			else if (rety && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				rety = false;
			//D
			if ((rety || ax == 0) && vx > 399 - abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ax += 0.2;
					fxp = true;
					rety = true;
				}
			}
			else if (rety && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				rety = false;
		}
		if (az > 135 && az <= 180 || az >= -180 && az < -135)
		{
			//A
			if ((rety || ax == 0) && vy > 399 - abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ax -= 0.2;
					fxm = true;
					rety = true;
				}
			}
			else if (rety && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				rety = false;
			//D
			if ((rety || ax == 0) && vy < -399 + abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ax += 0.2;
					fxp = true;
					rety = true;
				}
			}
			else if (rety && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				rety = false;
		}
	}

	if (fxm)
	{
		if (ax < 0)
			ax += 0.1;
		else
		{
			ax = 0;
			fxm = false;
		}
		sx = ax*0.2*sin(az*pi / 180);
		sy = ax*0.2*cos(az*pi / 180);
		vx += sx;
		vy -= sy;
		if (az > -45 && az < 45)
		{
			//A
			if ((rety || ax == 0) && vy < -399 + abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ax -= 0.2;
					fxm = true;
					rety = true;
				}
			}
			else if (rety && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				rety = false;
			//D
			if ((rety || ax == 0) && vy > 399 - abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ax += 0.2;
					fxp = true;
					rety = true;
				}
			}
			else if (rety && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				rety = false;
		}
		if (az > 45 && az < 135)
		{
			//A
			if ((rety || ax == 0) && vx > 399 - abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ax -= 0.2;
					fxm = true;
					rety = true;
				}
			}
			else if (rety && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				rety = false;
			//D
			if ((rety || ax == 0) && vx < -399 + abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ax += 0.2;
					fxp = true;
					rety = true;
				}
			}
			else if (rety && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				rety = false;
		}
		if (az > -135 && az < -45)
		{
			//A
			if ((rety || ax == 0) && vx < -399 + abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ax -= 0.2;
					fxm = true;
					rety = true;
				}
			}
			else if (rety && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				rety = false;
			//D
			if ((rety || ax == 0) && vx > 399 - abs(vy))
			{
				if (abs(vy) <= 399)
				{
					ax += 0.2;
					fxp = true;
					rety = true;
				}
			}
			else if (rety && vx > -399 + abs(vy) && vx < 399 - abs(vy))
				rety = false;
		}
		if (az > 135 && az <= 180 || az >= -180 && az < -135)
		{
			//A
			if ((rety || ax == 0) && vy > 399 - abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ax -= 0.2;
					fxm = true;
					rety = true;
				}
			}
			else if (rety && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				rety = false;
			//D
			if ((rety || ax == 0) && vy < -399 + abs(vx))
			{
				if (abs(vx) <= 399)
				{
					ax += 0.2;
					fxp = true;
					rety = true;
				}
			}
			else if (rety && vy > -399 + abs(vx) && vy < 399 - abs(vx))
				rety = false;
		}
	}

	glTranslated(vx, vy, vz);
	glRotated(az, 0, 0, 1);
	glRotated(ax, 1, 0, 0);
	glRotated(ay, 0, 1, 0);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambn);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difn);

	//лопости//
	glColor3d(0.2, 0.2, 0.2);
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	lopost(-2, -2, 0.026, 0);
	lopost(-2, -2, -0.026, 1);

	lopost(2.5, -2, 0.026, 0);
	lopost(2.5, -2, -0.026, 1);

	lopost(2.5, 2, 0.026, 0);
	lopost(2.5, 2, -0.026, 1);

	lopost(-2, 2, 0.026, 0);
	lopost(-2, 2, -0.026, 1);
	//лопости//

	//крепление лопостей//
	glColor3d(0.5, 0.5, 0.5); 
	krepeg(-2, -2, 0);

	krepeg(2.5, -2, 0);

	krepeg(2.5, 2, 0);

	krepeg(-2, 2, 0);
	//крепление лопостей//

	if (day)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specd);
	}
	else
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambn1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difn1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specn1);
	}

	//основа//
	glColor3d(0.8, 0.8, 0.8);
	model();
	//основа//

	glRotated(-ay, 0, 1, 0);
	glRotated(-ax, 1, 0, 0);
	glRotated(-az, 0, 0, 1);
	glTranslated(-vx, -vy, -vz);
	
	ambd[0] = 1;
	ambd[1] = 1;
	ambd[2] = 1;
	ambd[3] = 1;
	difd[0] = 1;
	difd[1] = 1;
	difd[2] = 1;
	difd[3] = 1;

	if (day)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambd);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difd);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shn);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specd);
	}
	else
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambn);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difn);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shn);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specn);

	}

	ambd[0] = 0.2;
	ambd[1] = 0.2;
	ambd[2] = 0.1;
	ambd[3] = 1;
	difd[0] = 0.4;
	difd[1] = 0.65;
	difd[2] = 0.5;
	difd[3] = 1;
	
	if (g)
	{
		game(start, finish, RSF);
	}

	//фон// 
	glColor3d(1, 1, 0);
	if (alpha)
		sun(suny, sunz);

	glColor3d(0, 1, 0);
	room();
	//фон// 



	char kom[4] = {0};
	if (az<45 && az>-45)
	{
		kom[0] = 'С';
	}
	if (az<135 && az>45)
	{
		kom[0] = 'З';
	}
	if (az>-135 && az<-45)
	{
		kom[0] = 'В';
	}
	if (az>=-180 && az<-135 || az<=180 && az>135)
	{
		kom[0] = 'Ю';
	}
	double x = vx;
	double y = vy;
	double z = vz + 49.5;
	double a = az;
	int ch = time / 3600;
	double m = time / 60 - ch*60;
	if (ch < 6)
		ch += 6;
	else
		ch -= 6;
	char d[] = "день";
	if (day)
	{
		d[0] = 'д';
		d[1] = 'е';
		d[2] = 'н';
		d[3] = 'ь';
	}
	else
	{
		d[0] = 'н';
		d[1] = 'о';
		d[2] = 'ч';
		d[3] = 'ь';
	}
	//текст сообщения вверху слева, если надоест - закоментировать, или заменить =)
	char c[1000];  //максимальная длина сообщения
	sprintf_s(c, "Управление:\n Графика:\n  \"T\"-текстуры(%d)\n  \"L\"-свет(%d)\n  \"C\"-прозрачность(%d)\n"
		" Модель:\n  \"W\"-вперед \"S\"-назад\n  \"A\"-влево \"D\"-вправо\n  \"SPACE\"-вверх \"Z\"-вниз\n  \"Q\"-поворот влево \"E\"-поворот вправо\n"
		"  \"R\"-установить модель и камеру в начальное положение\n"
		" Миниигра:\n  \"1\"-легко \"2\"-средне \"3\"-тяжело\n  \"P\"-отмена игры\n"
		" Время:\n  \"Стрелка вправо\"-ускорить время\n  \"Стрелка влево\"-замедлить время\n  \"Стрелка вниз\"-сбросить ускорение(замедление)\n  \"Стрелка вверх\"-в реальном времени\n\n"
		"Данные:\n Победы/Поражения: %d/%d\n"
		" Время: %d:%.0f x%.2f (%s)\n"
		" Компас: %.0f° (%s)\n"
		" GPS: X: %.2f; Y: %.2f;\nВысота: %.1f\n"
		" Скорость: VX: %.2f; VY: %.2f;", textureMode, lightMode, alpha, win, lose, ch, m, vr/1000, d, a, kom, x, y, z, sx, sy);
	ogl->message = std::string(c);
	if (ch == 5)
	{
		light.pos = Vector3(0, -460, -20.25);
		vd = true;
		suny = -600;
		sunz = 12.5;
	}
	if (ch == 4)
	{
		light.pos = Vector3(0, -460, 6.5);
		suny = -600;
		sunz = 75;
	}
	if (ch == 3)
	{
		light.pos = Vector3(0, -460, 33.25);
		suny = -600;
		sunz = 137.5;
	}
	if (ch == 2)
	{
		light.pos = Vector3(0, -230, 60);
		suny = -400;
		sunz = 200;
	}
	if (ch == 1)
	{
		light.pos = Vector3(0, -115, 60);
		suny = -200;
		sunz = 200;
	}
	if (ch == 0)
	{
		light.pos = Vector3(0, 0, 60);
		suny = 0;
		sunz = 200;
	}
	if (ch == 11)
	{
		light.pos = Vector3(0, 115, 60);
		suny = 200;
		sunz = 200;
	}
	if (ch == 10)
	{
		light.pos = Vector3(0, 230, 60);
		suny = 400;
		sunz = 200;
	}
	if (ch == 9)
	{
		light.pos = Vector3(0, 460, 33.25);
		suny = 600;
		sunz = 137.5;
	}
	if (ch == 8)
	{
		light.pos = Vector3(0, 460, 6.5);
		suny = 600;
		sunz = 75;
	}
	if (ch == 7)
	{
		light.pos = Vector3(0, 460, -20.25);
		suny = 600;
		sunz = 12.5;
	}
	if (ch == 6)
	{
		light.pos = Vector3(0, 460, -47);
		suny = 600;
		sunz = -50;
		if (vd)
		{
			day = !day;
			vd = false;
		}
	}

	time = time + 2 * vr / 1000;
	if (time > 43200)
	{
		time = 0;
	}

}   //конец тела функции

