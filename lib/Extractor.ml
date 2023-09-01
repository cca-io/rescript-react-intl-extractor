exception PathNotFound of string
exception DuplicateMessageId of string
exception DefaultMessageNotMatching of string

let extract ?(duplicatesAllowed = false) ?(verbose = false) paths =
  let messages = ref StringMap.empty in
  let iterator =
    ExtractionIterator.getIterator (fun message ->
        let {Message.id; defaultMessage} = message in
        match !messages |> StringMap.find_opt id with
        | None -> messages := !messages |> StringMap.add id message
        | Some existingMessage
          when duplicatesAllowed
               && defaultMessage = existingMessage.defaultMessage ->
          messages := !messages |> StringMap.add id message
        | Some existingMessage
          when duplicatesAllowed
               && defaultMessage <> existingMessage.defaultMessage ->
          raise (DefaultMessageNotMatching id)
        | Some _ -> raise (DuplicateMessageId id))
  in
  let extractMessages ast = iterator.structure iterator ast in
  let processFile path =
    if verbose then Printf.eprintf "Processing file: %s\n%!" path;
    let channel = open_in_bin path in
    let src = really_input_string channel (in_channel_length channel) in
    close_in channel;
    let parser =
      let open Res_parser in
      make ~mode:Default src path
    in
    let ast = Res_core.parseImplementation parser in
    extractMessages ast
  in
  let rec processPath path =
    if not (Sys.file_exists path) then raise (PathNotFound path);
    if Sys.is_directory path then
      Sys.readdir path
      |> Array.iter (fun filename ->
             processPath (Filename.concat path filename))
    else
      match Filename.extension path with
      | ".res" -> processFile path
      | _ -> ()
  in
  paths |> List.iter processPath;
  !messages |> StringMap.bindings
  |> List.map (fun (_id, message) -> message)
  |> List.sort Message.compare
