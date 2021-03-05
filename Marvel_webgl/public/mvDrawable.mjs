import mvIndexBuffer from "./bindables/mvIndexBuffer.mjs";

export default class mvDrawable {

    constructor() {

        this._bindables = [];
        this._index_buffer = null;

    }

    addBindable(graphics, bindable)
    {
        if(bindable instanceof mvIndexBuffer)
            this._index_buffer = bindable;

        bindable.setParent(this);
        bindable.bind(graphics);

        this._bindables.push(bindable);
    }

    bind(graphics) {

        for(let i = 0; i < this._bindables.length; i++) {
            this._bindables[i].bind(graphics);
        }
    }

    draw(graphics) {
        graphics.draw(this._index_buffer.getCount());
    }

    getTransform()
    {

    }

}