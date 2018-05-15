open Ast_iterator;

open Parsetree;

let getLabelString = (key, labels) =>
  switch (labels |> List.assoc_opt(Asttypes.Labelled(key))) {
  | Some({pexp_desc: Pexp_constant(Pconst_string(text, _))}) => Some(text)
  | _ => None
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

let getIterator = callback => {
  ...default_iterator,
  expr: (iterator, expr) => {
    switch (getMessage(expr)) {
    | Some(message) => callback(message)
    | None => ()
    };
    default_iterator.expr(iterator, expr);
  },
};
