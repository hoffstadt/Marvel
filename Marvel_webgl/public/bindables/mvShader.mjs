
import mvBindable from "./mvBindable.mjs";

export default class mvShader extends mvBindable {

    constructor(graphics, vertex_code, pixel_code) {
        super();

        let gl = graphics.getContext();

        let vertex_shader = gl.createShader(WebGL2RenderingContext.VERTEX_SHADER);
        gl.shaderSource(vertex_shader, vertex_code);
        gl.compileShader(vertex_shader);

        if (!gl.getShaderParameter(vertex_shader, gl.COMPILE_STATUS)) {
            console.log(vertex_code, gl.getShaderInfoLog(vertex_shader));
        }

        let pixel_shader = gl.createShader(WebGL2RenderingContext.FRAGMENT_SHADER);
        gl.shaderSource(pixel_shader, pixel_code);
        gl.compileShader(pixel_shader);

        if (!gl.getShaderParameter(pixel_shader, gl.COMPILE_STATUS)) {
            console.log(pixel_code, gl.getShaderInfoLog(pixel_shader));
        }

        this._shader = gl.createProgram();
        gl.attachShader(this._shader, vertex_shader);
        gl.attachShader(this._shader, pixel_shader);
        gl.linkProgram(this._shader);

    }

    bind(graphics) {
        graphics._program = this._shader;
        graphics.getContext().useProgram(this._shader);
    }

}