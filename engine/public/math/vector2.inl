namespace Math
{
	inline Vector2::Vector2()
		: m_x(0.0f)
		, m_y(0.0f)
	{

	}

	inline Vector2::Vector2(float x, float y)
		: m_x(x)
		, m_y(y)
	{
	}

	inline Vector2::Vector2(float v)
		: m_x(v)
		, m_y(v)
	{
	}

	inline bool Vector2::operator!=(const Vector2& t) const
	{
		return (m_x != t.m_x || m_y != t.m_y);
	}

	inline bool Vector2::operator==(const Vector2& t) const
	{
		return !(t != *this);
	}

	inline Vector2 Vector2::operator*(float f) const
	{
		return Vector2(m_x*f, m_y*f);
	}

	inline void Vector2::operator*=(float f)
	{
		*this = *this * f;
	}

	inline Vector2 Vector2::operator+(Vector2 f) const 
	{
		return Vector2(m_x + f.m_x, m_y + f.m_y);
	}

	inline void Vector2::operator+=(Vector2 f)
	{
		*this = *this + f;
	}

	inline Vector2 Vector2::operator-(Vector2 f) const
	{
		return Vector2((m_x - f.m_x), (m_y - f.m_y));
	}

	inline void Vector2::operator-=(Vector2 f)
	{
		*this = *this - f;
	}

	inline float Vector2::Length() const
	{
		return Math::SquareRoot((m_x*m_x) + (m_y*m_y));
	}

	inline Vector2 Vector2::Normalised() const
	{
		float l = Length();
		return *this * (1.0f / l);
	}

}