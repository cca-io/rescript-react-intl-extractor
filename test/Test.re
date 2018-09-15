let checkRes = (params, expected) => {
  let res = CmdLine.run(params);
  Alcotest.check(CmdLine.res, "res", expected, res);
};

module Usage = {
  let usage = "Usage: _build/default/bin/Extract.exe [path...]
  -v shows the program version
  -help  Display this list of options
  --help  Display this list of options
";

  let testNoArgs = () => checkRes("", ("", usage));
  let testHelp = () => checkRes("-help", (usage, ""));

  let testSet = [("No args", `Quick, testNoArgs), ("Help", `Quick, testHelp)];
};

module Version = {
  let version = "0.5.0";

  let testVersion = () => checkRes("-v", (version ++ "\n", ""));

  let testSet = [("Version", `Quick, testVersion)];
};

module Extract = {
  let full = {|[
  { "id": "test1.msg1.1", "defaultMessage": "This is message 1.1" },
  { "id": "test1.msg1.2", "defaultMessage": "This is message 1.2" },
  {
    "id": "test1.msg1.3",
    "defaultMessage": "This is message 1.3",
    "description": "Description for message 1.3"
  },
  { "id": "test1.msg1.4", "defaultMessage": "This is message 1.4" },
  { "id": "test1.msg1.5", "defaultMessage": "This is message 1.5" },
  { "id": "test1.msg1.6", "defaultMessage": "This is message 1.6" },
  {
    "id": "test1.msg1.7",
    "defaultMessage": "This is message 1.7",
    "description": "Description for message 1.7"
  },
  { "id": "test1.msg2.1", "defaultMessage": "This is message 2.1" },
  {
    "id": "test1.msg2.2",
    "defaultMessage": "This is message 2.2",
    "description": "Description for message 2.2"
  },
  { "id": "test2.msg1.1", "defaultMessage": "This is message 2.1.1" }
]
|};

  let testExtractFull = () => checkRes("test/test1 test/test2", (full, ""));

  let partial = {|[
  { "id": "test1.msg2.1", "defaultMessage": "This is message 2.1" },
  {
    "id": "test1.msg2.2",
    "defaultMessage": "This is message 2.2",
    "description": "Description for message 2.2"
  },
  { "id": "test2.msg1.1", "defaultMessage": "This is message 2.1.1" }
]
|};

  let testExtractPartial = () => checkRes("test/test1/subdir/Test_1_2.re test/test2", (partial, ""));

  let testSet = [("Extract full", `Quick, testExtractFull), ("Extract partial", `Quick, testExtractPartial)];
};

let () =
  Alcotest.run(
    "bs-react-intl-extractor",
    [("usage", Usage.testSet), ("version", Version.testSet), ("extract", Extract.testSet)],
  );
