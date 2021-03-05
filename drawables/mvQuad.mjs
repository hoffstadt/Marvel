import mvDrawable from "../mvDrawable.mjs";
import {ElementType, mvDynamicVertex, mvVertexLayout} from "../mvVertex.mjs";
import mvVertexBuffer from "../bindables/mvVertexBuffer.mjs";
import mvIndexBuffer from "../bindables/mvIndexBuffer.mjs";
import mvShader from "../bindables/mvShader.mjs";
import mvInputLayout from "../bindables/mvInputLayout.mjs";
import mvTransformConstantBuffer from "../bindables/mvTransformConstantBuffer.mjs";
import M4 from "../mvMath.mjs";
import {vs_simple, ps_simple} from "../shaders/simple.mjs";

export default class mvQuad extends mvDrawable {

    constructor(graphics) {
        super();

        this.x = 0.0;
        this.y = 0.0;
        this.z = 0.0;
        this.xangle = 0.0;
        this.yangle = 0.0;
        this.zangle = 0.0;

        let vertex_layout = new mvVertexLayout();
        vertex_layout.append(ElementType.POS_3D);

        let dynamic_vertex = new mvDynamicVertex();
        dynamic_vertex.emplaceBack([-0.5,0.5,0.0]);
        dynamic_vertex.emplaceBack([-0.5,-0.5,0.0]);
        dynamic_vertex.emplaceBack([0.5,-0.5,0.0]);
        dynamic_vertex.emplaceBack([0.5, 0.5,0.0]);

        let vertex_buffer = new mvVertexBuffer(graphics, dynamic_vertex);
        let index_buffer = new mvIndexBuffer(graphics, [
            0,1,2,
            0,2,3
        ]);

        let shader = new mvShader(graphics, vs_simple, ps_simple);

        let input_layout = new mvInputLayout(graphics, shader, vertex_layout);

        let tc_buffer = new mvTransformConstantBuffer();

        this.addBindable(graphics, input_layout);
        this.addBindable(graphics, vertex_buffer);
        this.addBindable(graphics, index_buffer);
        this.addBindable(graphics, shader);
        this.addBindable(graphics, tc_buffer);

    }

    getTransform()
    {
        let matrix = M4.identity();
        matrix = M4.translate(matrix, this.x, this.y, this.z);
        matrix = M4.xRotate(matrix, this.xangle);
        matrix = M4.yRotate(matrix, this.yangle);
        matrix = M4.zRotate(matrix, this.zangle);

        return matrix;
    }

}