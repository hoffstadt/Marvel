#pragma once
#include <vector>
#include <array>
#include <type_traits>
#include "mvBindable.h"

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
	class mvUniformMatrix : public mvUniform<T>
	{

		using mvUniform<T>::m_data;
		using mvUniform<T>::m_slot;

	public:

		mvUniformMatrix(mvGraphics& graphics, const T& data, int slot)
			:
			mvUniform(graphics, data, slot)
		{
		}

		void bind(mvGraphics& graphics) override
		{
			if (std::is_same<T, std::array<float, 4>>::value)
			{
				glUniformMatrix4fv(m_slot, 1, false, m_data.data());
			}

			else if (std::is_same<T, std::array<float, 3>>::value)
			{
				glUniformMatrix3fv(m_slot, 1, false, m_data.data());
			}

			else if (std::is_same<T, std::array<float, 2>>::value)
			{
				glUniformMatrix2fv(m_slot, 1, false, m_data.data());
			}
		}

	};
}