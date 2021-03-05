import mvProjection from "./mvProjection.mjs";
import M4 from "./mvMath.mjs";

export default class mvCamera {

    constructor(graphics, home_position, home_rotation) {

        this._pos = home_position;
        this._rotation = home_rotation;
        this._projection = new mvProjection(graphics, graphics._target.width, graphics._target.height, 0.1, 100.0);
        graphics.setProjection(this._projection.getMatrix());
        this.bind(graphics);

    }

    getMatrix() {

        let x_rotation = M4.xRotation(this._rotation[0]);
        let y_rotation = M4.yRotation(this._rotation[1]);

        let roll_pitch_yaw = M4.multiply(x_rotation, y_rotation);

        let forward_base_vector = [0, 0, -1, 0];

        let look_vector = M4.transformVector(roll_pitch_yaw, forward_base_vector);

        let look_target = M4.addVectors(this._pos, look_vector);

        let camera_matrix = M4.lookAt(this._pos, look_target, [0, 1, 0, 0]);

        return M4.inverse(camera_matrix);

    }

    setPosition(x, y, z) {
        this._pos[0] = x;
        this._pos[1] = y;
        this._pos[2] = z;
    }

    setRotation(x, y, z) {
        this._rotation[0] = x;
        this._rotation[1] = y;
        this._rotation[2] = z;
    }

    translate(x, y, z) {

        let matrix = M4.xRotation(this._rotation[0]);
        matrix = M4.yRotate(matrix, this._rotation[1]);

        let translation = [x, y, z, 0];
        translation = M4.transformVector(matrix, translation);

        this._pos[0] += translation[0];
        this._pos[1] += translation[1];
        this._pos[2] += translation[2];
    }

    rotate(dx, dy, dz) {
        this._rotation[0] += dx;
        this._rotation[1] += dy;
        this._rotation[2] += dz;
    }

    getPosition() {
        return this._pos;
    }

    getRotation() {
        return this._rotation;
    }


    bind(graphics) {
        graphics.setCamera(this.getMatrix());
        graphics.setProjection(this._projection.getMatrix());
    }


}