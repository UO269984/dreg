import {APP_LOAD_MANAGER} from "./AppLoadManager.js"
import initModule from "./dreg/testPerformance.js"

const Module = initModule()

class CustomConsole {
	constructor(container) {
		this.container = container
	}
	log(msg) {
		console.log(msg)
		this.container.value += msg
		this.container.scrollTo(0, this.container.scrollHeight)
	}
}
var customConsole = null

class Input {
	constructor(inputName) {
		this.name = inputName
		this.input = document.querySelector(`[name=${inputName}]`)
	}
	getValue() {
		return Math.floor(Number(this.input.value))
	}
	setValue(newValue) {
		this.input.value = newValue
	}
}
class Textarea {
	constructor(textareaId) {
		this.name = textareaId
		this.texarea = document.getElementById(textareaId)
	}
	getValue() {
		return this.texarea.value
	}
	setValue(newValue) {
		this.texarea.value = newValue
	}
}
class TestsConfigUI {
	constructor() {
		this.inputs = [
			new Input("bezierSamples"),
			new Input("singleVehicleIters"),
			new Input("multipleVehiclesIters"),
			new Input("multipleVehiclesCount"),
			new Input("createVehiclesCount"),
			new Input("loadConfigIters"),
			new Textarea("vehicleConfigVehicle"),
			new Textarea("vehicleConfigLoadConfig")
		]
	}
	getConfig() {
		let obj = {}
		for (let input of this.inputs)
			obj[input.name] = input.getValue()
		
		return obj
	}
	setConfig(newConfig) {
		for (let input of this.inputs)
			input.setValue(newConfig[input.name])
	}
}
var testsConfigUI = null

function runTest() {
	let testsConfig = Module.createConfig()
	Module.updateConfigStruct(testsConfig, testsConfigUI.getConfig())
	if (! Module.runTest(testsConfig))
		alert("Invalid config")
	
	Module.deleteConfig(testsConfig)
}

function getDefaultConfig() {
	let obj = {}
	let testsConfig = Module.createConfig()
	
	Module.loadDefaultConfig(testsConfig)
	Module.updateConfigObj(testsConfig, obj)
	Module.deleteConfig(testsConfig)
	
	return obj
}

function main() {
	customConsole = new CustomConsole(document.getElementById("consoleContainer"))
	Module.setTestPrintFunc({func: customConsole.log.bind(customConsole)})
	
	testsConfigUI = new TestsConfigUI()
	testsConfigUI.setConfig(getDefaultConfig())
	
	let runBt = document.getElementById("runTestBt")
	runBt.onclick = () => {
		customConsole.log("Running test...\n")
		setTimeout(runTest, 0)
	}
	runBt.style.display = "block"
}

window.onload = APP_LOAD_MANAGER.addLoadTask()
Module.onRuntimeInitialized = APP_LOAD_MANAGER.addLoadTask()
APP_LOAD_MANAGER.setOnLoad(main)