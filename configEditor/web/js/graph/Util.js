
export default {
	max: array => {
		let max = array[0]
		let arrayLength = array.length
		
		for (var i = 1; i < arrayLength; i++) {
			if (array[i] > max)
				max = array[i]
		}
		return max
	},
	min: array => {
		let min = array[0]
		let arrayLength = array.length
		
		for (var i = 1; i < arrayLength; i++) {
			if (array[i] < min)
				min = array[i]
		}
		return min
	}
}