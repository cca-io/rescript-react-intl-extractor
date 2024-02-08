# Changelog

## 0.15.0 - next

- Build with OCaml 4.14.1.

## 0.14.0 - 2023-09-04

- Updated ReScript parser version to c4cf5fc (ReScript 10.1.4).
- Fix crash when trying to unescape default message.
- Add --verbose flag.

## 0.13.0 - 2022-09-18

- Updated ReScript parser version to 241fefd (ReScript 10.0.1).
- Removed Reason support.
- Unescape extracted strings to fix issue with duplicate backslash (`\\n` vs. `\n`).
- Build with OCaml 4.14.0.
- Perform Linux build on ubuntu-20.04 as ubuntu-18.04 is deprecated.

## 0.12.0 - 2021-05-29

- [bs-react-intl-ppx](https://github.com/ahrefs/bs-react-intl-ppx) support added.
- Reason 3.7.0.
- Changed build process to directly use OPAM instead of esy. This fixes the Windows build issues.
- Switched back from Rely to Alcotest as Rely is not on OPAM and Alcotest seems to be better maintained.
- Perform Linux build on ubuntu-18.04 to fix GLIBC mismatch on Debian 10.

## 0.11.0 - 2021-05-17

- Repo renamed to `rescript-react-intl-extractor` and moved to the cca-io organization.
- NPM package renamed to `rescript-react-intl-extractor-bin`.
- Updated ReScript parser version to bd995f (ReScript 9.1.2).

CI build is currently broken for Windows, therefore there is no Windows binary for this release.

## 0.10.0 - 2021-01-10

- Added ReScript support (ReScript 7cc70c9). 🎉
- Removed support for bs-react-intl 1.x style message definition.
- Downgraded to OCaml 4.06 to be able to build ReScript parser.

## 0.9.1 - 2020-11-15

- Added optional description support for message records via `@intl.description` attribute.

## 0.9.0 - 2020-06-19

- Upgraded Reason parser to Reason 3.6.0 (release).
- OCaml 4.08, Dune 2.
- Migrated tests to [Rely](https://reason-native.com/docs/rely/).
- Moved CI to GitHub Actions.
- macOS, Linux and Windows binaries built by CI.

## 0.8.0 - 2020-02-12

- Upgraded Reason parser to Reason 3.6.0 (8f71db0)
- Upgraded dependencies.

## 0.7.0 - 2019-02-12

- Upgraded Reason parser to Reason 3.4.0.
- Upgraded dependencies.

## 0.6.0 - 2018-12-09

- Upgraded Reason parser to Reason 3.3.7.
- New command line option `--allow-duplicates` to allow duplicate messages ([@alexfedoseev](https://github.com/alexfedoseev)).
- Upgraded dependencies.

## 0.5.0 - 2018-09-15

- Upgraded Reason parser to Reason 3.3.3.
- Ported tests to Alcotest.
- jbuilder -> dune.
- Upgraded dependencies.

## 0.4.0 - 2018-05-25

- Directly use Reason parser api instead of forking the refmt exe for each file.
  Message extraction is now lightning fast! 😀
- Command line args
  - allow multiple source directories to be specified
  - support `-v` and `-help` options
- Support message extraction when `open ReactIntl;` was used.
- Added tests + Travis CI.

## 0.3.0 - 2018-05-16

- Support `[@intl.messages]` for marking messages for extraction.
- Error on duplicate message ids.

## 0.2.0 - 2018-05-15

- Extract messages from `ReactIntl.defineMessages`, too.

## 0.1.0 - 2018-05-13

- Initial Release.
