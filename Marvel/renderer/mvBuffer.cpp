#include "mvBuffer.h"

namespace Marvel {

	struct mvStructData : public mvLayoutData
	{
		std::vector<std::pair<std::string, mvBufferLayoutEntry>> entries;
	};

	struct mvArrayData : public mvLayoutData
	{
		std::optional<mvBufferLayoutEntry> layoutEntry;
		size_t elementSize;
		size_t size;
	};

	size_t AdvanceToBoundary(size_t offset)
	{
		return offset + (16u - offset % 16u) % 16u;
	}

	mvBufferLayoutEntry::mvBufferLayoutEntry(mvDataType type)
	{
		m_type = type;

		if (m_type == Struct)
			m_data = std::make_unique<mvStructData>();
		if (m_type == Array)
			m_data = std::make_unique<mvArrayData>();
	}

	mvBufferLayoutEntry& mvBufferLayoutEntry::getEntry(const std::string& key)
	{
		assert("Keying into non-struct" && m_type == Struct);

		auto& entries = static_cast<mvStructData&>(*m_data).entries;

		for (auto& mem : entries)
		{
			if (mem.first == key)
			{
				return mem.second;
			}
		}
		static mvBufferLayoutEntry empty{Empty};
		return empty;
	}

	mvBufferLayoutEntry& mvBufferLayoutEntry::getArray()
	{
		assert("Keying into non-array" && m_type == Array);
		return *static_cast<mvArrayData&>(*m_data).layoutEntry;
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

		case Bool:
			m_offset = AdvanceIfCrossesBoundary(offset, mvMap<Bool>::hlslSize);
			return m_offset + mvMap<Bool>::hlslSize;

		case Struct:
			return finalizeStruct(offset);

		case Array:
			return finalizeArray(offset);

		default:
			assert("Bad type in size computation" && false);
			return 0u;
		}
	}

	size_t mvBufferLayoutEntry::finalizeStruct(size_t offset)
	{
		auto& entries = static_cast<mvStructData&>(*m_data).entries;
		assert(entries.size() != 0u);
		m_offset = AdvanceToBoundary(offset);
		
		auto current_offset = m_offset;
		for (auto& entry : entries)
			current_offset = entry.second.finalize(current_offset);
		
		return current_offset;
	}

	size_t mvBufferLayoutEntry::finalizeArray(size_t offset)
	{
		auto& data = static_cast<mvArrayData&>(*m_data);
		assert(data.size != 0u);
		m_offset = AdvanceToBoundary(offset);
		data.layoutEntry->finalize(m_offset);
		data.elementSize = AdvanceToBoundary(data.layoutEntry->getSizeInBytes());

		return getEndingOffset();
	}

	mvBufferLayoutEntry& mvBufferLayoutEntry::add(mvDataType type, std::string name)
	{
		assert("Add to non-struct in layout" && m_type == Struct);
		auto& data = static_cast<mvStructData&>(*m_data);
		data.entries.emplace_back(std::move(name), mvBufferLayoutEntry{ type });
		return *this;
	}

	mvBufferLayoutEntry& mvBufferLayoutEntry::set(mvDataType type, size_t size)
	{
		assert("Set on non-array in layout" && m_type == Array);
		auto& data = static_cast<mvArrayData&>(*m_data);
		data.layoutEntry = mvBufferLayoutEntry(type);
		data.size = size;
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

		case Bool:
			return m_offset + mvMap<Bool>::hlslSize;

		case Struct:
		{
			auto& entries = static_cast<const mvStructData&>(*m_data).entries;
			return AdvanceToBoundary(entries.back().second.getEndingOffset());
		}

		case Array:
		{
			const auto& data = static_cast<const mvArrayData&>(*m_data);
			return m_offset + AdvanceToBoundary(data.layoutEntry->getSizeInBytes()) * data.size;
		}

		default:
			assert("Bad type in size computation" && false);
			return 0u;
		}
	}

	size_t mvBufferLayoutEntry::getSizeInBytes() const
	{
		return getEndingOffset() - getBeginningOffset();
	}

	bool mvBufferLayoutEntry::exists() const
	{
		return m_type != Empty;
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

	bool mvBufferElement::exists() const
	{
		return m_entry->exists();
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