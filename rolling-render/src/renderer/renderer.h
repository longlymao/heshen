/*
 * Copyright (c) 2025 longlymao
 * All rights reserved.
 * MIT License
 */

#pragma once

#include "commandlist.h"

#include "image.h"

#include <queue>
#include <functional>

namespace rolling {

	class Renderer {
	public:
		using PresentFunc = std::function<void(const rolling::Image<unsigned int>&)>;

		static Renderer& GetInstance();

		Renderer() {}
		

		void SetPresentFunc(PresentFunc func);

		void Prepare(const rolling::Rect<int>& clientRect);
		void Render();
		void Present();

		void AddCommandList(const CommandList& commandList);

	private:
		std::queue<CommandList> m_CommandQueue;

		void ExecuteCommandList(const CommandList& commandList);

		Image<unsigned int> m_RenderTarget;

		PresentFunc m_PresentFunc;
	};

};