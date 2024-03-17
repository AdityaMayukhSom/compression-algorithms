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

Benchmark done on AMD Ryzen 7 5700U, 8GB RAM

```sh
Compression Ratio = Uncompressed File Size / Compressed File Size
```

#### RLE Algorithm

| File                 | Compression Time (Sec) | Compression Ratio | Compression Speed (Mbps) | Space Saving (%) | Decompression Time (Sec) | Decompression Speed (Mbps) |
| -------------------- | ---------------------- | ----------------- | ------------------------ | ---------------- | ------------------------ | -------------------------- |
| test1.txt(101 B)     | 0.002487               | 0.655844          | 0.0387298                | -52.4752         | 0.000819                 | 0.179323                   |
| test2.txt(5.21 MB)   | 2.10128                | 0.549729          | 2.47723                  | -81.9078         | 1.6814                   | 5.63157                    |
| test3.txt(352.02 KB) | 0.150867               | 0.514797          | 2.27864                  | -94.2514         | 0.110763                 | 6.02891                    |
| test4.txt(6.19 MB)   | 2.59919                | 0.5146            | 2.38077                  | -94.3258         | 2.11249                  | 5.69235                    |
| test5.txt(4.15 MB)   | 1.77609                | 0.511183          | 2.33637                  | -95.6245         | 1.40145                  | 5.79235                    |

#### LZW Algorithm

| File                 | Compression Time (Sec) | Compression Ratio | Compression Speed (Mbps) | Space Saving (%) | Decompression Time (Sec) | Decompression Speed (Mbps) |
| -------------------- | ---------------------- | ----------------- | ------------------------ | ---------------- | ------------------------ | -------------------------- |
| test1.txt(101 B)     | 0.001601               | 0.814516          | 0.0601631                | -22.7723         | 0.000821                 | 0.144039                   |
| test2.txt(5.21 MB)   | 5.02104                | 1.65273           | 1.03671                  | 39.4942          | 1.10529                  | 2.84952                    |
| test3.txt(352.02 KB) | 0.252149               | 1.19282           | 1.36336                  | 16.165           | 0.08207                  | 3.51164                    |
| test4.txt(6.19 MB)   | 6.04637                | 1.63383           | 1.02344                  | 38.7941          | 1.33047                  | 2.84671                    |
| test5.txt(4.15 MB)   | 3.74809                | 1.75613           | 1.10713                  | 43.0566          | 0.794145                 | 2.97544                    |

#### LZ77 Algorithm

| File                 | Compression Time (Sec) | Compression Ratio | Compression Speed (Mbps) | Space Saving (%) | Decompression Time (Sec) | Decompression Speed (Mbps) |
| -------------------- | ---------------------- | ----------------- | ------------------------ | ---------------- | ------------------------ | -------------------------- |
| test1.txt(101 B)     | 0.000739               | 2.80556           | 0.13034                  | 64.3564          | 0.000347                 | 0.0989403                  |
| test2.txt(5.21 MB)   | 11.3154                | 0.638687          | 0.460023                 | -56.5711         | 0.332092                 | 24.5416                    |
| test3.txt(352.02 KB) | 0.856642               | 0.619015          | 0.401301                 | -61.5469         | 0.020386                 | 27.2418                    |
| test4.txt(6.19 MB)   | 14.5446                | 0.61786           | 0.425456                 | -61.8489         | 0.353034                 | 28.3693                    |
| test5.txt(4.15 MB)   | 8.07736                | 0.713048          | 0.513734                 | -40.2431         | 0.224386                 | 25.9354                    |

#### LZ78 Algorithm

| File                 | Compression Time (Sec) | Compression Ratio | Compression Speed (Mbps) | Space Saving (%) | Decompression Time (Sec) | Decompression Speed (Mbps) |
| -------------------- | ---------------------- | ----------------- | ------------------------ | ---------------- | ------------------------ | -------------------------- |
| test1.txt(101 B)     | 0.000321               | 0.721429          | 0.300066                 | -38.6139         | 0.000291                 | 0.458812                   |
| test2.txt(5.21 MB)   | 4.31937                | 1.50818           | 1.20512                  | 33.6947          | 0.433402                 | 7.96355                    |
| test3.txt(352.02 KB) | 0.198921               | 1.13534           | 1.72818                  | 11.9205          | 0.028223                 | 10.7285                    |
| test4.txt(6.19 MB)   | 5.51734                | 1.48224           | 1.12157                  | 32.5344          | 0.510542                 | 8.17723                    |
| test5.txt(4.15 MB)   | 3.25623                | 1.58414           | 1.27436                  | 36.8743          | 0.340974                 | 7.68232                    |
