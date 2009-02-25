#include "swftypes.h"
#include "geometry.h"

void Shape::Render() const
{
	std::cout << "winding " << winding << std::endl;
	std::cout << "graphic.filled0 " << graphic.filled0 << std::endl;
	std::cout << "graphic.color0 " << graphic.color0 << std::endl;
	std::cout << "graphic.filled1 " << graphic.filled1 << std::endl;
	std::cout << "graphic.color1 " << graphic.color1 << std::endl;
	std::cout << "graphic.stroked " << graphic.stroked << std::endl;
	std::cout << "graphic.stroke_color " << graphic.stroke_color << std::endl;

	if(winding==0)
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthFunc(GL_ALWAYS);
		glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
		if(graphic.filled0)
			glStencilFunc(GL_ALWAYS,1,1);
		else
			glStencilFunc(GL_ALWAYS,0,1);
		std::vector<Triangle>::const_iterator it2=interior.begin();
		glBegin(GL_TRIANGLES);
		for(it2;it2!=interior.end();it2++)
		{
			glVertex2i(it2->v1.x,it2->v1.y);
			glVertex2i(it2->v2.x,it2->v2.y);
			glVertex2i(it2->v3.x,it2->v3.y);
		}
		glEnd();
	}
	else if(winding==1)
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthFunc(GL_ALWAYS);
		glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
		if(graphic.filled1)
			glStencilFunc(GL_ALWAYS,2,1);
		else
			glStencilFunc(GL_ALWAYS,0,1);
		std::vector<Triangle>::const_iterator it2=interior.begin();
		glBegin(GL_TRIANGLES);
		for(it2;it2!=interior.end();it2++)
		{
			glVertex2i(it2->v1.x,it2->v1.y);
			glVertex2i(it2->v2.x,it2->v2.y);
			glVertex2i(it2->v3.x,it2->v3.y);
		}
		glEnd();
	}


	if(graphic.stroked)
	{
		std::vector<Vector2>::const_iterator it=outline.begin();
		glColor3ub(graphic.stroke_color.Red,graphic.stroke_color.Green,graphic.stroke_color.Blue);
		if(closed)
			glBegin(GL_LINE_LOOP);
		else
			glBegin(GL_LINE_STRIP);
		for(it;it!=outline.end();it++)
		{
			glVertex2i(it->x,it->y);
		}
		glEnd();
	}
}

bool Edge::xIntersect(int x,int32_t& d)
{
	int u1,u2;
	u1=min(x1,x2);
	u2=max(x1,x2);
	if((x>=u1) && (x<=u2))
	{
		if(x2==x1)
			d=y1;
		else
		{
			float m=(y2-y1);
			m/=(x2-x1);
			d=m*(x-x1);
			d+=y1;
		}
		return true;
	}
	else
		return false;
}

bool Edge::yIntersect(int y,int32_t& d, int x)
{
	int u1,u2;
	u1=min(y1,y2);
	u2=max(y1,y2);

	if((y>u1) && (y<u2))
	{
		float m=(x2-x1);
		m/=(y2-y1);
		d=m*(y-y1);
		d+=x1;
		return true;
	}
	else if(y==y1 || y==y2)
	{
		if(y1==y2)
		{
			int v1,v2;
			v1=min(x1,x2);
			v2=max(x1,x2);
			if(!(x>=v1 && x<=v2))
			{
				return false;
			}
			if(x1==x)
				d=x2;
			else
				d=x1;
			return true;
		}
		else if(y==u1)
		{
			if(u1==y1)
				d=x1;
			else
				d=x2;
			return true;
		}
		else
		{
/*			if(u1==y1)
				d=x2;
			else
				d=x1;*/
			return false;
		}
	}
/*	else if(y==y1 && y==y2)
	{
		d=x1;
		return true;
	}*/
	else
	{
		return false;
	}
}

bool Edge::edgeIntersect(const Edge& e)
{
	float ua=(e.x2-e.x1)*(y1-e.y1)-(e.y2-e.y1)*(x1-e.x1);
	float ub=(x2-x1)*(y1-e.y1)-(y2-y1)*(x1-e.x1);

	ua/=(e.y2-e.y1)*(x2-x1)-(e.x2-e.x1)*(y2-y1);
	ub/=(e.y2-e.y1)*(x2-x1)-(e.x2-e.x1)*(y2-y1);

	if(ua>0 && ua<1 && ub>0 && ub<1)
		return true;
	else
		return false;
}

FilterIterator::FilterIterator(const std::vector<Vector2>::const_iterator& i, const std::vector<Vector2>::const_iterator& e, int f):it(i),end(e),filter(f)
{
	if(it!=end && it->index==filter)
		it++;
}
FilterIterator FilterIterator::operator++(int)
{
	FilterIterator copy(*this);
	it++;
	if(it!=end && it->index==filter)
		it++;
	return copy;
}

const Vector2& FilterIterator::operator*()
{
	return *it;
}

bool FilterIterator::operator==(FilterIterator& i)
{
	return i.it==it;
}

bool FilterIterator::operator!=(FilterIterator& i)
{
	return i.it!=it;
}
