#include "mvBuffer.h"

namespace Marvel {

	size_t AdvanceToBoundary(size_t offset)
	{
		return offset + (16u - offset % 16u) % 16u;
	}

	mvBufferLayoutEntry::mvBufferLayoutEntry(mvDataType type)
	{
		m_type = type;
	}

	mvBufferLayoutEntry& mvBufferLayoutEntry::getEntry(const std::string& key)
	{
		assert("Keying into non-struct" && m_type == Struct);
		for (auto& mem : m_entries)
		{
			if (mem.first == key)
			{
				return mem.second;
			}
		}
		static mvBufferLayoutEntry empty{Empty};
		return empty;
	}

	size_t mvBufferLayoutEntry::finalize(size_t offset)
	{

		auto CrossesBoundary = [](size_t offset, size_t size)
		{
			const auto end = offset + size;
			const auto pageStart = offset / 16u;
			const auto pageEnd = end / 16u;
			return (pageStart != pageEnd && end % 16 != 0u) || size > 16u;
		};

		auto AdvanceIfCrossesBoundary = [CrossesBoundary](size_t offset, size_t size)
		{
			return CrossesBoundary(offset, size) ? AdvanceToBoundary(offset) : offset;
		};

		switch (m_type)
		{
		case Float:
			m_offset = AdvanceIfCrossesBoundary(offset, mvMap<Float>::hlslSize);
			return m_offset + mvMap<Float>::hlslSize;

		case Float3:
			m_offset = AdvanceIfCrossesBoundary(offset, mvMap<Float3>::hlslSize);
			return m_offset + mvMap<Float3>::hlslSize;

		case Matrix:
			m_offset = AdvanceIfCrossesBoundary(offset, mvMap<Matrix>::hlslSize);
			return m_offset + mvMap<Matrix>::hlslSize;

		case Struct:
			return finalizeStruct(offset);

		default:
			assert("Bad type in size computation" && false);
			return 0u;
		}
	}

	size_t mvBufferLayoutEntry::finalizeStruct(size_t offset)
	{
		assert(m_entries.size() != 0u);
		m_offset = offset;
		
		auto current_offset = m_offset;
		for (auto& entry : m_entries)
			current_offset = entry.second.finalize(current_offset);
		
		return current_offset;
	}

	mvBufferLayoutEntry& mvBufferLayoutEntry::add(mvDataType type, std::string name)
	{
		assert("Add to non-struct in layout" && m_type == Struct);
		m_entries.emplace_back(std::move(name), mvBufferLayoutEntry{ type });
		return *this;
	}

	size_t mvBufferLayoutEntry::getBeginningOffset() const
	{
		return m_offset;
	}

	size_t mvBufferLayoutEntry::getEndingOffset() const
	{
		switch (m_type)
		{
		case Float:
			return m_offset + mvMap<Float>::hlslSize;

		case Float3:
			return m_offset + mvMap<Float3>::hlslSize;

		case Matrix:
			return m_offset + mvMap<Matrix>::hlslSize;

		case Struct:
			return AdvanceToBoundary(m_entries.back().second.getEndingOffset());

		default:
			assert("Bad type in size computation" && false);
			return 0u;
		}
	}

	size_t mvBufferLayoutEntry::getSizeInBytes() const
	{
		return getEndingOffset() - getBeginningOffset();
	}

	mvBufferLayout::mvBufferLayout(std::shared_ptr<mvBufferLayoutEntry> root)
		:
		m_root(std::move(root))
	{

	}

	mvBufferLayoutEntry& mvBufferLayout::add(mvDataType type, std::string key)
	{
		return m_root->add(type, key);
	}

	std::shared_ptr<mvBufferLayoutEntry> mvBufferLayout::getRoot()
	{
		return m_root;
	}

	mvBufferElement::mvBufferElement(const mvBufferLayoutEntry* entry, char* bytes, size_t offset)
		:
		m_offset(offset),
		m_bytes(bytes),
		m_entry(entry)
	{

	}

	mvBuffer::mvBuffer(mvBufferLayout&& layout)
		:
		m_layoutRoot(layout.getRoot()),
		m_bytes(m_layoutRoot->getEndingOffset())
	{

	}

	mvBufferElement mvBuffer::getElement(const std::string& key)
	{
		return mvBufferElement(&(*m_layoutRoot).getEntry(key), m_bytes.data(), 0u);
	}

	size_t mvBuffer::getSizeInBytes() const
	{
		return m_bytes.size();
	}

	const char* mvBuffer::getData() const
	{
		return m_bytes.data();
	}

}