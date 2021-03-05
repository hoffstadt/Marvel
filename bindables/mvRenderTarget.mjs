
import mvBindable from "./mvBindable.mjs";

export default class mvRenderTarget extends mvBindable {

    constructor(canvasID) {
        super();

        let canvas = document.getElementById(canvasID);
        this._context = canvas.getContext('webgl2')
        this.width = canvas.width;
        this.height = canvas.height;

    }

    bind(graphics) {

        //graphics.getContext().clearDepth(1.0);                 // Clear everything
        //graphics.getContext().depthFunc(WebGL2RenderingContext.LEQUAL);            // Near things obscure far things

        // Clear the canvas
        graphics.getContext().clearColor(0, 0, 0, 1.0);

        // Enable the depth test
        graphics.getContext().enable(WebGL2RenderingContext.DEPTH_TEST);
        graphics.getContext().enable(WebGL2RenderingContext.CULL_FACE);

        // Clear the color buffer bit
        graphics.getContext().clear(WebGL2RenderingContext.COLOR_BUFFER_BIT | WebGL2RenderingContext.DEPTH_BUFFER_BIT);

        // Set the view port
        graphics.getContext().viewport(0,0,this.width,this.height);
    }

    getContext() {
        return this._context;
    }


}