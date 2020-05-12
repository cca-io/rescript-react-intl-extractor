open Lib;

module StringMap = Map.Make(String);

let messages = ref(StringMap.empty);

let duplicatesAreAllowed = ref(false);

let iterator =
  ExtractionIterator.getIterator(message => {
    let {Message.id, defaultMessage} = message;
    switch (messages^ |> StringMap.find_opt(id)) {
    | None => messages := messages^ |> StringMap.add(id, message)
    | Some(existingMessage) when duplicatesAreAllowed^ && defaultMessage == existingMessage.defaultMessage =>
      messages := messages^ |> StringMap.add(id, message)
    | Some(existingMessage) when duplicatesAreAllowed^ && defaultMessage != existingMessage.defaultMessage =>
      Printf.eprintf("Error: duplicate message id: %s with different default messages\n", id);
      exit(3);
    | Some(_) =>
      Printf.eprintf("Error: duplicate message id: %s\n", id);
      exit(3);
    };
  });

let extractMessages = ast => iterator.structure(iterator, ast);

let processReasonFile = path => {
  let channel = open_in_bin(path);
  let lexbuf = Lexing.from_channel(channel);
  let ast = Reason_toolchain.(RE.implementation(lexbuf) |> To_current.copy_structure);
  close_in(channel);

  extractMessages(ast);
};

let rec processPath = path => {
  if (!Sys.file_exists(path)) {
    Printf.eprintf("Error: file or directory does not exist: %s\n", path);
    exit(1);
  };

  if (Sys.is_directory(path)) {
    Sys.readdir(path) |> Array.iter(filename => processPath(Filename.concat(path, filename)));
  } else if (Filename.extension(path) == ".re") {
    processReasonFile(path);
  };
};

let outputJson = () => {
  let sortedJsonObjects =
    messages^
    |> StringMap.bindings
    |> List.map(((_id, message)) => message)
    |> List.sort(Message.compare)
    |> List.map(Message.toJson);

  Yojson.Basic.pretty_to_channel(stdout, `List(sortedJsonObjects));
  print_newline();
};

let inputFilenames = ref([]);

let processInputFilename = filename => inputFilenames := [filename, ...inputFilenames^];

let showVersion = () => {
  print_endline(Version.version);
  exit(0);
};

let allowDuplicates = () => duplicatesAreAllowed := true;

let args = [
  ("-v", Arg.Unit(showVersion), "shows the program version"),
  (
    "--allow-duplicates",
    Arg.Unit(allowDuplicates),
    "allows messages with identical `id` props if `defaultMessage` props are identical as well",
  ),
];

let usage = "Usage: " ++ Sys.argv[0] ++ " [path...]";

Arg.parse(args, processInputFilename, usage);

switch (inputFilenames^) {
| [] => Arg.usage(args, usage)
| filenames =>
  filenames |> List.rev |> List.iter(processPath);
  outputJson();
};
