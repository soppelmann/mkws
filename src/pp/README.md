# `pp(1)`

a preprocessor

`pp(1)` allows embedding
[`sh(1)`](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/sh.html)
code in files of any type by nesting it inside the `#!\n` token, where
`\n` is a new line. That means that if you'd like a simple loop or an
`if` inside an HTML file for instance,
you could use `pp(1)`.

## Basic usage

For the following code:

	<!doctype html>
	<title>pp(1) example</title>
	<ul>
	#!
	i=1
	while test $i -le 10
	do
	if test $((i % 2)) -eq 0
	then
	#!
		<li class=even>$i</li>
	#!
	else
	#!
		<li class=odd>$i</li>
	#!
	fi
	i=$((i + 1))
	done
	#!
	</ul>

`pp(1)` outputs:

	<!doctype html>
	<title>pp(1) example</title>
	<ul>
	        <li class=odd>1</li>
	        <li class=even>2</li>
	        <li class=odd>3</li>
	        <li class=even>4</li>
	        <li class=odd>5</li>
	        <li class=even>6</li>
	        <li class=odd>7</li>
	        <li class=even>8</li>
	        <li class=odd>9</li>
	        <li class=even>10</li>
	</ul>

## Arguments

You can also pass arguments to it by appending them to the command call:

	<!doctype html>
	<title>$1</title>
	<p>
	#!
		echo $2
	#!
	</p>

Calling the above code with
`pp tmp.upphtml 'pp(1) example' 'hello, world'`
will result in:

	<!doctype html>
	<title>pp(1) example</title>
	<p>
	hello world
	</p>

## Pipes

`pp(1)`'s `stdin` is sent to the child
[`sh(1)`](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/sh.html)

	<!doctype html>
	<title>pp(1) example</title>
	<p>
	#!
		cat
	#!
	</p>

`echo 'hello, world' | pp tmp.upphtml` will output:

	<!doctype html>
	<title>pp(1) example</title>
	<p>
	hello, world
	</p>

## Debugging

`pp(1)` also takes an optional `-d` flag. If passed, `pp(1)` will dump
the generated
[`sh(1)`](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/sh.html)
code instead of executing it:

	<!doctype html>
	<title>pp(1) example</title>
	<ul>
	#!
	i=1
	while test $i -le 10
	do
	if test $((i % 2)) -eq 0
	then
	#!
		<li class=even>$i</li>
	#!
	else
	#!
		<li class=odd>$i</li>
	#!
	fi
	i=$((i + 1))
	done
	#!
	</ul>

`pp -d tmp.upphtml` will output:

	echo "<!doctype html>
	<title>pp(1) example</title>
	<ul>"
	i=1
	while test $i -le 10
	do
	if test $((i % 2)) -eq 0
	then
	echo "  <li class=even>$i</li>"
	else
	echo "  <li class=odd>$i</li>"
	fi
	i=$((i + 1))
	done
	echo "</ul>"

## Example

	<!doctype html>
	<html lang=en>
	<title>pp(1)</title>
	<meta charset=UTF-8>
	<meta name=viewport content='width=device-width'>
	<link rel=stylesheet href=../theme.css>
	<body>
	<a href=../#myprojects>Home</a>
	#!
	addcls() {
		sed "s/<code>/<code class=language-bash>/g"
	}
	smu README | addcls
	#!
	<h2>Files</h2>
	<ul>
	#!
	for f in *
	do
	case $f in
		*.upp*|*.html|*.tgz)
		continue
		;;
	esac
	#!
	<li><a href=$f>$f</a></li>
	#!
	done
	#!
	</ul>
	<h2>Download</h2>
	<a href=pp.tgz>pp.tgz</a>
	<script src=../prism.js data-manual></script>
	<script src=../enhance.js></script>
	</body>
	</html>

## See also

* <https://github.com/radare/spp>
* <https://werc.cat-v.org/docs/rc-template-lang>
* <https://code.9front.org/hg/werc/file/92f7463dac1a/bin/template.awk>
