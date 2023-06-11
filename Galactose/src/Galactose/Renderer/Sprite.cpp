#include "Sprite.h"
#include "Texture.h"

namespace Galactose {
	Sprite::Sprite(const std::shared_ptr<Texture>& a_texture, const Vector4& a_color, const Vector2& a_pivot) :
		m_texture(a_texture),
		m_color(a_color),
		m_pivot(a_pivot)
	{
		autoResize();
	}

	void Sprite::autoResize() {
		const float PIXELS_PER_UNIT = 100;
		m_size = m_texture ? Vector2(m_texture->width(), m_texture->height()) / PIXELS_PER_UNIT : Vector2(1, 1);
	}
}
