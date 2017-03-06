#pragma once
#include "Transform.h"
#include "Vec2.h"
#include <vector>
using std::vector;
#include "sfwdraw.h"
#include <iostream>
using std::cout;

struct SpriteRenderer
{
	unsigned m_color, m_sprite;
	Vec2 m_size;

	SpriteRenderer::SpriteRenderer()
	{
		m_color = GREEN;
	}

	void SpriteRenderer::draw(const Mat3 & T, const Transform & trans)
	{
		Mat3 glob = T * trans.getGlobalTransform();

		Vec3 C = glob * Vec3{  0, 0, 1 };
		//Vec3 L = glob * Vec3{ -1, -3, 1 };
		//Vec3 R = glob * Vec3{  1, -3, 1 };
		//Vec3 B = glob * Vec3{  0, -1, 1 };

		//sfw::drawLine(L.x, L.y, F.x, F.y, m_color);
		//sfw::drawLine(F.x, F.y, R.x, R.y, m_color);
		//sfw::drawLine(R.x, R.y, B.x, B.y, m_color);
		//sfw::drawLine(B.x, B.y, L.x, L.y, m_color);
		//
		//cout << "trans: " << trans.m_position.x << ", " << trans.m_position.y << "\n";
		//cout << "C: " << C.x << ", " << C.y << "\n";
		//cout << m_size.x << ", " << m_size.y << "\n";

		sfw::drawTexture(m_sprite, C.x, C.y, m_size.x, m_size.y, trans.m_facing, true, 0, WHITE);
	}
};