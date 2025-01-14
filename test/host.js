const { readFileSync } = require("fs");
const http = require("http");
const path = require("path");

const PORT = process.env.PORT || 8080;
const request = new Uint8Array([0x0]);

function createServer(params) {
  const server = http.createServer();

  server.on("request", async (request, response) => {
    try {
      const fileName =
        request.url === "/" ? "index.html" : request.url.replace("/", "");
      const file = readFileSync(path.join(__dirname, fileName));
      response.end(file, "utf-8");
    } catch (error) {
      const code = error.code === "ENOENT" ? 404 : 500;
      response.writeHead(code);
      response.end();
    }
  });

  server.listen(PORT, "0.0.0.0", () => {
    console.log(`starting server at port ${PORT}`);
    openBrowser(`http://localhost:${PORT}`);
  });

  let returns = {};
  let { WebSocketServer } = require("ws");
  let wss = new WebSocketServer({ server });
  wss.on("connection", function connection(ws) {
    console.log("Client connected");
    returns.wsc = ws;
    ws.on("error", console.error);
    ws.on("message", function message(data) {
      params.openni_client.write(request);
    });
  });

  return returns;
}

function openBrowser(url) {
  const start = process.platform == "win32" ? "start" : "xdg-open";
  require("child_process").exec(start + " " + url);
}

module.exports = {
  createServer,
};
