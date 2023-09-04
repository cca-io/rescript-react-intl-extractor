type t = {id: string; defaultMessage: string; description: string option}

let compare a b = compare a.id b.id

let fromStringMap ?description map =
  let id = map |> StringMap.find_opt "id" in
  let defaultMessage = map |> StringMap.find_opt "defaultMessage" in
  let description =
    if Option.is_none description then map |> StringMap.find_opt "description"
    else description
  in
  match (id, defaultMessage) with
  | Some id, Some defaultMessage -> Some {id; defaultMessage; description}
  | _ -> None

let tryUnescape s =
  try Scanf.unescaped s
  with Scanf.Scan_failure err ->
    (* ignore and return original string instead *)
    s

let toJson {id; defaultMessage; description} : Yojson.Basic.t =
  let defaultMessage = tryUnescape defaultMessage in
  match description with
  | Some description ->
    `Assoc
      [
        ("id", `String id);
        ("defaultMessage", `String defaultMessage);
        ("description", `String description);
      ]
  | None ->
    `Assoc [("id", `String id); ("defaultMessage", `String defaultMessage)]
