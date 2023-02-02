import {TEMPLATES} from "./Templates.js"
import {VehicleAPI, GraphAPI} from "./DregApi.js"
import {MobileSplitter} from "./MobileSplitter.js"
import {BasicGraphUI} from "./graph/GraphUI.js"
import {EditorConfigUI, VehicleConfigUI} from "./ConfigUI.js"

const VEHICLE_ATTRIBS = [
	{name: "Throttle", path: "controls.throttle"},
	{name: "Brake", path: "controls.brake"},
	{name: "Steering wheel", path: "controls.steeringWheel"},
	{name: "Clutch", path: "controls.clutch"},
	{name: "Gear", path: "controls.gear"},
	
	{name: "Pos x", path: "state.pos.x"},
	{name: "Pos y", path: "state.pos.y"},
	{name: "Pos z", path: "state.pos.z"},
	{name: "Rot x", path: "state.rotation.x"},
	{name: "Rot y", path: "state.rotation.y"},
	{name: "Rot z", path: "state.rotation.z"},
	
	{name: "Speed", path: "props.speed"},
	{name: "Acceleration", path: "props.acceleration"},
	
	{name: "Wheel rpm", path: "props.wheelRpm"},
	{name: "Engine rpm", path: "props.engineRpm"},
	{name: "Engine torque", path: "props.engineTorque"},
	{name: "Clutch torque", path: "props.clutchTorque"},
	
	{name: "Power torque", path: "props.powerTorque"},
	{name: "Brake torque", path: "props.brakeTorque"},
	{name: "Wheel torque", path: "props.wheelTorque"}
]

const COMPUTED_VEHICLE_ATTRIBS = {
	originDist: {
		name: "Origin dist",
		func: vehicle => {
			let pos = vehicle.getAttrib(["state", "pos"])
			return (pos.x**2 + pos.y**2 + pos.z**2)**0.5
		}
	}
}

class FileLoader {
	constructor(fileInput, fileLoadedFunc) {
		this.fileLoadedFunc = fileLoadedFunc
		
		this.fileInput = fileInput
		this.fileInput.addEventListener("change", this.loadFile.bind(this))
	}
	
	loadFile() {	
		if (this.fileInput.files.length > 0) {
			let reader = new FileReader()
			reader.readAsText(this.fileInput.files[0], "UTF-8")
			
			reader.onload = e => {
				this.fileLoadedFunc(this.fileInput.files[0].name, e.target.result)
				this.fileInput.value = ""
			}
			reader.onerror = e => console.warn(`Error reading ${this.fileInput.files[0].name}`)
		}
	}
}

class SimGraph {
	constructor(editor) {
		this.editor = editor
		this.#clearArrays()
		
		this.graphUI = new BasicGraphUI()
		
		this.container = document.createElement("div")
		this.container.innerHTML = TEMPLATES.simGraph
		this.container = this.container.children[0]
		this.container.replaceChild(this.graphUI.container, this.container.getElementsByClassName("simple-graph")[0])
		
		this.container.getElementsByClassName("delete-graph-bt")[0].onclick = e => this.editor.deleteSimGraph(this)
		this.#loadAttribSelector()
	}
	
	#loadAttribSelector() {
		this.attribChanged = false
		
		this.attribSelector = this.container.getElementsByClassName("attrib-selector")[0]
		this.attribSelector.onchange = e => {
			this.path = this.attribSelector.value.split(".")
			
			this.attribChanged = true
			this.editor.runSimulation()
			this.attribChanged = false
		}
		
		for (let attrib of VEHICLE_ATTRIBS) {
			let option = document.createElement("option")
			option.innerText = attrib.name
			option.value = attrib.path
			
			this.attribSelector.appendChild(option)
		}
		
		for (let id in COMPUTED_VEHICLE_ATTRIBS) {
			let option = document.createElement("option")
			option.innerText = COMPUTED_VEHICLE_ATTRIBS[id].name
			option.value = "COMPUTED." + id
			
			this.attribSelector.appendChild(option)
		}
		
		this.attribSelector.children[VEHICLE_ATTRIBS.map(attrib => attrib.path).indexOf("props.speed")].selected = true
		this.path = this.attribSelector.value.split(".")
	}
	
	sample(time) {
		this.xValues.push(time)
		this.yValues.push(this.path[0] == "COMPUTED" ?
			COMPUTED_VEHICLE_ATTRIBS[this.path[1]].func(this.editor.vehicle) :
			this.editor.vehicle.getAttrib(this.path))
	}
	
	#clearArrays() {
		this.xValues = new Array()
		this.yValues = new Array()
	}
	
	endSampling() {
		let values = [this.xValues, this.yValues]
		
		this.graphUI.graph.setAxisAutoFor(values, this.attribChanged)
		this.graphUI.graph.updateValues(values)
		this.#clearArrays()
	}
}

