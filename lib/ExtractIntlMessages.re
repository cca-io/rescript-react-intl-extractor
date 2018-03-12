open Ast_mapper;

open Asttypes;

open Parsetree;

let wrapper = argv => {
  ...default_mapper,
  expr: (mapper, expr) =>
    switch (expr) {
    | {
        pexp_desc:
          Pexp_apply(
            {pexp_desc: Pexp_ident({txt: Ldot(Ldot(Lident("ReactIntl"), "FormattedMessage"), "make"), loc})},
            [
              ("id", {pexp_desc: Pexp_constant(Const_string(id, _))}),
              ("defaultMessage", {pexp_desc: Pexp_constant(Const_string(defaultMessage, _))}),
              ..._,
            ],
          ),
      } =>
      print_endline(id ++ ": " ++ defaultMessage);
      default_mapper.expr(mapper, expr);
    | expr => default_mapper.expr(mapper, expr)
    },
};