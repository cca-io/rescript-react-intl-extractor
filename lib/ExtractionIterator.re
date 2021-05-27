open Ast_iterator;

open Asttypes;

open Parsetree;

open Longident;

let extractMessageFromLabels = (callback, labels) => {
  let map =
    labels
    |> List.fold_left(
         (map, assoc) =>
           switch (assoc) {
           | (Asttypes.Labelled(key), {pexp_desc: Pexp_constant(Pconst_string(value, _))}) =>
             map |> StringMap.add(key, value)
           | _ => map
           },
         StringMap.empty,
       );

  Message.fromStringMap(map) |> Option.iter(callback);
};

let extractMessageFromRecord = (~description=?, callback, fields) => {
  let map =
    fields
    |> List.fold_left(
         (map, field) =>
           switch (field) {
           | ({txt: Lident(key)}, {pexp_desc: Pexp_constant(Pconst_string(value, _))}) =>
             map |> StringMap.add(key, value)
           | _ => map
           },
         StringMap.empty,
       );

  Message.fromStringMap(~description?, map) |> Option.iter(callback);
};

let extractMessageFromExtension = (callback, item) => {
  // TODO share this function with intl ppx
  let makeId = (~description="", message) => message ++ "|" ++ description |> Digest.string |> Digest.to_hex;

  switch (item) {
  | PStr([{pstr_desc: Pstr_eval({pexp_desc: Pexp_constant(Pconst_string(defaultMessage, _)), _}, _), _}]) =>
    let id = defaultMessage |> makeId;
    callback(Message.{id, defaultMessage, description: None});
  | PStr([
      {
        pstr_desc:
          Pstr_eval(
            {
              pexp_desc:
                Pexp_record(
                  [
                    ({txt: Lident("msg"), _}, {pexp_desc: Pexp_constant(Pconst_string(defaultMessage, _)), _}),
                    ({txt: Lident("desc"), _}, {pexp_desc: Pexp_constant(Pconst_string(description, _)), _}),
                  ] |
                  [
                    ({txt: Lident("desc"), _}, {pexp_desc: Pexp_constant(Pconst_string(description, _)), _}),
                    ({txt: Lident("msg"), _}, {pexp_desc: Pexp_constant(Pconst_string(defaultMessage, _)), _}),
                  ],
                  None,
                ),
              _,
            },
            _,
          ),
        _,
      },
    ]) =>
    let id = defaultMessage |> makeId(~description);
    callback(Message.{id, defaultMessage, description: Some(description)});
  | _ => ()
  };
};

let hasIntlAttribute = (items: structure) =>
  items
  |> List.exists(item =>
       switch (item) {
       | {pstr_desc: Pstr_attribute(({txt: "intl.messages"}, _))} => true
       | _ => false
       }
     );

let extractMessagesFromValueBindings = (callback, valueBindings: list(value_binding)) =>
  valueBindings
  |> List.iter(valueBinding =>
       switch (valueBinding) {
       // Match with [@intl.description "i am description"] let foo = { ... };
       | {
           pvb_pat: {ppat_desc: Ppat_var(_)},
           pvb_expr: {
             pexp_desc: Pexp_record(fields, None),
             pexp_attributes: [
               (
                 {txt: "intl.description"},
                 PStr([
                   {
                     pstr_desc: Pstr_eval({pexp_desc: Pexp_constant(Pconst_string(description, _))}, _),
                     pstr_loc: _,
                   },
                 ]),
               ),
             ],
           },
         } =>
         extractMessageFromRecord(~description, callback, fields)
       | {pvb_pat: {ppat_desc: Ppat_var(_)}, pvb_expr: {pexp_desc: Pexp_record(fields, None)}} =>
         extractMessageFromRecord(callback, fields)
       | _ => ()
       }
     );

let extractMessagesFromModule = (callback, items: structure) =>
  if (hasIntlAttribute(items)) {
    items
    |> List.iter(item =>
         switch (item) {
         | {pstr_desc: Pstr_value(Nonrecursive, valueBindings)} =>
           extractMessagesFromValueBindings(callback, valueBindings)
         | _ => ()
         }
       );
  };

let matchesFormattedMessage = ident =>
  switch (ident) {
  | Ldot(Ldot(Lident("ReactIntl"), "FormattedMessage"), "createElement")
  | Ldot(Lident("FormattedMessage"), "createElement") => true
  | _ => false
  };

let getIterator = callback => {
  ...default_iterator,

  // Match records in modules with [@intl.messages]
  // (structure is the module body - either top level or of a submodule)
  structure: (iterator, structure) => {
    extractMessagesFromModule(callback, structure);
    default_iterator.structure(iterator, structure);
  },

  expr: (iterator, expr) => {
    switch (expr) {
    // Match (ReactIntl.)FormattedMessage.createElement
    | {pexp_desc: Pexp_apply({pexp_desc: Pexp_ident({txt, _})}, labels)} when matchesFormattedMessage(txt) =>
      extractMessageFromLabels(callback, labels)

    // Match [%intl "message"]
    // [%intl.s "message"]
    // [%intl.el "message"]
    // [%intl {msg: "message", desc: "description}]
    // [%intl.s {msg: "message", desc: "description}]
    // [%intl.el {msg: "message", desc: "description}]
    // [%intl {desc: "description", msg: "message"}]
    // [%intl.s {desc: "description", msg: "message"}]
    // [%intl.el { desc: "description", msg: "message"}]
    | {pexp_desc: Pexp_extension(({txt: "intl" | "intl.s" | "intl.el"}, item)), _} =>
      extractMessageFromExtension(callback, item)

    | _ => ()
    };

    default_iterator.expr(iterator, expr);
  },
};
