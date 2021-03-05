
export const ElementType = {
    POS_3D: "position 3D",
    NORM: "normals",
    TEX: "texture"
}

export class mvElement {

    constructor(type, sys_type, size, semantic, normalize, glsize) {
        this.type = type;
        this.sys_type = sys_type;
        this.size = size;
        this.semantic = semantic;
        this.normalize = normalize;
        this.glsize = glsize;
    }

}

export class mvVertexLayout {

    constructor() {
        this._elements = [];
    }

    append(element_type) {

        switch (element_type) {

            case ElementType.POS_3D: {
                let element = new mvElement(element_type, WebGL2RenderingContext.FLOAT, 3, "coordinates", false, 4);
                this._elements.push(element);
                break;
            }

            case ElementType.NORM: {
                let element = new mvElement(element_type, WebGL2RenderingContext.FLOAT, 3, "normals", false, 4);
                this._elements.push(element);
                break;
            }

            case ElementType.TEX: {
                let element = new mvElement(element_type, WebGL2RenderingContext.FLOAT, 2, "texcoord", true, 4);
                this._elements.push(element);
                break;
            }

        }

    }

    getElements() {
        return this._elements;
    }

    getStride()
    {
        let stride = 0;
        for(let i = 0; i < this._elements.length; i++)
        {
            stride += this._elements[i].glsize*this._elements[i].size;
        }

        return stride;
    }

}

export class mvDynamicVertex {

    constructor() {
        this._data = [];
    }

    emplaceBack(...args) {
        for (let arg of args) {
            for(let inner of arg) {
                this._data.push(inner);
            }

        }
    }

}