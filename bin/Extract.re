open Migrate_parsetree;

open Lib;

let messages = ref([]);

let iterator = ExtractionIterator.getIterator(message => (messages := [message, ...messages^]) |> ignore);

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

let processFile = filename => {
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
         processFile(path);
       };
     });

let outputJson = () => {
  let sortedJsonObjects = messages^ |> List.sort(Message.compare) |> List.map(Message.toJson);
  Yojson.Basic.pretty_to_channel(stdout, `List(sortedJsonObjects));
  print_newline();
};

switch (Array.to_list(Sys.argv)) {
| [_exe, dir] =>
  if (! Sys.is_directory(dir)) {
    Printf.eprintf("Not a directory: %s\n", dir);
    exit(2);
  };
  processDirectory(dir);
  outputJson();
| [exe, ..._params] =>
  Printf.eprintf("Usage: %s <directory>\n", exe);
  exit(1);
| _ => () /* cannot happen */
};
