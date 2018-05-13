open Ast_mapper;

open Parsetree;

let getLabelString = (key, labels) =>
  try (
    {
      let value = labels |> List.assoc(Asttypes.Labelled(key));
      switch (value) {
      | {pexp_desc: Pexp_constant(Pconst_string(text, _))} => Some(text)
      | _ => None
      };
    }
  ) {
  | Not_found => None
  };

let getMessage = expr =>
  switch (expr) {
  | {
      pexp_desc:
        Pexp_apply(
          {
            pexp_desc: Pexp_ident({txt: Ldot(Ldot(Lident("ReactIntl"), "FormattedMessage"), "createElement"), loc}),
          },
          labels,
        ),
    } =>
    let id = labels |> getLabelString("id");
    let defaultMessage = labels |> getLabelString("defaultMessage");
    let description = labels |> getLabelString("description");
    switch (id, defaultMessage) {
    | (Some(id), Some(defaultMessage)) => Some({Message.id, defaultMessage, description})
    | _ => None
    };
  | _ => None
  };

let getMapper = callback => {
  ...default_mapper,
  expr: (mapper, expr) => {
    switch (getMessage(expr)) {
    | Some(message) => callback(message)
    | None => ()
    };
    default_mapper.expr(mapper, expr);
  },
};