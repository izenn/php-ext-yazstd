# yazstd
> Yet Another zstd is a small PHP extension to support zStandard chunks streaming compression/decompression it was created to give zStandard support for NSP Indexer but can be used also for other purposes

# How To Install

``` bash
% git clone --recursive --depth=1 https://github.com/proconsule/php-ext-yazstd
% phpize
% ./configure
% make
% make install
```

# How To Use

Compression

```
$compressionLevel = 15;
$yazstd = new yazstd_compress(compressionLevel);
$compressedChunk = $yazstd->decompress($rawChunk); //with all chunks you have
$compressedChunk = $yazstd->endstream(); //at the end of streaming data (may be 0 if no data remain to flush)
```

Decompression
```
$yazstd = new yazstd_decompress();
$decompressedChunk = $yazstd->decompress($compressedChunk);
```

You can feed a streaming of data (tested on very very big files) with any chunksize, it returns zero len string if need more data for decompression.

This extension is in a very early stage so expect any kind of problems!