class EditorConfig {
	constructor() {
		this.simInterval = 0.01
		this._graphSamples = 20
		
		this.inputLog = null
		this.inputLogTime = 1
	}
	
	get simSamples() {
		return Math.round(this.inputLogTime / this.simInterval)
	}
	
	set simSamples(samplesCount) {
		this.simInterval = this.inputLogTime / samplesCount
	}
	
	get graphSamples() {
		return this._graphSamples
	}
	
	set graphSamples(samplesCount) {
		GraphAPI.setDefaultBezierSamples(samplesCount)
		this._graphSamples = samplesCount
	}
	
	getAttrib(path) {
		return this[path[0]]
	}
	
	setConfigAttrib(path, value) {
		this[path[0]] = value
	}
}

export class Editor {
	constructor() {
		GraphAPI.setSaveInitData(true)
		new MobileSplitter(document.getElementById("splitter"),
			document.querySelector("main"), document.getElementById("simulation"))
		
		this.config = new EditorConfig()
		this.vehicle = new VehicleAPI()
		
		let fileInput = document.getElementById("vehicleInputLog-input")
		new FileLoader(fileInput, this.#loadInputLog.bind(this))
		document.getElementById("vehicleInputLog-bt").onclick = e => fileInput.click()
		
		this.simGraphs = new Array()
		this.simGraphsContainer = document.getElementById("simulation-graphs")
		document.getElementById("add-graph-bt").onclick = this.addSimGraph.bind(this)
		
		this.vehicleConfigUI = new VehicleConfigUI(this)
		this.editorConfigUI = new EditorConfigUI(this, this.vehicleConfigUI.resampleGraphs.bind(this.vehicleConfigUI))
		this.#configureSliders()
	}
	
	#configureSliders() {
		for (let slider of document.querySelectorAll("input[type=range]")) {
			let valueTx = slider.nextElementSibling
			slider.value = valueTx.value
			
			valueTx.addEventListener("input", () => slider.value = valueTx.value)
			slider.addEventListener("input", () => {
				valueTx.value = slider.value
				valueTx.dispatchEvent(new Event("input", {bubbles: true, cancelable: true}))
			})
		}
	}
	
	addSimGraph() {
		let simGraph = new SimGraph(this)
		this.simGraphs.push(simGraph)
		this.simGraphsContainer.appendChild(simGraph.container)
		
		this.runSimulation()
	}
	
	deleteSimGraph(simGraph) {
		this.simGraphsContainer.removeChild(simGraph.container)
		this.simGraphs.splice(this.simGraphs.indexOf(simGraph), 1)
	}
	
	#loadInputLog(filename, inputLogCSV) {
		let inputLog = new Array()
		
		let lines = inputLogCSV.split("\n")
		lines.splice(0, 1) //Delete first line with column titles
		
		for (let line of lines.filter(line => line.length > 0)) {
			let splitted = line.split(";").map(Number)
			
			if (splitted.length == 6 && splitted.indexOf(NaN) < 0) {
				inputLog.push({
					time: splitted[0],
					throttle: splitted[1],
					brake: splitted[2],
					steeringWheel: splitted[3],
					clutch: splitted[4],
					gear: splitted[5]
				})
			}
			else {
				console.warn(`Invalid line in input file: ${line}`)
				inputLog = null
				break
			}
		}
		
		if (inputLog != null) {
			this.config.inputLog = inputLog
			this.config.inputLogTime = inputLog[inputLog.length - 1].time - inputLog[0].time
			
			document.getElementById("loaded-log-filename").innerText = filename
			this.editorConfigUI.updateInputs()
			this.runSimulation()
		}
	}
	
	runSimulation() {
		if (this.config.inputLog == null)
			return
		
		this.vehicle.reset()
		let time = 0
		
		for (let simGraph of this.simGraphs)
			simGraph.sample(time)
		
		for (let i = 0; i < this.config.inputLog.length - 1; i++) {
			this.vehicle.setInput(this.config.inputLog[i])
			
			while (time < this.config.inputLog[i + 1].time) {
				this.vehicle.update(this.config.simInterval)
				time += this.config.simInterval
				
				for (let simGraph of this.simGraphs)
					simGraph.sample(time)
			}
		}
		
		for (let simGraph of this.simGraphs)
			simGraph.endSampling()
	}
}