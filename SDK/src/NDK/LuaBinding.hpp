// Copyright (C) 2015 Jérôme Leclercq
// This file is part of the "Nazara Development Kit"
// For conditions of distribution and use, see copyright notice in Prerequesites.hpp

#pragma once

#ifndef NDK_LUABINDING_HPP
#define NDK_LUABINDING_HPP

#include <Nazara/Core.hpp>
#include <Nazara/Lua.hpp>
#include <Nazara/Math.hpp>
#include <Nazara/Network.hpp>
#include <Nazara/Utility.hpp>
#include <NDK/Components.hpp>
#include <NDK/Entity.hpp>
#include <NDK/Systems.hpp>
#include <NDK/World.hpp>

#ifndef NDK_SERVER
#include <Nazara/Audio.hpp>
#include <Nazara/Graphics.hpp>
#include <Nazara/Renderer.hpp>
#endif

namespace Ndk
{
	class LuaBinding
	{
		public:
			LuaBinding();
			~LuaBinding() = default;

			void RegisterClasses(Nz::LuaInstance& instance);

		private:
			void BindCore();
			void BindMath();
			void BindNetwork();
			void BindSDK();
			void BindUtility();

			template<typename T> void EnableComponentBinding();

			void RegisterCore(Nz::LuaInstance& instance);
			void RegisterMath(Nz::LuaInstance& instance);
			void RegisterNetwork(Nz::LuaInstance& instance);
			void RegisterSDK(Nz::LuaInstance& instance);
			void RegisterUtility(Nz::LuaInstance& instance);

			#ifndef NDK_SERVER
			void BindAudio();
			void BindGraphics();
			void BindRenderer();

			void RegisterAudio(Nz::LuaInstance& instance);
			void RegisterGraphics(Nz::LuaInstance& instance);
			void RegisterRenderer(Nz::LuaInstance& instance);
			#endif

			// Core
			Nz::LuaClass<Nz::Clock> clockClass;
			Nz::LuaClass<Nz::Directory> directoryClass;
			Nz::LuaClass<Nz::File> fileClass;
			Nz::LuaClass<Nz::Stream> streamClass;

			// Math
			Nz::LuaClass<Nz::EulerAnglesd> eulerAnglesClass;
			Nz::LuaClass<Nz::Quaterniond> quaternionClass;
			Nz::LuaClass<Nz::Vector2d> vector2dClass;
			Nz::LuaClass<Nz::Vector3d> vector3dClass;

			// Network
			Nz::LuaClass<Nz::AbstractSocket> abstractSocketClass;
			Nz::LuaClass<Nz::IpAddress> ipAddressClass;

			// Utility
			Nz::LuaClass<Nz::AbstractImage*> abstractImage;
			Nz::LuaClass<Nz::Node> nodeClass;

			// SDK
			Nz::LuaClass<EntityHandle> entityClass;
			Nz::LuaClass<NodeComponentHandle> nodeComponent;
			Nz::LuaClass<WorldHandle> worldClass;

			using AddComponentFunc = int(*)(Nz::LuaInstance&, EntityHandle&);
			using GetComponentFunc = int(*)(Nz::LuaInstance&, BaseComponent&);

			struct ComponentBinding
			{
				AddComponentFunc adder;
				GetComponentFunc getter;
				bool valid = false;
			};

			std::vector<ComponentBinding> m_componentBinding;

			#ifndef NDK_SERVER
			// Audio
			Nz::LuaClass<Nz::Sound> soundClass;
			Nz::LuaClass<Nz::SoundBufferRef> soundBuffer;
			Nz::LuaClass<Nz::SoundEmitter> soundEmitter;

			// Graphics
			Nz::LuaClass<Nz::InstancedRenderableRef> instancedRenderable;
			Nz::LuaClass<Nz::ModelRef> modelClass;

			// SDK
			Nz::LuaClass<GraphicsComponentHandle> graphicsComponent;
			#endif
	};
}

#endif // NDK_LUABINDING_HPP
