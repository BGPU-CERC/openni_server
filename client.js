let { WebSocketServer } = require("ws");
let wss = new WebSocketServer({ port: 8080 });
let wsc;
wss.on("connection", function connection(ws) {
  ws.on("error", console.error);

  ws.on("message", function message(data) {
    console.log("received: %s", data);
  });

  wsc = ws;
});

let net = require("net");
let client = new net.Socket();
let request = new Uint8Array([0x0]);
client.connect(8081, "127.0.0.1", function () {
  console.log("Connected");
  client.write(request);
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
      client.write(request);
      break;
  }
});

client.on("error", function (e) {
  console.log(e);
});

client.on("close", function () {
  console.log("Connection closed");
});
