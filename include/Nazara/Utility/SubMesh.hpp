// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILITY_SUBMESH_HPP
#define NAZARA_UTILITY_SUBMESH_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Core/Error.hpp>
#include <Nazara/Math/Box.hpp>
#include <Nazara/Utility/Enums.hpp>
#include <Nazara/Utility/IndexBuffer.hpp>
#include <Nazara/Utility/VertexBuffer.hpp>
#include <Nazara/Utils/Signal.hpp>

namespace Nz
{
	class Mesh;

	class NAZARA_UTILITY_API SubMesh
	{
		friend Mesh;

		public:
			SubMesh();
			SubMesh(const SubMesh&) = delete;
			SubMesh(SubMesh&&) = delete;
			virtual ~SubMesh();

			void GenerateNormals();
			void GenerateNormalsAndTangents();
			void GenerateTangents();

			virtual const Boxf& GetAABB() const = 0;
			virtual AnimationType GetAnimationType() const = 0;
			virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
			std::size_t GetMaterialIndex() const;
			PrimitiveMode GetPrimitiveMode() const;
			UInt64 GetTriangleCount() const;
			virtual UInt64 GetVertexCount() const = 0;

			virtual bool IsAnimated() const = 0;

			void SetMaterialIndex(std::size_t matIndex);
			void SetPrimitiveMode(PrimitiveMode mode);

			SubMesh& operator=(const SubMesh&) = delete;
			SubMesh& operator=(SubMesh&&) = delete;

			// Signals:
			NazaraSignal(OnSubMeshInvalidateAABB, const SubMesh* /*subMesh*/);

		protected:
			PrimitiveMode m_primitiveMode;
			std::size_t m_matIndex;
	};
}

#endif // NAZARA_UTILITY_SUBMESH_HPP
