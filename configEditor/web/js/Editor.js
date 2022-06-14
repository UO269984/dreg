import {TEMPLATES} from "./AppLoadManager.js"
import {VehicleAPI} from "./drivingEngineAPI.js"
import {Graph} from "./graph/Graph.js"
import {BasicGraphUI} from "./graph/GraphUI.js"

const VEHICLE_ATTRIBS = [
	{name: "Throttle", path: "controls.throttle"},
	{name: "Brake", path: "controls.brake"},
	{name: "Steering wheel", path: "controls.steeringWheel"},
	
	{name: "Pos x", path: "state.pos.x"},
	{name: "Pos y", path: "state.pos.y"},
	{name: "Pos z", path: "state.pos.z"},
	{name: "Rot x", path: "state.rotation.x"},
	{name: "Rot y", path: "state.rotation.y"},
	{name: "Rot z", path: "state.rotation.z"},
	
	{name: "Speed", path: "props.speed"},
	{name: "Acceleration", path: "props.acceleration"}
]

class FileLoader {
	constructor(fileInputID, fileLoadedFunc) {
		this.fileLoadedFunc = fileLoadedFunc
		
		this.fileInput = document.getElementById(fileInputID)
		this.fileInput.addEventListener("change", this.loadFile.bind(this))
	}
	
	loadFile() {	
		if (this.fileInput.files.length > 0) {
			let reader = new FileReader()
			reader.readAsText(this.fileInput.files[0], "UTF-8")
			
			reader.onload = e => this.fileLoadedFunc(e.target.result)
			reader.onerror = e => console.warn(`Error reading ${this.fileInput.files[0].name}`)
		}
	}
}

class SimGraph {
	constructor(editor) {
		this.editor = editor
		this.#clearArrays()
		
		this.graphUI = new BasicGraphUI()
		let graph = new Graph(this.graphUI.canvas, {min: 0, max: 1}, {min: 0, max: 1})
		this.graphUI.setGraph(graph)
		
		this.container = document.createElement("div")
		this.container.innerHTML = TEMPLATES.simGraph
		this.container = this.container.children[0]
		this.container.replaceChild(this.graphUI.container, this.container.getElementsByClassName("simple-graph")[0])
		
		this.container.getElementsByClassName("delete-graph-bt")[0].onclick = e => this.editor.deleteSimGraph(this)
		this.#loadAttribSelector()
	}
	
	#loadAttribSelector() {
		this.attribSelector = this.container.getElementsByClassName("attrib-selector")[0]
		this.attribSelector.onchange = e => {
			this.path = this.attribSelector.value.split(".")
			this.editor.runSimulation()
		}
		
		for (let attrib of VEHICLE_ATTRIBS) {
			let option = document.createElement("option")
			option.innerText = attrib.name
			option.value = attrib.path
			
			this.attribSelector.appendChild(option)
		}
		
		this.attribSelector.children[VEHICLE_ATTRIBS.map(attrib => attrib.path).indexOf("props.speed")].selected = true
		this.path = this.attribSelector.value.split(".")
	}
	
	sample(time) {
		this.xValues.push(time)
		this.yValues.push(this.editor.vehicle.getAttrib(this.path))
	}
	
	#clearArrays() {
		this.xValues = new Array()
		this.yValues = new Array()
	}
	
	endSampling() {
		let values = [this.xValues, this.yValues]
		
		this.graphUI.graph.setAxisAutoFor(values)
		this.graphUI.graph.updateValues(values)
		this.#clearArrays()
	}
}

export class Editor {
	constructor() {
		this.simInterval = 0.01
		this.vehicle = new VehicleAPI()
		this.inputLog = null
		new FileLoader("vehicleInputLog-input", this.#loadInputLog.bind(this))
		
		this.simGraphs = new Array()
		this.simGraphsContainer = document.getElementById("simulationGraphs")
		document.getElementById("addGraphBt").onclick = this.addSimGraph.bind(this)
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
	
	#loadInputLog(inputLogCSV) {
		this.inputLog = new Array()
		
		let lines = inputLogCSV.split("\n")
		lines.splice(0, 1) //Delete first line with column titles
		
		for (let line of lines.filter(line => line.length > 0)) {
			let splitted = line.split(";").map(Number)
			
			if (splitted.length == 4 && splitted.indexOf(NaN) < 0) {
				this.inputLog.push({
					time: splitted[0],
					throttle: splitted[1],
					brake: splitted[2],
					steeringWheel: splitted[3]
				})
			}
			else {
				console.warn(`Invalid line in input file: ${line}`)
				this.inputLog = null
				break
			}
		}
		this.runSimulation()
	}
	
	runSimulation() {
		if (this.inputLog == null)
			return
		
		this.vehicle.reset()
		let time = 0
		
		for (let i = 0; i < this.inputLog.length - 1; i++) {
			this.vehicle.setInput(this.inputLog[i])
			
			while (time < this.inputLog[i + 1].time) {
				this.vehicle.update(this.simInterval)
				
				for (let simGraph of this.simGraphs)
					simGraph.sample(time)
				
				time += this.simInterval
			}
		}
		
		for (let simGraph of this.simGraphs)
			simGraph.endSampling()
	}
}