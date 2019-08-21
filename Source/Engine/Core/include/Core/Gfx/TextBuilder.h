//
// Created by bentoo on 18.04.17.
//

#ifndef YAGE_TEXT_H
#define YAGE_TEXT_H

#include <Utils/String.h>
#include <Utils/Handle.h>
#include <Utils/Color.hpp>
#include <Utils/MemoryBlock.h>

// temporary
#include <Core/Resources/Material/Material.h>

#include <fmt/core.h>

namespace eastl
{
    template <typename Char>
    inline fmt::basic_string_view<Char>
        to_string_view(eastl::basic_string_view<Char> s) { return { s.begin(), s.length() }; }
}

namespace Core
{
    class Material;
}

namespace Resources
{
    class Font;
}

namespace Gfx
{
    class Camera;
    class Renderer;
    class SpriteBatch;

    namespace VerticalAlign
    {
        enum Type
        {
            Top,
            Center,
            Bottom
        };
    }

    namespace HorizontalAlign
    {
        enum Type
        {
            Left,
            Center,
            Right
        };
    }

    class TextBuilder
    {
        Memory::IMemoryBlock&           _memory;
        eastl::string   				text;
        Resources::Font* font;
        Utils::Color					color;
        Utils::Handle<Core::Material>	material;
        glm::vec2						posOffset;
        float 							zOrder;
        HorizontalAlign::Type 			horizontal;
        VerticalAlign::Type 			vertical;

        std::uint32_t computeBatchSize(const eastl::vector<eastl::string_view>& lines) const;
        void drawTextInternal(eastl::string_view line, Gfx::SpriteBatch& batch, glm::vec2 offset);
        int lineLength(eastl::string_view line);

    public:
        explicit TextBuilder(Memory::IMemoryBlock& memory);
        virtual ~TextBuilder();

        Utils::String& getString();
        Gfx::SpriteBatch& getBatch(Renderer& renderer, Gfx::Camera& camera);

        TextBuilder& clearText();
        TextBuilder& withFont(Resources::Font& font);
        TextBuilder& withMaterial(Utils::Handle<Core::Material> material);
        TextBuilder& withOffset(glm::vec2 offset);
        TextBuilder& withZOrder(float zOrder);
        TextBuilder& horizontalAlign(HorizontalAlign::Type align);
        TextBuilder& verticalAlign(VerticalAlign::Type align);
        void         drawAsSpriteBatch(Renderer& renderer, Gfx::Camera& camera);

        template <typename ... Args>
        TextBuilder& appendText(eastl::string_view format, Args&& ... args)
        {
            fmt::format_to(std::back_inserter(text), format, args...);
            return *this;
        }
    };
}

#endif //YAGE_TEXT_H
