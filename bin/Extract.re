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

let readMessagesFromChannel = channel =>
  switch (Ast_io.from_channel(channel)) {
  | Result.Ok((_, Ast_io.Impl((module Version), ast))) =>
    module Convert = Convert(Version, OCaml_406);
    let converted = Obj.magic(Convert.copy_structure(ast));
    extractMessages(converted);
  | Result.Ok((_, Ast_io.Intf(_, _))) => Printf.eprintf("Interface AST not supported, only implementation AST\n")
  | Result.Error(Not_a_binary_ast(_)) => Printf.eprintf("Error: input is not a binary AST\n")
  | Result.Error(Unknown_version(v)) => Printf.eprintf("Error: unknown AST version: %s\n", v)
  };

let processReasonFile = filename => {
  let channel = Unix.open_process_in("refmt -p binary " ++ filename);
  readMessagesFromChannel(channel);
  Unix.close_process_in(channel) |> ignore;
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
