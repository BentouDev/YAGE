//
// Created by Bentoo on 2017-04-03.
//

#ifndef YAGE_DEBUGMATERIAL_H
#define YAGE_DEBUGMATERIAL_H

namespace Resources
{
	constexpr const char* const DEBUG_MATERIAL_VERTEX_SOURCE = "#version 330\n"
			"#extension GL_ARB_explicit_uniform_location : require\n"
			"\n"
			"layout(location = 1) uniform mat4 projection;\n"
			"layout(location = 2) uniform mat4 view;\n"
			"layout(location = 3) uniform mat4 model;\n"
			"\n"
			"in vec4 position;\n"
			"in vec2 texcoord;\n"
			"in vec4 color;\n"
			"\n"
			"out vec4 Color;\n"
			"out vec2 Texcoord;\n"
			"\n"
			"void main() {\n"
			"    Color = color;\n"
			"    Texcoord = texcoord;\n"
			"    gl_Position = projection * view * model * position;\n"
			"}";

	constexpr const char* const DEBUG_MATERIAL_FRAGMENT_SOURCE = "#version 150\n"
			"\n"
			"layout (packed) uniform GlobalUniform {\n"
			"    mat4 projection;\n"
			"    mat4 view;\n"
			"} cameraTtransform;\n"
			"\n"
			"layout (packed) uniform LocalUniform {\n"
			"    mat4 transform;\n"
			"} objectTransform;\n"
			"\n"
			"uniform sampler2D diffuseTex;\n"
			"\n"
			"in vec2 Texcoord;\n"
			"in vec4 Color;\n"
			"\n"
			"void main() {\n"
	//		"\tgl_FragColor = Color;\n"
			"\tgl_FragColor = Color * texture(diffuseTex, Texcoord);\n"
			"}";
}

#endif //YAGE_DEBUGMATERIAL_H
