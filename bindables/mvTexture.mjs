
import mvBindable from "./mvBindable.mjs";

export default class mvTexture extends mvBindable {

    constructor(graphics, image) {
        super();

        let gl = graphics.getContext();

        this.texture = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_2D, this.texture);

        // Fill the texture with a 1x1 blue pixel.
        //gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 1, 1, 0, gl.RGBA, gl.UNSIGNED_BYTE,
            //new Uint8Array([0, 0, 255, 255]));

        this.image = new Image();
        this.image.src = image;

        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, this.image);
        this.width = this.image.width;
        this.height = this.image.height;

    }

    bind(graphics) {
        let gl = graphics.getContext();
        gl.bindTexture(WebGL2RenderingContext.TEXTURE_2D, this.texture);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, this.image);
        gl.generateMipmap(WebGL2RenderingContext.TEXTURE_2D);
    }

}