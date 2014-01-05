namespace Math
{
	inline Rect::Rect()
		: m_origin(0)
		, m_size(0)
	{
	}
	
	inline Rect::Rect(Vector2 origin, Vector2 size)
		: m_origin(origin)
		, m_size(size)
	{
	}
	
	inline Rect::Rect(int32_t x, int32_t y, int32_t w, int32_t h)
		: m_origin(x, y)
		, m_size(w, h) 
	{
	}

	inline bool Rect::operator!=(const Rect& t) const
	{
		return (m_origin != t.m_origin) || (m_size != t.m_size);
	}
	
	inline bool Rect::operator==(const Rect& t) const
	{
		return !(*this != t);
	}

	inline Rect Rect::Translate(Vector2 translation)
	{
		Rect result = *this;
		result.m_origin += translation;
		return result;
	}
	
	inline Rect Rect::Scale(float scaleX, float scaleY)
	{
		Rect result = *this;
		result.m_size.m_x = (int32_t)(result.m_size.m_x * scaleX);
		result.m_size.m_y = (int32_t)(result.m_size.m_y * scaleY);
		return result;
	}
}