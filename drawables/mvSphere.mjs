import mvDrawable from "../mvDrawable.mjs";
import {ElementType, mvDynamicVertex, mvVertexLayout} from "../mvVertex.mjs";
import mvVertexBuffer from "../bindables/mvVertexBuffer.mjs";
import mvIndexBuffer from "../bindables/mvIndexBuffer.mjs";
import mvShader from "../bindables/mvShader.mjs";
import mvInputLayout from "../bindables/mvInputLayout.mjs";
import mvTransformConstantBuffer from "../bindables/mvTransformConstantBuffer.mjs";
import M4 from "../mvMath.mjs";
import {vs_simple, ps_simple} from "../shaders/simple.mjs";
import {vs_phong, ps_phong} from "../shaders/phong.mjs";
import Sphere from "../geometry/Sphere.mjs";



export default class mvSphere extends mvDrawable {

    constructor(graphics, radius, type) {
        super();

        this.x = 0.0;
        this.y = 0.0;
        this.z = 0.0;

        let vertex_layout = new mvVertexLayout();
        vertex_layout.append(ElementType.POS_3D);
        vertex_layout.append(ElementType.NORM);

        let dynamic_vertex = new mvDynamicVertex();

        let sphere = new Sphere(radius, 36, 18, true);

        let vertices = sphere.vertices;
        let indices = sphere.indices;
        let normals = sphere.normals;
        let nvertices = [];
        for(let i = 0; i<vertices.length; i = i +3){
            nvertices.push(vertices[i]);
            nvertices.push(vertices[i+1]);
            nvertices.push(vertices[i+2]);
            nvertices.push(normals[i]);
            nvertices.push(normals[i+1]);
            nvertices.push(normals[i+2]);
        }

        dynamic_vertex.emplaceBack(nvertices);

        let vertex_buffer = new mvVertexBuffer(graphics, dynamic_vertex);
        let index_buffer = new mvIndexBuffer(graphics, indices);

        let shader = null;
        if(type === 0)
            shader = new mvShader(graphics, vs_simple, ps_simple);
        if(type === 1)
            shader = new mvShader(graphics, vs_phong, ps_phong);

        let input_layout = new mvInputLayout(graphics, shader, vertex_layout);

        let tc_buffer = new mvTransformConstantBuffer();

        this.addBindable(graphics, input_layout);
        this.addBindable(graphics, vertex_buffer);
        this.addBindable(graphics, index_buffer);
        this.addBindable(graphics, shader);
        this.addBindable(graphics, tc_buffer);

    }

    translate(x, y, z){
        this.x += x;
        this.y += y;
        this.z += z;
    }

    setPosition(x, y, z){
        this.x = x;
        this.y = y;
        this.z = z;
    }

    getTransform()
    {
        let matrix = M4.identity();
        matrix = M4.translate(matrix, this.x, this.y, this.z);
        return matrix;
    }

}