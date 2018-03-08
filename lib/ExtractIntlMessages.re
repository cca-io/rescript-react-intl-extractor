open Ast_mapper;

open Ast_helper;

open Asttypes;

open Parsetree;

open Longident;

let isInFormattedMessage = ref(false);

let wrapper = argv => {
  ...default_mapper,
  label_declaration: (mapper, expr) => {
    if (isInFormattedMessage^) {
      print_endline(">>> HURRA");
    };
    print_endline(">>> Label ");
    default_mapper.label_declaration(mapper, expr);
  },
  expr: (mapper, expr) =>
    switch (expr) {
    | {pexp_desc: Pexp_ident({txt, loc}), pexp_attributes: attributes} =>
      switch (Longident.flatten(txt)) {
      | ["ReactIntl", "FormattedMessage", "make"] =>
        print_endline(">>> FormattedMessage");
        isInFormattedMessage := true;
      | _ => ()
      };
      if (isInFormattedMessage^) {
        print_endline(">>> HURRAAAAAAAAAAAAAAA");
      };
      let res = default_mapper.expr(mapper, expr);
      isInFormattedMessage := false;
      res;
    | expr => default_mapper.expr(mapper, expr)
    },
};