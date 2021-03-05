
export const vs_texture = `#version 300 es
            
            in vec4 coordinates;
            in vec2 texcoord;
            
            out vec2 v_texcoord;
            
            uniform mat4 uModelMatrix;
            uniform mat4 uModelViewMatrix;
            uniform mat4 uModelViewProjectionMatrix;
    
            void main() {
              gl_Position = uModelViewProjectionMatrix * coordinates;
              // Pass the texcoord to the fragment shader.
              v_texcoord = texcoord;
            }
          `;

export const ps_texture =`#version 300 es
            precision highp float;
            
            // Passed in from the vertex shader.
            in vec2 v_texcoord;
            
            // The texture.
            uniform sampler2D u_texture;
            
            out vec4 outputColor;
            void main() {
                outputColor = texture(u_texture, v_texcoord);
            }`;