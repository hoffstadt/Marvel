
import M4 from "./mvMath.mjs";

export default class mvProjection {

    constructor(graphics, width, height, nearZ, farZ) {

        this.width = width;
        this.height = height;
        this.nearZ = nearZ;
        this.farZ = farZ;
    }

    getMatrix() {

        const fieldOfView = 45 * Math.PI / 180;   // in radians
        let aspect = this.width / this.height;

        return M4.perspective(fieldOfView, aspect, this.nearZ, this.farZ);

    }


}