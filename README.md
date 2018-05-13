# bs-react-intl-extractor

Extracts messages for localization from [Reason] source files.

This assumes that you are using the
[bs-react-intl] bindings for [react-intl] and your source files define formatted messages as follows:

```re
<FormattedMessage id="some.message.id" defaultMessage="Some message" />
```

The output (a JSON array of all extracted messages sorted by id) is written to stdout. It will look like this:

```
[
  {
    "id": "some.message.id",
    "defaultMessage": "Some message"
  },
  {
    "id": "some.other.message.id",
    "defaultMessage": "Some other message"
  },
  ...
]
```

## Download

A macOS binary is available for download on the [releases page].

Download the binary and make it executable using `chmod +x`.

## Usage

```sh
bs-react-intl-extractor <directory>
```

where `directory` is the directory containing yuor Reason source files.

The reason formatter (`refmt`) needs to be on the path.

## Known Issues

Performance is not optimal right now as the `refmt` executable is forked for the processing of each file.

## How to build

Install [esy] as follows:

    % npm install -g esy@latest

Then you can install the project dependencies using:

    % esy install

Then build the project dependencies along with the project itself:

    % esy build

Now you can run your editor within the environment (which also includes merlin):

    % esy $EDITOR
    % esy vim

After you make some changes to source code, you can re-run project's build
using:

    % esy build

And test compiled executable:

    % esy ./_build/default/bin/Extract.exe

Shell into environment:

    % esy shell

[reason]: https://reasonml.github.io
[bs-react-intl]: https://github.com/alexfedoseev/bs-react-intl
[react-intl]: https://github.com/yahoo/react-intl
[releases page]: https://github.com/cknitt/bs-react-intl-extractor/releases
[esy]: https://esy.sh
