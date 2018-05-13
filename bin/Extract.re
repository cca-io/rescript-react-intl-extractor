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

let processReasonFile = filename => {
  let channel = open_in_bin(filename);
  let lexbuf = Lexing.from_channel(channel);
  let ast = Reason_toolchain.RE.implementation(lexbuf);
  close_in(channel);
  extractMessages(ast);
};

let rec processDirectory = dir =>
  Sys.readdir(dir)
  |> Array.iter(filename => {
       let path = Filename.concat(dir, filename);

       if (Sys.is_directory(path)) {
         processDirectory(path);
       } else if (Filename.extension(filename) == ".re") {
         processReasonFile(path);
       };
     });

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

let usage = "Usage: " ++ Sys.argv[0] ++ " directory ...";

Arg.parse(args, processInputFilename, usage);

switch (inputFilenames^) {
| [] => Arg.usage(args, usage)
| filenames => filenames |> List.rev |> List.iter(processDirectory)
};

outputJson();
