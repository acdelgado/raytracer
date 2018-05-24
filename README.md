Program 4 (Lots of stuff)

I only got to implementing transforms, and it mostly works.

Problems:
  - Your latest transform test files aren't read well by my parser (my fault,
    I'll start using your nice lovely parser for next time).

  - The commit before this works with your new test files, but probably messes
    up the old ones (again, my parser's problem, that thing is a mess).
    I thought I would point that out so you can see that the transforms
    work when they are parsed correctly.

  - I did not have time to implement the Fresnel effect, Beer's law,
    or Anti-Aliasing. Those tests are obviously going to fail.

  - Last time I checked, the triangle in transforms1.pov does not
    have the proper shading(though all the other shapes should).

  - In any reflective sphere I render, there's a weird portion that just
    uses the solid color of the sphere. What's weird, though, is that the
    bug didn't happen until after I implemented refractions, and I don't
    know what the problem is. Any ideas? My best guess is that it has to do
    with whatever epsilon I'm using when I recursively call raytrace from a point.

    Actually, I'm gonna go check that I actually apply that epsilon for reflection.

Anyways, I'll probably come to you in person and tell you about the transform/
parser thing later just in case.
