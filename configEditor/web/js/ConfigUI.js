import {EditableGraphUI} from "./graph/GraphUI.js"
import {GraphAPI} from "./DregApi.js"

const VEHICLE_CONFIG_ATTRIBS = [
	{name: "frontShaft-x", path: "config.frontShaft.x"},
	{name: "frontShaft-y", path: "config.frontShaft.y"},
	{name: "frontShaft-z", path: "config.frontShaft.z"},
	{name: "rearShaft-x", path: "config.rearShaft.x"},
	{name: "rearShaft-y", path: "config.rearShaft.y"},
	{name: "rearShaft-z", path: "config.rearShaft.z"},
	{name: "maxSteeringAngle", path: "config.maxSteeringAngle"},
	{name: "mass", path: "config.mass"},
	{name: "torqueToRpmAccel", path: "config.power.torqueToRpmAccel"},
	{name: "driveRatio", path: "config.power.driveRatio"},
	{name: "wheelDiameter", path: "config.wheels.diameter"},
	{name: "brakeDiameter", path: "config.wheels.brakeDiameter"},
	{name: "brakeStaticFrictionCoef", path: "config.wheels.brakeStaticFrictionCoef"},
	{name: "brakeKineticFrictionCoef", path: "config.wheels.brakeKineticFrictionCoef"}
]

const VEHICLE_CONFIG_GRAPHS = [
	{name: "throttleCurve", path: "config.power.throttleCurve"},
	{name: "engineCurve", path: "config.power.engineCurve"},
	{name: "looseEngineRpmCurve", path: "config.power.looseEngineRpmCurve"},
	{name: "engineBrakeCurve", path: "config.power.engineBrakeCurve"},
	{name: "clutchCurve", path: "config.power.clutchCurve"},
	{name: "brakeCurve", path: "config.brakeCurve"}
]

class ConfigInput {
	constructor(attrib, configContainer, configObj, updateCallback) {
		this.configObj = configObj
		this.updateCallback = updateCallback
		this.path = attrib.path.split(".")
		this.attribFilter = attrib.filter != null ? attrib.filter : val => true
		
		this.attribInput = configContainer.querySelector(`input#${attrib.name}`)
		this.attribInput.addEventListener("input", this.updateConfig.bind(this))
		
		this.updateInput()
	}
	
	updateConfig() {
		let newVal = Number(this.attribInput.value)
		
		if (this.attribFilter(newVal)) {
			this.configObj.setConfigAttrib(this.path, newVal)
			this.updateCallback()
			this.attribInput.style.background = ""
		}
		else
			this.attribInput.style.background = "#d04f4b"
	}
	
	updateInput() {
		this.attribInput.value = this.configObj.getAttrib(this.path)
	}
}

class ConfigGraph {
	constructor(attrib, vehicle, updateCallback) {
		this.updateCallback = updateCallback
		this.graphApi = new GraphAPI(vehicle.getAttrib(attrib.path.split(".")))
		
		this.graphUI = new EditableGraphUI(this.graphApi, updateCallback)
		let container = document.getElementById(attrib.name)
		container.parentNode.replaceChild(this.graphUI.container, container)
	}
	
	updateGraph() {
		this.graphUI.updateInitData()
	}
	
	resample() {
		this.graphUI.graph.updateRefValues()
	}
}

class ConfigGears {
	constructor(vehicle, updateCallback) {
		this.vehicle = vehicle
		this.updateCallback = updateCallback
		this.gearInputs = new Array()
		this.gearRatiosPath = ["config", "power", "gearRatios"]
		
		this.gearRatiosContainer = document.getElementById("gear-ratios")
		document.getElementById("add-gear-bt").onclick = e => {
			this.#addGear()
			this.#updateGearsConfig()
		}
		document.getElementById("delete-gear-bt").onclick = e => {
			this.#deleteGear()
			this.#updateGearsConfig()
		}
		this.updateGears()
	}
	
