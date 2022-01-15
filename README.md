# omaudiolib

`omaudiolib` is a Juce-based audio playback library providing a C API
for use in music applications.

## Compilation

To build `omaudiolib`, get the sources from

  https://github.com/openmusic-project/omaudiolib

The 3 platforms Linux, MacOSX and Windows have files for build systems
(Makefile, XCode, VisualStudio) inside the Builds directory.  So
fex. to build your own version of omaudiolib.so for Linux, you'd do
something like

```
  cd Builds/Linux
  make
```

> ### Notes for maintainers on Linux, dependencies, bugs between OS'es
>
> `omaudiolib` is based on JUCE, and some of the core Juce-modules are
> dependent on libs with different default variants between OS'es
> (Fedora: `libcurl`, Ubuntu: `libcurl-gnutls`), while none of these libs'
> symbols are ever used by OM.

To compile `omaudiolib.so` so that it doesn't break because of these
inconsistencies between OS'es, edit the `JUCE_LDFLAGS` lines down in the
Makefile, adding:

```
  -Wl,--as-needed $(shell pkg-config --libs alsa libcurl)
```

## Using omaudiolib with OM (om-sharp, OpenMusic)

OM already comes with a precompiled version of `omaudiolib` installed as
part of its resources.  If you compile your own version and want to
use that with OM, install it where OM has already installed its
version of `omaudiolib` (taking care to backup the original version).


## Bug reports

If you observe bugs or strange behavior, place an issue on the issue
tracker at github:

 https://github.com/openmusic-project/omaudiolib/issues
