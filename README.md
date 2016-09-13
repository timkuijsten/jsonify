# jsonify

Convert relaxed json formatted documents and output as strict json.

Only tested with OS X 10.11, Ubuntu 12.04 and 14.04.

## Installation

Download and compile jsonify:

    $ git clone https://github.com/timkuijsten/jsonify.git
    $ cd jsonify
    $ cc main.c common.c jsmn.c jsonify.c


## Usage examples

Read a "relaxed" json document and convert to strict json:

    $ echo '{ foo: "bar" }' | ./a.out
    {"foo":"bar"}

## Tests

    $ cc common.c jsmn.c jsonify.c test.c && ./a.out


## License

ISC

Copyright (c) 2016 Tim Kuijsten

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
