(lang dune 2.7)

(name rescript-react-intl-extractor)

(version 0.14.0)

(generate_opam_files true)

(license MIT)

(authors "Christoph Knittel <ck@cca.io>")

(maintainers "Christoph Knittel <ck@cca.io>")

(source
 (github cca-io/rescript-react-intl-extractor))

(bug_reports
 "https://github.com/cca-io/rescript-react-intl-extractor/issues")

(homepage "https://github.com/cca-io/rescript-react-intl-extractor")

(documentation "https://github.com/cca-io/rescript-react-intl-extractor")

(package
 (name rescript-react-intl-extractor)
 (synopsis "Extracts messages for localization from ReScript source files")
 (depends
  (alcotest :with-test)
  (ocaml
   (and
    (>= 4.10.0)
    (< 4.15.0)))
  (ocamlformat
   (= 0.24.1))
  (yojson
   (= 2.0.2))
  dune))
