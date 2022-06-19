open Lib

let extract ~duplicatesAllowed paths =
  try
    let messages = Extractor.extract ~duplicatesAllowed paths in
    `List (messages |> List.map Message.toJson)
    |> Yojson.Basic.pretty_to_channel stdout;
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

type options = {showVersion: bool; paths: string list; duplicatesAllowed: bool}

let run () =
  let options =
    ref {showVersion = false; paths = []; duplicatesAllowed = false}
  in
  let processInputFilename filename =
    options := {!options with paths = filename :: !options.paths}
  in
  let allowDuplicates () =
    options := {!options with duplicatesAllowed = true}
  in
  let showVersion () = options := {!options with showVersion = true} in
  let args =
    [
      ("-v", Arg.Unit showVersion, "shows the program version");
      ( "--allow-duplicates",
        Arg.Unit allowDuplicates,
        "allows messages with identical `id` props if `defaultMessage` props \
         are identical as well" );
    ]
  in
  let usage = "Usage: " ^ Sys.argv.(0) ^ " [path...]" in
  Arg.parse args processInputFilename usage;
  match !options with
  | {showVersion = true} -> print_endline Version.version
  | {paths = []} -> Arg.usage args usage
  | {paths; duplicatesAllowed} -> extract ~duplicatesAllowed paths
;;

run ()
