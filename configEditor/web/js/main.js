import {APP_LOAD_MANAGER} from "./AppLoadManager.js"
import {setModuleLoadFunc} from "./DregApi.js"
import {Editor} from "./Editor.js"

function main() {
	new Editor()
}

window.onload = APP_LOAD_MANAGER.addLoadTask()
setModuleLoadFunc(APP_LOAD_MANAGER.addLoadTask())

APP_LOAD_MANAGER.setOnLoad(main)