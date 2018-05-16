open Ast_iterator;

open Asttypes;

open Parsetree;

open Longident;

module StringMap = Map.Make(String);

let extractMessageFromLabels = labels => {
  let map = ref(StringMap.empty);
  labels
  |> List.iter(assoc =>
       switch (assoc) {
       | (Asttypes.Labelled(key), {pexp_desc: Pexp_constant(Pconst_string(value, _))}) =>
         map := map^ |> StringMap.add(key, value)
       | _ => ()
       }
     );
  Message.fromStringMap(map^);
};

let extractMessageFromRecord = fields => {
  let map = ref(StringMap.empty);
  fields
  |> List.iter(field =>
       switch (field) {
       | ({txt: Lident(key)}, {pexp_desc: Pexp_constant(Pconst_string(value, _))}) =>
         map := map^ |> StringMap.add(key, value)
       | _ => ()
       }
     );
  Message.fromStringMap(map^);
};

let extractMessagesFromRecords = (callback, records) =>
  records
  |> List.iter(field =>
       switch (field) {
       | (
           {txt: Lident(s)},
           {
             pexp_desc:
               Pexp_extension((
                 {txt: "bs.obj"},
                 PStr([{pstr_desc: Pstr_eval({pexp_desc: Pexp_record(fields, _)}, _), pstr_loc: _}]),
               )),
           },
         ) =>
         switch (extractMessageFromRecord(fields)) {
         | Some(message) => callback(message)
         | _ => ()
         }
       | _ => ()
       }
     );

let getIterator = callback => {
  ...default_iterator,
  expr: (iterator, expr) => {
    switch (expr) {
    /* Match ReactIntl.FormattedMessage.createElement */
    | {
        pexp_desc:
          Pexp_apply(
            {
              pexp_desc: Pexp_ident({txt: Ldot(Ldot(Lident("ReactIntl"), "FormattedMessage"), "createElement"), _}),
            },
            labels,
          ),
      } =>
      switch (extractMessageFromLabels(labels)) {
      | Some(message) => callback(message)
      | _ => ()
      }
    /* Match ReactIntl.defineMessages */
    | {
        pexp_desc:
          Pexp_apply(
            {pexp_desc: Pexp_ident({txt: Ldot(Lident("ReactIntl"), "defineMessages"), _})},
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
      } =>
      extractMessagesFromRecords(callback, fields)
    /* Match [@intl.messages] */
    | {
        pexp_desc:
          Pexp_extension((
            {txt: "bs.obj"},
            PStr([{pstr_desc: Pstr_eval({pexp_desc: Pexp_record(fields, _)}, _), pstr_loc: _}]),
          )),
        pexp_attributes: [({txt: "intl.messages"}, _)],
      } =>
      extractMessagesFromRecords(callback, fields)
    | _ => ()
    };
    default_iterator.expr(iterator, expr);
  },
};
