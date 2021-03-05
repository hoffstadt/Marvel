
import mvBindable from "./mvBindable.mjs";

export default class mvVertexBuffer extends mvBindable {

    constructor(graphics, dynamic_vertex_buffer) {
        super();

        let gl = graphics.getContext();

        // Create an empty buffer object to store vertex buffer
        this.vertex_buffer = gl.createBuffer();

        // Bind appropriate array buffer to it
        gl.bindBuffer(WebGL2RenderingContext.ARRAY_BUFFER, this.vertex_buffer);

        // Pass the vertex data to the buffer
        gl.bufferData(WebGL2RenderingContext.ARRAY_BUFFER, new Float32Array(dynamic_vertex_buffer._data), WebGL2RenderingContext.STATIC_DRAW);

    }

    bind(graphics) {
        graphics.getContext().bindBuffer(WebGL2RenderingContext.ARRAY_BUFFER, this.vertex_buffer);
    }

}