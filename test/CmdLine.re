let res = Alcotest.(pair(string, string));

/* Make fast pipe available. */
let (|.) = (a, b) => b(a);

let getChannelContents = channel => {
  open Buffer;

  let buf = create(1024);
  try (
    while (true) {
      buf->add_string(input_line(channel));
      buf->add_string("\n");
    }
  ) {
  | End_of_file => ()
  };

  buf->contents;
};

let run = params => {
  let (in_, out, err) = Unix.open_process_full("_build/default/bin/Extract.exe " ++ params, [||]);

  let res = (getChannelContents(in_), getChannelContents(err));

  Unix.close_process_full((in_, out, err)) |> ignore;

  res;
};
