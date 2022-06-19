type t = {id: string; defaultMessage: string; description: string option}

let compare a b = compare a.id b.id

let fromStringMap ?description map =
  let id = map |> StringMap.find_opt "id" in
  let defaultMessage = map |> StringMap.find_opt "defaultMessage" in
  let description =
    match description |> Option.is_none with
    | true -> map |> StringMap.find_opt "description"
    | false -> description
  in
  match (id, defaultMessage) with
  | Some id, Some defaultMessage -> Some {id; defaultMessage; description}
  | _ -> None

let toJson {id; defaultMessage; description} : Yojson.Basic.t =
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
