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

#### A briefing of the Compression alorithms:

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

### Performance

Benchmark done on AMD Ryzen 7 5700U, 16GB RAM
```
Compression Ratio = Uncompressed File Size / Compressed File Size
```

#### RLE Algorithm

| Filename  | Compression Time (Seconds) | Compression Ratio | Space Saving (%) | Decompression Time (Seconds) |
| --------- | -------------------------- | ----------------- | ---------------- | ---------------------------- |
| test1.txt | 0.001110                   | 0.655844          | -52.48           | 0.000863                     |
| test2.txt | 0.813794                   | 0.549729          | -81.91           | 0.642899                     |
| test3.txt | 0.060198                   | 0.514797          | -94.25           | 0.046645                     |
| test4.txt | 1.010073                   | 0.514600          | -94.33           | 0.808600                     |
| test5.txt | 0.724845                   | 0.511183          | -95.62           | 0.594582                     |

#### LZW Algorithm

| Filename  | Compression Time (Seconds) | Compression Ratio | Space Saving (%) | Decompression Time (Seconds) |
| --------- | -------------------------- | ----------------- | ---------------- | ---------------------------- |
| test1.txt | 0.000358                   | 0.814516          | -22.77           | 0.000285                     |
| test2.txt | 2.575863                   | 1.652734          | 39.49            | 0.506286                     |
| test3.txt | 0.111178                   | 1.192819          | 16.17            | 0.036491                     |
| test4.txt | 3.005140                   | 1.633830          | 38.79            | 0.605782                     |
| test5.txt | 2.122859                   | 1.756131          | 43.06            | 0.386197                     |

#### LZ77 Algorithm

| Filename  | Compression Time (Seconds) | Compression Ratio | Space Saving (%) | Decompression Time (Seconds) |
| --------- | -------------------------- | ----------------- | ---------------- | ---------------------------- |
| test1.txt | 0.000246                   | 2.805556          | 64.36            | 0.000232                     |
| test2.txt | 4.558055                   | 0.638687          | -56.57           | 0.118950                     |
| test3.txt | 0.306998                   | 0.619015          | -61.55           | 0.008442                     |
| test4.txt | 5.499575                   | 0.617860          | -61.85           | 0.142949                     |
| test5.txt | 3.463130                   | 0.713053          | -40.24           | 0.096550                     |

#### LZ78 Algorithm

| Filename  | Compression Time (Seconds) | Compression Ratio | Space Saving (%) | Decompression Time (Seconds) |
| --------- | -------------------------- | ----------------- | ---------------- | ---------------------------- |
| test1.txt | 0.000252                   | 0.721429          | -38.61           | 0.000116                     |
| test2.txt | 2.278759                   | 1.508175          | 33.69            | 0.206536                     |
| test3.txt | 0.087744                   | 1.135339          | 11.92            | 0.013205                     |
| test4.txt | 2.623203                   | 1.482238          | 32.53            | 0.227594                     |
| test5.txt | 1.745687                   | 1.584142          | 36.87            | 0.169976                     |
