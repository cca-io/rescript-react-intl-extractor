let checkRes = (params, expected) => {
  let res = CmdLine.run(params);
  Alcotest.check(CmdLine.res, "res", expected, res);
};

module Usage = {
  let usage =
    "Usage: "
    ++ CmdLine.pathToExtractExe
    ++ " [path...]
  -v shows the program version
  --allow-duplicates allows messages with identical `id` props if `defaultMessage` props are identical as well
  -help  Display this list of options
  --help  Display this list of options
";

  let testNoArgs = () => checkRes("", ("", usage));
  let testHelp = () => checkRes("-help", (usage, ""));

  let testSet = [("No args", `Quick, testNoArgs), ("Help", `Quick, testHelp)];
};

module Version = {
  let version = "0.6.0";

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

  let dup = {|[
  { "id": "test3.msg1.1", "defaultMessage": "This is message 1.1" },
  { "id": "test3.msg1.2", "defaultMessage": "This is message 1.2" },
  { "id": "test3.msg1.3", "defaultMessage": "This is message 1.3" }
]
|};

  let testExtractDup = () => checkRes("--allow-duplicates test/test3/Test_1_1.re test/test3/Test_1_2.re", (dup, ""));

  let dupWithoutFlagError = "Error: duplicate message id: test3.msg1.1\n";

  let testExtractDupWithoutFlagError = () => checkRes("test/test3/Test_1_1.re test/test3/Test_1_2.re", ("", dupWithoutFlagError));

  let dupWithDifferentDefaultMessageError = "Error: duplicate message id: test3.msg1.1 with different default messages\n";

  let testExtractDupWithDifferentDefaultMessageError = () =>
    checkRes(
      "--allow-duplicates test/test3/Test_1_1.re test/test3/Test_1_2.re test/test3/Test_1_3.re",
      ("", dupWithDifferentDefaultMessageError),
    );

  let testSet = [
    ("Extract full", `Quick, testExtractFull),
    ("Extract partial", `Quick, testExtractPartial),
    ("Extract dup", `Quick, testExtractDup),
    ("Extract dup without flag error", `Quick, testExtractDupWithoutFlagError),
    ("Extract dup with different defaultMessage error", `Quick, testExtractDupWithDifferentDefaultMessageError),
  ];
};

let () =
  Alcotest.run(
    "bs-react-intl-extractor",
    [("usage", Usage.testSet), ("version", Version.testSet), ("extract", Extract.testSet)],
  );
