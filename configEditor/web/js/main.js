import {APP_LOAD_MANAGER} from "./AppLoadManager.js"
import {setModuleLoadFunc, GraphAPI} from "./drivingEngineAPI.js"
import {Editor} from "./Editor.js"
import {EditableGraphUI} from "./graph/GraphUI.js"

function main() {
	new Editor()
	
	let graphApi = new GraphAPI()
	let testGraphs = document.getElementById("testGraphs")
	testGraphs.appendChild(new EditableGraphUI(graphApi.linearRefsToGraph.bind(graphApi), graphApi.bezierRefsToGraph.bind(graphApi)).container)
	testGraphs.appendChild(new EditableGraphUI(graphApi.linearRefsToGraph.bind(graphApi), graphApi.bezierRefsToGraph.bind(graphApi)).container)
}

window.onload = APP_LOAD_MANAGER.addLoadTask()
setModuleLoadFunc(APP_LOAD_MANAGER.addLoadTask())

APP_LOAD_MANAGER.setOnLoad(main)