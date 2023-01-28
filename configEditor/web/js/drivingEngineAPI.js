import initModule from "./dreg/dreg.js"

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
		this.ptr = Module.createVehicle()
		
		this.controlsPtr = new Module.VehicleControls(0, 0, 0, 0, 0)
		this.statePtr = Module.getVehicleState(this.ptr)
		this.propsPtr = Module.getVehicleProps(this.ptr)
		
		this.configManager = new ConfigManagerAPI()
		this.configManager.loadDefault()
		Module.setVehicleConfig(this.ptr, this.configManager.ptr)
		
		//controls do not have struct type attributes, so it can be edited directly
		this.controls = new UpdatedVehicleStruct(this.controlsPtr)
		this.state = new VehicleStruct({pos: {}, rotation: {}}, this.statePtr, Module.updateVehicleStateObj)
		this.props = new VehicleStruct({}, this.propsPtr, Module.updateVehiclePropsObj)
		
		this.config = new UpdatedVehicleStruct(this.configManager.configObj)
	}
	
	delete() {
		Module.deleteVehicle(this.ptr)
		this.configManager.delete()
	}
	
	reset() {
		this.state.invalidate()
		this.props.invalidate()
		Module.resetVehicle(this.ptr)
	}
	
	setInput(input) {
		this.controlsPtr.throttle = input.throttle
		this.controlsPtr.brake = input.brake
		this.controlsPtr.steeringWheel = input.steeringWheel
		this.controlsPtr.clutch = input.clutch
		this.controlsPtr.gear = input.gear
		Module.setVehicleInput(this.ptr, this.controlsPtr)
	}
	
	update(delta) {
		this.state.invalidate()
		this.props.invalidate()
		Module.update(this.ptr, delta)
	}
	
	getAttrib(path) {
		let obj = this[path[0]].getObj()
		
		for (let i = 1; i < path.length; i++)
			obj = obj[path[i]]
		
		return obj
	}
	
	setConfigAttrib(path, value) {
		this.configManager.setConfigAttrib(path, value)
	}
}

class ConfigManagerAPI {
	constructor(configManagerPtr) {
		this.ptr = configManagerPtr != null ? configManagerPtr : Module.createConfigManager(true)
		this.configPtr = Module.getVehicleConfig(this.ptr)
		this.configObj = {frontShaft: {}, rearShaft: {}, power: {}, wheels: {}}
		this.#loadConfigObj()
	}
	
	delete() {
		Module.deleteConfigManager(this.ptr)
	}
	
	clone() {
		return new ConfigManagerAPI(Module.cloneConfig(this.ptr))
	}
	
	update() {
		Module.updateConfig(this.ptr)
	}
	
	loadDefault() {
		Module.loadDefaultConfig(this.ptr)
		this.#loadConfigObj()
	}
	
	loadSerialized(serializedConfig) {
		let success = Module.loadSerializedConfig(this.configPtr, serializedConfig)
		this.#loadConfigObj()
		return success
	}
	
	serialize() {
		return Module.serializeConfig(this.configPtr)
	}
	
	#loadConfigObj() {
		Module.updateVehicleConfigObj(this.configPtr, this.configObj)
	}
	
	setConfigAttrib(path, value) {
		let obj = this.configObj
		for (let i = 1; i < path.length - 1; i++) //Skip the first element in path ("config")
			obj = obj[path[i]]
		
		obj[path[path.length - 1]] = value
		Module.updateVehicleConfigStruct(this.configPtr, this.configObj)
	}
}

export class GraphAPI {
	static setSaveInitData(saveInitData) {
		Module.setGraphSaveInitData(saveInitData)
	}
	
	static setDefaultBezierSamples(samplesPerSegment) {
		Module.setDefaultBezierSamples(samplesPerSegment)
	}
	
	constructor(graph) {
		this.graph = graph != null ? graph : Module.createGraph()
	}
	
	delete() {
		Module.deleteGraph(this.graph)
	}
	
	clone() {
		return new GraphAPI(Module.cloneGraph(this.graph))
	}
	
	loadLinearGraph(refs) {
		Module.loadLinearGraph(this.graph, refs)
	}
	
	loadBezierGraph(refs, samplesPerSegment) {
		Module.loadBezierGraph(this.graph, refs, samplesPerSegment)
	}
	
	getInitData() {
		return Module.getGraphInitData(this.graph)
	}
	
	getPoints() {
		return Module.getGraphPoints(this.graph)
	}
	
	getY(x) {
		return Module.getGraphY(this.graph, x)
	}
}