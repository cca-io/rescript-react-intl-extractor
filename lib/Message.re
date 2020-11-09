type t = {
  id: string,
  defaultMessage: string,
  description: option(string),
};

let compare = (a, b) => compare(a.id, b.id);

let fromStringMap = (~description=?, map) => {
  let id = map |> StringMap.find_opt("id");
  let defaultMessage = map |> StringMap.find_opt("defaultMessage");
  let description = description |> Option.is_none ? map |> StringMap.find_opt("description") : description;
  switch (id, defaultMessage) {
  | (Some(id), Some(defaultMessage)) => Some({id, defaultMessage, description})
  | _ => None
  };
};

let toJson = ({id, defaultMessage, description}): Yojson.Basic.t =>
  switch (description) {
  | Some(description) =>
    `Assoc([
      ("id", `String(id)),
      ("defaultMessage", `String(defaultMessage)),
      ("description", `String(description)),
    ])
  | None => `Assoc([("id", `String(id)), ("defaultMessage", `String(defaultMessage))])
  };
