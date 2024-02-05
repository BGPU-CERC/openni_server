const OPENNI_HOST = "127.0.0.1";
const OPENNI_PORT = 8081;
const SOCKET_PORT = 8080;

let net = require("net");
let client = new net.Socket();
let request = new Uint8Array([0x0]);
client.connect(OPENNI_PORT, OPENNI_HOST, function () {
  console.log("Connected to OpenNI server");
  console.log("Now open client.html");
});

let bytes_to_receive = 0;
let bytes_to_offset = 0;
let buffer = Buffer.from([]);
client.on("data", function (data) {
  switch (bytes_to_receive) {
    case 0:
      bytes_to_receive = data.readInt32LE();
      buffer = Buffer.alloc(bytes_to_receive);
      bytes_to_offset = 4;
      break;

    default:
      bytes_to_offset = 0;
      break;
  }

  data.copy(buffer, buffer.byteLength - bytes_to_receive, bytes_to_offset);
  bytes_to_receive -= data.byteLength - bytes_to_offset;

  switch (bytes_to_receive) {
    case 0:
      wsc?.send(buffer);
      break;
  }
});

client.on("error", console.error);

client.on("close", function () {
  console.log("Connection to OpenNI server closed");
});

let wsc;
let { WebSocketServer } = require("ws");
let wss = new WebSocketServer({ port: SOCKET_PORT });
wss.on("connection", function connection(ws) {
  console.log("Client connected");
  wsc = ws;
  ws.on("error", console.error);
  ws.on("message", function message(data) {
    client.write(request);
  });
});
