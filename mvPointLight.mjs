import mvSphere from "./drawables/mvSphere.mjs";
import {mvConstantBuffer, UniformType} from "./bindables/mvConstantBuffer.mjs";
import M4 from "./mvMath.mjs";

export default class mvPointLight {

    constructor(graphics, position) {

        this._sphere = new mvSphere(graphics, 0.1, 0);
        this._sphere.x = position[0];
        this._sphere.y = position[1];
        this._sphere.z = position[2];

        this.uPosition = new mvConstantBuffer("uPosition", position, UniformType.VECTOR3_FLOAT);
        this.uAmbient = new mvConstantBuffer("uAmbient", [0.25,0.25,0.25], UniformType.VECTOR3_FLOAT);
        this.uDiffuse = new mvConstantBuffer("uDiffuse", [1.0,1.0,1.0], UniformType.VECTOR3_FLOAT);
        this.uDiffuseIntensity = new mvConstantBuffer("uDiffuseIntensity", 1.0, UniformType.FLOAT);
        this.uAttConst = new mvConstantBuffer("uAttConst", 1.0, UniformType.FLOAT);
        this.uAttLin = new mvConstantBuffer("uAttLin", 0.045, UniformType.FLOAT);
        this.uAttQuad = new mvConstantBuffer("uAttQuad", 0.0075, UniformType.FLOAT);

    }

    bind(graphics, view){

        let newposition = M4.transformVector(view, [this._sphere.x, this._sphere.y, this._sphere.z, 1]);

        this.uPosition.update([newposition[0], newposition[1], newposition[2]]);
        this.uAmbient.update([0.05,0.05,0.05]);
        this.uDiffuse.update([1.0,1.0,1.0]);
        this.uDiffuseIntensity.update(1);
        this.uAttConst.update(1);
        this.uAttLin.update(0.045);
        this.uAttQuad.update(0.0075);

        this.uPosition.bind(graphics);
        this.uAmbient.bind(graphics);
        this.uDiffuse.bind(graphics);
        this.uDiffuseIntensity.bind(graphics);
        this.uAttConst.bind(graphics);
        this.uAttLin.bind(graphics);
        this.uAttQuad.bind(graphics);
        this._sphere.bind(graphics);

    }

    translate(x, y, z){
        this._sphere.x += x;
        this._sphere.y += y;
        this._sphere.z += z;
    }

    setPosition(x, y, z){
        this._sphere.x = x;
        this._sphere.y = y;
        this._sphere.z = z;
    }

    draw(graphics) {
        this._sphere.draw(graphics);
    }

}