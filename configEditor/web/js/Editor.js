import {Graph} from "./graph/Graph.js"
import {BasicGraphUI} from "./graph/GraphUI.js"

export class Editor {
	constructor() {
		this.simGraphs = new Array()
		this.simGraphsContainer = document.getElementById("simulationGraphs")
		document.getElementById("addGraphBt").onclick = this.addSimGraph.bind(this)
	}
	
	addSimGraph() {
		let graphUI = new BasicGraphUI()
		let graph = new Graph(graphUI.canvas, {min: 0, max: 1}, {min: 0, max: 1})
		graphUI.setGraph(graph)
		
		this.simGraphs.push(graph)
		this.simGraphsContainer.appendChild(graphUI.container)
	}
	
	deleteSimGraph(graphUI) {
		this.simGraphsContainer.removeChild(graphUI.container)
		this.simGraphs.splice(this.simGraphs.indexOf(graphUI.graph), 1)
	}
}