# Usage of libmgba in Poke_Transporter_GB

Libmgba has been copied from https://github.com/mgba-emu/mgba/tree/master/opt/libgba

It offers a way to push debug logging to the MGBA emulator. This is useful during development.

While we are using libmgba in Poke\_Transporter\_GB for debugging purposes, we have taken special care that the code doesn't end up in an actual release build.
(static assert)
Only debug builds _can_ include libmgba into Poke\_Transporter\_GB. However, these shouldn't end up in end-users hands.

