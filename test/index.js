const OPENNI_HOST = "127.0.0.1";
const OPENNI_PORT = 8081;

let net = require("net");
const { createServer } = require("./host");

let params = {
  openni_client: undefined,
};

let server = {
  wsc: undefined,
};

let client_create = () => {
  params.openni_client = new net.Socket();
  const client = params.openni_client;
  client.connect(OPENNI_PORT, OPENNI_HOST, function () {
    console.log("Connected to OpenNI server");
  });
  client.on("error", console.error);
  client.on("close", async function () {
    console.log("Connection to OpenNI server closed");
    await new Promise((res) => setTimeout(res, 1000));
    client_create();
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
        server.wsc?.send(buffer);
        break;
    }
  });
};

client_create();
server = createServer(params);
