### Exercise for CERN-HSF GSoC 2024 : Lossless compression of raw data for the ATLAS experiment at CERN

Name - Aditya Mayukh Som

### Description

- Implemented C++ functions to (de)compress input data using 'Run-Length encoding' and 'Dictionary coder'
- Implemented the following algorithms:
  1. Run-Length encoding
  2. LZW
  3. LZ77
  4. LZ78
- Performance: Comparision of the methods in terms of Compression ratio, (de)compression speed and memory usage.

#### A briefing of the Compression alorithms

The following sections give a brief overview about the compression algorithms.

### _Run-Length encoding_

> The Run-Length Encoding (RLE) algorithm implemented here is a simple compression technique that compresses data by representing consecutive repeated symbols with a count and the symbol itself.
> Conversely, during decompression, it expands the encoded data back to its original form by repeating the symbols according to the count provided.

### _LZW_

> The Lempel-Ziv-Welch (LZW) algorithm implemennted here is a lossless data compression algorithm. It operates by replacing repetitive sequences of symbols (such as characters in a text file) with references to a dictionary of previously encountered sequences.
> LZW achieves compression by exploiting redundancy in the input data, effectively representing repetitive sequences with shorter codes, thereby reducing the overall size of the data.

### _LZ77_

> The LZ77 algorithms implemented here is a dictionary-based, lossless compression algorithm widely used in various compression utilities. LZ77 achieves compression by replacing repeated occurrences of data with references to a single copy of that data existing earlier in the uncompressed data stream.

### _LZ78_

> The LZ78 compression algorithm implemented here is another widely-used dictionary-based, lossless compression algorithm. LZ78 algorithm achieves compression by replacing repetitive substrings with references to previously encountered phrases.

### Local Setup

To locally build the executable, you need to have `clang` toolchain and `make` available. To build the executable, run -

```sh
make
```

Then to run, use -

```sh
./encoder filename1.ext filename2.ext filename3.txt
```

**Note:** Files need to reside inside `./original` directory where `./encoder` is run.

### Performance And Comparison

Benchmarking done on AMD Ryzen 7 5700U With Raedon Graphics, 1800MHz, 8 Cores, 16GB RAM

```sh
Compression Ratio = Uncompressed File Size / Compressed File Size
Compression Speed = Input File Size / Compression Time
Decompression Speed = Decompressed File Size / Decompression Time
```

### Results

![results from running the algorithms](./results.png)