	#updateGearsConfig() {
		this.vehicle.setConfigAttrib(this.gearRatiosPath, this.gearInputs.map(input => Number(input.value)))
		this.updateCallback()
	}
	
	updateGears() {
		this.#setGearInputs(this.vehicle.getAttrib(this.gearRatiosPath))
	}
	
	#setGearInputs(gearRatios) {
		while (this.gearInputs.length < gearRatios.length)
			this.#addGear()
		
		while (this.gearInputs.length > gearRatios.length)
			this.#deleteGear()
		
		for (let i = 0; i < this.gearInputs.length; i++)
			this.gearInputs[i].value = gearRatios[i]
	}
	
	#addGear() {
		let gearNum = this.gearInputs.length
		let gearId = `gear-${gearNum}`
		
		let gearContainer = document.createElement("div")
		gearContainer.classList = "input-block gear-input"
		
		let gearLabel = document.createElement("label")
		gearLabel.innerText = gearNum
		gearLabel.htmlFor = gearId
		gearContainer.appendChild(gearLabel)
		
		let gearInput = document.createElement("input")
		gearInput.id = gearId
		gearInput.type = "number"
		gearInput.value = 0
		gearInput.addEventListener("input", this.#updateGearsConfig.bind(this))
		gearContainer.appendChild(gearInput)
		
		this.gearRatiosContainer.appendChild(gearContainer)
		this.gearInputs.push(gearInput)
	}
	
	#deleteGear() {
		if (this.gearInputs.length > 0)
			this.gearRatiosContainer.removeChild(this.gearInputs.splice(this.gearInputs.length - 1, 1)[0].parentNode)
	}
}

class ConfigImportExport {
	constructor(vehicle, updateCallback) {
		this.vehicle = vehicle
		this.updateCallback = updateCallback
		this.configTextarea = document.getElementById("config-textarea")
		
		document.getElementById("import-config-bt").onclick = this.importConfig.bind(this)
		document.getElementById("export-config-bt").onclick = this.exportConfig.bind(this)
	}
	
	importConfig() {
		if (this.vehicle.configManager.loadSerialized(this.configTextarea.value))
			this.updateCallback()
		
		else
			alert("Invalid config\nSet a valid config and re-import");
	}
	
	exportConfig() {
		this.configTextarea.value = this.vehicle.configManager.serialize()
	}
}

export class EditorConfigUI {
	constructor(editor, resampleGraphsCallback) {
		this.editor = editor
		
		let configContainer = document.getElementById("editor-config")
		this.configInputs = new Array()
		
		this.configInputs.push(new ConfigInput(
			{name: "simInterval", path: "simInterval", filter: val => val >= 0.0001},
			configContainer, this.editor.config, this.#updateAndRunSim.bind(this)))
		
		this.configInputs.push(new ConfigInput(
			{name: "simSamples", path: "simSamples", filter: val => val > 0},
			configContainer, this.editor.config, this.#updateAndRunSim.bind(this)))
		
		this.configInputs.push(new ConfigInput(
			{name: "graphSamples", path: "graphSamples", filter: val => val > 0},
			configContainer, this.editor.config, () => {
				resampleGraphsCallback()
				this.editor.runSimulation()
			}))
	}
	
	#updateAndRunSim() {
		this.updateInputs()
		this.editor.runSimulation()
	}
	
	updateInputs() {
		for (let input of this.configInputs)
			input.updateInput()
	}
}

export class VehicleConfigUI {
	constructor(editor) {
		this.updateCallback = editor.runSimulation.bind(editor)
		let configContainer = document.getElementById("vehicle-config")
		
		this.configInputs = new Array()
		for (let configAttrib of VEHICLE_CONFIG_ATTRIBS)
			this.configInputs.push(new ConfigInput(configAttrib, configContainer, editor.vehicle, this.updateCallback))
		
		this.configGraphs = new Array()
		for (let configGraph of VEHICLE_CONFIG_GRAPHS)
			this.configGraphs.push(new ConfigGraph(configGraph, editor.vehicle, this.updateCallback))
		
		this.configGears = new ConfigGears(editor.vehicle, this.updateCallback)
		new ConfigImportExport(editor.vehicle, () => {
			this.updateInputs()
			this.updateCallback()
		})
	}
	
	updateInputs() {
		this.configGears.updateGears()
		
		for (let configInput of this.configInputs)
			configInput.updateInput()
		
		for (let configGraph of this.configGraphs)
			configGraph.updateGraph()
	}
	
	resampleGraphs() {
		for (let configGraph of this.configGraphs)
			configGraph.resample()
	}
}