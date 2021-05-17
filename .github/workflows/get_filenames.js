const fs = require("fs");
const os = require("os");

const packageJson = fs.readFileSync("package.json");
const { version } = JSON.parse(packageJson);

const exeName = getExeName();
const platform = getPlatformName();
const artifactName = `rescript-react-intl-extractor-${version}-${platform}`;

// For passing output to subsequent GitHub actions
console.log(`::set-output name=exe_name::${exeName}`);
console.log(`::set-output name=artifact_name::${artifactName}`);

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
