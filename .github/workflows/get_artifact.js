const fs = require("fs");
const os = require("os");

const version = getVersion();
const exeName = getExeName();
const platform = getPlatformName();
const artifactName = `rescript-react-intl-extractor-${version}-${platform}`;

fs.copyFileSync("_build/default/bin/Extract.exe", exeName);

// Pass exeName and artifactName to subsequent GitHub actions
console.log(`::set-output name=exe_name::${exeName}`);
console.log(`::set-output name=artifact_name::${artifactName}`);

function getVersion() {
  const duneProject = fs.readFileSync("dune-project");
  const match = /\(version (.*)\)/.exec(duneProject);

  return match[1];
}

function getPlatformName() {
  const platform = os.platform();

  switch (platform) {
    case "linux":
      return "linux";
    case "darwin":
      return "macos";
    case "win32":
      return "windows";
    default:
      throw new Error(`Unexepected platform ${platform}`);
  }
}

function getExeName() {
  const platform = os.platform();

  switch (platform) {
    case "win32":
      return "rescript-react-intl-extractor.exe";
    default:
      return "rescript-react-intl-extractor";
  }
}
