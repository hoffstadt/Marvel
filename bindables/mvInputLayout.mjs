
import mvBindable from "./mvBindable.mjs";

export default class mvInputLayout extends mvBindable {

    constructor(graphics, shader, layout) {
        super();

        this._shader = shader;
        this._layout = layout;
        this._vao = graphics.getContext().createVertexArray();

        let gl = graphics.getContext();

        let elements = this._layout.getElements();


        gl.bindVertexArray(this._vao);

        let offset = 0;

        for(let i = 0; i < elements.length; i++) {

            // Get the attribute location
            //let loc = gl.getAttribLocation(this._shader._shader, elements[i].semantic);

            // Point an attribute to the currently bound VBO
            gl.vertexAttribPointer(i, elements[i].size, elements[i].sys_type, elements[i].normalize,
                layout.getStride(), offset);

            // Enable the attribute
            gl.enableVertexAttribArray(i);

            gl.bindAttribLocation(this._shader._shader, i, elements[i].semantic);

            offset += elements[i].glsize*elements[i].size;

        }

        gl.bindVertexArray(null);


    }

    bind(graphics) {
        graphics.getContext().bindVertexArray(this._vao);
    }

}