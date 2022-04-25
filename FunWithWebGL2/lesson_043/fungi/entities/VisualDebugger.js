import Renderable		from "./Renderable.js";
import DynamicBuffer	from "../util/DynamicBuffer.js";
import gl, {VAO, ATTR_POSITION_LOC} from "../gl.js";

class VisualDebugger extends Renderable{
	constructor(){
		super(null,"MatVecWColor");
		this.drawMode = gl.ctx.LINES;
		this.isModified = false;
		this.name = "VisualDebugger";

		var bufByteSize = Float32Array.BYTES_PER_ELEMENT * 4 * 100;

		//Build VAO and Buffers for rendering.
		this.vao = VAO.create();
		VAO.emptyFloatArrayBuffer(this.vao,"bVertices",bufByteSize,ATTR_POSITION_LOC,4,0,0).finalize(this.vao,"VisDebug");

		this.vertBuffer = DynamicBuffer.newFloat(this.vao.bVertices.ptr,4,100);

		/*
		VAO.create(this.vao)
			.emptyFloatArrayBuffer(this.vao,"vert",bufByteSize,ATTR_POSITION_LOC,4,0,0,false)
			.finalize(this.vao,"VisDebug");

		this.vertBuffer = DynamicBuffer.newFloat(this.vao.buffers["vert"].buf,4,100);
		*/

		
	}

	drawPoints(){		this.drawMode = gl.clx.POINTS;		return this; }
	drawLines(){		this.drawMode = gl.clx.LINES;		return this; }
	drawTriangles(){	this.drawMode = gl.clx.TRIANGLES;	return this; }

	addRawPoint(x,y,z,w){
		this.vertBuffer.data.push(x,y,z,w);
		this.isModified = true;
		return this;
	}
	addVecPoint(v,w){
		this.vertBuffer.data.push(v.x,v.y,v.z,w);
		this.isModified = true;
		return this;
	}

	addRawLine(x1,y1,z1,w1, x2,y2,z2,w2){
		this.vertBuffer.data.push(x1,y1,z1,w1, x2,y2,z2,w2);
		this.isModified = true;	
		return this;
	}
	addVecLine(v1,w1, v2,w2){
		this.vertBuffer.data.push(v1.x,v1.y,v1.z,w1, v2.x,v2.y,v2.z,w2);
		this.isModified = true;	
		return this;
	}

	reset(){
		this.vertBuffer.data.length = 0;
		this.vao.count = 0;
		this.isModified = true;
		return this;
	}

	draw(){
		if(this.isModified){
			this.vertBuffer.pushToGPU();
			this.vao.count	= this.vertBuffer.getComponentCnt();
			this.isModified	= false;
		}
		super.draw();
	}
}

export default VisualDebugger;