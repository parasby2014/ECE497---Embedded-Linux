 var socket;
    var firstconnect = true,
        i2cNum = "0x70",
        disp = [];

// Create a matrix of LEDs inside the <table> tags.
var matrixData;
for(var j=7; j>=0; j--) {
        matrixData += '<tr>';
        for(var i=0; i<8; i++) {
         matrixData += '<td><div class="LED" id="id'+i+'_'+j+
                '" onclick="LEDclick('+i+','+j+')">'+
                i+','+j+'</div></td>';
         }
        matrixData += '</tr>';
}
$('#matrixLED').append(matrixData);

// Send one column when LED is clicked.
function LEDclick(i, j) 
{
    //alert(i+","+j+" clicked");
  	// multiply i by 2 to get all the 16 bits
	i = (i<<1);
	
	//check current color and run the pattern green, both, red, off

	//if green, toggle red on to get both
	if ((((disp[i] >> j) & 0x1) === 1)&&(((disp[i+1] >> j) & 0x1) === 0)) 
	{
        	disp[i+1] ^= 0x1<<j;
    	} // if both, toggle green off to get red    
	else if ((((disp[i] >> j) & 0x1) === 1)&&(((disp[i+1] >> j) & 0x1) === 1))
	{
		disp[i] ^= 0x1<<j;
	} // if red, then toggle red off to keep all off 
	else if ((((disp[i] >> j) & 0x1) === 0)&&(((disp[i+1] >> j) & 0x1) === 1))
	{
		disp[i+1] ^= 0x1<<j;
	} // otherwise, toggle green on
	else
	{
		disp[i] ^= 0x1<<j;
	}

	// send to bone
	socket.emit('i2cset', {i2cNum: i2cNum, i: i,
                         disp: '0x'+disp[i].toString(16)});

        socket.emit('i2cset', {i2cNum: i2cNum, i: i+1,
                         disp: '0x'+disp[i+1].toString(16)});
   	
	
	// Toggle bit on display, divide i by 2 to undo the multiplcation
	// if green, set browser to green
	if ((((disp[i] >> j) & 0x1) === 1)&&(((disp[i+1] >> j) & 0x1) === 0)) 
	{
        	$('#id' + (i>>1) + '_' + j).addClass('onG');
    	} // if both, set browser to both
	else if ((((disp[i] >> j) & 0x1) === 1)&&(((disp[i+1] >> j) & 0x1) === 1))
	{
		$('#id' + (i>>1)  + '_' + j).removeClass('onG').addClass('onGR');
	} //if both set browser to red
	else if ((((disp[i] >> j) & 0x1) === 0)&&(((disp[i+1] >> j) & 0x1) === 1))
	{
		$('#id' + (i>>1)  + '_' + j).removeClass('onGR').addClass('onR');
	} // othwersie turn it off
	else
	{
		$('#id' + (i>>1)  + '_' + j).removeClass('onR');
	}       

}

    function connect() {
      if(firstconnect) {
        socket = io.connect(null);

        // See https://github.com/LearnBoost/socket.io/wiki/Exposed-events
        // for Exposed events
        socket.on('message', function(data)
            { status_update("Received: message " + data);});
        socket.on('connect', function()
            { status_update("Connected to Server"); });
        socket.on('disconnect', function()
            { status_update("Disconnected from Server"); });
        socket.on('reconnect', function()
            { status_update("Reconnected to Server"); });
        socket.on('reconnecting', function( nextRetry )
            { status_update("Reconnecting in " + nextRetry/1000 + " s"); });
        socket.on('reconnect_failed', function()
            { message("Reconnect Failed"); });

        socket.on('matrix', matrix);
        // Read display for initial image. Store in disp[]
        socket.emit("matrix", i2cNum);

        firstconnect = false;
      }
      else {
        socket.socket.reconnect();
      }
    }

    function disconnect() {
      socket.disconnect();
    }

    // When new data arrives, convert it and display it.
    // data is a string of 16 values, each a pair of hex digits.
    function matrix(data) {
        var i, j;
        disp = [];
        // status_update("i2c: " + data);
        // Make data an array, each entry is a pair of digits
        data = data.split(" ");
        status_update("data: " + data);
        // Every other pair of digits are Green. The others are red.
        // Ignore the red.
        // Convert from hex.
        for (i = 0; i < data.length; i += 1) {
            disp[i] = parseInt(data[i], 16);
        }
        // status_update("disp: " + disp);
        // i cycles through each column
        for (i = 0; i < (disp.length); i+=2) 
	{
            // j cycles through each bit
		for (j = 0; j < 8; j++) 
		{
		//clear the colors first
		$('#id' + i + '_' + j).removeClass('onG onGR onR');
		
		//check if green LED is on or if both are on or red LED is on and set it on the browser
                if ((((disp[i] >> j) & 0x1) === 1)&&(((disp[i+1] >> j) & 0x1) === 0)) 
		{
                	$('#id' + (i>>1) + '_' + j).addClass('onG');
                } 
		else if ((((disp[i] >> j) & 0x1) === 1)&&(((disp[i+1] >> j) & 0x1) === 1))
		{
			$('#id' + (i>>1) + '_' + j).addClass('onGR');
		}
		else if ((((disp[i] >> j) & 0x1) === 0)&&(((disp[i+1] >> j) & 0x1) === 1))
		{
                    	$('#id' + (i>>1) + '_' + j).addClass('onR');
                }
            }
        }
    }

    function status_update(txt){
        $('#status').html(txt);
    }

    function updateFromLED(){
      socket.emit("matrix", i2cNum);
    }

connect();

$(function () {
    // setup control widget
    $("#i2cNum").val(i2cNum).change(function () {
        i2cNum = $(this).val();
    });
});
