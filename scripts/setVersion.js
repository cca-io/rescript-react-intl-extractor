const fs = require("fs");
const { version } = require("../package.json");

fs.writeFileSync("bin/Version.re", `let version = "${version}";`);

console.log(`Version set to ${version}`);
