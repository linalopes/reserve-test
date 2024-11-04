const WebSocket = require('ws');   // WebSocket library
const dgram = require('dgram');    // Library for UDP transmission
const osc = require('osc');        // Updated OSC library

// Configure the WebSocket server on port 1234
// const wss = new WebSocket.Server({ port: 1234 });
const wss = new WebSocket.Server({ host: '0.0.0.0', port: 1234 });


// Configure the UDP socket for communication with the ESP8266
const udp = dgram.createSocket('udp4');

// Set the IP and port of the ESP8266
const ESP_HOST = '192.168.1.201';  // Fixed IP from ESP8266
const ESP_PORT = 54321;            // Port configured to receive OSC

// When a client connects to the WebSocket
wss.on('connection', (ws) => {
    console.log('WebSocket client connected');

    // When the server receives a message from the WebSocket client
    ws.on('message', (message) => {
        const position = parseFloat(message); // Ensure the value is a float number

        // Create the OSC message with the value directly
        const oscMsg = {
            address: '/servo',    // OSC address configured on the ESP
            args: [position]      // Send the value as an argument
        };

        // Convert the message to OSC buffer format
        const oscBuffer = osc.writePacket(oscMsg);

        // Send the OSC message to the ESP8266
        udp.send(oscBuffer, ESP_PORT, ESP_HOST, (err) => {
            if (err) console.error('UDP Error:', err);
            else console.log(`Sent position ${position} to ESP8266`);
        });
    });

    ws.on('close', () => console.log('WebSocket client disconnected'));
});

