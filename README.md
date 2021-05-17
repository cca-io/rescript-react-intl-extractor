# rescript-react-intl-extractor

Extracts messages for localization from [ReScript] or [Reason] source files.
This assumes that you are using the [rescript-react-intl] 2.x bindings for [react-intl].

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

where `path` is a ReScript (`*.res`) or Reason (`*.re`) source file or a directory containing such source files. Multiple files/directories may be specified.

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
open ReactIntl;
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

## Message Definition (Reason Syntax)

Formatted messages may be defined in your source files in one of the following ways:

1.  inline in `FormattedMessage`:

```reason
<ReactIntl.FormattedMessage id="some.message.id" defaultMessage="Some message" />
```

or

```reason
open ReactIntl;
...
<FormattedMessage id="some.message.id" defaultMessage="Some message" />
```

2.  within a module with the `[@intl.messages]` attribute:

```reason
open ReactIntl;

module Msg = {
  [@intl.messages];

  let hello = {id: "message.hello", defaultMessage: "Hello"};
  let world = {id: "message.world", defaultMessage: "World"};
};
```

You also can pass descriptions to the records with:

```reason
let foo = [@intl.description "Hello description"] {id: "message.hello", defaultMessage: "Hello"};
```

## Building and Testing

The ReScript parser is included as a git submodule. Therefore, after checking out the sources, first run

    git submodule update --init --recursive

Install [esy] as follows:

    % npm install -g esy

Then you can install the project dependencies using:

    % esy install

Then build the project dependencies along with the project itself:

    % esy build

Run the compiled executable:

    % esy x Extract.exe

Run the tests:

    % esy test

[rescript]: https://rescript-lang.org/
[reason]: https://reasonml.github.io
[rescript-react-intl]: https://github.com/cca-io/rescript-react-intl
[react-intl]: https://github.com/yahoo/react-intl
[releases page]: https://github.com/cca-io/rescript-react-intl-extractor/releases
[esy]: https://esy.sh
