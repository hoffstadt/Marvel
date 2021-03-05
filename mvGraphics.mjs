import mvCamera from "./mvCamera.mjs";
import mvRenderTarget from "./bindables/mvRenderTarget.mjs";

export default class mvGraphics {

    constructor(canvasID) {
        this._target = new mvRenderTarget(canvasID);
        this._projection = null;
        this._camera = new mvCamera(this, [0, 0, 0], [0, 0, 0]);
        this._program = null;
    }

    getContext() {
        return this._target.getContext();
    }

    setRenderTarget(target) {
        this._target = target;
    }

    getRenderTarget() {
        return this._target;
    }

    setProjection(projection) {
        this._projection = projection;
    }

    getProjection() {
        return this._projection;
    }

    setCamera(camera) {
        this._camera = camera;
    }

    getCamera() {
        return this._camera;
    }

    draw(count) {

        this.getContext().drawElements(WebGL2RenderingContext.TRIANGLES, count, WebGL2RenderingContext.UNSIGNED_SHORT,0);

    }

}