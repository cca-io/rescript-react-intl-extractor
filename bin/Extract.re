open Migrate_parsetree;

open Ast_mapper;

open Parsetree;

open Lib;

let messages = ref([]);

let mapper = ExtractionMapper.getMapper(message => messages := [message, ...messages^]);

let extractMessages = ast => {
  mapper.structure(mapper, Obj.magic(ast)) |> ignore;
  let sortedJsonObjects = messages^ |> List.sort(Message.compare) |> List.map(Message.toJson);
  Yojson.Basic.pretty_to_channel(stdout, `List(sortedJsonObjects));
  print_newline();
};

switch (Ast_io.from_channel(stdin)) {
| Result.Ok((_, Ast_io.Impl((module Version), ast))) =>
  module Convert = Convert(Version, OCaml_406);
  let converted = Obj.magic(Convert.copy_structure(ast));
  extractMessages(converted);
| Result.Ok((_, Ast_io.Intf(_, _))) => print_endline("Intf AST!")
| Result.Error(Not_a_binary_ast(_)) => print_endline("Error: not a binary AST: ")
| Result.Error(Unknown_version(v)) => print_endline("Error: unknown version: " ++ v)
};