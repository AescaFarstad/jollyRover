var Module = {};
console.log("Canvas script");
console.log(Module);

Module.canvas = (function() { 
	console.log("Canvas requested");
	var c = document.getElementById('canvas'); 
	if (!c) 
	throw "No canvas!"; 
	else 
		{console.log("Canvas:"); console.log(c); } 
	return c;
})();

var memoryFile = 'web.js.mem';
Module.memoryInitializerRequest = new XMLHttpRequest();
Module.memoryInitializerRequestURL = memoryFile;
Module.memoryInitializerRequest.open('GET', Module.memoryInitializerRequestURL, true);
Module.memoryInitializerRequest.responseType = 'arraybuffer';
Module.memoryInitializerRequest.send(null);
console.log("memoryFile request send");

var script = document.createElement('script');
script.src = "web.js";
console.log("attach web.js script");
document.body.appendChild(script);
console.log("attach web.js script end");
/**/

 Module.preRun = [];
 Module.postRun = [];
 
 Module.print = (function()  
 {
	var element = document.getElementById('output');
	if (element) 
		element.value = ''; // clear browser cache
	return function(text) {
		if (arguments.length > 1) 
			text = Array.prototype.slice.call(arguments).join(' ');
		console.log(text);
		if (element) 
		{
			element.value += text + "\n";
			element.scrollTop = element.scrollHeight; // focus on bottom
		}
	};
})();

Module.printErr = function(text) 
{
	if (arguments.length > 1) 
	text = Array.prototype.slice.call(arguments).join(' ');
	console.error(text);
};