
import mvBindable from "./mvBindable.mjs";

export const UniformType = {
    MATRIX4_FLOAT: "MATRIX4_FLOAT",
    VECTOR3_FLOAT: "VECTOR3_FLOAT",
    FLOAT: "FLOAT"
}

export class mvConstantBuffer extends mvBindable {

    constructor(name, data, type) {
        super();

        this._name = name;
        this._data = data;
        this._type = type;

    }

    bind(graphics) {

        if(this._type === UniformType.MATRIX4_FLOAT) {
            graphics.getContext().uniformMatrix4fv(graphics.getContext().getUniformLocation(graphics._program, this._name), false, this._data);
        }
        else if(this._type === UniformType.VECTOR3_FLOAT) {
            graphics.getContext().uniform3fv(graphics.getContext().getUniformLocation(graphics._program, this._name), this._data);
        }
        else if(this._type === UniformType.FLOAT) {
            graphics.getContext().uniform1f(graphics.getContext().getUniformLocation(graphics._program, this._name), this._data);
        }
    }


    update(data) {
        this._data = data;
    }
}