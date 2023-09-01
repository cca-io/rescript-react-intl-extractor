open Lib

let extract ~duplicatesAllowed ~verbose ~paths =
  try
    let messages = Extractor.extract ~duplicatesAllowed ~verbose paths in
    let json = `List (messages |> List.map Message.toJson) in
    json |> Yojson.Basic.pretty_to_channel stdout;
    print_newline ()
  with
  | Extractor.PathNotFound path ->
    Printf.eprintf "Error: file or directory does not exist: %s\n" path;
    exit 1
  | Extractor.DuplicateMessageId id ->
    Printf.eprintf "Error: duplicate message id: %s\n" id;
    exit 2
  | Extractor.DefaultMessageNotMatching id ->
    Printf.eprintf
      "Error: duplicate message id: %s with different default messages\n" id;
    exit 3
  | exn ->
    Printf.eprintf "Unexpected error: %s\n" (Printexc.to_string exn);
    exit 10

type options = {
  mutable showVersion: bool;
  mutable paths: string list;
  mutable duplicatesAllowed: bool;
  mutable verbose: bool;
}

let run () =
  let options =
    {
      showVersion = false;
      paths = [];
      duplicatesAllowed = false;
      verbose = false;
    }
  in
  let processInputFilename filename =
    options.paths <- filename :: options.paths
  in
  let allowDuplicates () = options.duplicatesAllowed <- true in
  let showVersion () = options.showVersion <- true in
  let verbose () = options.verbose <- true in

  let args =
    [
      ("-v", Arg.Unit showVersion, "shows the program version");
      ( "--verbose",
        Arg.Unit verbose,
        "log some information like the current processed file path" );
      ( "--allow-duplicates",
        Arg.Unit allowDuplicates,
        "allows messages with identical `id` props if `defaultMessage` props \
         are identical as well" );
    ]
  in

  let usage = "Usage: " ^ Sys.argv.(0) ^ " [path...]" in
  Arg.parse args processInputFilename usage;

  match options with
  | {showVersion = true} -> print_endline Version.version
  | {paths = []} -> Arg.usage args usage
  | {paths; duplicatesAllowed; verbose} ->
    extract ~duplicatesAllowed ~verbose ~paths

let () = run ()
