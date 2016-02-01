# softgate

When playing around with Tetra communications I came back to a problem that I was thinking about for a long time and easy to solve in the analogue domain. How can you generate a continous stream of data (audio) and mix in an external source when it arrives? Typically this is called a noise gate.

## why would you want to have this?

Streaming solutions such as icecast require continous audio streams, and not individual audio blocks. In my perspective that is a deficiency on their side, but considering I can't change the world in one night lets solve a problem step by step.

## implementation details

After solving issues with the terminal in such way that newlines aren't required for stdin, the second problem pops up. A simplistic C program using stdin cannot cope with EOF from an incoming pipe. This will result in select always marking the stdin filepointer as available, and reading will always return 0 bytes. It cannot be solved with an external pipe or fifo either. A workaround is opening and closing the fifo in the C program itself.

So this program is my first attempt to do the following:

 - create a continuous stream of audio to stdout
 - upon timeout generate signed 16 bit, 8000Hz, little endian, or just a bunch of zero's
 - when data arrives prior to timeout /tmp/audio\_fifo this is copied to stdout
 - don't use an entire networkstack for it

## todo

It seems that the timeout value of select either isn't that exact, or the consumer at stdout results in non-realtime behavior. Hence something should be calculated in such way that the generated stream is generating data in realtime and it is not delaying input from /tmp/audio\_fifo.

