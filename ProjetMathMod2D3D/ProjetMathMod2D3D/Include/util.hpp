#ifndef __UTIL_H__
#define __UTIL_H__
#include <include/GL/glew.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
class Util
{
public:
	void drawLine(float x1, float y1, float x2, float y2)
	{
		glBegin(GL_LINE_LOOP);
		
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
		glEnd();
	}
	void drawLine(Point p1, Point p2)
	{
		glBegin(GL_LINE_LOOP);
		glVertex3f(p1.getX(), p1.getY(), p1.getZ());
		glVertex3f(p2.getX(), p2.getY(), p2.getZ());
		glEnd();
	}
	void drawRect(float x, float y, float length)
	{
		float x1 = x - length / 2;
		float x2 = x + length / 2;
		float y1 = y - length / 2;
		float y2 = y + length / 2;

		glBegin(GL_LINE_LOOP);
		glVertex2f(x1, y1);
		glVertex2f(x2, y1);
		glVertex2f(x2, y2);
		glVertex2f(x1, y2);
		glEnd();
	}
	void drawRect(Point p, float length)
	{
		/*glTranslatef(p._x, p._y, p._z);
		glutWireCube(length);
		glTranslatef(-p._x, -p._y, -p._z);*/
		glBegin(GL_LINE_LOOP);
		glVertex3f(p.getX(), p.getY(), p.getZ());
		glEnd();
	}
};
#endif






