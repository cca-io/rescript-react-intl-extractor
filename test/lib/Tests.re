open TestFramework;

describe("Version and Usage", ({test}) => {
  let version = "0.9.0";

  let usage =
    "Usage: "
    ++ CmdLine.pathToExtractExe
    ++ " [path...]
  -v shows the program version
  --allow-duplicates allows messages with identical `id` props if `defaultMessage` props are identical as well
  -help  Display this list of options
  --help  Display this list of options
";

  test("version", ({expect}) => {
    let (out, err) = CmdLine.run("-v");

    expect.string(err).toEqual("");
    expect.string(out).toEqual(version ++ "\n");
  });

  test("usageNoArgs", ({expect}) => {
    let (out, err) = CmdLine.run("");

    expect.string(err).toEqual(usage);
    expect.string(out).toEqual("");
  });

  test("usageHelp", ({expect}) => {
    let (out, err) = CmdLine.run("-help");

    expect.string(err).toEqual("");
    expect.string(out).toEqual(usage);
  });
});

describe("Extract", ({test}) => {
  test("full", ({expect}) => {
    let (out, err) = CmdLine.run("testData/test1 testData/test2");

    expect.string(err).toEqual("");
    expect.string(out).toMatchSnapshot();
  });

  test("partial", ({expect}) => {
    let (out, err) = CmdLine.run("testData/test1/subdir/Test_1_2.re testData/test2");

    expect.string(err).toEqual("");
    expect.string(out).toMatchSnapshot();
  });
});

describe("Duplicates", ({test}) => {
  test("allowOk", ({expect}) => {
    let (out, err) = CmdLine.run("--allow-duplicates testData/test3/Test_3_1.re testData/test3/Test_3_2.re");

    expect.string(err).toEqual("");
    expect.string(out).toMatchSnapshot();
  });

  test("allowNok", ({expect}) => {
    let (out, err) =
      CmdLine.run(
        "--allow-duplicates testData/test3/Test_3_1.re testData/test3/Test_3_2.re testData/test3/Test_3_3.re",
      );

    expect.string(err).toEqual("Error: duplicate message id: test3.msg1.1 with different default messages\n");
    expect.string(out).toEqual("");
  });

  test("notAllowed", ({expect}) => {
    let (out, err) = CmdLine.run("testData/test3/Test_3_1.re testData/test3/Test_3_2.re");

    expect.string(err).toEqual("Error: duplicate message id: test3.msg1.1\n");
    expect.string(out).toEqual("");
  });
});
