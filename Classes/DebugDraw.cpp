#include "DebugDraw.h"

DebugDraw* DebugDraw::create()
{
    DebugDraw* draw = new DebugDraw();
    draw->autorelease();
    return draw;
}

DebugDraw::DebugDraw()
{
    _lines = new std::vector<DebugLine>();
    _points = new std::vector<DebugPoint>();
	_rects = new std::vector<DebugRect>();
}

DebugDraw::~DebugDraw()
{
    delete _lines;
    delete _points;
	delete _rects;
}

void DebugDraw::draw(void)
{
    int c = _lines->size();
    for (int i = 0; i < c; i++)
    {
        DebugLine line = _lines->at(i);
        glColor4f(line.r, line.g, line.b, 1);
        ccDrawLine(line.pt1, line.pt2);
    }

    c = _points->size();
    for (int i = 0; i < c; i++)
    {
        DebugPoint pt = _points->at(i);
        glColor4f(pt.r, pt.g, pt.b, 1);
        ccDrawPoint(pt.pt);
    }
	c = _rects->size();
    for (int i = 0; i < c; i++)
    {
        DebugRect rect = _rects->at(i);
        glColor4f(rect.r, rect.g, rect.b, 1);
		ccDrawRect(rect.origin, rect.destination);
    }
}

void DebugDraw::appendLine(CCPoint pt1, CCPoint pt2, float r, float g, float b)
{
    DebugLine line;
    line.pt1 = pt1;
    line.pt2 = pt2;
    line.r = r;
    line.g = g;
    line.b = b;
    _lines->push_back(line);
}

void DebugDraw::appendPoint(float x, float y, float r, float g, float b)
{
    appendPoint(ccp(x, y), r, g, b);
}

void DebugDraw::appendPoint(CCPoint pt, float r, float g, float b)
{
    DebugPoint dp;
    dp.pt = pt;
    dp.r = r;
    dp.g = g;
    dp.b = b;
    _points->push_back(dp);
}

void DebugDraw::appendRect(CCPoint origin, float x, float y, float r, float g, float b)
{
	DebugRect rect;
	rect.origin = origin;
	rect.destination.x = origin.x + x;
	rect.destination.y = origin.y + y;
	rect.r = r;
	rect.g = g;
	rect.b = b;
	_rects->push_back(rect);
}

void DebugDraw::removeAllRect()
{
	_rects->clear();
}