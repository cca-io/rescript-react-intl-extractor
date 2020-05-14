#!/bin/bash

refmt --in-place -w 120 `find bin lib test -name '*.re'`
