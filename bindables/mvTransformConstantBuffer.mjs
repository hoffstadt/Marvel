
import {mvConstantBuffer, UniformType} from "./mvConstantBuffer.mjs";
import mvBindable from "./mvBindable.mjs";
import M4 from "../mvMath.mjs";


export default class mvTransformConstantBuffer extends mvBindable {

    constructor() {
        super();

        this.model = new mvConstantBuffer("uModelMatrix", M4.identity(), UniformType.MATRIX4_FLOAT);
        this.modelView = new mvConstantBuffer("uModelViewMatrix", M4.identity(), UniformType.MATRIX4_FLOAT);
        this.modelViewProjection = new mvConstantBuffer("uModelViewProjectionMatrix", M4.identity(), UniformType.MATRIX4_FLOAT);

    }

    bind(graphics) {

        let data = this.getTransforms(graphics);
        let model = data[0];
        let view = data[1];
        let projection = data[2];

        let modelView = M4.multiply(view, model);
        let modelViewProjection = M4.multiply(projection, modelView);

        this.model.update(model);
        this.modelView.update(modelView);
        this.modelViewProjection.update(modelViewProjection);

        this.model.bind(graphics);
        this.modelView.bind(graphics);
        this.modelViewProjection.bind(graphics);
    }


    getTransforms(graphics) {

        let model = this.parent.getTransform();
        let view = graphics.getCamera();
        let projection = graphics.getProjection();

        return [model, view, projection]
    }
}