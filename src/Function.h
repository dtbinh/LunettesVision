#pragma once

class Function
{
public:
	Function(void);
	~Function(void);

	virtual float getX(float x);
	virtual float getY(float x);

	void setSize(float x, float y);

protected:
	float width,height;
};
