// COMP710 GP 2D Framework 2017
#ifndef __SPRITE_H__
#define __SPRITE_H__

// Forward Declarations:
class BackBuffer;
class Texture;

#include <cstdint>


class Sprite
{
	//Member Methods:
public:
	Sprite();
	~Sprite();

	bool Initialise(Texture& texture);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void SetX(int x);
	void SetY(int y);

	int GetX() const;
	int GetY() const;

	void setOffsetX(int offset);
	void setOffsetY(int offset);

	void SetAngle(float angle);
	float GetAngle() const;

	void SetCenter(int x, int y);
	void SetHandleCenter();

	int GetWidth() const;
	int GetHeight() const;

	Texture* GetTexture();

	void SetColor(uint8_t r, uint8_t g, uint8_t b);

protected:

private:
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite& sprite);

	//Member Data:
public:

protected:
	Texture* m_pTexture;

	int m_x;
	int m_y;

	int offsetX;
	int offsetY;

	float m_angle;
	int m_centerX;
	int m_centerY;

	int m_width;
	int m_height;

private:

};

#endif // __SPRITE_H__
