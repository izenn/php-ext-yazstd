# yazstd
> a small PHP extension to support zStandard chunks streaming decompression on PHP it was created to give zStandard support for NSP Indexer but can be used also for other purposes

# How To Install

clone the repository

run phpize

./configure

make && make install

# How To Use

```
$yazstd = new yazstd_decompress();
$decompressedChunk = $yazstd->decompress($compressedChunk);
```

You can feed a streaming of data (tested on very very big files) with any chunksize, it returns zero len string if need more data for decompression.

This extension is in a very early stage so expect any kind of problems!

