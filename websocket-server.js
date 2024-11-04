const fs = require('fs');
const https = require('https');
const WebSocket = require('ws');
const dgram = require('dgram');    // Library for UDP transmission
const osc = require('osc');        // Updated OSC library

// Configurações HTTPS
const serverOptions = {
    key: fs.readFileSync('cert/key.pem'),
    cert: fs.readFileSync('cert/cert.pem')
};

// Configure the UDP socket for communication with the ESP8266
const udp = dgram.createSocket('udp4');

// Set the IP and port of the ESP8266
const ESP_HOST = '192.168.1.201';  // Fixed IP from ESP8266
const ESP_PORT = 54321;            // Port configured to receive OSC


// Servidor HTTPS para o WebSocket
const server = https.createServer(serverOptions);
const wss = new WebSocket.Server({ server });

wss.on('connection', (ws) => {
    console.log('Client connected via WebSocket');

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

    ws.on('close', () => {
        console.log('Client disconnected');
    });
});

// Iniciar o servidor WebSocket na porta 1234
server.listen(1234, () => {
    console.log('WebSocket server listening on wss://localhost:1234');
});
