
const VEHICLE_CONFIG_ATTRIBS = [
	{name: "frontShaft-x", path: "config.frontShaft.x"},
	{name: "frontShaft-y", path: "config.frontShaft.y"},
	{name: "frontShaft-z", path: "config.frontShaft.z"},
	{name: "rearShaft-x", path: "config.rearShaft.x"},
	{name: "rearShaft-y", path: "config.rearShaft.y"},
	{name: "rearShaft-z", path: "config.rearShaft.z"},
	{name: "maxSteeringAngle", path: "config.maxSteeringAngle"}
]

class ConfigInput {
	constructor(attrib, configContainer, configObj, updateCallback) {
		this.configObj = configObj
		this.updateCallback = updateCallback
		this.path = attrib.path.split(".")
		this.attribFilter = attrib.filter != null ? attrib.filter : val => true
		
		this.attribInput = configContainer.querySelector(`input[name=${attrib.name}]`)
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

export class EditorConfigUI {
	constructor(editor) {
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
			configContainer, this.editor.config, () => {}))
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
		let configContainer = document.getElementById("vehicle-config")
		
		for (let configAttrib of VEHICLE_CONFIG_ATTRIBS) {
			new ConfigInput(configAttrib, configContainer, editor.vehicle,
				editor.runSimulation.bind(editor))
		}
	}
}