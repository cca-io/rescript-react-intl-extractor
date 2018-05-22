const CliTest = require("command-line-test");

const expectedJson = [
  { id: "test1.msg1.1", defaultMessage: "This is message 1.1" },
  { id: "test1.msg1.2", defaultMessage: "This is message 1.2" },
  {
    id: "test1.msg1.3",
    defaultMessage: "This is message 1.3",
    description: "Description for message 1.3"
  },
  { id: "test1.msg1.4", defaultMessage: "This is message 1.4" },
  { id: "test1.msg1.5", defaultMessage: "This is message 1.5" },
  { id: "test1.msg1.6", defaultMessage: "This is message 1.6" },
  {
    id: "test1.msg1.7",
    defaultMessage: "This is message 1.7",
    description: "Description for message 1.7"
  },
  { id: "test1.msg2.1", defaultMessage: "This is message 2.1" },
  {
    id: "test1.msg2.2",
    defaultMessage: "This is message 2.2",
    description: "Description for message 2.2"
  }
];

describe("test", function() {
  it("extracts messages", function*() {
    const cliTest = new CliTest();
    const res = yield cliTest.exec("../_build/default/bin/Extract.exe test1");
    expect(JSON.parse(res.stdout)).toEqual(expectedJson);
  });
});
