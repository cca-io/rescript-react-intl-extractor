const CliTest = require("command-line-test");

const expectedJson = [
  { id: "test1.msg2.1", defaultMessage: "This is message 2.1" },
  {
    id: "test1.msg2.2",
    defaultMessage: "This is message 2.2",
    description: "Description for message 2.2"
  },
  {
    id: "test2.msg1.1",
    defaultMessage: "This is message 2.1.1"
  }
];

describe("test", function() {
  it("extracts messages", function*() {
    const cliTest = new CliTest();
    const res = yield cliTest.exec(
      "../_build/default/bin/Extract.exe test1/subdir/Test_1_2.re test2"
    );
    expect(JSON.parse(res.stdout)).toEqual(expectedJson);
  });
});
