open Ast_iterator;

open Asttypes;

open Parsetree;

open Longident;

let idLength = 8;

let generateId = value => {
  "_" ++ (value |> Digest.string |> Digest.to_hex |> String.sub(_, 0, idLength));
};

let findByLabelName = (key, labels) =>
  labels
  |> List.find_opt(label =>
       switch (label) {
       | (Labelled(labelName), _) when labelName == key => true
       | _ => false
       }
     );
let findByFieldName = (key, fields) =>
  fields
  |> List.find_opt(field =>
       switch (field) {
       | ({txt: Lident(fieldName), _}, _) when fieldName == key => true
       | _ => false
       }
     );

let getValueFromField = field =>
  switch (field) {
  | (_, {pexp_desc: Pexp_constant(Pconst_string(value, _)), _}) => value
  | _ => raise(Not_found)
  };

let replaceIdFromRecord = (~loc, fields) => {
  let id = fields |> findByFieldName("id");
  let defaultMessage = fields |> findByFieldName("defaultMessage");

  switch (id, defaultMessage) {
  | (None, Some(defaultMessage)) =>
    fields
    |> List.append([
         (
           {txt: Lident("id"), loc: Ast_helper.default_loc.contents},
           defaultMessage |> getValueFromField |> generateId |> Ast_helper.Const.string |> Ast_helper.Exp.constant,
         ),
       ])
  | (Some(_), _) => fields
  | (None, None) => raise(Location.Error(Location.error(~loc, "Missing id for ReactIntl message")))
  };
};
let getValueFromLabel = label =>
  switch (label) {
  | (_, {pexp_desc: Pexp_constant(Pconst_string(value, _)), _}) => value
  | _ => raise(Not_found)
  };
let replaceIdFromLabels = (~loc, labels) => {
  let id = labels |> findByLabelName("id");
  let defaultMessage = labels |> findByLabelName("defaultMessage");

  switch (id, defaultMessage) {
  | (None, Some(defaultMessage)) =>
    labels
    |> List.append(
         {
           [
             (
               Labelled("id"),
               defaultMessage |> getValueFromLabel |> generateId |> Ast_helper.Const.string |> Ast_helper.Exp.constant,
             ),
           ];
         },
       )
  | (Some(_), _) => labels
  | (None, None) => raise(Location.Error(Location.error(~loc, "Missing id for ReactIntl message")))
  };
};

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

  labels;
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

  fields;
};

let hasIntlAttribute = (items: structure) =>
  items
  |> List.exists(item =>
       switch (item) {
       | {pstr_desc: Pstr_attribute(({txt: "intl.messages"}, _))} => true
       | _ => false
       }
     );

let extractMessagesFromValueBindings = (callback, _iterator, valueBindings: list(value_binding)) =>
  valueBindings
  |> List.map(valueBinding =>
       switch (valueBinding) {
       // Match with [@intl.description "i am description"] let foo = { ... };
       | {
           pvb_pat: {ppat_desc: Ppat_var(_)} as pattern,
           pvb_expr:
             {
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
             } as expr,
           _,
         } =>
         Ast_helper.Vb.mk(
           ~attrs=valueBinding.pvb_attributes,
           ~loc=valueBinding.pvb_loc,
           pattern,
           Ast_helper.Exp.record(
             ~attrs=expr.pexp_attributes,
             ~loc=expr.pexp_loc,
             extractMessageFromRecord(~description, callback, fields |> replaceIdFromRecord(~loc=expr.pexp_loc)),
             None,
           ),
         )
       | {
           pvb_pat: {ppat_desc: Ppat_var(_), _} as pattern,
           pvb_expr: {pexp_desc: Pexp_record(fields, None), _} as expr,
           _,
         } =>
         Ast_helper.Vb.mk(
           ~attrs=valueBinding.pvb_attributes,
           ~loc=valueBinding.pvb_loc,
           pattern,
           Ast_helper.Exp.record(
             ~attrs=expr.pexp_attributes,
             ~loc=expr.pexp_loc,
             extractMessageFromRecord(callback, fields |> replaceIdFromRecord(~loc=expr.pexp_loc)),
             None,
           ),
         )
       //  | _ => default_iterator.value_binding(iterator, valueBinding)
       | _ => valueBinding
       }
     );

// let extractMessagesFromModule = (callback, items: structure) =>
//   if (hasIntlAttribute(items)) {
//     items
//     |> List.iter(item =>
//          switch (item) {
//          | {pstr_desc: Pstr_value(Nonrecursive, valueBindings)} =>
//            extractMessagesFromValueBindings(callback, valueBindings)
//          | _ => ()
//          }
//        );
//   };

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
  structure: (iterator, structure) =>
    if (hasIntlAttribute(structure)) {
      default_iterator.structure(
        iterator,
        structure
        |> List.map(item =>
             switch (item) {
             | {pstr_desc: Pstr_value(Nonrecursive, valueBindings), pstr_loc: loc} =>
               Ast_helper.Str.value(
                 ~loc,
                 Nonrecursive,
                 extractMessagesFromValueBindings(callback, iterator, valueBindings),
               )
             //  | _ => default_iterator.structure_item(iterator, item)
             | _ => item
             }
           ),
      );
    } else {
      default_iterator.structure(iterator, structure);
    },

  expr: (iterator, expr) => {
    switch (expr) {
    // Match (ReactIntl.)FormattedMessage.createElement
    | {pexp_desc: Pexp_apply({pexp_desc: Pexp_ident({txt, _}), _} as applyExpr, labels), _}
        when matchesFormattedMessage(txt) =>
      Ast_helper.Exp.apply(
        ~attrs=expr.pexp_attributes,
        ~loc=expr.pexp_loc,
        applyExpr,
        extractMessageFromLabels(callback, labels |> replaceIdFromLabels(~loc=applyExpr.pexp_loc)),
      )
      |> default_iterator.expr(iterator)
    | _ => default_iterator.expr(iterator, expr)
    };
  },
};
