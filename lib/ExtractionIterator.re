open Ast_iterator;

open Asttypes;

open Parsetree;

open Longident;

module StringMap = Map.Make(String);

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

let extractMessageFromRecord = (callback, fields) => {
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

  Message.fromStringMap(map) |> Option.iter(callback);
};

let extractMessagesFromRecords = (callback, records) =>
  records
  |> List.iter(field =>
       switch (field) {
       | (
           {txt: Lident(_)},
           {
             pexp_desc:
               Pexp_extension((
                 {txt: "bs.obj"},
                 PStr([{pstr_desc: Pstr_eval({pexp_desc: Pexp_record(fields, _)}, _), pstr_loc: _}]),
               )),
           },
         ) =>
         extractMessageFromRecord(callback, fields)
       | _ => ()
       }
     );

let hasIntlAttribute = (items: structure) =>
  items
  |> List.exists(item =>
       switch (item) {
       | {pstr_desc: Pstr_attribute({attr_name: {txt: "intl.messages"}})} => true
       | _ => false
       }
     );

let extractMessagesFromValueBindings = (callback, valueBindings: list(value_binding)) =>
  valueBindings
  |> List.iter(valueBinding =>
       switch (valueBinding) {
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

let matchesDefineMessages = ident =>
  switch (ident) {
  | Ldot(Lident("ReactIntl"), "defineMessages")
  | Lident("defineMessages") => true
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

    // Match (ReactIntl.)defineMessages
    | {
        pexp_desc:
          Pexp_apply(
            {pexp_desc: Pexp_ident({txt, _})},
            [
              (
                Asttypes.Nolabel,
                {
                  pexp_desc:
                    Pexp_extension((
                      {txt: "bs.obj"},
                      PStr([{pstr_desc: Pstr_eval({pexp_desc: Pexp_record(fields, _)}, _), pstr_loc: _}]),
                    )),
                },
              ),
            ],
          ),
      }
        when matchesDefineMessages(txt) =>
      extractMessagesFromRecords(callback, fields)

    // Match [@intl.messages] on objects
    | {
        pexp_desc:
          Pexp_extension((
            {txt: "bs.obj"},
            PStr([{pstr_desc: Pstr_eval({pexp_desc: Pexp_record(fields, _)}, _), pstr_loc: _}]),
          )),
        pexp_attributes: [{attr_name: {txt: "intl.messages"}}],
      } =>
      extractMessagesFromRecords(callback, fields)

    | _ => ()
    };

    default_iterator.expr(iterator, expr);
  },
};
