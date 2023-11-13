md tests
========

simple tests
------------

first paragraph.
testing surround: _emph_ then **strong** and `code`.

`\`escaped backticks\``.

`x = *y * 6;`

horizontal rule:

- - -


blocks and entities
-------------------

preformatted block:
	.'''' .'.'. |  |
	 '''. | ' | |  |
	''''  '   '  ""

quoted text:
> When in doubt,
> use brute force.

list:
* Make each program do one thing well.
* Expect the output of every program to become the input to another,
as yet unknown, program.
* Design and build software, even operating systems, to be tried early,
ideally within weeks.
* Use tools in preference to unskilled help to lighten a programming task.

list in list:
* a
  * b
    1. c
    2. d
  * e
* f

entity: &, <, >

```
int powerof2(unsigned int n) {
	return !((n - 1) & n) && n > 0;
}
```
links
-----

link: [suckless](http://suckless.org/)

link with title: [suckless](http://suckless.org/ "software that sucks less")

link with title (single quote): [suckless](http://suckless.org/ 'software that sucks less')


images
------

Add images with inline html instead

inline html
-----------

<figure>
    <img src='http://9front.org/img/1331681362.gif' alt='missing' />
    <figcaption>Figure caption</figcaption>
</figure>


# End of test
