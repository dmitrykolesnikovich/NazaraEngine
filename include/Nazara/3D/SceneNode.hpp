// Copyright (C) 2012 Jérôme Leclercq
// This file is part of the "Nazara Engine - 3D Module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_SCENENODE_HPP
#define NAZARA_SCENENODE_HPP

#include <Nazara/Prerequesites.hpp>
#include <Nazara/3D/Enums.hpp>
#include <Nazara/3D/RenderQueue.hpp>
#include <Nazara/3D/Scene.hpp>
#include <Nazara/Math/BoundingBox.hpp>
#include <Nazara/Math/Frustum.hpp>
#include <Nazara/Utility/Node.hpp>

class NAZARA_API NzSceneNode : public NzNode
{
	friend class NzScene;

	public:
		NzSceneNode();
		NzSceneNode(const NzSceneNode& node);
		virtual ~NzSceneNode();

		virtual void AddToRenderQueue(NzRenderQueue& renderQueue) const = 0;

		virtual const NzBoundingBoxf& GetBoundingBox() const = 0;
		nzNodeType GetNodeType() const final;
		NzScene* GetScene() const;
		virtual nzSceneNodeType GetSceneNodeType() const = 0;

		bool IsVisible() const;

	protected:
		virtual void OnParenting(const NzNode* parent) override;
		virtual void OnVisibilityChange(bool visibility);
		void RecursiveSetScene(NzScene* scene, NzNode* node);
		virtual void Register();
		void SetScene(NzScene* scene);
		virtual void Unregister();
		virtual void Update();
		virtual bool VisibilityTest(const NzFrustumf& frustum) = 0;

		NzScene* m_scene;
		bool m_visible;

	private:
		void UpdateVisibility(const NzFrustumf& frustum);
};

#endif // NAZARA_SCENENODE_HPP
