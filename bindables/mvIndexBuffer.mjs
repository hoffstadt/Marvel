
import mvBindable from "./mvBindable.mjs";

export default class mvIndexBuffer extends mvBindable {

    constructor(graphics, indices) {
        super();

        let gl = graphics.getContext();
        this._count = indices.length;

        // Create an empty buffer object to store Index buffer
        this.index_Buffer = gl.createBuffer();

        // Bind appropriate array buffer to it
        gl.bindBuffer(WebGL2RenderingContext.ELEMENT_ARRAY_BUFFER, this.index_Buffer);

        // Pass the vertex data to the buffer
        gl.bufferData(WebGL2RenderingContext.ELEMENT_ARRAY_BUFFER, new Uint16Array(indices), WebGL2RenderingContext.STATIC_DRAW);

        // Unbind the buffer
        gl.bindBuffer(WebGL2RenderingContext.ELEMENT_ARRAY_BUFFER, null);

    }

    bind(graphics) {
        graphics.getContext().bindBuffer(WebGL2RenderingContext.ELEMENT_ARRAY_BUFFER, this.index_Buffer);
    }

    getCount() {
        return this._count;
    }

}