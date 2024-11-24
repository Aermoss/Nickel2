#include <Nickel2/nkpch.hpp>
#include <Nickel2/Renderer/RenderCommand.hpp>

namespace Nickel2 {
    std::unique_ptr<RendererAPI> RenderCommand::api = RendererAPI::Create();
}