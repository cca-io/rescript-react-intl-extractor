type 'a t = 'a option

let iter f o =
  match o with
  | None -> ()
  | Some v -> f v

let is_none o =
  match o with
  | Some _ -> false
  | None -> true
