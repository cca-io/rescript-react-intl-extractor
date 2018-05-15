type t = {
  id: string,
  defaultMessage: string,
  description: option(string),
};

let compare = (a, b) => compare(a.id, b.id);

let toJson = ({id, defaultMessage, description}) =>
  switch (description) {
  | Some(description) =>
    `Assoc([
      ("id", `String(id)),
      ("defaultMessage", `String(defaultMessage)),
      ("description", `String(description)),
    ])
  | None => `Assoc([("id", `String(id)), ("defaultMessage", `String(defaultMessage))])
  };
