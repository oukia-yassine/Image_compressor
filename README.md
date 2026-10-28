# Colour Histogram & Popularity Quantization (C)

A C project that builds a memory-efficient colour histogram from a PPM image and uses it to reduce the image to its **K most frequent colours** (popularity quantization).

Developed as part of the *Fondements de l'informatique* course at **ENSICAEN** (2025–2026), supervised by Luc Brun.

**Authors:** Yassine Oukia, Yassir Douraid

---

## Table of contents

- [Overview](#overview)
- [Project structure](#project-structure)
- [Requirements](#requirements)
- [Build](#build)
- [Usage](#usage)
- [How it works](#how-it-works)
- [API summary](#api-summary)
- [Tests](#tests)
- [Known limitations](#known-limitations)

---

## Overview

A full RGB histogram would need a 256³ array (16,777,216 entries, i.e. 64 MiB with 4-byte counters), which is wasteful for ordinary images. This project instead stores the histogram as a **256 × 256 array of sorted linked lists**:

```
histo[R][G]  ──►  (B₁, f₁) ─► (B₂, f₂) ─► (B₃, f₃) ─► NULL      with B₁ < B₂ < B₃
```

Each cell of a list holds a blue value `B` and its frequency `freq`. If a cell with value `B` exists in `histo[R][G]`, then the image contains `freq` pixels of colour `(R, G, B)`. Only colours that actually appear in the image consume memory.

On top of this histogram, the project provides:

1. **`ppmhist`** – prints every distinct colour of an image with its luminance and number of occurrences.
2. **`prog_quantification`** – reduces an image to its K most popular colours and writes the result to `output.ppm`.

## Project structure

```
.
├── Makefile
├── include/
│   ├── histogram.h          # Histogram + iterator API (public types and prototypes)
│   ├── quantification.h     # quantification() and mapping()
│   ├── image.h              # Provided image module (load/save PPM, pixel access)
│   ├── minunit.h            # Minimal unit-test framework
│   └── ...                  # Other headers from the provided image module
├── lib/
│   └── libimage.a           # Provided image library
└── src/
    ├── histogram.c          # Linked lists, histogram, iterator
    ├── quantification.c     # Top-K selection + nearest-colour mapping (squared L2 distance)
    ├── quantificationL_1_2.c# Variant of the mapping using an (L1 + L∞)/2 distance
    ├── ppmhist.c            # Program: list colours of an image
    ├── prog_quantification.c      # Program: quantize an image to K colours
    ├── prog_quantificationL_1_2.c # Program driver for the L1/L∞ variant
    └── test/
        └── test_projet.c    # Unit tests (MinUnit)
```

> The `cell` and `histo_iter` structures are **private** to `histogram.c`, as required by the assignment; `histogram.h` only exposes opaque pointer types.

## Requirements

- `gcc` (C89/ANSI-compatible; the test target uses C99)
- `make`
- The provided `libimage.a` (already included in `lib/`)

## Build

Create the output directory first (the Makefile writes binaries to `bin/`):

```bash
mkdir -p bin
make ppmhist
make prog_quantification
make test_projet
```

Debug build (adds `-g`):

```bash
make DEBUG=1 prog_quantification
```

Clean:

```bash
make clean
```

## Usage

### `ppmhist` – colour listing

```bash
./bin/ppmhist image.ppm
```

Prints one line per distinct colour, in row-major order of `(R, G)` then increasing `B`:

```
R G B Lum Freq
12 40 200 84 3
12 40 201 84 1
...
```

`Lum = (R + G + B) / 3` (integer division) and `Freq` is the number of pixels with that exact colour.

### `prog_quantification` – quantization to K colours

```bash
./bin/prog_quantification image.ppm K
```

- `image.ppm`: input image in PPM format
- `K`: number of colours to keep (positive integer)

The quantized image is written to **`output.ppm`** in the current directory.

Example:

```bash
./bin/prog_quantification images/lena.ppm 16
```

## How it works

### Histogram construction

For each pixel `(R, G, B)`, `init_histo` inserts `B` into the list `histo[R][G]` through `insert_cell`, which:

- creates a new cell with `freq = 1` if `B` is absent, inserted at the right position to keep the list **sorted in increasing order of `B`**;
- increments `freq` if `B` is already present.

### Iterator

`histo_iter` walks through all colours of the histogram without exposing its internals. It keeps the current `(R, G)` position and the current cell, and moves along the list of `B` values before jumping to the next non-empty `histo[R][G]` (top-left to bottom-right reading order).

### Popularity quantization

`quantification(h, tab, K)` selects the K most frequent colours:

1. **Initialisation** – the first K colours of the histogram are inserted into a list sorted by frequency (insertion sort).
2. **Scan** – for each remaining colour, if its frequency is greater than the smallest frequency currently kept, it is inserted at its sorted position and the least frequent one is dropped.
3. **Output** – the K colours are written to `tab`, a flat array of size `K × 3`.

Complexity: O(N·K) in the worst case for N distinct colours, with O(K) extra memory.

### Mapping

`mapping(input, output, tab, K)` replaces every pixel of the input with the nearest colour in the palette `tab`. The default distance is the squared Euclidean distance in RGB space. The nearest-neighbour search is a linear scan of the palette, i.e. O(K) per pixel.

### Distance variant (`quantificationL_1_2.c`)

An alternative mapping uses the distance `(‖·‖₁ + ‖·‖∞) / 2` instead of the squared Euclidean distance. To try it, build the driver against this file instead of `quantification.c`:

```bash
gcc -Wall -pedantic -ansi -Iinclude \
    src/prog_quantificationL_1_2.c src/quantificationL_1_2.c src/histogram.c \
    -o bin/prog_quantificationL -Llib -limage
```

## API summary

**Lists / cells** (`histogram.h`)

| Function | Description |
|---|---|
| `cell create_cell(int B, cell next)` | Allocates a cell with `freq = 1` |
| `cell insert_cell(cell head, int B)` | Sorted insertion, or frequency increment |
| `cell delete_list(cell list)` | Frees a whole list |

**Histogram**

| Function | Description |
|---|---|
| `histo create_histo()` | Allocates the 256×256 array (all `NULL`) |
| `void init_histo(histo h, image img)` | Fills the histogram from an image |
| `void delete_histo(histo h)` | Frees all lists and the array |
| `int give_freq_histo(histo h, int R, int G, int B)` | Number of pixels of colour `(R,G,B)` |

**Iterator**

| Function | Description |
|---|---|
| `histo_iter create_histo_iter(histo h)` | Positions on the first colour (asserts if the histogram is empty) |
| `void start_histo_iter(histo_iter, histo)` | Resets to the first colour |
| `boolean next_histo_iter(histo_iter, histo)` | Advances; `false` at the end |
| `void give_color_histo_iter(histo_iter, int *rgb)` | Writes the current `(R,G,B)` into `rgb[3]` |
| `void delete_histo_iter(histo_iter)` | Frees the iterator |

**Quantization** (`quantification.h`)

| Function | Description |
|---|---|
| `void quantification(histo h, int *tab, int K)` | Fills `tab` (size `K×3`) with the K most frequent colours |
| `void mapping(image in, image out, int *tab, int K)` | Remaps each pixel to its nearest palette colour |

## Tests

Unit tests use the bundled [MinUnit](include/minunit.h) framework and cover histogram initialisation, a full iterator scan, and top-K selection on a small synthetic image.

```bash
mkdir -p bin
make test_projet
./bin/test_projet
```

Expected output ends with:

```
3 tests, 9 assertions, 0 failures
```

## Known limitations

- The **optional extension** from the assignment (merging colours with `R/2`, `G`, `B/4` equality and using representatives in `mapping`) is **not implemented**.
- The output file name is fixed to `output.ppm`.
- Nearest-colour search is linear in K per pixel; a spatial structure (e.g. k-d tree) could improve this for large K.
- The `all` target in the Makefile refers to a `quantification` target that doesn't exist; build the targets listed above individually. The `prog_quantificationL` target also links `quantification.c` rather than `quantificationL_1_2.c`, hence the manual command shown in the variant section.
- Only the PPM format is supported.

## Acknowledgements

The image module (`image.h`, `libimage.a`) is provided by the course (L. Brun, ENSICAEN / GREYC).
