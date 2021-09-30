--TEST--
yazstd_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('yazstd')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = yazstd_test1();

var_dump($ret);
?>
--EXPECT--
The extension yazstd is loaded and working!
NULL
