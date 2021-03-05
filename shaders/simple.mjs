
export const vs_simple = `#version 300 es
            in vec4 coordinates;
            uniform mat4 uModelMatrix;
            uniform mat4 uModelViewMatrix;
            uniform mat4 uModelViewProjectionMatrix;
            void main() {
              gl_Position = uModelViewProjectionMatrix * coordinates;
            }
          `;

export const ps_simple =`#version 300 es
            precision highp float;
            out vec4 outputColor;
            void main() {
                outputColor = vec4(1.0, 1.0, 1.0, 1.0);
            }`;