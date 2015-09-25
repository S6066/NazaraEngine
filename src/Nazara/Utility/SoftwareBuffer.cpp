// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utility/SoftwareBuffer.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Utility/Config.hpp>
#include <cstring>
#include <stdexcept>
#include <Nazara/Utility/Debug.hpp>

namespace Nz
{
	SoftwareBuffer::SoftwareBuffer(Buffer* parent, BufferType type) :
	m_type(type)
	{
		NazaraUnused(parent);
	}

	SoftwareBuffer::~SoftwareBuffer()
	{
	}

	bool SoftwareBuffer::Create(unsigned int size, BufferUsage usage)
	{
		NazaraUnused(usage);

		// Cette allocation est protégée car sa taille dépend directement de paramètres utilisateurs
		try
		{
			m_buffer = new UInt8[size];
		}
		catch (const std::exception& e)
		{
			NazaraError("Failed to allocate software buffer (" + String(e.what()) + ')');
			return false;
		}

		m_mapped = false;

		return true;
	}

	void SoftwareBuffer::Destroy()
	{
		delete[] m_buffer;
	}

	bool SoftwareBuffer::Fill(const void* data, unsigned int offset, unsigned int size, bool forceDiscard)
	{
		NazaraUnused(forceDiscard);

		#if NAZARA_UTILITY_SAFE
		if (m_mapped)
		{
			NazaraError("Buffer already mapped");
			return false;
		}
		#endif

		std::memcpy(&m_buffer[offset], data, size);

		return true;
	}

	bool SoftwareBuffer::IsHardware() const
	{
		return false;
	}

	void* SoftwareBuffer::Map(BufferAccess access, unsigned int offset, unsigned int size)
	{
		NazaraUnused(access);
		NazaraUnused(size);

		#if NAZARA_UTILITY_SAFE
		if (m_mapped)
		{
			NazaraError("Buffer already mapped");
			return nullptr;
		}
		#endif

		m_mapped = true;

		return &m_buffer[offset];
	}

	bool SoftwareBuffer::Unmap()
	{
		#if NAZARA_UTILITY_SAFE
		if (!m_mapped)
		{
			NazaraError("Buffer not mapped");
			return true;
		}
		#endif

		m_mapped = false;

		return true;
	}
}
