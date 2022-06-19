const fs = require("fs");
const child_process = require("child_process");

const packageJson = fs.readFileSync("package.json");
const packageVersion = JSON.parse(packageJson).version;

const reScriptVersion = child_process
  .execSync("git submodule")
  .toString()
  .trim()
  .split(" ")[0]
  .substr(0, 7);

const version = `${packageVersion} (ReScript ${reScriptVersion})`;

fs.writeFileSync(
  "bin/Version.ml",
  `(*** GENERATED BY "yarn setVersion" ***)

let version = "${version}"
`
);

console.log(`Version set to ${version}`);
