// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILITY_INDEXMAPPER_HPP
#define NAZARA_UTILITY_INDEXMAPPER_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Utility/BufferMapper.hpp>
#include <Nazara/Utility/IndexBuffer.hpp>

namespace Nz
{
	class IndexIterator;
	class SubMesh;

	class NAZARA_UTILITY_API IndexMapper
	{
		public:
			IndexMapper(IndexBuffer& indexBuffer, std::size_t indexCount = 0);
			IndexMapper(SubMesh& subMes);
			~IndexMapper() = default;

			UInt32 Get(std::size_t i) const;
			const IndexBuffer* GetBuffer() const;
			std::size_t GetIndexCount() const;

			void Set(std::size_t i, UInt32 value);

			void Unmap();

			// Méthodes STD
			IndexIterator begin();
			//IndexConstIterator begin() const;
			IndexIterator end();
			//IndexIterator end() const;
			// Méthodes STD

		private:
			using Getter = UInt32(*)(const void* buffer, std::size_t i);
			using Setter = void(*)(void* buffer, std::size_t i, UInt32 value);

			BufferMapper<IndexBuffer> m_mapper;
			Getter m_getter;
			Setter m_setter;
			std::size_t m_indexCount;
};
}

#endif // NAZARA_UTILITY_INDEXMAPPER_HPP
