let res = Alcotest.(pair(string, string));

let getChannelContents = channel => {
  open Buffer;

  let buf = create(1024);
  try(
    while (true) {
      buf->add_string(input_line(channel));
      buf->add_string("\n");
    }
  ) {
  | End_of_file => ()
  };

  buf->contents;
};

let binDir = Sys.argv[0]->Filename.dirname;
let pathToExtractExe = binDir->Filename.concat("Extract.exe");

let run = params => {
  let (in_, out, err) = Unix.open_process_full(pathToExtractExe ++ " " ++ params, [||]);

  let res = (getChannelContents(in_), getChannelContents(err));

  Unix.close_process_full((in_, out, err)) |> ignore;

  res;
};
