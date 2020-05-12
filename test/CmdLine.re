let res = Alcotest.(pair(string, string));

let getChannelContents = channel => {
  open Buffer;

  let buf = create(1024);
  try(
    while (true) {
      add_string(buf, input_line(channel));
      add_string(buf, "\n");
    }
  ) {
  | End_of_file => ()
  };

  contents(buf);
};

let binDir = Filename.dirname(Sys.argv[0]);
let pathToExtractExe = Filename.concat(binDir, "Extract.exe");

let run = params => {
  let (in_, out, err) = Unix.open_process_full(pathToExtractExe ++ " " ++ params, [||]);

  let res = (getChannelContents(in_), getChannelContents(err));

  Unix.close_process_full((in_, out, err)) |> ignore;

  res;
};
