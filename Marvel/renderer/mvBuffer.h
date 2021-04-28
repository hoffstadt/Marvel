#pragma once

#include <vector>
#include <memory>
#include <string>
#include <optional>
#include "mvMath.h"

namespace Marvel {

	enum mvDataType
	{
		Float,
		Float3,
		Matrix,
		Bool,
		Struct,
		Array,
		Empty
	};

	// static map of attributes of each leaf type
	template<mvDataType type>
	struct mvMap
	{
		static constexpr bool valid = false;
	};

	struct mvLayoutData
	{

	};

	//-----------------------------------------------------------------------------
	// mvBufferLayoutEntry
	//-----------------------------------------------------------------------------
	class mvBufferLayoutEntry
	{

		friend class mvBufferLayout;

	public:

		mvBufferLayoutEntry& add     (mvDataType type, std::string name);
		mvBufferLayoutEntry& set     (mvDataType type, size_t size);
		mvBufferLayoutEntry& getEntry(const std::string& key);
		mvBufferLayoutEntry& getArray();

		size_t getBeginningOffset() const;
		size_t getEndingOffset   () const;
		size_t getSizeInBytes    () const;
		bool   exists            () const;

	public:

		mvBufferLayoutEntry(mvDataType type);
		
		size_t finalize      (size_t offset);
		size_t finalizeStruct(size_t offset);
		size_t finalizeArray(size_t offset);

	private:

		size_t                                                   m_offset = 0;
		mvDataType                                               m_type = Empty;
		std::unique_ptr<mvLayoutData>                            m_data;

	};


	//-----------------------------------------------------------------------------
	// mvBufferLayout
	//-----------------------------------------------------------------------------
	class mvBufferLayout
	{

	public:

		mvBufferLayout(std::shared_ptr<mvBufferLayoutEntry> root);

		mvBufferLayoutEntry& add(mvDataType type, std::string key);

		std::shared_ptr<mvBufferLayoutEntry> getRoot();
	
	private:

		std::shared_ptr<mvBufferLayoutEntry> m_root;
	};

	//-----------------------------------------------------------------------------
	// mvBufferElement
	//-----------------------------------------------------------------------------
	class mvBufferElement
	{

	public:

		mvBufferElement(const mvBufferLayoutEntry* entry, char* bytes, size_t offset);

		template<typename T>
		operator T& () const
		{
			return *reinterpret_cast<T*>(m_bytes + m_offset + m_entry->getBeginningOffset());
		}

		template<typename T>
		T& operator=(const T& rhs) const
		{
			return static_cast<T&>(*this) = rhs;
		}

		bool exists() const;

		template<typename S>
		bool setIfExists(const S& val)
		{
			if (exists())
			{
				*this = val;
				return true;
			}
			return false;
		}

	public:

		size_t m_offset;
		char* m_bytes;
		const mvBufferLayoutEntry* m_entry;

	};

	//-----------------------------------------------------------------------------
	// mvBuffer
	//-----------------------------------------------------------------------------
	class mvBuffer
	{

	public:

		mvBuffer(mvBufferLayout&& layout);

		mvBufferElement getElement(const std::string& key);

		size_t      getSizeInBytes() const;
		const char* getData       () const;

	private:

		std::shared_ptr<mvBufferLayoutEntry> m_layoutRoot;
		std::vector<char> m_bytes;

	};

	//-----------------------------------------------------------------------------
	// Type map specializations
	//-----------------------------------------------------------------------------
	template<> struct mvMap<Float>
	{
		using SysType = float;                              // type used in the CPU side
		static constexpr size_t hlslSize = sizeof(SysType); // size of type on GPU side
		static constexpr bool valid = true;                 // metaprogramming flag to check validity of Map <param>
	};

	template<> struct mvMap<Float3>
	{
		using SysType = glm::vec3;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
	};

	template<> struct mvMap<Matrix>
	{
		using SysType = glm::mat4;
		static constexpr size_t hlslSize = sizeof(SysType);
		static constexpr bool valid = true;
	};

	template<> struct mvMap<Bool>
	{
		using SysType = bool;
		static constexpr size_t hlslSize = 4u;
		static constexpr bool valid = true;
	};
}