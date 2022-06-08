import initModule from "./driving-engine/driving-engine.js"

const Module = initModule()
export const setModuleLoadFunc = func => Module.onRuntimeInitialized = func

export class GraphAPI {
	constructor() {
		this.graph = Module.createGraph()
		this.samplesPerSegment = 80
	}
	
	setSamplesPerSegment(samplesPerSegment) {
		this.samplesPerSegment = samplesPerSegment
	}
	
	delete() {
		Module.deleteGraph(this.graph)
	}
	
	#refsToPoints(refs, loadGraphFunc) {
		//Conver the refs from js Array to C array and load the graph
		let nBytes = refs.length * 2 * Float32Array.BYTES_PER_ELEMENT
		let refsPtr = Module._malloc(nBytes)
		
		let curRefPtr = refsPtr / Float32Array.BYTES_PER_ELEMENT
		for (let ref of refs) {
			Module.HEAPF32[curRefPtr++] = ref[0]
			Module.HEAPF32[curRefPtr++] = ref[1]
		}
		
		loadGraphFunc(this.graph, refsPtr, refs.length, this.samplesPerSegment)
		
		//Convert the graph points from C array to js arrays
		let pointsPtr = Module.getGraphPoints(this.graph, refsPtr).$$.ptr / Float32Array.BYTES_PER_ELEMENT
		let pointsCount = Module.HEAP32[refsPtr / Int32Array.BYTES_PER_ELEMENT]
		
		let xValues = new Array(pointsCount + 2)
		let yValues = new Array(pointsCount + 2)
		
		for (let i = 1; i < xValues.length - 1; i++) {
			xValues[i] = Module.HEAPF32[pointsPtr++]
			yValues[i] = Module.HEAPF32[pointsPtr++]
		}
		Module._free(refsPtr)
		
		//Add extreme points
		let minX = xValues[1]
		let maxX = xValues[xValues.length - 2]
		let interv = (maxX - minX) * 10
		
		xValues[0] = minX - interv
		yValues[0] = this.getY(minX - interv)
		
		xValues[xValues.length - 1] = maxX + interv
		yValues[xValues.length - 1] = this.getY(maxX + interv)
		return [xValues, yValues]
	}
	
	linearRefsToGraph(refs) {
		return this.#refsToPoints(refs, Module.loadLinearGraph)
	}
	
	bezierRefsToGraph(refs) {
		return this.#refsToPoints(refs, Module.loadBezierGraph)
	}
	
	getY(x) {
		return Module.getGraphY(this.graph, x)
	}
}