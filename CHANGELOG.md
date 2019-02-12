# Changelog

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
