// npm install express
// const NTPServer = require('ntp-time').Server;
// const server = new NTPServer();

// // Define your custom handler for requests
// server.handle((message, response) => {
//   console.log('Server message:', message);
// 	message.transmitTimestamp = Math.floor(Date.now() / 1000);
// 	response(message);
// });

// // Check if node has the necessary permissions
// // to listen on ports less than 1024
// // https://stackoverflow.com/questions/413807/is-there-a-way-for-non-root-processes-to-bind-to-privileged-ports-on-linux
// server.listen(123, err => {
// 	if (err) throw err;
// 	console.log('NTPServer listening on port 123');
// });

const express = require('express')
const app = express()
const port = 3000
var bodyParser = require('body-parser')

app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: false
}));
app.use( bodyParser.json() );       // to support JSON-encoded bodies

app.get('/', (req, res) => {
  res.send('Hello World!')
});

app.post("/", function(req, res) {
    // res.send(JSON.stringify({"response": "ok"}));
    res.sendStatus(200);
    console.log('Received JSON:', req.body);
});

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
});