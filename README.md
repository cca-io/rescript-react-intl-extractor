# rescript-react-intl-extractor

Extracts messages for localization from [ReScript] source files.

This assumes that you are using the [rescript-react-intl] bindings for [react-intl].

[![NPM](https://nodei.co/npm/rescript-react-intl-extractor-bin.png?compact=true)](https://nodei.co/npm/rescript-react-intl-extractor-bin/)  
![Build Status](https://github.com/cca-io/rescript-react-intl-extractor/workflows/CI/badge.svg)

## Installation

Binaries for macOS, Linux and Windows can be installed via npm or yarn. For global installation, run

```sh
npm install -g rescript-react-intl-extractor-bin
```

Alternatively, the binaries are also available for download on the [releases page].

## Usage

```sh
rescript-react-intl-extractor [--allow-duplicates] [path...]
```

where `path` is a ReScript (`*.res`) source file or a directory containing such source files. Multiple files/directories may be specified.

The `--allow-duplicates` option allows messages with identical `id` props if the `defaultMessage` props are identical as well.

## Output

The output (a JSON array of all extracted messages sorted by id) is written to stdout. It will look like this:

```json
[
  {
    "id": "message.hello",
    "defaultMessage": "Hello"
  },
  {
    "id": "message.world",
    "defaultMessage": "World"
  },
  {
    "id": "some.message.id",
    "defaultMessage": "Some message"
  }
]
```

## Message Definition (ReScript Syntax)

Formatted messages may be defined in your source files in one of the following ways:

1.  inline in `FormattedMessage`:

```rescript
<ReactIntl.FormattedMessage id="some.message.id" defaultMessage="Some message" />
```

or

```rescript
open ReactIntl
...
<FormattedMessage id="some.message.id" defaultMessage="Some message" />
```

2.  within a module with the `[@intl.messages]` attribute:

```rescript
open ReactIntl

module Msg = {
  @@intl.messages

  let hello = {id: "message.hello", defaultMessage: "Hello"}
  let world = {id: "message.world", defaultMessage: "World"}
}
```

You also can pass descriptions to the records with:

```rescript
let foo = @intl.description("Hello description") {id: "message.hello", defaultMessage: "Hello"}
```

## Building and Testing

The ReScript parser is included as a git submodule. Therefore, after checking out the sources, first run

    git submodule update --init --recursive

Install OCaml and OPAM, and create an OPAM switch with OCaml version 4.14.1.

Then run

    opam pin add rescript-react-intl-extractor.dev . --no-action
    opam install . --deps-only --with-doc --with-test

To build the project, run

    opam exec -- dune build

To run the tests, do

    opam exec -- dune runtest

[rescript]: https://rescript-lang.org/
[rescript-react-intl]: https://github.com/cca-io/rescript-react-intl
[bs-react-intl-ppx]: https://github.com/ahrefs/bs-react-intl-ppx
[react-intl]: https://github.com/yahoo/react-intl
[releases page]: https://github.com/cca-io/rescript-react-intl-extractor/releases
[esy]: https://esy.sh
