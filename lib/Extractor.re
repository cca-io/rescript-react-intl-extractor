exception PathNotFound(string);
exception DuplicateMessageId(string);
exception DefaultMessageNotMatching(string);

let extract = (~duplicatesAllowed=false, paths) => {
  let messages = ref(StringMap.empty);

  let iterator =
    ExtractionIterator.getIterator(message => {
      let {Message.id, defaultMessage} = message;

      switch (messages^ |> StringMap.find_opt(id)) {
      | None => messages := messages^ |> StringMap.add(id, message)

      | Some(existingMessage) when duplicatesAllowed && defaultMessage == existingMessage.defaultMessage =>
        messages := messages^ |> StringMap.add(id, message)

      | Some(existingMessage) when duplicatesAllowed && defaultMessage != existingMessage.defaultMessage =>
        raise(DefaultMessageNotMatching(id))

      | Some(_) => raise(DuplicateMessageId(id))
      };
    });

  let extractMessages = ast => iterator.structure(iterator, ast);

  let processReasonFile = path => {
    let channel = open_in_bin(path);
    let lexbuf = Lexing.from_channel(channel);
    let ast = Reason_toolchain.(RE.implementation(lexbuf) |> To_current.copy_structure);
    close_in(channel);

    extractMessages(ast);
  };

  let processReScriptFile = path => {
    let channel = open_in_bin(path);
    let src = really_input_string(channel, in_channel_length(channel));
    close_in(channel);

    let parser = Res_parser.(make(~mode=Default, src, path));
    let ast = Res_core.parseImplementation(parser);

    extractMessages(ast);
  };

  let rec processPath = path => {
    if (!Sys.file_exists(path)) {
      raise(PathNotFound(path));
    };

    if (Sys.is_directory(path)) {
      Sys.readdir(path) |> Array.iter(filename => processPath(Filename.concat(path, filename)));
    } else {
      switch (Filename.extension(path)) {
      | ".re" => processReasonFile(path)
      | ".res" => processReScriptFile(path)
      | _ => ()
      };
    };
  };

  paths |> List.iter(processPath);
  messages^ |> StringMap.bindings |> List.map(((_id, message)) => message) |> List.sort(Message.compare);
};
