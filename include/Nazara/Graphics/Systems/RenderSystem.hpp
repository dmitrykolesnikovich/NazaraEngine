// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Graphics module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_GRAPHICS_SYSTEMS_RENDERSYSTEM_HPP
#define NAZARA_GRAPHICS_SYSTEMS_RENDERSYSTEM_HPP

#include <Nazara/Prerequisites.hpp>
#include <Nazara/Graphics/Graphics.hpp>
#include <Nazara/Graphics/Components/GraphicsComponent.hpp>
#include <Nazara/Graphics/Components/LightComponent.hpp>
#include <Nazara/Utility/Node.hpp>
#include <Nazara/Utils/MemoryPool.hpp>
#include <entt/entt.hpp>
#include <array>
#include <memory>
#include <set>
#include <unordered_map>

namespace Nz
{
	class CommandBufferBuilder;
	class FramePipeline;
	class RenderFrame;
	class RenderWindow;
	class UploadPool;

	class NAZARA_GRAPHICS_API RenderSystem
	{
		public:
			static constexpr bool AllowConcurrent = false;
			static constexpr Int64 ExecutionOrder = 1'000;

			RenderSystem(entt::registry& registry);
			RenderSystem(const RenderSystem&) = delete;
			RenderSystem(RenderSystem&&) = delete;
			~RenderSystem();

			template<typename T = RenderWindow, typename... Args> T& CreateWindow(Args&&... args);

			void Update(float elapsedTime);

			RenderSystem& operator=(const RenderSystem&) = delete;
			RenderSystem& operator=(RenderSystem&&) = delete;

		private:
			void OnCameraDestroy(entt::registry& registry, entt::entity entity);
			void OnGraphicsDestroy(entt::registry& registry, entt::entity entity);
			void OnLightDestroy(entt::registry& registry, entt::entity entity);
			void OnNodeDestroy(entt::registry& registry, entt::entity entity);
			void UpdateInstances();
			void UpdateObservers();
			void UpdateVisibility();

			struct CameraEntity
			{
				entt::entity entity;
				std::size_t poolIndex;
				std::size_t viewerIndex;

				NazaraSlot(Node, OnNodeInvalidation, onNodeInvalidation);
			};

			struct GraphicsEntity
			{
				entt::entity entity;
				std::array<std::size_t, GraphicsComponent::MaxRenderableCount> renderableIndices;
				std::size_t poolIndex;
				std::size_t worldInstanceIndex;

				NazaraSlot(GraphicsComponent, OnRenderableAttached, onRenderableAttached);
				NazaraSlot(GraphicsComponent, OnRenderableDetach, onRenderableDetach);
				NazaraSlot(GraphicsComponent, OnScissorBoxUpdate, onScissorBoxUpdate);
				NazaraSlot(GraphicsComponent, OnVisibilityUpdate, onVisibilityUpdate);
				NazaraSlot(Node, OnNodeInvalidation, onNodeInvalidation);
			};

			struct LightEntity
			{
				entt::entity entity;
				std::array<std::size_t, LightComponent::MaxLightCount> lightIndices;
				std::size_t poolIndex;

				NazaraSlot(LightComponent, OnLightAttached, onLightAttached);
				NazaraSlot(LightComponent, OnLightDetach, onLightDetach);
				NazaraSlot(LightComponent, OnVisibilityUpdate, onVisibilityUpdate);
				NazaraSlot(Node, OnNodeInvalidation, onNodeInvalidation);
			};

			entt::connection m_cameraDestroyConnection;
			entt::connection m_graphicsDestroyConnection;
			entt::connection m_lightDestroyConnection;
			entt::connection m_nodeDestroyConnection;
			entt::observer m_cameraConstructObserver;
			entt::observer m_graphicsConstructObserver;
			entt::observer m_lightConstructObserver;
			entt::registry& m_registry;
			std::set<CameraEntity*> m_invalidatedCameraNode;
			std::set<GraphicsEntity*> m_invalidatedGfxWorldNode;
			std::set<LightEntity*> m_invalidatedLightWorldNode;
			std::unique_ptr<FramePipeline> m_pipeline;
			std::unordered_map<entt::entity, CameraEntity*> m_cameraEntities;
			std::unordered_map<entt::entity, GraphicsEntity*> m_graphicsEntities;
			std::unordered_map<entt::entity, LightEntity*> m_lightEntities;
			std::unordered_set<GraphicsEntity*> m_newlyHiddenGfxEntities;
			std::unordered_set<GraphicsEntity*> m_newlyVisibleGfxEntities;
			std::unordered_set<LightEntity*> m_newlyHiddenLightEntities;
			std::unordered_set<LightEntity*> m_newlyVisibleLightEntities;
			std::vector<std::unique_ptr<RenderWindow>> m_renderWindows;
			MemoryPool<CameraEntity> m_cameraEntityPool;
			MemoryPool<GraphicsEntity> m_graphicsEntityPool;
			MemoryPool<LightEntity> m_lightEntityPool;
	};
}

#include <Nazara/Graphics/Systems/RenderSystem.inl>

#endif // NAZARA_GRAPHICS_SYSTEMS_RENDERSYSTEM_HPP
