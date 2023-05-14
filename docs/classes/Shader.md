---
layout: default
title: Shader
parent: API Documentation
---

## Shader
In Astro shaders provide a way to manipulate how textures and sprites are rendered.
You may load shaders in a vertex/fragment pair and then tell the renderer to use
your shader instead of its default shader with [shader=](Renderer.md#shader).
The default shader just outputs the texture/sprite with the colour mod applied.

 + [load](#load)
 + [data=](#data)
 
### load
`construct load(vertex_file, frag_file, buffer_size)`

Parameters
 + `vertex_file -> String` Filename of the SPIR-V compiled vertex shader.
 + `frag_file -> String` Filename of the SPIR-V compiled fragment shader.
 + `buffer_size -> Num` Size of the uniform buffer in your shaders in bytes. Must be a multiple of 4.

Loads a new shader from provided SPIR-V compiled files. How you make these
shaders is very strict.

The vertex shader must use this as a header:

    layout(set = 0, binding = 0) uniform UniformBufferObject {
        mat4 viewproj;
    } ubo;
    
    layout(push_constant) uniform PushBuffer {
        mat4 model;
        vec4 colourMod;
        vec4 textureCoords;
    } pushBuffer;
    
    layout(location = 1) out vec2 fragTexCoord;
    
    vec2 vertices[] = {
        vec2(0.0f, 0.0f),
        vec2(1.0f, 0.0f),
        vec2(1.0f, 1.0f),
        vec2(1.0f, 1.0f),
        vec2(0.0f, 1.0f),
        vec2(0.0f, 0.0f),
    };
    
    vec2 texCoords[] = {
        vec2(0.0f, 0.0f),
        vec2(1.0f, 0.0f),
        vec2(1.0f, 1.0f),
        vec2(1.0f, 1.0f),
        vec2(0.0f, 1.0f),
        vec2(0.0f, 0.0f),
    };
    
    out gl_PerVertex {
        vec4 gl_Position;
    };

You may notice that the renderer does not pass vertex coordinates for texture and instead
just embeds them in the shader. This means that the fragment shader applies UV changes
internally. The fragment shader must have this header:

    layout(set = 1, binding = 1) uniform sampler texSampler;
    layout(set = 2, binding = 2) uniform texture2D tex;
    
    layout(push_constant) uniform PushBuffer {
        mat4 model;
        vec4 colourMod;
        vec4 textureCoords;
    } pushBuffer;
    
    layout(location = 1) in vec2 fragTexCoord;
    
    layout(location = 0) out vec4 outColor;

If `buffer_size` is not 0, you must also add the following to both files:

    layout(set = 3, binding = 3) uniform UserData {
        // your data here...
    } userData;

This is how your shaders will access data from your program.

Astro uses [Vulkan2D](https://github.com/PaoloMazzon/Vulkan2D) internally, so if you wish to see the default shaders,
[fragment shader](https://github.com/PaoloMazzon/Vulkan2D/blob/master/shaders/tex.frag),
and [vertex shader](https://github.com/PaoloMazzon/Vulkan2D/blob/master/shaders/tex.vert).

### data=
`data=(data)`

Parameters
 + `data -> Buffer` Buffer containing the uniform buffer's data.

Sets the uniform buffer's data. This must be used at least once if the `buffer_size`
size set in [load](#load) was non-zero. `data.size` must be at least that specified
amount. Each time the shader is used, this data will be provided to the shader until
this is updated. You may update shader data as many times as you wish per frame.

