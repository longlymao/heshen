#include "renderer.h"

#include "shader/lineshader.h"

namespace rolling {
	Renderer& Renderer::GetInstance()
	{
		static Renderer instance{};
		return instance;
	}

	void Renderer::SetPresentFunc(PresentFunc func)
	{
		m_PresentFunc = func;
	}

	void Renderer::Prepare(const rolling::Rect<int>& clientRect)
	{
		m_RenderTarget = Image<unsigned int>{ clientRect.GetWidth(), clientRect.GetHeight() };
		while (!m_CommandQueue.empty()) {
			m_CommandQueue.pop();
		}
	}

	void Renderer::Render()
	{
		while (!m_CommandQueue.empty()) {
			CommandList& commandList = m_CommandQueue.front();
			ExecuteCommandList(commandList);
			m_CommandQueue.pop();
		}
	}

	void Renderer::Present()
	{
		if (m_PresentFunc) {
			m_PresentFunc(m_RenderTarget);
		}
	}

	void Renderer::AddCommandList(const CommandList& commandList)
	{
		m_CommandQueue.push(commandList);
	}

	void Renderer::ExecuteCommandList(const CommandList& commandList)
	{
		for (const auto& command : commandList.m_CommandList) {
			switch (command.primitiveType) {
			case PrimitiveType::LINE:
			{
				LineShaderPipeLine shader;
				shader.ProcessBuffer(command.buffer, m_RenderTarget);
			}
				break;
			case PrimitiveType::TRIANGLE:
				// Execute triangle rendering
				break;
			default:
				break;
			}
		}
	}
}