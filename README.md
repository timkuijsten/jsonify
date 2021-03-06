# jsonify

Convert relaxed json formatted documents and output as strict json.

Only tested with OS X 10.11, Ubuntu 12.04 and 14.04.


## API

* `dst` destination string, strict version of src
* `dstsize` size of dst
* `src` source string, relaxed json to tidy
* `srcsize` size of src, must be <= LONG_MAX
* `firstonly` if truthy, only the first encountered document will be parsed

Returns the number of characters parsed in `src`, or -1 on error.
```c
long relaxed_to_strict(char *dst, size_t dstsize, const char *src, size_t srcsize, int firstonly);
```


## Tests

    $ cc -Wall -Wextra jsmn.c jsonify.c test.c && ./a.out; echo $?


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

---

jsonify uses the JSMN [JSON parser] which is distributed under the MIT license.
