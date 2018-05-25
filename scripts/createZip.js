const fs = require("fs");
const os = require("os");
const child_process = require("child_process");
const { version } = require("../package.json");

const exeName = "bs-react-intl-extractor";
let platform = os.platform();
if (platform === "darwin") {
  platform = "macos";
}
const zipName = `${exeName}-${version}-${platform}.zip`;

if (fs.existsSync(zipName)) {
  fs.unlinkSync(zipName);
}
fs.copyFileSync("_build/default/bin/Extract.exe", exeName);
child_process.execSync(`zip ${zipName} ${exeName}`);
fs.unlinkSync(exeName);
