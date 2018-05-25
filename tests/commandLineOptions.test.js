const CliTest = require("command-line-test");

let usage =
  "Usage: ../_build/default/bin/Extract.exe [path...]\n  -v shows the program version\n  -help  Display this list of options\n  --help  Display this list of options";

describe("test", function() {
  it("shows usage when run without params", function*() {
    const cliTest = new CliTest();
    const res = yield cliTest.exec("../_build/default/bin/Extract.exe");
    expect(res.stderr).toEqual(usage);
  });

  it("shows help", function*() {
    const cliTest = new CliTest();
    const res = yield cliTest.exec("../_build/default/bin/Extract.exe -help");
    expect(res.stdout).toEqual(usage);
  });

  it("shows version", function*() {
    const cliTest = new CliTest();
    const res = yield cliTest.exec("../_build/default/bin/Extract.exe -v");
    expect(res.stdout).toEqual("0.4.0");
  });
});
