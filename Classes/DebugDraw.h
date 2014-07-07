#ifndef __DEBUGDRAW_H__
#define __DEBUGDRAW_H__

#include "cocos2d.h" 
using namespace cocos2d;

typedef struct
{
    CCPoint pt1;
    CCPoint pt2;
    float r;
    float g;
    float b;
} DebugLine;

typedef struct
{
    CCPoint pt;
    float r;
    float g;
    float b;
} DebugPoint;

typedef struct
{
	CCPoint origin;
	CCPoint destination;
	float r;
	float g;
	float b;
} DebugRect;

class DebugDraw : public CCNode
{
public:
    static DebugDraw* create();

    DebugDraw();
    ~DebugDraw();
    virtual void draw(void);

    void appendLine(CCPoint pt1, CCPoint pt2, float r = 1, float g = 1, float b = 1);
    void appendPoint(float x, float y, float r = 1, float g = 1, float b = 1);
    void appendPoint(CCPoint pt, float r = 1, float g = 1, float b = 1);
	void appendRect(CCPoint origin, float x, float y, float r = 1, float g = 1, float b = 1);
	void removeAllRect();
	int	 numberOfPoint() { return _points->size(); }
	int	 numberOfLine() { return _lines->size(); }
	bool isRectEmpty() { return _rects->empty();}

private:
    std::vector<DebugLine>* _lines;
    std::vector<DebugPoint>* _points;
	std::vector<DebugRect>* _rects;
};
#endif