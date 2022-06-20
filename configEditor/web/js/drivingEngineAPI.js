import initModule from "./driving-engine/driving-engine.js"

const Module = initModule()
export const setModuleLoadFunc = func => Module.onRuntimeInitialized = func

class VehicleStruct {
	constructor(obj, structPtr, updateObjFunc) {
		this.obj = obj
		this.structPtr = structPtr
		this.updateObjFunc = updateObjFunc
		
		this.updated = false
	}
	
	getObj() {
		if (! this.updated) {
			this.updateObjFunc(this.structPtr, this.obj)
			this.updated = true
		}
		return this.obj
	}
	
	invalidate() {
		this.updated = false
	}
}

class UpdatedVehicleStruct {
	constructor(struct) {
		this.struct = struct
	}
	getObj() {
		return this.struct
	}
	invalidate() {}
}

export class VehicleAPI {
	constructor() {
		this.vehicle = Module.createVehicle()
		
		this.controlsPtr = new Module.VehicleControls(0, 0, 0)
		this.statePtr = Module.getVehicleState(this.vehicle)
		this.propsPtr = Module.getVehicleProps(this.vehicle)
		this.configPtr = Module.getVehicleConfig(this.vehicle)
		
		//controls do not have struct type attributes, so it can be edited directly
		this.controls = new UpdatedVehicleStruct(this.controlsPtr)
		this.state = new VehicleStruct({pos: {}, rotation: {}}, this.statePtr, Module.updateVehicleStateObj)
		this.props = new VehicleStruct({}, this.propsPtr, Module.updateVehiclePropsObj)
		
		this.configObj = {frontShaft: {}, rearShaft: {}}
		Module.updateVehicleConfigObj(this.configPtr, this.configObj)
		this.config = new UpdatedVehicleStruct(this.configObj)
	}
	
	delete() {
		Module.deleteVehicle(this.vehicle)
	}
	
	reset() {
		Module.resetVehicle(this.vehicle)
	}
	
	setInput(input) {
		this.controlsPtr.throttle = input.throttle
		this.controlsPtr.brake = input.brake
		this.controlsPtr.steeringWheel = input.steeringWheel
		Module.setVehicleInput(this.vehicle, this.controlsPtr)
	}
	
	updateConfig() {
		Module.updateVehicleConfig(this.vehicle)
	}
	
	update(delta) {
		this.state.invalidate()
		this.props.invalidate()
		Module.update(this.vehicle, delta)
	}
	
	getAttrib(path) {
		let obj = this[path[0]].getObj()
		
		for (let i = 1; i < path.length; i++)
			obj = obj[path[i]]
		
		return obj
	}
	
	setConfigAttrib(path, value) {
		let obj = this.configObj
		for (let i = 1; i < path.length - 1; i++) //Skip the first element in path ("config")
			obj = obj[path[i]]
		
		obj[path[path.length - 1]] = value
		Module.updateVehicleConfigStruct(this.configPtr, this.configObj);
	}
}

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