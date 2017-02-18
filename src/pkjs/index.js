// Function to send a message to the Pebble using AppMessage API
// We are currently only sending a message using the "status" appKey defined in appinfo.json/Settings
function sendStatus(val) {
	Pebble.sendAppMessage({"status": val}, messageSuccessHandler, messageFailureHandler);
}

function sendMenuData(val) {
	Pebble.sendAppMessage({"menudata": val}, messageSuccessHandler, messageFailureHandler);
}


// Called when the message send attempt succeeds
function messageSuccessHandler() {
  console.log("Message send succeeded.");  
}

// Called when the message send attempt fails
function messageFailureHandler() {
  console.log("Message send failed.");
  sendStatus(1);
}

// Called when JS is ready
Pebble.addEventListener("ready", function(e) {
  console.log("JS is ready!");
  sendStatus(1);
});
												
// Called when incoming message from the Pebble is received
// We are currently only checking the "message" appKey defined in appinfo.json/Settings
Pebble.addEventListener("appmessage", function(e) {
	var dict = e.payload;
	console.log("Message heard from phone... analyzing dictionary\n" + "status: " + dict.status + "\nrequestmenu: " + dict.requestmenu + "\nmenudata: " + dict.menudata + "\nmenusize: "+ dict.menusize );
	if(dict['requestmenu']){
		console.log("Received Message: " + dict.requestmenu);
		sendMenuData("DaisyHillGrill,Food1,Food2,Food3,!BBQ Land,Food1,Food2,!");
	}
  
	if(dict['menudata']||dict['menusize']||dict['status']){
		console.log("Watch sent an invlaid  KEY ");
	}
});