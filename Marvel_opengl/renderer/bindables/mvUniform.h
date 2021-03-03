#pragma once
#include <vector>
#include <array>
#include <type_traits>
#include "mvBindable.h"
#include <GL/gl3w.h>
#include "mvMath.h"

namespace Marvel
{

	template<typename T>
	class mvUniform : public mvBindable
	{

	public:

		mvUniform(mvGraphics& graphics, const T& data, int slot)
			:
			m_data(data),
			m_slot(slot)
		{
		}

		void update(T data)
		{
			m_data = data;
		}

	protected:

		T m_data;
		int m_slot;

	};


	template<typename T>
	class mvUniformVector : public mvUniform<T>
	{

		using mvUniform<T>::m_data;
		using mvUniform<T>::m_slot;

	public:

		mvUniformVector(mvGraphics& graphics, const T& data, int slot)
			:
			mvUniform(graphics, data, slot)
		{
		}

		void bind(mvGraphics& graphics) override
		{
			if (std::is_same<T, std::array<float, 4>>::value)
			{
				glUniform4fv(m_slot, 1, m_data.data());
			}

			else if (std::is_same<T, std::array<float, 3>>::value)
			{
				glUniform3fv(m_slot, 1, m_data.data());
			}

			else if (std::is_same<T, std::array<float, 2>>::value)
			{
				glUniform2fv(m_slot, 1, m_data.data());
			}

			else if (std::is_same<T, std::array<float, 1>>::value)
			{
				glUniform1fv(m_slot, 1, m_data.data());
			}
		}

	};

	template<typename T>
	class mvUniform4Matrix : public mvUniform<T>
	{

		using mvUniform<T>::m_data;
		using mvUniform<T>::m_slot;

	public:

		mvUniform4Matrix(mvGraphics& graphics, const T& data, int slot)
			:
			mvUniform(graphics, data, slot)
		{
		}

		void bind(mvGraphics& graphics) override
		{
			glUniformMatrix4fv(m_slot, 1, false, &m_data[0][0]);
		}

	};
}