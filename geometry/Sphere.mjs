
export default class Sphere{

    constructor(radius, sectorCount, stackCount, smooth) {
        this.radius = radius;
        this.sectorCount = sectorCount;
        this.stackCount = stackCount;
        this.smooth = smooth;
        this.vertices = [];
        this.normals = [];
        this.texCoords = [];
        this.indices = [];
        this.lineIndices = [];
        this.interleavedVertices = [];
        this.interleavedStride = 0;

        if(smooth)
            this._buildVerticesSmooth();
        else
            this._buildVerticesFlat();
    }

    addVertex(x, y, z) {
        this.vertices.push(x);
        this.vertices.push(y);
        this.vertices.push(z);
    }

    addNormal(x, y, z) {
        this.normals.push(x);
        this.normals.push(y);
        this.normals.push(z);
    }

    addTexCoord(x, y) {
        this.texCoords.push(x);
        this.texCoords.push(y);
    }

    addIndices(x, y, z) {
        this.indices.push(x);
        this.indices.push(y);
        this.indices.push(z);
    }

    _buildVerticesSmooth(){

        // clear memory of prev arrays
        //clearArrays();

        let x, y, z, xy;                              // vertex position
        let nx, ny, nz, lengthInv = 1.0 / this.radius;    // normal
        let s, t;                                     // texCoord

        let sectorStep = 2 * Math.PI / this.sectorCount;
        let stackStep = Math.PI / this.stackCount;
        let sectorAngle, stackAngle;

        for (let i = 0; i <= this.stackCount; ++i) {
            stackAngle = Math.PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = this.radius * Math.cos(stackAngle);             // r * cos(u)
            z = this.radius * Math.sin(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (let j = 0; j <= this.sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position
                x = xy * Math.cos(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * Math.sin(sectorAngle);             // r * cos(u) * sin(v)
                this.addVertex(x, y, z);

                // normalized vertex normal
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                this.addNormal(nx, ny, nz);

                // vertex tex coord between [0, 1]
                s = j / this.sectorCount;
                t = i / this.stackCount;
                this.addTexCoord(s, t);
            }
        }

        // indices
        //  k1--k1+1
        //  |  / |
        //  | /  |
        //  k2--k2+1
        let k1, k2;
        for (let i = 0; i < this.stackCount; ++i) {
            k1 = i * (this.sectorCount + 1);     // beginning of current stack
            k2 = k1 + this.sectorCount + 1;      // beginning of next stack

            for (let j = 0; j < this.sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding 1st and last stacks
                if (i !== 0)
                {
                    this.addIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
                }

                if (i !== (this.stackCount - 1))
                {
                    this.addIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
                }

                // vertical lines for all stacks
                this.lineIndices.push(k1);
                this.lineIndices.push(k2);
                if (i !== 0)  // horizontal lines except 1st stack
                {
                    this.lineIndices.push(k1);
                    this.lineIndices.push(k1 + 1);
                }
            }
        }

        // generate interleaved vertex array as well
        this._buildInterleavedVertices();
    }

    _buildVerticesFlat(){

        // tmp vertex definition (x,y,z,s,t)
        class Vertex{
            constructor(x, y, z, s, t) {
                this.x = x;
                this.y = y;
                this.z = z;
                this.s = s;
                this.t = t;
            }
        }

        let tmpVertices = [];

        let sectorStep = 2 * Math.PI / this.sectorCount;
        let stackStep = Math.PI / this.stackCount;
        let sectorAngle, stackAngle;

        // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
        for (let i = 0; i <= this.stackCount; ++i) {
            stackAngle = Math.PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            let xy = this.radius * Math.cos(stackAngle);       // r * cos(u)
            let z = this.radius * Math.sin(stackAngle);        // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (let j = 0; j <= this.sectorCount; ++j) {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                let vertex = new Vertex(
                xy * Math.cos(sectorAngle),      // x = r * cos(u) * cos(v)
                xy * Math.sin(sectorAngle),      // y = r * cos(u) * sin(v)
                 z,                           // z = r * sin(u)
                j / this.sectorCount,        // s
                i / this.stackCount,        // t
            );
                tmpVertices.push(vertex);
            }
        }

        // clear memory of prev arrays
        //clearArrays();

        let v1 = [], v2 = [], v3 = [], v4 = [];                          // 4 vertex positions and tex coords
        let n = [];                           // 1 face normal

        let i, j, k, vi1, vi2;
        let index = 0;                                  // index for vertex
        for (i = 0; i < this.stackCount; ++i) {
            vi1 = i * (this.sectorCount + 1);                // index of tmpVertices
            vi2 = (i + 1) * (this.sectorCount + 1);

            for (j = 0; j < this.sectorCount; ++j, ++vi1, ++vi2) {
                // get 4 vertices per sector
                //  v1--v3
                //  |    |
                //  v2--v4
                v1 = tmpVertices[vi1];
                v2 = tmpVertices[vi2];
                v3 = tmpVertices[vi1 + 1];
                v4 = tmpVertices[vi2 + 1];

                // if 1st stack and last stack, store only 1 triangle per sector
                // otherwise, store 2 triangles (quad) per sector
                if (i === 0) // a triangle for first stack ==========================
                {
                    // put a triangle
                    this.addVertex(v1.x, v1.y, v1.z);
                    this.addVertex(v2.x, v2.y, v2.z);
                    this.addVertex(v4.x, v4.y, v4.z);

                    // put tex coords of triangle
                    this.addTexCoord(v1.s, v1.t);
                    this.addTexCoord(v2.s, v2.t);
                    this.addTexCoord(v4.s, v4.t);

                    // put normal
                    n = this.computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
                    for (k = 0; k < 3; ++k){
                        this.addNormal(n[0], n[1], n[2]);
                    }

                    // put indices of 1 triangle
                    this.addIndices(index, index + 1, index + 2);

                    // indices for line (first stack requires only vertical line)
                    this.lineIndices.push(index);
                    this.lineIndices.push(index + 1);

                    index += 3;     // for next
                }
                else if (i === (this.stackCount - 1)) // a triangle for last stack =========
                {
                    // put a triangle
                    this.addVertex(v1.x, v1.y, v1.z);
                    this.addVertex(v2.x, v2.y, v2.z);
                    this.addVertex(v3.x, v3.y, v3.z);

                    // put tex coords of triangle
                    this.addTexCoord(v1.s, v1.t);
                    this.addTexCoord(v2.s, v2.t);
                    this.addTexCoord(v3.s, v3.t);

                    // put normal
                    n = this.computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                    for (k = 0; k < 3; ++k){
                        this.addNormal(n[0], n[1], n[2]);
                    }

                    // put indices of 1 triangle
                    this.addIndices(index, index + 1, index + 2);

                    // indices for lines (last stack requires both vert/hori lines)
                    this.lineIndices.push(index);
                    this.lineIndices.push(index + 1);
                    this.lineIndices.push(index);
                    this.lineIndices.push(index + 2);

                    index += 3;     // for next
                }
                else // 2 triangles for others ====================================
                {
                    // put quad vertices: v1-v2-v3-v4
                    this.addVertex(v1.x, v1.y, v1.z);
                    this.addVertex(v2.x, v2.y, v2.z);
                    this.addVertex(v3.x, v3.y, v3.z);
                    this.addVertex(v4.x, v4.y, v4.z);

                    // put tex coords of quad
                    this.addTexCoord(v1.s, v1.t);
                    this.addTexCoord(v2.s, v2.t);
                    this.addTexCoord(v3.s, v3.t);
                    this.addTexCoord(v4.s, v4.t);

                    // put normal
                    n = this.computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
                    for (k = 0; k < 4; ++k){
                        this.addNormal(n[0], n[1], n[2]);
                    }

                    // put indices of quad (2 triangles)
                    this.addIndices(index, index + 1, index + 2);
                    this.addIndices(index + 2, index + 1, index + 3);

                    // indices for lines
                    this.lineIndices.push(index);
                    this.lineIndices.push(index + 1);
                    this.lineIndices.push(index);
                    this.lineIndices.push(index + 2);

                    index += 4;     // for next
                }
            }
        }

        // generate interleaved vertex array as well
        this._buildInterleavedVertices();
    }
    _buildInterleavedVertices()
    {

        let i, j;
        let count = this.vertices.length;
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            this.interleavedVertices.push(this.vertices[i]);
            this.interleavedVertices.push(this.vertices[i + 1]);
            this.interleavedVertices.push(this.vertices[i + 2]);

            this.interleavedVertices.push(this.normals[i]);
            this.interleavedVertices.push(this.normals[i + 1]);
            this.interleavedVertices.push(this.normals[i + 2]);

            this.interleavedVertices.push(this.texCoords[j]);
            this.interleavedVertices.push(this.texCoords[j + 1]);
        }
    }

    computeFaceNormal(x1, y1, z1, x2, y2, z2, x3, y3, z3){
        const EPSILON = 0.000001;

        let normal = [0, 0, 0];     // default return value (0,0,0)
        let nx, ny, nz;

        // find 2 edge vectors: v1-v2, v1-v3
        let ex1 = x2 - x1;
        let ey1 = y2 - y1;
        let ez1 = z2 - z1;
        let ex2 = x3 - x1;
        let ey2 = y3 - y1;
        let ez2 = z3 - z1;

        // cross product: e1 x e2
        nx = ey1 * ez2 - ez1 * ey2;
        ny = ez1 * ex2 - ex1 * ez2;
        nz = ex1 * ey2 - ey1 * ex2;

        // normalize only if the length is > 0
        let length = Math.sqrt(nx * nx + ny * ny + nz * nz);
        if (length > EPSILON)
        {
            // normalize
            let lengthInv = 1.0 / length;
            normal[0] = nx * lengthInv;
            normal[1] = ny * lengthInv;
            normal[2] = nz * lengthInv;
        }

        return normal;
    }
}