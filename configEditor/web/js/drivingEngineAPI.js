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
		
		this.controlsPtr = new Module.VehicleControls(0, 0, 0, 0, 0)
		this.statePtr = Module.getVehicleState(this.vehicle)
		this.propsPtr = Module.getVehicleProps(this.vehicle)
		this.configPtr = Module.getVehicleConfig(this.vehicle)
		
		//controls do not have struct type attributes, so it can be edited directly
		this.controls = new UpdatedVehicleStruct(this.controlsPtr)
		this.state = new VehicleStruct({pos: {}, rotation: {}}, this.statePtr, Module.updateVehicleStateObj)
		this.props = new VehicleStruct({}, this.propsPtr, Module.updateVehiclePropsObj)
		
		this.configObj = {frontShaft: {}, rearShaft: {}, power: {}, wheels: {}}
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
		this.controlsPtr.clutch = input.clutch
		this.controlsPtr.gear = input.gear
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
	constructor(graph) {
		this.graph = graph != null ? graph : Module.createGraph()
	}
	
	delete() {
		Module.deleteGraph(this.graph)
	}
	
	loadLinearGraph(refs) {
		Module.loadLinearGraph(this.graph, refs)
	}
	
	loadBezierGraph(refs, samplesPerSegment) {
		Module.loadBezierGraph(this.graph, refs, samplesPerSegment)
	}
	
	getGraphPoints() {
		return Module.getGraphPoints(this.graph)
	}
	
	getY(x) {
		return Module.getGraphY(this.graph, x)
	}
}