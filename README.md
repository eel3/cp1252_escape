cp1252\_escape
==============

Command line tool to escape 8bit characters greater than 0x7F.

License
-------

zlib License.

Target environments
-------------------

Windows.

cp1252\_escape is written in ANSI C89 and Go 1.3, and so probably works fine on other OS.

Set up
------

All you have to do is compile escape.c or escape.go.

Usage
-----

Please check help message `escape -h`

Example
-------

    $ # using hexdecode (https://github.com/eel3/hexdecode)
    $ echo 68656C6C6F20776F726C640D0A | hexdecode
    hello world
    $ echo 68656C6C6F20776F726C640D0A | hexdecode | escape
    hello world
    $ echo 7D7E8081 | hexdecode | escape
    }~\x80\x81
    $ _
