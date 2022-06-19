open Ast_iterator
open Asttypes
open Parsetree
open Longident

let extractMessageFromLabels callback labels =
  let map =
    labels
    |> List.fold_left
         (fun map assoc ->
           match assoc with
           | ( Asttypes.Labelled key,
               {pexp_desc = Pexp_constant (Pconst_string (value, _))} ) ->
             map |> StringMap.add key value
           | _ -> map)
         StringMap.empty
  in
  Message.fromStringMap map |> Option.iter callback

let extractMessageFromRecord ?description callback fields =
  let map =
    fields
    |> List.fold_left
         (fun map field ->
           match field with
           | ( {txt = Lident key},
               {pexp_desc = Pexp_constant (Pconst_string (value, _))} ) ->
             map |> StringMap.add key value
           | _ -> map)
         StringMap.empty
  in
  Message.fromStringMap ?description map |> Option.iter callback

let extractMessageFromExtension callback item =
  let makeId ?(description = "") message =
    message ^ "|" ^ description |> Digest.string |> Digest.to_hex
  in
  match item with
  | PStr
      [
        {
          pstr_desc =
            Pstr_eval
              ( {pexp_desc = Pexp_constant (Pconst_string (defaultMessage, _))},
                _ );
          _;
        };
      ] ->
    let id = defaultMessage |> makeId in
    callback
      (let open Message in
      {id; defaultMessage; description = None})
  | PStr
      [
        {
          pstr_desc =
            Pstr_eval
              ( {
                  pexp_desc =
                    Pexp_record
                      ( ( [
                            ( {txt = Lident "msg"; _},
                              {
                                pexp_desc =
                                  Pexp_constant
                                    (Pconst_string (defaultMessage, _));
                                _;
                              } );
                            ( {txt = Lident "desc"; _},
                              {
                                pexp_desc =
                                  Pexp_constant (Pconst_string (description, _));
                                _;
                              } );
                          ]
                        | [
                            ( {txt = Lident "desc"; _},
                              {
                                pexp_desc =
                                  Pexp_constant (Pconst_string (description, _));
                                _;
                              } );
                            ( {txt = Lident "msg"; _},
                              {
                                pexp_desc =
                                  Pexp_constant
                                    (Pconst_string (defaultMessage, _));
                                _;
                              } );
                          ] ),
                        None );
                  _;
                },
                _ );
          _;
        };
      ] ->
    let id = defaultMessage |> makeId ~description in
    callback
      (let open Message in
      {id; defaultMessage; description = Some description})
  | _ -> ()

let hasIntlAttribute (items : structure) =
  items
  |> List.exists (fun item ->
         match item with
         | {pstr_desc = Pstr_attribute ({txt = "intl.messages"}, _)} -> true
         | _ -> false)

let extractMessagesFromValueBindings callback
    (valueBindings : value_binding list) =
  valueBindings
  |> List.iter (fun valueBinding ->
         match valueBinding with
         | {
          pvb_pat = {ppat_desc = Ppat_var _};
          pvb_expr =
            {
              pexp_desc = Pexp_record (fields, None);
              pexp_attributes =
                [
                  ( {txt = "intl.description"},
                    PStr
                      [
                        {
                          pstr_desc =
                            Pstr_eval
                              ( {
                                  pexp_desc =
                                    Pexp_constant
                                      (Pconst_string (description, _));
                                },
                                _ );
                          pstr_loc = _;
                        };
                      ] );
                ];
            };
         } ->
           extractMessageFromRecord ~description callback fields
         | {
          pvb_pat = {ppat_desc = Ppat_var _};
          pvb_expr = {pexp_desc = Pexp_record (fields, None)};
         } ->
           extractMessageFromRecord callback fields
         | _ -> ())

let extractMessagesFromModule callback (items : structure) =
  if hasIntlAttribute items then
    items
    |> List.iter (fun item ->
           match item with
           | {pstr_desc = Pstr_value (Nonrecursive, valueBindings)} ->
             extractMessagesFromValueBindings callback valueBindings
           | _ -> ())

let matchesFormattedMessage ident =
  match ident with
  | Ldot (Ldot (Lident "ReactIntl", "FormattedMessage"), "createElement")
  | Ldot (Lident "FormattedMessage", "createElement") ->
    true
  | _ -> false

let getIterator callback =
  {
    default_iterator with
    structure =
      (fun iterator structure ->
        extractMessagesFromModule callback structure;
        default_iterator.structure iterator structure);
    expr =
      (fun iterator expr ->
        ( match expr with
        | {pexp_desc = Pexp_apply ({pexp_desc = Pexp_ident {txt; _}}, labels)}
          when matchesFormattedMessage txt ->
          extractMessageFromLabels callback labels
        | {
         pexp_desc = Pexp_extension ({txt = "intl" | "intl.s" | "intl.el"}, item);
         _;
        } ->
          extractMessageFromExtension callback item
        | _ -> () );
        default_iterator.expr iterator expr);
  }
