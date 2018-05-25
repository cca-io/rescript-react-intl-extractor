open Migrate_parsetree;

open Lib;

module StringMap = Map.Make(String);

let messages = ref(StringMap.empty);

let iterator =
  ExtractionIterator.getIterator(message => {
    let {Message.id} = message;
    if (messages^ |> StringMap.mem(id)) {
      Printf.eprintf("Error: duplicate message id: %s\n", id);
      exit(3);
    } else {
      messages := messages^ |> StringMap.add(id, message);
      ();
    };
  });

let extractMessages = ast => iterator.structure(iterator, Obj.magic(ast));

let processReasonFile = path => {
  let channel = open_in_bin(path);
  let lexbuf = Lexing.from_channel(channel);
  let ast = Reason_toolchain.RE.implementation(lexbuf);
  close_in(channel);
  extractMessages(ast);
};

let rec processPath = path => {
  if (! Sys.file_exists(path)) {
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

let args = [("-v", Arg.Unit(showVersion), "shows the program version")];

let usage = "Usage: " ++ Sys.argv[0] ++ " [path...]";

Arg.parse(args, processInputFilename, usage);

switch (inputFilenames^) {
| [] => Arg.usage(args, usage)
| filenames =>
  filenames |> List.rev |> List.iter(processPath);
  outputJson();
};
