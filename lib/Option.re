type t('a) = option('a);

let iter = (f, o) =>
  switch (o) {
  | None => ()
  | Some(v) => f(v)
  };

let is_none = o =>
  switch (o) {
  | Some(_) => false
  | None => true
  };
