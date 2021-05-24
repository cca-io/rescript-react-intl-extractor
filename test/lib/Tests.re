open TestFramework;
open Lib;

let extractAndGetJson = (~duplicatesAllowed=?, paths) => {
  let messages = Extractor.extract(~duplicatesAllowed?, paths);
  let jsonObj = `List(messages |> List.map(Message.toJson));

  Yojson.Basic.pretty_to_string(jsonObj);
};

describe("Extract", ({test}) => {
  test("full", ({expect}) => {
    let json = extractAndGetJson(["testData/test1"]);
    expect.string(json).toMatchSnapshot();
  });

  test("partial", ({expect}) => {
    let json = extractAndGetJson(["testData/test1/subdir/Test_1_3.re"]);
    expect.string(json).toMatchSnapshot();
  });
});

describe("Duplicates", ({test}) => {
  test("allowedOk", ({expect}) => {
    let json =
      extractAndGetJson(~duplicatesAllowed=true, ["testData/test3/Test_3_1.re", "testData/test3/Test_3_2.re"]);
    expect.string(json).toMatchSnapshot();
  });

  test("allowedNok", ({expect}) => {
    let extract = () =>
      extractAndGetJson(~duplicatesAllowed=true, ["testData/test3/Test_3_1.re", "testData/test3/Test_3_3.re"]);

    expect.fn(extract).toThrowException(Extractor.DefaultMessageNotMatching("test3.msg1.1"));
  });

  test("notAllowed", ({expect}) => {
    let extract = () =>
      extractAndGetJson(~duplicatesAllowed=false, ["testData/test3/Test_3_1.re", "testData/test3/Test_3_3.re"]);

    expect.fn(extract).toThrowException(Extractor.DuplicateMessageId("test3.msg1.1"));
  });
});

describe("Path not found", ({test}) => {
  test("dirNotFound", ({expect}) => {
    let extract = () => extractAndGetJson(["someDir"]);
    expect.fn(extract).toThrowException(Extractor.PathNotFound("someDir"));
  });

  test("fileNotFound", ({expect}) => {
    let extract = () => extractAndGetJson(["testData/test1/SomeFile.re"]);
    expect.fn(extract).toThrowException(Extractor.PathNotFound("testData/test1/SomeFile.re"));
  });
});

describe("ReScript", ({test}) => {
  test("full", ({expect}) => {
    let json = extractAndGetJson(["testData/test4"]);
    expect.string(json).toMatchSnapshot();
  })
});

describe("IntlPpx", ({test}) => {
  test("full", ({expect}) => {
    let json = extractAndGetJson(["testData/test5"]);
    expect.string(json).toMatchSnapshot();
  })
});
