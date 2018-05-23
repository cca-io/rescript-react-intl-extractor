const CliTest = require("command-line-test");

describe("test", function() {
  it("shows help", function*() {
    const cliTest = new CliTest();
    const res = yield cliTest.exec("../_build/default/bin/Extract.exe -help");
    expect(res.stdout).toEqual(
      "Usage: ../_build/default/bin/Extract.exe directory ...\n  -v shows the program version\n  -help  Display this list of options\n  --help  Display this list of options"
    );
  });

  it("shows version", function*() {
    const cliTest = new CliTest();
    const res = yield cliTest.exec("../_build/default/bin/Extract.exe -v");
    expect(res.stdout).toEqual("0.4.0");
  });
});
