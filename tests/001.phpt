--TEST--
Check if yazstd is loaded
--SKIPIF--
<?php
if (!extension_loaded('yazstd')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "yazstd" is available';
?>
--EXPECT--
The extension "yazstd" is available
