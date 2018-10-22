#include "Editor.h"
#include <Utils/MemorySizes.h>
#include <Utils/SafeDelete.h>
#include <Core/Engine.h>

namespace Editor
{
    Editor::Editor()
    {
        _Engine = new Core::Engine("Editor", Memory::MB(20));
    }

    int Editor::Exec()
    {
        
    }

    Editor::~Editor()
    {
        Memory::SafeDelete(_Engine);
    }
}
