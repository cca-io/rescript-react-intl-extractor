# bs-react-intl-extractor

Extracts messages for localization from [Reason] source files.

This assumes that you are using the
[bs-react-intl] bindings for [react-intl] and your source files define formatted messages as follows:

```re
<FormattedMessage id="some.message.id" defaultMessage="Some message" />
```

The reason formatter (`refmt`) needs to be on the path.

## Usage

```sh
bs-react-intl-extractor <directory>
```

where `directory` is the directory containing yuor Reason source files.

The output (a JSON array of all extracted messages sorted by id) is written to stdout.

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
[esy]: https://esy.sh
