open Lib

let extract ?duplicatesAllowed paths =
  let messages = Extractor.extract ?duplicatesAllowed paths in
  let jsonObj = `List (messages |> List.map Message.toJson) in
  Yojson.Basic.pretty_to_string jsonObj

let testExtract ?duplicatesAllowed paths expected =
  let jsonString = extract ?duplicatesAllowed paths in
  Alcotest.check Alcotest.string "extract" expected jsonString

let testExtractFull () =
  testExtract ["testData/test1"]
    {|[
  { "id": "test1.msg1.1", "defaultMessage": "This is message 1.1" },
  { "id": "test1.msg1.2", "defaultMessage": "This is message 1.2" },
  {
    "id": "test1.msg1.3",
    "defaultMessage": "This is message 1.3",
    "description": "Description for message 1.3"
  },
  { "id": "test1.msg1.4", "defaultMessage": "This is message 1.4" },
  { "id": "test1.msg1.5", "defaultMessage": "This is message 1.5" },
  { "id": "test1.msg1.8", "defaultMessage": "This is message 1.8" },
  {
    "id": "test1.msg1.9",
    "defaultMessage": "This is message 1.9",
    "description": "Description for message 1.9"
  },
  { "id": "test1.msg3.1", "defaultMessage": "This is message 3.1" }
]|}

let testExtractPartial () =
  testExtract
    ["testData/test1/subdir/Test_1_3.res"]
    {|[ { "id": "test1.msg3.1", "defaultMessage": "This is message 3.1" } ]|}

let testExtractReScript () =
  testExtract ["testData/test4"]
    {|[
  { "id": "test4.msg1.1", "defaultMessage": "This is message 1.1" },
  { "id": "test4.msg1.2", "defaultMessage": "This is message 1.2" },
  {
    "id": "test4.msg1.3",
    "defaultMessage": "This is message 1.3",
    "description": "Description for message 1.3"
  },
  { "id": "test4.msg1.4", "defaultMessage": "This is message 1.4" },
  { "id": "test4.msg1.5", "defaultMessage": "This is message 1.5" },
  { "id": "test4.msg1.8", "defaultMessage": "This is message 1.8" },
  {
    "id": "test4.msg1.9",
    "defaultMessage": "This is message 1.9",
    "description": "Description for message 1.9"
  },
  { "id": "test4.msg3.1", "defaultMessage": "This is message 3.1" }
]|}

let testExtractIntlPpx () =
  testExtract ["testData/test5"]
    {|[
  {
    "id": "07ef8ec9f2dd894c45b447292d527f4a",
    "defaultMessage": "This is message 5.2.2"
  },
  {
    "id": "09644115fd7f4e0803e5300442acaa3f",
    "defaultMessage": "This is message 5.2.9",
    "description": "This is description for message 5.2.9"
  },
  {
    "id": "50c182276f426aaed9bb73e932749c12",
    "defaultMessage": "This is message 5.1.1"
  },
  {
    "id": "517ecc8330114017ce1fa3358b0e19fb",
    "defaultMessage": "This is message 5.2.7",
    "description": "This is description for message 5.2.7"
  },
  {
    "id": "617b406e8ce2d6117eb79e480b7db916",
    "defaultMessage": "This is message 5.1.6",
    "description": "This is description for message 5.1.6"
  },
  {
    "id": "705ebca03144cedde0772b42454db9bc",
    "defaultMessage": "This is message 5.1.2"
  },
  {
    "id": "82d39a35baed93572eb9d3e2f31d9549",
    "defaultMessage": "This is message 5.1.7",
    "description": "This is description for message 5.1.7"
  },
  {
    "id": "9bd72b86b4707b49e954679611f24389",
    "defaultMessage": "This is message 5.1.9",
    "description": "This is description for message 5.1.9"
  },
  {
    "id": "aa90db9568631794111be2c0dbf3f290",
    "defaultMessage": "This is message 5.2.4",
    "description": "This is description for message 5.2.4"
  },
  {
    "id": "b5efe6bdb050410f0182c3e2b74b7add",
    "defaultMessage": "This is message 5.2.5",
    "description": "This is description for message 5.2.5"
  },
  {
    "id": "bbd2f42ed446c470db8326dd1ea169fe",
    "defaultMessage": "This is message 5.2.1"
  },
  {
    "id": "c26daf229e5a13904d33004e1e48fa87",
    "defaultMessage": "This is message 5.2.3"
  },
  {
    "id": "d48a62aec72f3b16408aacff820de2de",
    "defaultMessage": "This is message 5.1.3"
  },
  {
    "id": "da572261a2f0bc6550f8701e35aa0d13",
    "defaultMessage": "This is message 5.2.6",
    "description": "This is description for message 5.2.6"
  },
  {
    "id": "dec09d01d1cf004612cef6f2dc4c81e3",
    "defaultMessage": "This is message 5.1.4",
    "description": "This is description for message 5.1.4"
  },
  {
    "id": "deeb9aaca5b6be205f113483625b19e2",
    "defaultMessage": "This is message 5.1.8",
    "description": "This is description for message 5.1.8"
  },
  {
    "id": "e4a34c50454eec2a66088d7a5e7f00d5",
    "defaultMessage": "This is message 5.2.8",
    "description": "This is description for message 5.2.8"
  },
  {
    "id": "fc63eab6a84654a8a5d05d83f165db1e",
    "defaultMessage": "This is message 5.1.5",
    "description": "This is description for message 5.1.5"
  }
]|}

let testExtractEscaped () =
  testExtract ["testData/testEscape"]
    {|[
  {
    "id": "ReScript.decodeError",
    "defaultMessage":
      "The configuration could not be decoded.\n\nPlease contact your administrator."
  },
  {
    "id": "ReScript.trackingActive",
    "defaultMessage": "Tracking active in \"{incident}\""
  }
]|}

let testPathNotFoundError () =
  Alcotest.check_raises "dir not found"
    (Extractor.PathNotFound "testData/someDir") (fun () ->
      extract ["testData/someDir"] |> ignore);
  Alcotest.check_raises "file not found"
    (Extractor.PathNotFound "testData/test1/SomeFile.res") (fun () ->
      extract ["testData/test1/SomeFile.res"] |> ignore)

let testDuplicatesOk () =
  testExtract ~duplicatesAllowed:true
    ["testData/test3/Test_3_1.res"; "testData/test3/Test_3_2.res"]
    {|[
  { "id": "test3.msg1.1", "defaultMessage": "This is message 1.1" },
  { "id": "test3.msg1.2", "defaultMessage": "This is message 1.2" },
  { "id": "test3.msg1.3", "defaultMessage": "This is message 1.3" }
]|}

let testDuplicatesNok () =
  Alcotest.check_raises "default message not matching"
    (Extractor.DefaultMessageNotMatching "test3.msg1.1") (fun () ->
      extract ~duplicatesAllowed:true
        ["testData/test3/Test_3_1.res"; "testData/test3/Test_3_3.res"]
      |> ignore)

let testDuplicatesNotAllowed () =
  Alcotest.check_raises "duplicates not allowed"
    (Extractor.DuplicateMessageId "test3.msg1.1") (fun () ->
      extract ["testData/test3/Test_3_1.res"; "testData/test3/Test_3_2.res"]
      |> ignore)

open Alcotest
let testSetExtract =
  [
    test_case "Extract full" `Quick testExtractFull;
    test_case "Extract partial" `Quick testExtractPartial;
    test_case "Extract ReScript" `Quick testExtractReScript;
    test_case "Extract messages with escape chars" `Quick testExtractEscaped;
    test_case "Extract Intl PPX" `Quick testExtractIntlPpx;
    test_case "Path not found" `Quick testPathNotFoundError;
  ]

let testSetDuplicates =
  [
    test_case "Extract duplicates" `Quick testDuplicatesOk;
    test_case "Default message not matching" `Quick testDuplicatesNok;
    test_case "Duplicates not allowed" `Quick testDuplicatesNotAllowed;
  ]

let () =
  run "rescript-react-intl-extractor"
    [("extract", testSetExtract); ("duplicates", testSetDuplicates)]
